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

namespace scop::graphics {

class Device;
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
		Device& device,
		const super::RenderPassInfo& create_info) override;
	void		_createResources(
		Device& device,
		const super::RenderPassInfo& create_info);

	void		_destroyResources(Device& device);

}; // class SceneRenderPass

} // namespace scop::graphics