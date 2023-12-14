/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 16:08:29 by etran             #+#    #+#             */
/*   Updated: 2023/12/14 11:21:08 by etran            ###   ########.fr       */
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

	Block(MaterialType type) noexcept;

	Block() = default;
	Block(Block &&src) = default;
	Block &operator=(Block &&rhs) = default;
	~Block() = default;

	Block(const Block &src) = delete;
	Block &operator=(const Block &rhs) = delete;

	/* ========================================================================= */

	void				setType(MaterialType block_type) noexcept;
	void				setProperies(MaterialProperty block_type) noexcept;

	MaterialType		getType() const noexcept;
	MaterialProperty	getProperties() const noexcept;

	/* GAMEPLAY ================================================================ */

	bool				isEmpty() const noexcept;
	bool				isSolid() const noexcept;
	bool				isLiquid() const noexcept;
	bool				isDirected() const noexcept;

	/* GFX ===================================================================== */

	bool				isTransparent() const noexcept;
	bool				isSemiTransparent() const noexcept;
	bool				isLightSource() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	MaterialType		_type		= MaterialType::AIR;
	MaterialProperty	_properties	= = MaterialProperty::Default;

}; // class Block

} // namespace vox