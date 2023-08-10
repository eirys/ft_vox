/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_render_pass.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 13:32:01 by etran             #+#    #+#             */
/*   Updated: 2023/08/10 22:13:59 by etran            ###   ########.fr       */
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
	/*                                  METHODS                                  */
	/* ========================================================================= */

	SceneRenderPass() = default;
	~SceneRenderPass() = default;

	SceneRenderPass(SceneRenderPass&& other) = delete;
	SceneRenderPass(const SceneRenderPass& other) = delete;
	SceneRenderPass& operator=(SceneRenderPass&& other) = delete;
	SceneRenderPass& operator=(const SceneRenderPass& other) = delete;

	/* ========================================================================= */

	void			init(Device& device, SwapChain& swap_chain) override;

	using RenderPass::destroy;
	using RenderPass::getRenderPass;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	ImageBuffer		_color_image;
	ImageBuffer		_depth_image;

}; // class SceneRenderPass

} // namespace scop::graphics