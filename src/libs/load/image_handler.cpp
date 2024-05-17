/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_handler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 22:44:13 by eli               #+#    #+#             */
/*   Updated: 2023/06/08 20:31:42 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image_handler.h"

namespace scop {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Image::Image(
	const std::string& path,
	std::vector<uint32_t>&& pixels,
	std::size_t width,
	std::size_t height
):
path(path),
pixels(std::move(pixels)),
width(width),
height(height) {}

/* ========================================================================== */

const std::string&	Image::getPath() const noexcept {
	return path;
}

const uint32_t*		Image::getPixels() const noexcept {
	return pixels.data();
}

std::size_t	Image::getWidth() const noexcept {
	return width;
}

std::size_t	Image::getHeight() const noexcept {
	return height;
}

} // namespace scop