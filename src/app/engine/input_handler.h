/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:15:01 by etran             #+#    #+#             */
/*   Updated: 2023/12/24 15:31:14 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <memory>

# include "bounding_frustum.h"
# include "chunk_macros.h"

namespace vox {
class GameState;
class World;
}

namespace scop::core {
class Device;
}

namespace scop::gfx {
class TextureHandler;
class Buffer;
}

namespace scop {

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

	// void				updateVisibleChunks(
	// 	core::Device& device,
	// 	const gfx::BoundingFrustum::Camera& camera,
	// 	const vox::World& world);

	/* ========================================================================= */

	uint32_t			getVerticesCount() const noexcept;
	uint32_t			getInstancesCount() const noexcept;
	TextureHandlerPtr	getChunkMap() const;

	const gfx::Buffer&	getFrustumBuffer() const noexcept;
	gfx::Buffer&		getFrustumBuffer() noexcept;
	const gfx::Buffer&	getQuadCountBuffer() const noexcept;
	gfx::Buffer&		getQuadCountBuffer() noexcept;
	const gfx::Buffer&	getVerticesDataBuffer() const noexcept;
	gfx::Buffer&		getVerticesDataBuffer() noexcept;


private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	gfx::BoundingFrustum		_frustum;

	TextureHandlerPtr			_chunk_texture;
	// TODO put into 1 buffer
	gfx::Buffer					_frustum_buffer; // TODO map
	gfx::Buffer					_quad_count_buffer; // TODO map
	gfx::Buffer					_vertices_data_buffer;

	static constexpr uint32_t	_vertices_count = 4;
	uint32_t					_instances_count = 6 * CHUNK_VOLUME * RENDER_DISTANCE;

	// uint32_t				_instances_count = 25;
	// uint32_t				_vertices_count = CHUNK_AREA * 36;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void				_createBuffers(core::Device& device);

}; // class InputHandler

} // namespace scop