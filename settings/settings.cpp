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
#include "settings.hpp"
#include <filesystem>
#include <fstream>


void Settings::init()
{
    const char* c = getenv("HOME");
    if(c == NULL)
    {
        return;
    }
    
    settings_file = c;
    settings_file = settings_file + "/" + settings_file_name;
    
    if(!std::filesystem::exists(settings_file))
    {
        std::ofstream t(settings_file);
        t.close();
    }
    reader = std::make_unique<mINI::INIFile>(settings_file);

    reader->read(ini);

}

std::string Settings::get_value(const char* area, const char* key)
{
    return ini[area][key];
}


void Settings::set_value(const char* area, const char* key, const char* value)
{
    ini[area][key] = value;
    reader->write(ini);
}


Settings& Settings::get_instance()
{
    static Settings instance;
    return instance;
}


Settings::Settings()
{
    init();
}
