/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ppm_loader.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:17:49 by etran             #+#    #+#             */
/*   Updated: 2024/01/20 12:55:47 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <string>
# include <fstream>
# include <functional>

# include "image_loader.h"
# include "image_handler.h"

namespace scop {

enum FormatPPM {
	P3,		// ASCII
	P6		// BINARY
};

/**
 * PPM files parser.
*/
class PpmLoader: public ImageLoader {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	PpmLoader(const std::string& path);

	PpmLoader(PpmLoader&& x) = default;
	~PpmLoader() = default;

	PpmLoader() = delete;
	PpmLoader(const PpmLoader& x) = delete;
	PpmLoader&	operator=(PpmLoader&& x) = delete;
	PpmLoader&	operator=(const PpmLoader& x) = delete;

	/* ========================================================================= */

	scop::Image		load() override;

private:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	typedef		ImageLoader					base;
	typedef		std::vector<uint32_t>		Pixels;
	typedef		enum FormatPPM				Format;
	typedef		std::function<uint8_t()>	ParseNumberFn;

	/* ========================================================================= */
	/*                                    DATA                                   */
	/* ========================================================================= */

	Format		format;
	uint8_t		max_color;
	std::size_t	cursor = 0;	// Used to parse the file
	std::size_t	line = 1;

	/* ========================================================================= */

	void		parseHeader();
	Pixels		parseBody();

	Format		expectFormat();
	uint32_t	expectNumber();

	bool		skipWhitespace() noexcept;
	bool		skipComment() noexcept;
	void		ignoreChunk() noexcept;

	uint32_t	createPixel(uint8_t r, uint8_t g, uint8_t b) const noexcept;

	/* ========================================================================= */
	/*                                 EXCEPTION                                 */
	/* ========================================================================= */

	class PpmParseError: public std::exception {
	public:
		PpmParseError() = delete;
		PpmParseError(const std::string& spec):
			spec(spec) {}

		const char*	what() const noexcept override {
			return spec.c_str();
		}

	private:
		const std::string	spec;
	};

}; // class PpmLoader

} // namespace scop