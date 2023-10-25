/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:09:44 by etran             #+#    #+#             */
/*   Updated: 2023/10/06 12:40:09 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "window.h"
#include "uniform_buffer_object.h"
#include "descriptor_set.h"
#include "image_handler.h"
#include "timer.h"
#include "utils.h"

#include "chunk.h"
#include "mesh.h"
#include "game_state.h"
#include "scene_pipeline.h"
#include "shadows_pipeline.h"
#include "shadows_texture_handler.h"

#include <chrono>

#include <cstring> // std::strcmp

using UniformBufferObject = ::scop::UniformBufferObject;
using GameState = ::vox::GameState;
using Window = ::scop::Window;
using Chunk = ::vox::Chunk;
using Mesh = ::vox::Mesh;

using Vertex = ::vox::Vertex;
using Mat4 = ::scop::Mat4;
using Camera = ::scop::Camera;

namespace scop::graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	Engine::init(::scop::Window& window, const GameState& game) {
	_pipelines.scene = std::make_shared<ScenePipeline>();
	_pipelines.shadows = std::make_shared<ShadowsPipeline>();

	_createInstance();
	_debug_module.init(_vk_instance);
	_device.init(window, _vk_instance);
	_swap_chain.init(_device, window);
	_command_pool.init(_device);

	_input_handler.init(_device, game);

	_createGraphicsPipelines();
	_createGraphicsPipelineLayout();
	_assembleGraphicsPipelines();
	_createDescriptors();

	_draw_buffer.init(_device, _command_pool, max_frames_in_flight);
	_createSyncObjects();

	_pipelines.scene->update(_updateUbo(game));
}

void	Engine::destroy() {
	_swap_chain.destroy(_device);
	_pipelines.scene->destroy(_device);
	_pipelines.shadows->destroy(_device);

	_input_handler.destroy(_device);

	vkDestroyPipelineLayout(
		_device.getLogicalDevice(),
		_pipeline_layout,
		nullptr);

	_descriptor_pool.destroy(_device);

	// Remove sync objects
	vkDestroySemaphore(
		_device.getLogicalDevice(),
		_image_available_semaphores,
		nullptr);
	vkDestroySemaphore(
		_device.getLogicalDevice(),
		_render_finished_semaphores,
		nullptr);
	vkDestroyFence(
		_device.getLogicalDevice(),
		_in_flight_fences,
		nullptr);

	_command_pool.destroy(_device);
	_device.destroy(_vk_instance);
	_debug_module.destroy(_vk_instance);

	// Remove instance
	vkDestroyInstance(_vk_instance, nullptr);
}

/* ========================================================================== */

void	Engine::idle() {
	_device.idle();
}

void	Engine::render(
	::scop::Window& window,
	const vox::GameState& game,
	Timer& timer
) {
	// Wait fence available, lock it
	vkWaitForFences(
		_device.getLogicalDevice(),
		1, &_in_flight_fences,
		VK_TRUE,
		UINT64_MAX
	);

	// Next available image from swap chain
	uint32_t	image_index;
	VkResult	result = vkAcquireNextImageKHR(
		_device.getLogicalDevice(),
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
	vkResetFences(_device.getLogicalDevice(), 1, &_in_flight_fences);

	// Record buffer
	_draw_buffer.reset();
	_draw_buffer.begin(0);
	_pipelines.shadows->draw(
		_pipeline_layout,
		_draw_buffer,
		_input_handler,
		image_index);
	_pipelines.scene->draw(
		_pipeline_layout,
		_draw_buffer,
		_input_handler,
		image_index);
	_draw_buffer.end(_device, false);

	_pipelines.scene->update(_updateUbo(game));

	// Set synchronization objects
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

	// Submit command buffer to be processed by graphics queue
	if (vkQueueSubmit(_device.getGraphicsQueue(), 1, &submit_info, _in_flight_fences) != VK_SUCCESS) {
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
	result = vkQueuePresentKHR(_device.getPresentQueue(), &present_info);
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

/**
 * Create a Vulkan instance
*/
void	Engine::_createInstance() {
	// Check if validation layers are available
	if (DebugModule::enable_validation_layers && !_checkValidationLayerSupport())
		throw std::runtime_error("validation layers not available");

	// Provides information to driver
	VkApplicationInfo	app_info{};

	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = "Ft_Vox";
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "No engine";
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;

	// Pass those informations to the Vulkan driver
	VkInstanceCreateInfo	create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &app_info;

	std::vector<const char*>	extensions = _getRequiredExtensions();
	create_info.enabledExtensionCount = static_cast<uint32_t>(
		extensions.size()
	);
	create_info.ppEnabledExtensionNames = extensions.data();

	// Setup debug messenger to go along with the instance
	VkDebugUtilsMessengerCreateInfoEXT	debug_create_info{};
	if (DebugModule::enable_validation_layers) {
		create_info.enabledLayerCount = static_cast<uint32_t>(
			DebugModule::validation_layers.size()
		);
		create_info.ppEnabledLayerNames = DebugModule::validation_layers.data();
		_debug_module.populate(debug_create_info);
		create_info.pNext =
			(VkDebugUtilsMessengerCreateInfoEXT*)&debug_create_info;
	} else {
		create_info.enabledLayerCount = 0;
		create_info.pNext = nullptr;
	}

	// Create the instance
	if (vkCreateInstance(&create_info, nullptr, &_vk_instance) != VK_SUCCESS)
		throw std::runtime_error("failed to create _vk_instance");
}

/**
 * @brief Create generic pipeline createinfo
*/
void	Engine::_createGraphicsPipelines() {
	RenderPass::RenderPassInfo	rp_info {
		.width = _swap_chain.getExtent().width,
		.height = _swap_chain.getExtent().height,
		.depth_format = _swap_chain.findDepthFormat(_device),
		.depth_samples = _device.getMsaaSamples(),
		.color_format = _swap_chain.getImageFormat(),
		.color_samples = _device.getMsaaSamples() };
	Target::TargetInfo	tar_info { .swap_chain = &_swap_chain };

	_pipelines.scene->init(
		_device,
		rp_info,
		tar_info);

	rp_info.width = SHADOWMAP_SIZE;
	rp_info.height = SHADOWMAP_SIZE;
	rp_info.depth_format = DEPTH_FORMAT;
	rp_info.depth_samples = VK_SAMPLE_COUNT_1_BIT;

	_pipelines.shadows->init(
		_device,
		rp_info,
		tar_info);
}

void	Engine::_createDescriptors() {
	using DescriptorSetPtr = std::shared_ptr<DescriptorSet>;
	using ScenePipelinePtr = std::shared_ptr<ScenePipeline>;
	using ShadowsPipelinePtr = std::shared_ptr<ShadowsPipeline>;

	std::vector<DescriptorSetPtr>	descriptors = {
		_pipelines.scene->getDescriptor(),
		_pipelines.shadows->getDescriptor() };
	_descriptor_pool.init(_device, descriptors);

	ScenePipelinePtr scene_pipeline =
		std::dynamic_pointer_cast<ScenePipeline>(_pipelines.scene);
	ShadowsPipelinePtr shadows_pipeline =
		std::dynamic_pointer_cast<ShadowsPipeline>(_pipelines.shadows);

	scene_pipeline->plugDescriptor(
		_device,
		shadows_pipeline->getTextureHandler(),
		_input_handler);
	shadows_pipeline->plugDescriptor(
		_device,
		scene_pipeline->getUbo(),
		_input_handler);
}

void	Engine::_createGraphicsPipelineLayout() {
	std::vector<VkDescriptorSetLayout>	layouts = {
		_pipelines.scene->getDescriptor()->getLayout(),
		_pipelines.shadows->getDescriptor()->getLayout()
	};
	VkPipelineLayoutCreateInfo	pipeline_layout_info{};
	pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipeline_layout_info.setLayoutCount = static_cast<uint32_t>(layouts.size());
	pipeline_layout_info.pSetLayouts = layouts.data();
	pipeline_layout_info.pushConstantRangeCount = 0;
	pipeline_layout_info.pPushConstantRanges = nullptr;

	if (vkCreatePipelineLayout(_device.getLogicalDevice(), &pipeline_layout_info, nullptr, &_pipeline_layout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create _pipeline layout");
	}
}

void	Engine::_assembleGraphicsPipelines() {
	/* INPUT FORMAT ============================================================ */
	VkPipelineVertexInputStateCreateInfo	vert_input{};

	vert_input.sType =
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	vert_input.vertexBindingDescriptionCount = 0;
	vert_input.pVertexBindingDescriptions = nullptr;
	vert_input.vertexAttributeDescriptionCount = 0;
	vert_input.pVertexAttributeDescriptions = nullptr;

	/* INPUT ASSEMBLY ========================================================== */
	VkPipelineInputAssemblyStateCreateInfo	input_assembly{};
	input_assembly.sType =
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	input_assembly.primitiveRestartEnable = VK_FALSE;

	/* VIEWPORT ================================================================ */
	VkPipelineViewportStateCreateInfo	viewport_state{};
	viewport_state.sType =
		VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_state.viewportCount = 1;
	viewport_state.scissorCount = 1;

	/* RASTERIZER ============================================================== */
	VkPipelineRasterizationStateCreateInfo	rasterizing{};
	rasterizing.sType =
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizing.depthClampEnable = VK_FALSE;
	rasterizing.rasterizerDiscardEnable = VK_FALSE;
	rasterizing.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizing.lineWidth = 1.0f;
	rasterizing.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizing.depthBiasEnable = VK_FALSE;
	rasterizing.depthBiasConstantFactor = 0.0f;
	rasterizing.depthBiasClamp = 0.0f;
	rasterizing.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizing.depthBiasSlopeFactor = 0.0f;

	/* MULTISAMPLING =========================================================== */
	VkPipelineMultisampleStateCreateInfo	multisampling{};
	multisampling.sType =
		VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.minSampleShading = 1.0f;
	multisampling.pSampleMask = nullptr;
	multisampling.alphaToCoverageEnable = VK_FALSE;
	multisampling.alphaToOneEnable = VK_FALSE;
	multisampling.rasterizationSamples = _device.getMsaaSamples();

	/* COLOR BLENDING ========================================================== */
	VkPipelineColorBlendAttachmentState	color_blend_attachment{};
	color_blend_attachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;
	color_blend_attachment.blendEnable = VK_FALSE;
	color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
	color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo	color_blending{};
	color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blending.logicOpEnable = VK_FALSE;
	color_blending.logicOp = VK_LOGIC_OP_COPY;
	color_blending.blendConstants[0] = 0.0f;
	color_blending.blendConstants[1] = 0.0f;
	color_blending.blendConstants[2] = 0.0f;
	color_blending.blendConstants[3] = 0.0f;
	color_blending.attachmentCount = 1;
	color_blending.pAttachments = &color_blend_attachment;

	/* DEPTH STENCIL =========================================================== */
	VkPipelineDepthStencilStateCreateInfo	depth_stencil{};
	depth_stencil.sType =
		VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depth_stencil.depthTestEnable = VK_TRUE;			// fragment depth compared to depth buffer enabled
	depth_stencil.depthWriteEnable = VK_TRUE;			// if test passed, new depth saved in buffer enabled
	depth_stencil.depthBoundsTestEnable = VK_FALSE;		// unused. specifies min/max depth bounds
	depth_stencil.minDepthBounds = 0.0f;
	depth_stencil.maxDepthBounds = 1.0f;
	depth_stencil.stencilTestEnable = VK_FALSE;			// unused. typically used for reflection, shadow...
	depth_stencil.front = {};
	depth_stencil.back = {};
	depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS;

	/* DYNAMIC STATE =========================================================== */
	std::array<VkDynamicState, 2>	dynamic_states = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR };

	VkPipelineDynamicStateCreateInfo	dynamic_state{};
	dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamic_state.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
	dynamic_state.pDynamicStates = dynamic_states.data();

	/* PIPELINE ================================================================ */
	VkGraphicsPipelineCreateInfo	pipeline_info{};
	pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_info.pVertexInputState = &vert_input;
	pipeline_info.pInputAssemblyState = &input_assembly;
	pipeline_info.pViewportState = &viewport_state;
	pipeline_info.pRasterizationState = &rasterizing;
	pipeline_info.pMultisampleState = &multisampling;
	pipeline_info.pDepthStencilState = &depth_stencil;
	pipeline_info.pColorBlendState = &color_blending;
	pipeline_info.pDynamicState = &dynamic_state;
	pipeline_info.layout = _pipeline_layout;
	pipeline_info.subpass = 0;
	pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
	pipeline_info.basePipelineIndex = -1;

	_pipelines.scene->assemble(_device, pipeline_info);

	color_blending.attachmentCount = 0;
	rasterizing.cullMode = VK_CULL_MODE_NONE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;

	_pipelines.shadows->assemble(_device, pipeline_info);
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

	if (vkCreateSemaphore(_device.getLogicalDevice(), &semaphore_info, nullptr, &_image_available_semaphores) != VK_SUCCESS ||
		vkCreateSemaphore(_device.getLogicalDevice(), &semaphore_info, nullptr, &_render_finished_semaphores) != VK_SUCCESS ||
		vkCreateFence(_device.getLogicalDevice(), &fence_info, nullptr, &_in_flight_fences) != VK_SUCCESS) {
		throw std::runtime_error("failed to create semaphore");
	}
}

/* ========================================================================== */

/**
 * @brief Update presentation objects to match window size.
*/
void	Engine::_updatePresentation(::scop::Window& window) {
	_swap_chain.update(_device, window);

	RenderPass::RenderPassInfo	rp_info {
		.width = _swap_chain.getExtent().width,
		.height = _swap_chain.getExtent().height,
		.depth_format = _swap_chain.findDepthFormat(_device),
		.color_format = _swap_chain.getImageFormat() };
	Target::TargetInfo	tar_info {
		.swap_chain = &_swap_chain,
		.render_pass = _pipelines.scene->getRenderPass() };

	_pipelines.scene->getRenderPass()->updateResources(_device, rp_info);
	_pipelines.scene->getTarget()->update(_device, tar_info);
}

UniformBufferObject	Engine::_updateUbo(const GameState& game) {
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
		const Vect3 right = scop::normalize(scop::cross(front, {0.0f, 1.0f, 0.0f}));
		const Vect3 up = scop::cross(right, front);
		const Mat4	view = ::scop::lookAt(
			player.getPosition(),
			front,
			right,
			up);

		ubo.camera.vp = projection * view;
		ubo.chunks = _input_handler.updateVisibleChunks(
			BoundingFrustum::Camera{player.getPosition(), front, right, up},
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
		Vect3	light = Vect3(cos(period), sin(period), 0.0f);

		const Mat4	view = ::scop::lookAt(
			light * light_distance + game.getWorld().getOrigin(),
			game.getWorld().getOrigin(),
			Vect3(0.0f, 1.0f, 0.0f));

		ubo.projector.vp = projection * view;
		ubo.light = {
			.ambient_color = ::scop::Vect3(0.17f, 0.15f, 0.1f),
			.light_vector = light,
			.light_color = ::scop::Vect3(1.0f, 1.0f, 0.8f) };
	}

	return ubo;
}

/* ========================================================================== */

/**
 * Check if all required extensions are available for validation layers
*/
bool	Engine::_checkValidationLayerSupport() {
	uint32_t	layer_count;
	vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

	std::vector<VkLayerProperties>	available_layers(layer_count);
	vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

	for (const char* layer_name: DebugModule::validation_layers) {
		bool	found = false;
		for (const VkLayerProperties& layer_properties: available_layers) {
			if (!strcmp(layer_name, layer_properties.layerName)) {
				found = true;
				break;
			}
		}
		if (!found)
			return false;
	}
	return true;
}

/**
 *  Retrieve list of extensions if validation layers enabled
 */
std::vector<const char*>	Engine::_getRequiredExtensions() {
	uint32_t		glfw_extension_count = 0;
	const char**	glfw_extensions;
	glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

	std::vector<const char*>	extensions(
		glfw_extensions,
		glfw_extensions + glfw_extension_count
	);

	if (DebugModule::enable_validation_layers) {
		extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	return extensions;
}

} // namespace scop::graphics