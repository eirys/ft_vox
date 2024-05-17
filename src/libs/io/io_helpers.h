/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_helpers.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 07:51:09 by etran             #+#    #+#             */
/*   Updated: 2024/02/29 22:22:53 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <vector>
# include "types.h"

namespace io {

std::vector<u8>	readBinary(const char* filename);

} // namespace io