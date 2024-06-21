/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sampler.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:29:06 by etran             #+#    #+#             */
/*   Updated: 2024/06/20 16:07:40 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>

namespace vox::gfx {

class Device;


class Sampler final {
public:
    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class Filter {
        Nearest = VK_FILTER_NEAREST,
        Linear  = VK_FILTER_LINEAR
    };

    enum class MipMode {
        Nearest = VK_SAMPLER_MIPMAP_MODE_NEAREST,
        Linear  = VK_SAMPLER_MIPMAP_MODE_LINEAR
    };

    enum class Border {
        Repeat  = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        Edge    = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
        Color   = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
    };

    enum class BorderColor {
        WhiteInt    = VK_BORDER_COLOR_INT_OPAQUE_WHITE,
        WhiteFloat  = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
        BlackInt    = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        BlackFloat  = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK
    };

    enum class Compare {
        Never       = VK_COMPARE_OP_NEVER,
        Less        = VK_COMPARE_OP_LESS,
        Equal       = VK_COMPARE_OP_EQUAL,
        LessEqual   = VK_COMPARE_OP_LESS_OR_EQUAL,
        Greater     = VK_COMPARE_OP_GREATER,
        NotEqual    = VK_COMPARE_OP_NOT_EQUAL,
        GreaterEqual= VK_COMPARE_OP_GREATER_OR_EQUAL,
        Always      = VK_COMPARE_OP_ALWAYS
    };

    /* ====================================================================== */
    /*                              HELPER CLASS                              */
    /* ====================================================================== */

    struct Metadata final {
        Filter      m_filter = Filter::Linear;
        MipMode     m_mipMode = MipMode::Linear;
        Border      m_border = Border::Repeat;
        BorderColor m_borderColor = BorderColor::WhiteFloat;
        Compare     m_compare = Compare::Never;
        float       m_maxLod = 1.0f;

        bool        m_enableAnisotropy = false;


        bool operator==(const Metadata& other) const noexcept {
            return m_filter == other.m_filter
                && m_mipMode == other.m_mipMode
                && m_borderColor == other.m_borderColor
                && m_border == other.m_border
                && m_compare == other.m_compare
                && m_enableAnisotropy == other.m_enableAnisotropy
                && m_maxLod == other.m_maxLod;
        }
    };

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    Sampler() = default;
    Sampler(Sampler&& other) = default;
    Sampler& operator=(Sampler&& other) = default;
    ~Sampler() = default;

    Sampler(const Sampler& other) = delete;
    Sampler& operator=(const Sampler& other) = delete;

    /* ====================================================================== */

    void init(const Device& device, const Metadata& metadata);
    void destroy(const Device& device);

    /* ====================================================================== */

    VkSampler       getSampler() const noexcept;
    const Metadata& getMetadata() const noexcept;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    VkSampler   m_sampler = VK_NULL_HANDLE;
    Metadata    m_metadata{};

}; // class Sampler

} // namespace vox::gfx