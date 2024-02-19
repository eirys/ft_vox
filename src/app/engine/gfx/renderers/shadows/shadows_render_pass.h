/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows_render_pass.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 10:57:55 by etran             #+#    #+#             */
/*   Updated: 2024/01/20 12:55:47 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "render_pass.h"
# include "image_buffer.h"

namespace scop {
class SwapChain;
}

namespace scop::gfx {

class ShadowsRenderPass final: public RenderPass {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using super = RenderPass;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	ShadowsRenderPass() = default;
	~ShadowsRenderPass() = default;

	ShadowsRenderPass(ShadowsRenderPass&& other) = delete;
	ShadowsRenderPass(const ShadowsRenderPass& other) = delete;
	ShadowsRenderPass& operator=(ShadowsRenderPass&& other) = delete;
	ShadowsRenderPass& operator=(const ShadowsRenderPass& other) = delete;

	/* ========================================================================= */

	void		init(
		scop::core::Device& device,
		const RenderPassInfo& rp_info) override;
	void		destroy(scop::core::Device& device) override;
	void		updateResources(
		scop::core::Device& device,
		const RenderPassInfo& rp_info) override;

	/* ========================================================================= */

	using super::getRenderPass;
	using super::getWidth;
	using super::getHeight;

	const ImageBuffer&	getDepthResource() const noexcept;
	ImageBuffer&		getDepthResource() noexcept;

private:
	/* ========================================================================= */
	/*                                    DATA                                   */
	/* ========================================================================= */

	ImageBuffer	_depth_image;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void		_createRenderPass(
		scop::core::Device& device,
		const RenderPassInfo& rp_info) override;
	void		_importResources(const RenderPassInfo& rp_info);

}; // class ShadowsRenderPass

} // namespace scop::gfx