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
#ifndef NOTIFICATIONS_HPP
#define NOTIFICATIONS_HPP
#include <string>
#include "../audio/audio.hpp"
#define LOW_URGENCY "low"
#define NORMAL_URGENCY "normal"
#define CRITICAL_URGENCY "critical"

class Notifications 
{
    public:
        static Notifications& get_instance();
        void notify(const char* title, const char* message, const char* urgency, std::string audio);
    protected:
        Notifications();
        ~Notifications() = default;
        Notifications(const Notifications&) = delete;
        Notifications& operator=(const Notifications&) = delete;
        void init();
};

#endif