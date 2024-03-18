/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_handler.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:17:39 by etran             #+#    #+#             */
/*   Updated: 2024/03/18 15:03:57 by etran            ###   ########.fr       */
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

	Image() = default;
	Image(Image&& x) = default;
	Image(const Image& x) = default;
	Image&	operator=(Image&& x) = default;
	Image&	operator=(const Image& x) = default;
	~Image() = default;

	/* ========================================================================= */

	const std::string&		getPath() const noexcept;
	const uint32_t*			getPixels() const noexcept;
	std::size_t				getWidth() const noexcept;
	std::size_t				getHeight() const noexcept;

private:
	/* ========================================================================= */
	/*                                    DATA                                   */
	/* ========================================================================= */

	std::string				path;
	std::vector<uint32_t>	pixels;
	std::size_t				width = 0;
	std::size_t				height = 0;

}; // class Image

} // namespace scop