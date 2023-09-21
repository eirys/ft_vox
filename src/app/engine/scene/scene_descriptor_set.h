/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_descriptor_set.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 11:08:15 by etran             #+#    #+#             */
/*   Updated: 2023/09/01 15:30:34 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <memory> // std::shared_ptr

# include "descriptor_set.h"

namespace scop::graphics {

class Buffer;
class TextureHandler;

class SceneDescriptorSet final: public DescriptorSet {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using super = DescriptorSet;
	using TextureHandlerPtr = std::shared_ptr<TextureHandler>;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	SceneDescriptorSet() = default;
	~SceneDescriptorSet() = default;

	SceneDescriptorSet(SceneDescriptorSet&& other) = delete;
	SceneDescriptorSet(const SceneDescriptorSet& other) = delete;
	SceneDescriptorSet& operator=(SceneDescriptorSet&& rhs) = delete;
	SceneDescriptorSet& operator=(const SceneDescriptorSet& rhs) = delete;

	/* ========================================================================= */

	using super::destroy;
	using super::setDescriptors;

	void	init(Device& device) override;
	void	plug(
		Device& device,
		Buffer& buffer,
		TextureHandlerPtr textures,
		TextureHandlerPtr shadowmap,
		TextureHandlerPtr heightmap);

	/* ========================================================================= */

	using super::getLayout;
	using super::getPoolSizes;
	using super::getSet;
	using super::getSetIndex;

}; // class SceneDescriptorSet

} // namespace scop::graphics