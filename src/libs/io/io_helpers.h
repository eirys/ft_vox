/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_helpers.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 07:51:09 by etran             #+#    #+#             */
/*   Updated: 2024/02/21 11:45:29 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <fstream>
# include <vector>

# include "types.h"

namespace io {

/**
 * Read binary file and return in vector of char format.
*/
static
std::vector<u8>	readBinary(const std::string& filename) {
	// Read file as binary file, at the end of the file
	std::ifstream	file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file: " + filename);
	}

	std::size_t			file_size = static_cast<std::size_t>(file.tellg());
	std::vector<u8>	buffer(file_size);
	file.seekg(0);
	file.read((char*)buffer.data(), file_size);
	file.close();
	return buffer;
}

} // namespace io