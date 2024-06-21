/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   voxmap.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 21:22:50 by etran             #+#    #+#             */
/*   Updated: 2024/06/11 23:35:16 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "voxmap.h"
#include "io_helpers.h"
#include "debug.h"

#include <stdexcept>
#include <cstring>

namespace proc {

static constexpr const char* VXM_HEADER = "VXM ";
static constexpr const char* VXM_TYPE[(u32)VoxMap::VxmType::Count] = {
    "VOR",
    "PER",
};

static
i32 _searchNextChar(const std::vector<u8>& buffer, u32 start, char c) {
    for (u32 i = start; i < buffer.size(); i++) {
        if (buffer[i] == c) return i;
    }
    return -1;
}

static
VoxMap::VxmType _findVxmType(const char* typeName) {
    for (u32 i = 0; i < (u32)VoxMap::VxmType::Count; ++i) {
        if (memcmp(typeName, VXM_TYPE[i], strlen(VXM_TYPE[i])) == 0)
            return (VoxMap::VxmType)i;
    }
    return VoxMap::VxmType::Unknown;
}

/**
 * @brief Load a voxmap from a file.
 *
 * @note Expected format:
 * VXM <type> <size> <values>
 */
bool VoxMap::load(const char* path) {
    const std::vector<u8> buffer = io::readBinary(path);

    try {
        if (buffer.size() < strlen(VXM_HEADER) || memcmp(buffer.data(), VXM_HEADER, strlen(VXM_HEADER) - 1) != 0)
            throw std::runtime_error("invalid file format: missing header");

        u32 stringStart = strlen(VXM_HEADER);
        u32 nextWhiteSpace = _searchNextChar(buffer, stringStart, ' ');
        if (nextWhiteSpace == -1)
            throw std::runtime_error("invalid file format: missing type");

        m_type = _findVxmType((char*)buffer.data() + stringStart);

        stringStart = nextWhiteSpace + 1;
        nextWhiteSpace = _searchNextChar(buffer, stringStart, ' ');
        if (nextWhiteSpace == -1)
            throw std::runtime_error("invalid file format: missing size");

        m_size = (u32)std::stoi(std::string(buffer.begin() + stringStart, buffer.begin() + nextWhiteSpace));

        const u8* values = buffer.data() + nextWhiteSpace + 1;

        m_values.reserve(m_size * m_size);

        for (u32 i = 0; i < m_size * m_size; ++i) {
            const f32 value = *(f32*)(values + (i * sizeof(f32)));
            m_values.emplace_back(value);
        }

    } catch (const std::exception& e) {
        LINFO("Error loading voxmap: " << e.what());
        return false;
    }
    return true;
}

VoxMap::VxmType VoxMap::getType() const noexcept {
    return m_type;
}

u32 VoxMap::getSize() const noexcept {
    return m_size;
}

f32 VoxMap::getValue(const u32 x, const u32 y) const noexcept {
    return m_values[y * m_size + x];
}

} // namespace proc