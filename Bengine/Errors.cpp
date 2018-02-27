#include "Errors.h"

#include <iostream>
#include <cstdlib>
#include <SDL/SDL.h>

namespace Bengine {

	void fatalError(std::string errorString) {
		std::cout << errorString << std::endl;
		std::cout << "Press any key to continue.." << std::endl;
		int tmp;
		std::cin >> tmp;
		SDL_Quit();
		exit(1);
	}

	void fatalErrorSocket(std::string errorString)
	{
		std::cout << errorString << std::endl;
		std::cout << "Enter any key to quit" << std::endl;
		int tmp;
		std::cin >> tmp;
		exit(1);
	}
}