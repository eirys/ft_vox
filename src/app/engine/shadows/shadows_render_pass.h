/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows_render_pass.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 10:57:55 by etran             #+#    #+#             */
/*   Updated: 2023/09/15 16:00:01 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "render_pass.h"
# include "image_buffer.h"

namespace scop::graphics {

class Device;
class SwapChain;

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
		Device& device,
		const super::RenderPassInfo& rp_info) override;
	void		destroy(Device& device) override;
	void		updateResources(
		Device& device,
		const super::RenderPassInfo& rp_info) override;

	/* ========================================================================= */

	using super::getRenderPass;
	using super::getWidth;
	using super::getHeight;

	const ImageBuffer&	getDepthResource() const noexcept;
	ImageBuffer&		getDepthResource() noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	ImageBuffer	_depth_image;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void		_createRenderPass(
		Device& device,
		const super::RenderPassInfo& rp_info) override;
	void		_importResources(const super::RenderPassInfo& rp_info);

}; // class ShadowsRenderPass

} // namespace scop::graphics