/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 16:08:29 by etran             #+#    #+#             */
/*   Updated: 2024/01/20 14:15:54 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <cstdint>

# include "block_data.h"
// # include "block_properties.h"
// # include "block_types.h"

#ifdef __DEBUG
# include <ostream>
#endif

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
	MaterialType		getType() const noexcept;

	uint16_t			computePackedData() const noexcept;
	static Block		computeFromPackedData(const uint16_t packed_data);

	/* GAMEPLAY ================================================================ */

	bool				isEmpty() const noexcept;
	bool				isSolid() const noexcept;
	bool				isFluid() const noexcept;
	bool				isDirected() const noexcept;
	// TODO

	/* GFX ===================================================================== */

	bool				isTransparent() const noexcept;
	bool				isSemiTransparent() const noexcept;
	bool				isOpaque() const noexcept;

	bool				isLightSource() const noexcept;
	// TODO

private:
	/* ========================================================================= */
	/*                                    DATA                                   */
	/* ========================================================================= */

	BlockData			_data;

}; // class Block

#ifdef __DEBUG
	std::ostream&		operator<<(std::ostream& os, const Block& block);
#endif

} // namespace vox