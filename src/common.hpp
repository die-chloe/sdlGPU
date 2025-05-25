#pragma once

#include <stdexcept>
#include <vector>
#include <string>
#include <SDL3/SDL.h>

#include "Renderer/renderer.hpp"

void SDLException(const std::string& message);
extern const char* BasePath;