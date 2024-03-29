/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:15:01 by etran             #+#    #+#             */
/*   Updated: 2023/11/16 22:54:06 by etran            ###   ########.fr       */
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

	uint32_t			getVerticesCount() const noexcept;
	uint32_t			getInstancesCount() const noexcept;
	TextureHandlerPtr	getHeightMap() const noexcept;
	TextureHandlerPtr	getChunkMap() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	TextureHandlerPtr		_height_map;
	TextureHandlerPtr		_chunk_map;

	gfx::BoundingFrustum	_frustum;
	uint32_t				_instances_count = 25;
	uint32_t				_vertices_count = CHUNK_AREA * 36;

}; // class InputHandler

} // namespace scop