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
#include "notifications.hpp"
#include "../launch/launch.h"
#include "../logger/logger.hpp"
#include "../audio/audio.hpp"
#include "../resource/resource.hpp"

void Notifications::notify(const char* title, const char* message, const char* urgency, std::string audio)
{
    std::string cmd = "notify-send";
    launch(cmd.c_str(), "-u", urgency, title, message, NULL);
    Audio::get_instance().play(audio);
}

Notifications& Notifications::get_instance()
{
    static Notifications instance;
    return instance;
}

Notifications::Notifications()
{
}

