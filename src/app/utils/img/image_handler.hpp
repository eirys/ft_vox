/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_handler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 10:23:32 by eli               #+#    #+#             */
/*   Updated: 2023/05/23 10:21:05 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <string>
# include <vector>

# include "image_loader.hpp"

namespace scop {

/**
 * Image handler.
 * 
 * Only handles 32-bit images. (RGBA)
*/
class Image {
public:
	/* ========================================================================= */
	/*                                  TYPEDEF                                  */
	/* ========================================================================= */

	typedef		enum ImageType	ImageType;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Image(
		const std::string& path,
		ImageType type,
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
	ImageType					getType() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	const std::string			path;
	ImageType					type;
	const std::vector<uint32_t>	pixels;
	std::size_t					width;
	std::size_t					height;

}; // class Image

} // namespace scop