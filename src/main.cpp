/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 03:53:55 by eli               #+#    #+#             */
/*   Updated: 2024/01/05 13:46:32 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include "debug.h"

int main(int ac, char** av) {
	try {
		(void)av;
		if (ac != 1) {
			throw std::invalid_argument("Unexpected arguments.");
		}

		scop::App		app;
		app.run();
	} catch (const std::exception& e) {
		std::cerr << e.what() << __NL;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}