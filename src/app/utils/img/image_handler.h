/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_handler.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:17:39 by etran             #+#    #+#             */
/*   Updated: 2023/06/08 19:50:35 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <string>
# include <vector>

# include "image_loader.h"

namespace scop {

/**
 * Image handler.
 * 
 * Only handles 32-bit images.
*/
class Image {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Image(
		const std::string& path,
		std::vector<uint32_t>&& pixels,
		std::size_t width,
		std::size_t height
	);

	Image(Image&& x) = default;
	~Image() = default;

	Image() = delete;
	Image(const Image& x) = delete;
	Image&	operator=(const Image& x) = delete;

	/* ========================================================================= */

	const std::string&			getPath() const noexcept;
	const uint32_t*				getPixels() const noexcept;
	std::size_t					getWidth() const noexcept;
	std::size_t					getHeight() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	const std::string			path;
	const std::vector<uint32_t>	pixels;
	std::size_t					width;
	std::size_t					height;

}; // class Image

} // namespace scop