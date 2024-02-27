/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entrypoint.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 10:44:55 by etran             #+#    #+#             */
/*   Updated: 2024/02/27 18:31:28 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include <iostream>

int main() {
    try {
        App app;

        app.run();
    } catch (const std::runtime_error& e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}