/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_render_pass.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 13:32:01 by etran             #+#    #+#             */
/*   Updated: 2023/11/16 23:10:06 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "render_pass.h"
# include "image_buffer.h"

namespace scop::core {
class Device;
class SwapChain;
}

namespace scop::gfx {

class SceneRenderPass final: public RenderPass {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using super = RenderPass;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	SceneRenderPass() = default;
	~SceneRenderPass() = default;

	SceneRenderPass(SceneRenderPass&& other) = delete;
	SceneRenderPass(const SceneRenderPass& other) = delete;
	SceneRenderPass& operator=(SceneRenderPass&& other) = delete;
	SceneRenderPass& operator=(const SceneRenderPass& other) = delete;

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

	const ImageBuffer&	getColorResource() const noexcept;
	ImageBuffer&		getColorResource() noexcept;
	const ImageBuffer&	getDepthResource() const noexcept;
	ImageBuffer&		getDepthResource() noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	ImageBuffer	_color_image;
	ImageBuffer	_depth_image;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void		_createRenderPass(
		scop::core::Device& device,
		const RenderPassInfo& create_info) override;
	void		_createResources(
		scop::core::Device& device,
		const RenderPassInfo& create_info);

	void		_destroyResources(scop::core::Device& device);

}; // class SceneRenderPass

} // namespace scop::gfx