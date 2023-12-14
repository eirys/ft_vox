/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:09:44 by etran             #+#    #+#             */
/*   Updated: 2023/12/06 23:20:43 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "window.h"
#include "uniform_buffer_object.h"
#include "timer.h"
#include "utils.h"
#include "world_macros.h"

// Vox
#include "chunk.h"
#include "game_state.h"

// Gfx
#include "texture_handler.h"
#include "target.h"
#include "descriptor_set.h"
#include "render_pass.h"
#include "pipeline_manager.h"
#include "chunk_texture_handler.h"

#include <cstring> // std::strcmp

using Vertex = ::vox::Vertex;
using Mat4 = ::scop::Mat4;
using Camera = ::scop::Camera;

namespace scop {

using gfx::ChunkTextureHandler;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	Engine::init(Window& window, const vox::GameState& game) {
	_core.init(window);
	_swap_chain.init(_core.getDevice(), window);
	_command_pool.init(_core.getDevice());

	_draw_buffer.init(_core.getDevice(), _command_pool, gfx::CommandBufferType::DRAW);
	_compute_buffer.init(_core.getDevice(), _command_pool, gfx::CommandBufferType::COMPUTE);

	_input_handler.init(_core.getDevice(), game);
	_pipeline_manager.init(_core.getDevice(), _swap_chain);

	_createDescriptors();
	_createSyncObjects();

	// TODO
	// _pipelines.scene->update(_updateUbo(game));
}

void	Engine::destroy() {
	_descriptor_pool.destroy(_core.getDevice());

	// Remove sync objects
	vkDestroySemaphore(
		_core.getDevice().getLogicalDevice(),
		_image_available_semaphores,
		nullptr);
	vkDestroySemaphore(
		_core.getDevice().getLogicalDevice(),
		_render_finished_semaphores,
		nullptr);
	vkDestroyFence(
		_core.getDevice().getLogicalDevice(),
		_in_flight_fences,
		nullptr);

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
	// Wait fence available, lock it
	vkWaitForFences(
		_core.getDevice().getLogicalDevice(),
		1, &_in_flight_fences,
		VK_TRUE,
		UINT64_MAX);

	// Next available image from swap chain
	uint32_t	image_index;
	VkResult	result = vkAcquireNextImageKHR(
		_core.getDevice().getLogicalDevice(),
		_swap_chain.getSwapChain(),
		UINT64_MAX,
		_image_available_semaphores,
		VK_NULL_HANDLE,
		&image_index);
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		_updatePresentation(window);
		return;
	} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image");
	}

	// Work is done, unlock fence
	vkResetFences(_core.getDevice().getLogicalDevice(), 1, &_in_flight_fences);

	// Record buffer
	_draw_buffer.reset();
	_draw_buffer.begin(0);
	_pipeline_manager.getCullingPipeline()->compute(
		_core.getDevice(),
		_pipeline_manager.getPipelineLayout(),
		_compute_buffer);
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

	// Submit compute and draw command buffers
	std::array<VkSemaphore, 1>			wait_semaphores = { _image_available_semaphores };
	std::array<VkSemaphore, 1>			signal_semaphores = { _render_finished_semaphores };
	std::array<VkPipelineStageFlags, 1>	wait_stages = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	VkSubmitInfo	submit_info{};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.waitSemaphoreCount = static_cast<uint32_t>(wait_semaphores.size());
	submit_info.pWaitSemaphores = wait_semaphores.data();
	submit_info.pWaitDstStageMask = wait_stages.data();
	submit_info.signalSemaphoreCount = static_cast<uint32_t>(signal_semaphores.size());
	submit_info.pSignalSemaphores = signal_semaphores.data();
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = reinterpret_cast<VkCommandBuffer*>(&_draw_buffer);

	if (vkQueueSubmit(_core.getDevice().getGraphicsQueue(), 1, &submit_info, _in_flight_fences) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer");
	}

	// Set presentation for next swap chain image
	std::array<VkSwapchainKHR, 1>	swap_chains = { _swap_chain.getSwapChain() };
	VkPresentInfoKHR				present_info{};
	present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present_info.waitSemaphoreCount = static_cast<uint32_t>(wait_semaphores.size());
	present_info.pWaitSemaphores = signal_semaphores.data();
	present_info.swapchainCount = static_cast<uint32_t>(swap_chains.size());
	present_info.pSwapchains = swap_chains.data();
	present_info.pImageIndices = &image_index;
	present_info.pResults = nullptr;

	// Submit to swap chain, check if swap chain is still compatible
	result = vkQueuePresentKHR(_core.getDevice().getPresentQueue(), &present_info);
	timer.update();

	if (
		result == VK_ERROR_OUT_OF_DATE_KHR ||
		result == VK_SUBOPTIMAL_KHR ||
		window.resized()
	) {
		window.toggleFrameBufferResized(false);
		_updatePresentation(window);
	} else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swapchain image");
	}
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void	Engine::_createDescriptors() {
	using DescriptorSetPtr = std::shared_ptr<scop::gfx::DescriptorSet>;

	std::vector<DescriptorSetPtr>	descriptors = {
		_pipeline_manager.getScenePipeline()->getDescriptor(),
		_pipeline_manager.getShadowsPipeline()->getDescriptor(),
		_pipeline_manager.getCullingPipeline()->getDescriptor() };

	_descriptor_pool.init(_core.getDevice(), descriptors);
	_pipeline_manager.plugDescriptors(_core.getDevice());
}

/**
 * @brief Create semaphores and fences
*/
void	Engine::_createSyncObjects() {
	VkSemaphoreCreateInfo	semaphore_info{};
	semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo	fence_info{};
	fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	if (vkCreateSemaphore(_core.getDevice().getLogicalDevice(), &semaphore_info, nullptr, &_image_available_semaphores) != VK_SUCCESS ||
		vkCreateSemaphore(_core.getDevice().getLogicalDevice(), &semaphore_info, nullptr, &_render_finished_semaphores) != VK_SUCCESS ||
		vkCreateSemaphore(_core.getDevice().getLogicalDevice(), &semaphore_info, nullptr, &_compute_finished_semaphores) != VK_SUCCESS ||
		vkCreateFence(_core.getDevice().getLogicalDevice(), &fence_info, nullptr, &_in_flight_fences) != VK_SUCCESS) {
		throw std::runtime_error("failed to create semaphore");
	}
}

/* ========================================================================== */

/**
 * @brief Update presentation objects to match window size.
*/
// TODO fix
void	Engine::_updatePresentation(Window& window) {
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
		_input_handler.updateVisibleChunks(
			_core.getDevice(),
			gfx::BoundingFrustum::Camera{player.getPosition(), front, right, up},
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