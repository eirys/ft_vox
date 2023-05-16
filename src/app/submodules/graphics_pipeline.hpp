/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_pipeline.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:27:44 by etran             #+#    #+#             */
/*   Updated: 2023/05/16 14:25:03 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPHICS_PIPELINE_HPP
# define GRAPHICS_PIPELINE_HPP

// Graphics
# ifndef GLFW_INCLUDE_VULKAN
#  define GLFW_INCLUDE_VULKAN
# endif

# include <GLFW/glfw3.h>

// Std
# include <optional>	// std::optional
# include <vector>		// std::vector

# include "vector.hpp"
# include "image_handler.hpp"

# include "device.hpp"
# include "render_target.hpp"
# include "texture_sampler.hpp"
# include "descriptor_set.hpp"
# include "command_buffer.hpp"
# include "vertex_input.hpp"

namespace scop {
namespace graphics {

struct QueueFamilyIndices {
	std::optional<uint32_t>	graphics_family;
	std::optional<uint32_t>	present_family;

	bool	isComplete() {
		return graphics_family.has_value() && present_family.has_value();
	}
};

class GraphicsPipeline {
public:
	/* ========================================================================= */
	/*                               CONST MEMBERS                               */
	/* ========================================================================= */

	static constexpr size_t			max_frames_in_flight = 1;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	GraphicsPipeline() = default;
	GraphicsPipeline(GraphicsPipeline&& other) = default;
	~GraphicsPipeline() = default;

	GraphicsPipeline(const GraphicsPipeline& other) = delete;
	GraphicsPipeline& operator=(const GraphicsPipeline& other) = delete;

	/* ========================================================================= */

	void						init(
		Window& window, 
		const scop::Image& image,
		const std::vector<Vertex>& vertices,
		const std::vector<uint32_t>& indices
	);
	void						destroy();

	void						idle();
	void						render(
		Window& window,
		size_t indices_size
	);

	void						recordCommandBuffer(
		size_t indices_size,
		VkCommandBuffer command_buffer,
		uint32_t image_index
	);

private:
	/* ========================================================================= */
	/*                               HELPER OBJECTS                              */
	/* ========================================================================= */

	struct PushConstantData {
		scop::Vect2		offset;
		scop::Vect3		color;
	};

	/* ========================================================================= */
	/*                               CONST MEMBERS                               */
	/* ========================================================================= */

	const std::vector<const char*>	validation_layers = {
		"VK_LAYER_KHRONOS_validation"
	};
	static constexpr char*			vertex_shader_bin = "shaders/vert.spv";
	static constexpr char*			fragment_shader_bin = "shaders/frag.spv";

	#ifndef NDEBUG
	static constexpr bool			enable_validation_layers = false;
	#else
	static constexpr bool			enable_validation_layers = true;
	#endif

	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkInstance						vk_instance;
	VkDebugUtilsMessengerEXT		debug_messenger;

	Device							device;
	RenderTarget					render_target;
	TextureSampler					texture_sampler;
	DescriptorSet					descriptor_set;
	CommandBuffer					command_buffer;
	VertexInput						vertex_input;

	VkSemaphore						image_available_semaphores;
	VkSemaphore						render_finished_semaphores;
	VkFence							in_flight_fences;

	VkPipelineLayout				pipeline_layout;
	VkPipeline						graphics_pipeline;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	/* INIT ==================================================================== */

	void							createInstance();
	void							setupDebugMessenger();
	void							createGraphicsPipeline();
	void							createSyncObjects();

	/* UTILS =================================================================== */

	bool							checkValidationLayerSupport();
	std::vector<const char*>		getRequiredExtensions();
	void							populateDebugMessengerCreateInfo(
		VkDebugUtilsMessengerCreateInfoEXT& create_info
	);

	VkShaderModule					createShaderModule(
		const std::vector<char>& code
	);

}; // class GraphicsPipeline

/* ========================================================================== */
/*                                    UTILS                                   */
/* ========================================================================== */

inline VkResult	CreateDebugUtilsMessengerEXT(
	VkInstance instance,
	const VkDebugUtilsMessengerCreateInfoEXT* p_create_info,
	const VkAllocationCallbacks* p_allocator,
	VkDebugUtilsMessengerEXT* p_debug_messenger
);

inline void	DestroyDebugUtilsMessengerEXT(
	VkInstance instance,
	VkDebugUtilsMessengerEXT debug_messenger,
	const VkAllocationCallbacks* p_allocator
);

inline VKAPI_ATTR	VkBool32 VKAPI_CALL	debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
	VkDebugUtilsMessageTypeFlagsEXT message_type,
	const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
	void* p_user_data
);

inline VkCommandBuffer	beginSingleTimeCommands(
	VkDevice device,
	VkCommandPool command_pool
);

inline void	endSingleTimeCommands(
	VkDevice device,
	VkQueue queue,
	VkCommandPool command_pool,
	VkCommandBuffer command_buffer
);

inline VkImageView	createImageView(
	VkDevice logical_device,
	VkImage image,
	VkFormat format,
	VkImageAspectFlags aspect_flags,
	uint32_t mip_level
);

inline void	copyBuffer(
	VkDevice device,
	VkQueue queue,
	VkCommandPool command_pool,
	VkBuffer src_buffer,
	VkBuffer dst_buffer,
	VkDeviceSize size
);

inline void	copyBufferToImage(
	VkDevice device,
	VkQueue queue,
	VkCommandPool command_pool,
	VkBuffer buffer,
	VkImage image,
	uint32_t width,
	uint32_t height
);

} // namespace graphics
} // namespace scop

#endif