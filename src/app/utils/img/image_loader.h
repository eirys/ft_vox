/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_loader.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:17:44 by etran             #+#    #+#             */
/*   Updated: 2023/06/04 17:17:45 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <string>
# include <vector>

namespace scop {

class Image;

/**
 * Image extension.
*/
enum ImageType {
	PPM,
	// PNG,		TODO
	// JPEG,
	// BMP,
	// TGA,
};

/**
 * Image loader interface.
 * 
 * The files are loaded into a vector of char, which is then parsed.
 * This format is necessary to be able to load binary files (images).
*/
class ImageLoader {
public:
	/* ========================================================================= */
	/*                                  TYPEDEF                                  */
	/* ========================================================================= */

	typedef		enum ImageType	ImageType;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	virtual ~ImageLoader() = default;
	virtual scop::Image		load() = 0;

	/* ========================================================================= */
	/*                                 EXCEPTIONS                                */
	/* ========================================================================= */

	class FailedToLoadImage: public std::exception {
	public:
		FailedToLoadImage() = delete;
		FailedToLoadImage(const std::string& path, const std::string& spec):
			spec("Failed to load `" + path + "`: " + spec) {}

		const char*	what() const noexcept override {
			return spec.c_str();
		}
	private:
		const std::string	spec;
	};

protected:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	const std::string	path;		// File path
	ImageType			type;		// File extension
	std::vector<char>	data;		// Contains file entire content
	std::size_t			width;
	std::size_t			height;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	ImageLoader(const std::string& path, ImageType type): path(path), type(type) {}
	ImageLoader(ImageLoader&& x) = default;

	ImageLoader() = delete;
	ImageLoader(const ImageLoader& x) = delete;
	ImageLoader& operator=(const ImageLoader& x) = delete;

}; // class ImageLoader

} // namespace scop