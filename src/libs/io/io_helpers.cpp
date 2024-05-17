/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_helpers.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:21:46 by etran             #+#    #+#             */
/*   Updated: 2024/03/11 09:05:38 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io_helpers.h"

#include <fstream>
#include <string>

namespace io {

/**
 * Read binary file and return in vector of char format.
*/
std::vector<u8>	readBinary(const char* filename) {
    // Read file as binary file, at the end of the file
    const std::string   filePath = std::string(filename);
    std::ifstream       file(filePath, std::ios::ate | std::ios::binary);

    if (!file.is_open())
        throw std::runtime_error("failed to open file: " + filePath);

    const std::size_t   fileSize = (std::size_t)file.tellg();
    std::vector<u8>	buffer(fileSize);

    file.seekg(0);
    file.read((char*)buffer.data(), fileSize);
    file.close();

    return buffer;
}

} // namespace io