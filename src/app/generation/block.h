/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 16:08:29 by etran             #+#    #+#             */
/*   Updated: 2024/01/08 18:54:12 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <cstdint>

# include "block_properties.h"

namespace vox {

class Block {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Block(const MaterialType type) noexcept;

	Block() = default;
	Block(Block &&src) = default;
	Block &operator=(Block &&rhs) = default;
	~Block() = default;

	Block(const Block &src) = delete;
	Block &operator=(const Block &rhs) = delete;

	/* ========================================================================= */

	void				setType(const MaterialType block_type) noexcept;
	void				setGFXProperty(const MaterialGFXProperty property) noexcept;
	void				setStateProperty(const MaterialState property) noexcept;
	void				setInteractionProperty(const MaterialInteraction property) noexcept;
	void				setAspectProperty(const MaterialAspect property) noexcept;

	MaterialType		getType() const noexcept;
	MaterialGFXProperty	getGFXProperty() const noexcept;
	MaterialState		getStateProperty() const noexcept;
	MaterialInteraction	getInteractionProperty() const noexcept;
	MaterialAspect		getAspectProperty() const noexcept;

	uint16_t			computePackedData() const noexcept;

	/* GAMEPLAY ================================================================ */

	bool				isEmpty() const noexcept;
	bool				isSolid() const noexcept;
	bool				isFluid() const noexcept;
	bool				isDirected() const noexcept;
	// TODO

	/* GFX ===================================================================== */

	bool				isTransparent() const noexcept;
	bool				isSemiTransparent() const noexcept;
	bool				isLightSource() const noexcept;
	// TODO

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	MaterialType		_type			= MaterialType::AIR;
	MaterialGFXProperty	_gfx			= MaterialGFXProperty::OPAQUE;
	MaterialState		_state			= MaterialState::SOLID;
	MaterialInteraction	_interaction	= MaterialInteraction::INTANGIBLE;
	MaterialAspect		_aspect			= MaterialAspect::REGULAR;

}; // class Block

} // namespace vox