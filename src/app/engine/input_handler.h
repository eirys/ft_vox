/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:15:01 by etran             #+#    #+#             */
/*   Updated: 2023/12/06 23:19:00 by etran            ###   ########.fr       */
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

	void				updateVisibleChunks(
		core::Device& device,
		const gfx::BoundingFrustum::Camera& camera,
		const vox::World& world);

	/* ========================================================================= */

	uint32_t				getVerticesCount() const noexcept;
	uint32_t				getInstancesCount() const noexcept;
	TextureHandlerPtr		getHeightMap() const noexcept;
	TextureHandlerPtr		getChunkMap() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	TextureHandlerPtr		_chunk_texture;

	gfx::BoundingFrustum	_frustum;
	uint32_t				_instances_count = 25;
	uint32_t				_vertices_count = CHUNK_AREA * 36;

}; // class InputHandler

} // namespace scop