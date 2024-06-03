/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cache.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 14:31:24 by etran             #+#    #+#             */
/*   Updated: 2024/05/18 15:31:59 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "types.h"

enum class CacheType: u8 {
    UNDEFINED,
    CHUNK,
    PROCEDURAL_TEXTURE,
};

/**
 * @brief Cache the data.
*/
class Cache final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    Cache(const CacheType type, const u32 id) noexcept;

    Cache() = default;
    ~Cache() = default;

    Cache(Cache&& other) = delete;
    Cache(const Cache& other) = delete;
    Cache& operator=(Cache&& other) = delete;
    Cache& operator=(const Cache& other) = delete;

    /* ====================================================================== */

    std::vector<u8>     load() const;

    void                save(const std::vector<u8>& data) const;
    void                save(const u8* data, const u32 size) const;

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    CacheType   m_type = CacheType::UNDEFINED;
    u32         m_id = 0;

}; // class Cache