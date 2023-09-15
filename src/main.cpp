/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 03:53:55 by eli               #+#    #+#             */
/*   Updated: 2023/09/15 16:12:50 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include "utils.h"

int main(int ac, char** av) {
	try {
		(void)av;
		if (ac != 1) {
			throw std::invalid_argument("Unexcepted arguments.");
		}

		scop::App		app;
		app.run();
	} catch (const std::exception& e) {
		std::cerr << e.what() << __NL;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}