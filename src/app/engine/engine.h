/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:35 by etran             #+#    #+#             */
/*   Updated: 2023/10/25 20:59:58 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

# include "debug_module.h"
# include "device.h"
# include "swap_chain.h"
# include "texture_handler.h"
# include "pipeline.h"

# include "descriptor_pool.h"
# include "command_pool.h"
# include "command_buffer.h"

# include "input_handler.h"

namespace vox {
class GameState;
}

namespace scop {
class Timer;
struct Camera;
}

namespace scop::graphics {

class Engine {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using Texture = TextureHandler::Texture;
	using PipelinePtr = std::shared_ptr<Pipeline>;
	using TextureHandlerPtr = std::shared_ptr<TextureHandler>;

	using UniformBufferObject = ::scop::UniformBufferObject;
	using GameState = ::vox::GameState;
	using Window = ::scop::Window;

	/* ========================================================================= */
	/*                               CONST MEMBERS                               */
	/* ========================================================================= */

	static constexpr std::size_t	max_frames_in_flight = 1;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Engine() = default;
	~Engine() = default;

	Engine(Engine&& other) = delete;
	Engine(const Engine& other) = delete;
	Engine& operator=(Engine&& other) = delete;
	Engine& operator=(const Engine& other) = delete;

	/* ========================================================================= */

	void						init(Window& window, const GameState& game);
	void						destroy();

	void						idle();
	void						render(
		Window& window,
		const vox::GameState& game,
		Timer& timer);

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	// Core
	VkInstance					_vk_instance;
	DebugModule					_debug_module;

	Device						_device;
	SwapChain					_swap_chain;
	DescriptorPool				_descriptor_pool;
	CommandPool					_command_pool;
	CommandBuffer				_draw_buffer;
	VkSemaphore					_image_available_semaphores;
	VkSemaphore					_render_finished_semaphores;
	VkFence						_in_flight_fences;

	VkPipelineLayout			_pipeline_layout;
	struct {
		PipelinePtr				scene;
		PipelinePtr				shadows;
	}							_pipelines;

	InputHandler				_input_handler;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void						_createInstance();
	void						_createGraphicsPipelines();
	void						_assembleGraphicsPipelines();
	void						_createGraphicsPipelineLayout();
	void						_createSyncObjects();
	void						_createDescriptors();

	UniformBufferObject			_updateUbo(const GameState& game);
	void						_updatePresentation(Window& window);

	bool						_checkValidationLayerSupport();
	std::vector<const char*>	_getRequiredExtensions();

}; // class Engine

} // namespace scop::graphics