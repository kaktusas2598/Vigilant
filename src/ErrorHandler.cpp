#include "ErrorHandler.hpp"

#include <iostream>
#include <SDL2/SDL.h>

namespace Vigilant
{
    void exitWithError(std::string error)
    {
        std::cout << error << std::endl;

        SDL_Quit();
        exit(80);
    }
}
