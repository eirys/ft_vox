/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ubo.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 13:47:01 by etran             #+#    #+#             */
/*   Updated: 2024/03/07 13:47:45 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace vox::gfx::ubo {

class Ubo {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    Ubo() = default;
    ~Ubo() = default;

    Ubo(Ubo&& other) = delete;
    Ubo(const Ubo& other) = delete;
    Ubo& operator=(Ubo&& other) = delete;
    Ubo& operator=(const Ubo& other) = delete;

}; // class Ubo

} // namespace vox::gfx::ubo