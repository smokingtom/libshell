/*
 * libshell - A shell library
 * Copyright (C) 2026 smokingtom
 *
 * Author: smokingtom
 * Project: https://github.com/smokingtom/libshell
 *
 * This file is part of libshell.
 *
 * libshell is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libshell is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libshell. If not, see <https://www.gnu.org/licenses/>.
 */

#include "audio.hpp"
#include "../launch/launch.h"
#include "../resource/resource.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

Audio& Audio::get_instance()
{
    static Audio instance;
    return instance;
}

Audio::Audio()
{
}

void Audio::play(std::string f)
{
    launch("ogg123", f.c_str(), NULL);
}
    