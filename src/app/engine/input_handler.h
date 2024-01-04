/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:15:01 by etran             #+#    #+#             */
/*   Updated: 2024/01/01 15:00:33 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <memory>

# include "bounding_frustum.h"
# include "chunk_macros.h"
# include "buffer.h"

namespace vox {
class GameState;
class World;
}

namespace scop::core {
class Device;
}

namespace scop::gfx {
class TextureHandler;
}

namespace scop {

enum class InputBufferSize: uint32_t {
	Frustum			= sizeof(gfx::BoundingFrustum),
	QuadCount		= sizeof(uint32_t) * MAX_RENDER_PYRAMID,
	VerticesData	= sizeof(uint32_t) * MAX_RENDER_PYRAMID * CHUNK_VOLUME * 6
	// TODO: Overkill, need to be optimized
};

enum class InputBufferOffset: uint32_t {
	Frustum			= 0,
	QuadCount		= (uint32_t)InputBufferSize::Frustum,
	VerticesData	= QuadCount + (uint32_t)InputBufferSize::QuadCount
};

/**
 * @brief Handle input for rendering pipelines.
*/
class InputHandler {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using TextureHandlerPtr = std::shared_ptr<gfx::TextureHandler>;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	InputHandler() = default;
	InputHandler(InputHandler&& x) = default;
	InputHandler&	operator=(InputHandler&& x) = default;
	~InputHandler() = default;

	InputHandler(const InputHandler& x) = delete;
	InputHandler&	operator=(const InputHandler& x) = delete;

	/* ========================================================================= */

	void				init(core::Device& device, const vox::GameState& game);
	void				destroy(core::Device& device);

	void				updateData(
		core::Device& device,
		const gfx::BoundingFrustum::Camera& camera,
		const vox::World& world);
	void				retrieveData();

	/* ========================================================================= */

	uint32_t			getVerticesCount() const noexcept;
	uint32_t			getInstancesCount() const noexcept;
	TextureHandlerPtr	getChunkMap() const;

	const gfx::Buffer&	getInputBuffer() const noexcept;
	gfx::Buffer&		getInputBuffer() noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	gfx::BoundingFrustum		_frustum;

	TextureHandlerPtr			_chunk_texture;
	gfx::Buffer					_input_buffer;

	static constexpr uint32_t	_vertices_count = 4;
	uint32_t					_instances_count = 0;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void				_createBuffers(core::Device& device);

}; // class InputHandler

} // namespace scop