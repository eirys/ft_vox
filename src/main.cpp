/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 03:53:55 by eli               #+#    #+#             */
/*   Updated: 2023/05/14 11:24:52 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.hpp"

int main(int ac, char** av) {
	try {
		std::string	texture_path;

		if (ac == 1) {
			throw std::invalid_argument("No model path provided");
		} else if (ac == 3) {
			texture_path = av[2];
		} else if (ac > 3) {
			throw std::invalid_argument("Too many arguments");
		}

		scop::App		app(av[1], texture_path);
		app.run();
	} catch (const std::exception& e) {
		std::cerr << e.what() << __NL;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}