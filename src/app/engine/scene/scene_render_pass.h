/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_render_pass.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 13:32:01 by etran             #+#    #+#             */
/*   Updated: 2023/09/15 15:59:28 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "render_pass.h"
# include "image_buffer.h"

namespace scop::core {
class Device;
}

namespace scop::gfx {

class SwapChain;

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

	const scop::core::ImageBuffer&	getColorResource() const noexcept;
	scop::core::ImageBuffer&		getColorResource() noexcept;
	const scop::core::ImageBuffer&	getDepthResource() const noexcept;
	scop::core::ImageBuffer&		getDepthResource() noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	scop::core::ImageBuffer			_color_image;
	scop::core::ImageBuffer			_depth_image;

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