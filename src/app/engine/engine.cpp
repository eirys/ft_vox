/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:09:44 by etran             #+#    #+#             */
/*   Updated: 2023/09/01 16:13:48 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "window.h"
#include "uniform_buffer_object.h"
#include "descriptor_set.h"
#include "image_handler.h"
#include "timer.h"
#include "utils.h"

#include "game_state.h"
#include "scene_pipeline.h"
#include "shadows_pipeline.h"
#include "shadows_texture_handler.h"

#include <cstring> // std::strcmp

namespace scop::graphics {

// void Engine::TmpHandler::init(Device& device) {
// 	super::_texture_buffer.initImage(
// 			device, 1, 1, DEPTH_FORMAT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT|VK_IMAGE_USAGE_SAMPLED_BIT,VK_SAMPLE_COUNT_1_BIT);
// 	super::_texture_buffer.initView(
// 		device, DEPTH_FORMAT, VK_IMAGE_ASPECT_DEPTH_BIT
// 	);
// 		VkSamplerCreateInfo	sampler{};
// 	sampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
// 	sampler.magFilter = VK_FILTER_LINEAR;
// 	sampler.minFilter = VK_FILTER_LINEAR;
// 	sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
// 	sampler.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
// 	sampler.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
// 	sampler.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
// 	sampler.anisotropyEnable = VK_FALSE;
// 	sampler.maxAnisotropy = 1.0;
// 	sampler.compareEnable = VK_FALSE;
// 	sampler.compareOp = VK_COMPARE_OP_ALWAYS;
// 	sampler.mipLodBias = 0.0f;
// 	sampler.minLod = 0.0f;
// 	sampler.maxLod = 1.0f;
// 	sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

// 	if (vkCreateSampler(device.getLogicalDevice(), &sampler, nullptr, &(super::_texture_sampler)) != VK_SUCCESS) {
// 		throw std::runtime_error("failed to create texture sampler");
// 	}

// 	CommandBuffer	cmd = CommandPool::createBuffer(device);
// 	cmd.begin();

// 	VkImageSubresourceRange range{};

// 	range.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
// 	range.baseMipLevel = 0;
// 	range.baseArrayLayer = 0;
// 	range.layerCount = 1;
// 	range.levelCount = 1;

// 	_texture_buffer.setLayout(
// 		cmd,
// 		VK_IMAGE_LAYOUT_UNDEFINED,
// 		VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
// 		0,
// 		0,
// 		VK_PIPELINE_STAGE_HOST_BIT,
// 		VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
// 		range);

// 	cmd.end(device);
// 	CommandPool::destroyBuffer(device, cmd);
// }

// void Engine::TmpHandler::_createTextureSampler(Device& device) {

// }
// void Engine::TmpHandler::_createTextureImages(Device& device) {

// }
// void Engine::TmpHandler::_createTextureImageView(Device& device) {

// }

// Engine::TmpHandler	Engine::tmp;

const std::vector<const char*>	Engine::validation_layers = {
	"VK_LAYER_KHRONOS_validation"
};

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	Engine::init(
	::scop::Window& window,
	const ::vox::GameState& game,
	const std::vector<Vertex>& vertices,
	const std::vector<uint32_t>& indices
) {
	_pipelines.scene = std::make_shared<ScenePipeline>();
	_pipelines.shadows = std::make_shared<ShadowsPipeline>();

	_createInstance();
	_debug_module.init(_vk_instance);
	_device.init(window, _vk_instance);
	_swap_chain.init(_device, window);
	_command_pool.init(_device);
	_createGraphicsPipelines();
	_createDescriptors();
	_createGraphicsPipelineLayout();
	_assembleGraphicsPipelines();
	_input_handler.init(_device, _command_pool, vertices, indices);
	_draw_buffer.init(_device, _command_pool, max_frames_in_flight);
	_createSyncObjects();

	_initDescriptors(game);
}

void	Engine::destroy() {
	_swap_chain.destroy(_device);

	// Remove graphics _pipeline
	_pipelines.scene->destroy(_device);
	_pipelines.shadows->destroy(_device);

	vkDestroyPipelineLayout(
		_device.getLogicalDevice(),
		_pipeline_layout,
		nullptr);

	_descriptor_pool.destroy(_device);
	_input_handler.destroy(_device);

	// tmp.destroy(_device);

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
		image_index );
	_pipelines.scene->draw(
		_pipeline_layout,
		_draw_buffer,
		_input_handler,
		image_index );
	_draw_buffer.end(_device, false);

	_pipelines.scene->update(_updateUbo(game));

	// Set synchronization objects
	VkSemaphore				wait_semaphore[] = { _image_available_semaphores };
	VkSemaphore				signal_semaphore[] = { _render_finished_semaphores };
	VkPipelineStageFlags	wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSubmitInfo			submit_info{};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.waitSemaphoreCount = 1;
	submit_info.pWaitSemaphores = wait_semaphore;
	submit_info.pWaitDstStageMask = wait_stages;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = reinterpret_cast<VkCommandBuffer*>(&_draw_buffer);
	submit_info.signalSemaphoreCount = 1;
	submit_info.pSignalSemaphores = signal_semaphore;

	// Submit command buffer to be processed by graphics queue
	if (vkQueueSubmit(_device.getGraphicsQueue(), 1, &submit_info, _in_flight_fences) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer");
	}

	// Set presentation for next swap chain image
	VkSwapchainKHR	swap_chains[] = { _swap_chain.getSwapChain() };
	VkPresentInfoKHR	present_info{};
	present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present_info.waitSemaphoreCount = 1;
	present_info.pWaitSemaphores = signal_semaphore;
	present_info.swapchainCount = 1;
	present_info.pSwapchains = swap_chains;
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
	if (enable_validation_layers && !_checkValidationLayerSupport())
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
	if (enable_validation_layers) {
		create_info.enabledLayerCount = static_cast<uint32_t>(
			validation_layers.size()
		);
		create_info.ppEnabledLayerNames = validation_layers.data();
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
	rp_info.depth_format = VK_FORMAT_D16_UNORM;
	rp_info.depth_samples = VK_SAMPLE_COUNT_1_BIT;

	_pipelines.shadows->init(
		_device,
		rp_info,
		tar_info);
}

void	Engine::_createDescriptors() {
	// DescriptorPool::DescriptorSetPtr	scene_set = std::make_shared<DescriptorSet>();
	// DescriptorPool::DescriptorSetPtr	shadowmap_set = std::make_shared<DescriptorSet>();

	// tmp.init(_device);

	// _pipelines.scene->setDescriptor(scene_set);
	// _pipelines.shadows->setDescriptor(shadowmap_set);

	// _pipelines.scene->plugDescriptor(scene_set);
	// _pipelines.scene->plugDescriptor(shadowmap_set);
	// _pipelines.shadows->plugDescriptor(scene_set);

	// scene_set->init(_device);
	// shadowmap_set->init(_device);

	// const auto& ptr1 = _pipelines.scene->getDescriptor();
	// const auto& ptr2 = _pipelines.shadows->getDescriptor();
	// std::shared_ptr<SceneDescriptorSet>
	// auto shadows_descriptors =
		// std::dynamic_pointer_cast<ShadowsDescriptorSet>(_pipelines.shadows->getDescriptor());
	// std::shared_ptr<DescriptorSet>	scene_descriptor = _pipelines.scene->getDescriptor();
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
	scene_pipeline->plugDescriptor(_device, shadows_pipeline->getTextureHandler());
	shadows_pipeline->plugDescriptor(_device, scene_pipeline->getUbo());
}

void	Engine::_createGraphicsPipelineLayout() {
	// Pipeline layout setups
	VkPipelineLayoutCreateInfo	pipeline_layout_info{};
	pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipeline_layout_info.setLayoutCount = _descriptor_pool.getLayouts().size();
	pipeline_layout_info.pSetLayouts = _descriptor_pool.getLayouts().data();
	pipeline_layout_info.pushConstantRangeCount = 0;
	pipeline_layout_info.pPushConstantRanges = nullptr;

	if (vkCreatePipelineLayout(_device.getLogicalDevice(), &pipeline_layout_info, nullptr, &_pipeline_layout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create _pipeline layout");
	}
}

void	Engine::_assembleGraphicsPipelines() {
	/* INPUT FORMAT ============================================================ */
	VkPipelineVertexInputStateCreateInfo	vert_input{};
	auto	binding_description = scop::Vertex::getBindingDescription();

	vert_input.sType =
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vert_input.vertexBindingDescriptionCount = 1;
	vert_input.pVertexBindingDescriptions = &binding_description;

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
	rasterizing.depthBiasSlopeFactor = 0.0f;
	// rasterizing.cullMode

	/* MULTISAMPLING =========================================================== */
	VkPipelineMultisampleStateCreateInfo	multisampling{};
	multisampling.sType =
		VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.minSampleShading = 1.0f;
	multisampling.pSampleMask = nullptr;
	multisampling.alphaToCoverageEnable = VK_FALSE;
	multisampling.alphaToOneEnable = VK_FALSE;

	/* COLOR BLENDING ========================================================== */
	VkPipelineColorBlendStateCreateInfo	color_blending{};
	color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blending.logicOpEnable = VK_FALSE;
	color_blending.logicOp = VK_LOGIC_OP_COPY;
	color_blending.blendConstants[0] = 0.0f;
	color_blending.blendConstants[1] = 0.0f;
	color_blending.blendConstants[2] = 0.0f;
	color_blending.blendConstants[3] = 0.0f;

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

	/* DYNAMIC STATE =========================================================== */
	std::vector<VkDynamicState>	dynamic_states = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo	dynamic_state{};
	dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamic_state.dynamicStateCount = static_cast<uint32_t>(
		dynamic_states.size()
	);
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

	auto	attribute_descriptions = ::scop::Vertex::getSceneAttributeDescriptions();
	vert_input.vertexAttributeDescriptionCount =
		static_cast<uint32_t>(attribute_descriptions.size());
	vert_input.pVertexAttributeDescriptions = attribute_descriptions.data();
	color_blending.attachmentCount = 1;
	color_blending.pAttachments = &color_blend_attachment;
	rasterizing.cullMode = VK_CULL_MODE_BACK_BIT;
	multisampling.rasterizationSamples = _device.getMsaaSamples();
	depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS;

	_pipelines.scene->assemble(_device, pipeline_info);

	attribute_descriptions = ::scop::Vertex::getShadowAttributeDescriptions();
	vert_input.vertexAttributeDescriptionCount =
		static_cast<uint32_t>(attribute_descriptions.size());
	vert_input.pVertexAttributeDescriptions = attribute_descriptions.data();
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
	Target::TargetInfo	tar_info { .swap_chain = &_swap_chain };

	_pipelines.scene->getRenderPass()->updateResources(_device, rp_info);
	_pipelines.scene->getTarget()->update(_device, tar_info);
}

void	Engine::_initDescriptors(
	const ::vox::GameState& game
) noexcept {
	::scop::UniformBufferObject	ubo = _updateUbo(game);
	_pipelines.scene->update(ubo);
}

::scop::UniformBufferObject	Engine::_updateUbo(
	const ::vox::GameState& game
) const noexcept {
	::scop::UniformBufferObject	ubo{};

	float ratio =
		_swap_chain.getExtent().width /
		static_cast<float>(_swap_chain.getExtent().height);
	::scop::Mat4	view = ::scop::lookAtDir(
		game.getPlayer().getPosition(),
		game.getPlayer().getEyeDir(),
		::scop::Vect3(0.0f, 1.0f, 0.0f));
	::scop::Mat4	projection = ::scop::perspective(
		::scop::math::radians(70.0f),
		ratio,
		0.1f,
		1000.0f);
	ubo.camera.vp = projection * view;
	ubo.light = {
		.ambient_color = ::scop::Vect3(0.2f, 0.2f, 0.2f),
		.light_vector = ::scop::normalize(::scop::Vect3(0.1f, 1.0f, 0.3f)),
		.light_color = ::scop::Vect3(1.0f, 1.0f, 0.8f) };
		// .light_intensity = 0.4f };

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

	for (const char* layer_name: validation_layers) {
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

	if (enable_validation_layers) {
		extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	return extensions;
}

} // namespace scop::graphics