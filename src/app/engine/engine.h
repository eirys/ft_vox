/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:35 by etran             #+#    #+#             */
/*   Updated: 2023/12/28 22:21:43 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

// # include "debug_module.h"
// # include "device.h"
# include "core.h"
# include "swap_chain.h"
// # include "texture_handler.h"
// # include "graphics_pipeline.h"

# include "descriptor_pool.h"
# include "command_pool.h"
# include "command_buffer.h"
# include "pipeline_manager.h"

# include "input_handler.h"

namespace vox {
class GameState;
}

namespace scop::gfx {
class GraphicsPipeline;
class ComputePipeline;
}
namespace scop {

class Timer;
struct UniformBufferObject;

class Engine {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using TextureHandlerPtr = std::shared_ptr<gfx::TextureHandler>;
	using UniformBufferObject = scop::UniformBufferObject;

	/* ========================================================================= */
	/*                               CONST MEMBERS                               */
	/* ========================================================================= */

	static constexpr const std::size_t	max_frames_in_flight = 1;

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

	void						init(Window& window, const vox::GameState& game);
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

	core::Core					_core;

	SwapChain					_swap_chain;
	InputHandler				_input_handler;

	gfx::CommandBuffer			_draw_buffer;
	gfx::CommandBuffer			_compute_buffer;
	gfx::CommandPool			_command_pool;
	gfx::DescriptorPool			_descriptor_pool;

	gfx::PipelineManager		_pipeline_manager;

	VkSemaphore					_image_available_semaphores;
	VkSemaphore					_render_finished_semaphores;
	VkSemaphore					_compute_finished_semaphores;
	VkSemaphore					_graphics_ready_semaphores;
	VkFence						_in_flight_fences;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void						_createSyncObjects();
	void						_createDescriptors();

	UniformBufferObject			_updateUbo(const vox::GameState& game) const;
	void						_updatePresentation(Window& window);

}; // class Engine

} // namespace scop