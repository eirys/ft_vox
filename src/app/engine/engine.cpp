/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:09:44 by etran             #+#    #+#             */
/*   Updated: 2024/01/31 16:35:05 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "window.h"
#include "uniform_buffer_object.h"
#include "timer.h"
#include "world_macros.h"

// Vox
#include "chunk.h"
#include "game_state.h"

// Gfx
#include "culling_pipeline.h"
#include "scene_pipeline.h"
#include "shadows_pipeline.h"
#include "texture_handler.h"
#include "target.h"
#include "descriptor_set.h"
#include "render_pass.h"
#include "chunk_texture_handler.h"

#include "debug.h"

#include <cstring> // std::strcmp

using Mat4 = ::scop::Mat4;
using DescriptorSetPtr = std::shared_ptr<scop::gfx::DescriptorSet>;

namespace scop {

using gfx::ChunkTextureHandler;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	Engine::init(Window& window, const vox::GameState& game) {
	_core.init(window);
	_swap_chain.init(_core.getDevice(), window);

	_command_pool.init(_core.getDevice());
	_draw_buffer = _command_pool.createCommandBuffer(_core.getDevice(), gfx::CommandBufferType::DRAW);
	_compute_buffer = _command_pool.createCommandBuffer(_core.getDevice(), gfx::CommandBufferType::COMPUTE);

	_input_handler.init(_core.getDevice(), game);

	_pipeline_manager.init(_core.getDevice(), _swap_chain);
	{
		std::vector<DescriptorSetPtr>	descriptors = {
			_pipeline_manager.getScenePipeline()->getDescriptor(),
			_pipeline_manager.getShadowsPipeline()->getDescriptor(),
			_pipeline_manager.getCullingPipeline()->getDescriptor() };

		_descriptor_pool.init(_core.getDevice(), descriptors);
	}
	_pipeline_manager.plugDescriptors(_core.getDevice(), _input_handler);

	_synchronizer.init(_core.getDevice());

	// TODO
	// _pipelines.scene->update(_updateUbo(game));
}

void	Engine::destroy() {
	_synchronizer.destroy(_core.getDevice());

	_descriptor_pool.destroy(_core.getDevice());
	_pipeline_manager.destroy(_core.getDevice());
	_input_handler.destroy(_core.getDevice());
	_command_pool.destroy(_core.getDevice());
	_swap_chain.destroy(_core.getDevice());
	_core.destroy();
}

/* ========================================================================== */

void	Engine::idle() {
	_core.getDevice().idle();
}

void	Engine::render(
	Window& window,
	const vox::GameState& game,
	Timer& timer
) {
	const scop::Synchronizer::Semaphores& semaphores = _synchronizer.getSemaphores();
	const scop::Synchronizer::Fences& fences = _synchronizer.getFences();

	// Retrieve image for swap chain -----
	fences.graphics_in_flight.wait(_core.getDevice());
	uint32_t	image_index;
	if (_swap_chain.acquireNextImage(_core.getDevice(), _synchronizer, &image_index) == false)
		return _updatePresentation(window);
	fences.graphics_in_flight.reset(_core.getDevice());
	// -----------------------------------

	// Record buffers ----------------
	_draw_buffer.reset();
	_draw_buffer.begin(0);

	SCOP_DEBUG("Computing");
	_pipeline_manager.getCullingPipeline()->compute(
		_core.getDevice(),
		_pipeline_manager.getPipelineLayout(),
		_compute_buffer);
	_input_handler.retrieveData();

	SCOP_DEBUG("Drawing");
	_pipeline_manager.getShadowsPipeline()->draw(
		_pipeline_manager.getPipelineLayout(),
		_draw_buffer,
		_input_handler,
		image_index);
	_pipeline_manager.getScenePipeline()->draw(
		_pipeline_manager.getPipelineLayout(),
		_draw_buffer,
		_input_handler,
		image_index);
	_draw_buffer.end(_core.getDevice(), false);

	_pipeline_manager.getScenePipeline()->update(_updateUbo(game));
	// ------------------------------

	std::array<VkSemaphore, 2>				signal_semaphores;
	std::array<VkSemaphore, 2>				wait_semaphores;
	std::array<VkPipelineStageFlags, 2>		wait_stages;
	std::array<VkCommandBuffer, 1>			command_buffers;

	VkSubmitInfo	submit_info{};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.pWaitSemaphores = wait_semaphores.data();
	submit_info.pSignalSemaphores = signal_semaphores.data();
	submit_info.pWaitDstStageMask = wait_stages.data();
	submit_info.pCommandBuffers = command_buffers.data();
	submit_info.commandBufferCount = 1;

	// Compute -------
	fences.compute_in_flight.wait(_core.getDevice());
	fences.compute_in_flight.reset(_core.getDevice());

	command_buffers = { _compute_buffer.getBuffer() };
	signal_semaphores =	{ semaphores.compute_finished.getSemaphore() };
	submit_info.signalSemaphoreCount = 1;

	wait_stages = { VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT };
	wait_semaphores = { semaphores.graphics_finished.getSemaphore() };
	submit_info.waitSemaphoreCount = 1;

	SCOP_DEBUG("Submit compute command buffer");
	if (vkQueueSubmit(_core.getDevice().getComputeQueue(), 1, &submit_info, fences.compute_in_flight.getFence()) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit compute command buffer");
	}
	SCOP_DEBUG("Compute command buffer submitted");
	// ---------------

	// Graphics ---------------------
	command_buffers = { _draw_buffer.getBuffer() };
	signal_semaphores = { semaphores.render_finished.getSemaphore(), semaphores.graphics_finished.getSemaphore() };
	submit_info.signalSemaphoreCount = 2;

	wait_stages = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT };
	wait_semaphores = { semaphores.image_available.getSemaphore(), semaphores.compute_finished.getSemaphore() };
	submit_info.waitSemaphoreCount = 2;

	SCOP_DEBUG("Submit draw command buffer");
	if (vkQueueSubmit(_core.getDevice().getGraphicsQueue(), 1, &submit_info, fences.graphics_in_flight.getFence()) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer");
	}
	SCOP_DEBUG("Draw command buffer submitted");
	// -----------------------------

	if (_swap_chain.submitImage(_core.getDevice(), _synchronizer, &image_index) == false || window.resized()) {
		window.toggleFrameBufferResized(false);
		return _updatePresentation(window);
	}

	timer.update();
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

/**
 * @brief Update presentation objects to match window size.
*/
// TODO fix
void	Engine::_updatePresentation(Window& window) {
	(void)window;
	// _swap_chain.update(_core.getDevice(), window);

	// RenderPassInfo	rp_info {
	// 	.width = _swap_chain.getExtent().width,
	// 	.height = _swap_chain.getExtent().height,
	// 	.depth_format = _swap_chain.findDepthFormat(_core.getDevice()),
	// 	.color_format = _swap_chain.getImageFormat() };
	// TargetInfo	tar_info {
	// 	.swap_chain = &_swap_chain,
	// 	.render_pass = _pipelines.scene->getRenderPass() };

	// _pipelines.scene->getRenderPass()->updateResources(_core.getDevice(), rp_info);
	// _pipelines.scene->getTarget()->update(_core.getDevice(), tar_info);
}

// TODO Change
UniformBufferObject	Engine::_updateUbo(const vox::GameState& game) {
	UniformBufferObject	ubo{};

	const ::vox::Player& player = game.getPlayer();

	{	/* UPDATE CAMERA & CHUNK DATA ============================================= */
		// const constexpr float window_ratio =
			// _swap_chain.getExtent().width /
			// static_cast<float>(_swap_chain.getExtent().height);
		const Mat4	projection = ::scop::perspective(
			PLAYER_FOV,
			WINDOW_RATIO,
			CAMERA_ZNEAR,
			CAMERA_ZFAR);

		const Vect3 front = player.getEyeDir();
		const Vect3 right = scop::normalize(scop::cross(front, {VOX_UP_VECTOR}));
		const Vect3 up = scop::cross(right, front);
		const Mat4	view = ::scop::lookAt(
			player.getPosition(),
			front,
			up,
			right);

		ubo.camera.vp = projection * view;

		gfx::BoundingFrustum::Camera	camera {
			.position = player.getPosition(),
			.front = front,
			.right = right,
			.up = up };

		_input_handler.updateData(
			_core.getDevice(),
			camera,
			game.getWorld());
	}
	{	/* UPDATE LIGHT AND PROJECTOR ============================================== */
		constexpr const float	terrain_length = CHUNK_SIZE * RENDER_DISTANCE;
		constexpr const float	terrain_length_half = terrain_length / 2.0f;
		constexpr const float	light_distance = terrain_length_half;

		static const Mat4		projection = ::scop::orthographic(
			-terrain_length_half, terrain_length_half,
			-terrain_length_half, terrain_length_half,
			0.0f, terrain_length);

		float	period = game.getElapsedTime();
		Vect3	sun_pos = Vect3(cos(period), sin(period), 0.0f);

		const Mat4	view = ::scop::lookAt(
			sun_pos * light_distance + game.getWorld().getOrigin(),
			game.getWorld().getOrigin(),
			Vect3(VOX_UP_VECTOR));

		ubo.projector.vp = projection * view;
		ubo.light = {
			.ambient_color = ::scop::Vect3(0.17f, 0.15f, 0.1f),
			.light_vector = sun_pos,
			.light_color = ::scop::Vect3(1.0f, 1.0f, 0.8f) };
	}

	return ubo;
}

} // namespace scop