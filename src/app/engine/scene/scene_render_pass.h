/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_render_pass.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 13:32:01 by etran             #+#    #+#             */
/*   Updated: 2023/08/10 22:34:35 by etran            ###   ########.fr       */
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
		const super::RenderPassInfo& rp_info,
		const super::ResourcesInfo& res_info) override;
	void		destroy(Device& device) override;
	void		updateResources(
		Device& device,
		const ResourcesInfo& res_info) override;

	/* ========================================================================= */

	using super::getRenderPass;

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
		const ResourcesInfo& res_info) override;
	void		_destroyResources(Device& device) override;

}; // class SceneRenderPass

} // namespace scop::graphics