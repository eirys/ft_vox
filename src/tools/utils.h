/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:16:25 by etran             #+#    #+#             */
/*   Updated: 2024/01/01 14:03:19 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <cmath>
# include <fstream>
# include <vector>

namespace scop::utils {

/**
 * Read binary file and return in vector of char format.
*/
inline std::vector<uint8_t>	readFile(const std::string& filename) {
	// Read file as binary file, at the end of the file
	std::ifstream	file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file: " + filename);
	}

	std::size_t			file_size = static_cast<std::size_t>(file.tellg());
	std::vector<uint8_t>	buffer(file_size);
	file.seekg(0);
	file.read((char*)buffer.data(), file_size);
	file.close();
	return buffer;
}

template <typename EnumClass>
constexpr inline uint32_t	enumSize() {
	return static_cast<uint32_t>(EnumClass::Last) - static_cast<uint32_t>(EnumClass::First) + 1;
}

} // namespace scop::utils