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
#include "resource.hpp"
#include <cstdlib>
#include <filesystem>
#include "../logger/logger.hpp"

void Resource::init()
{
    Logger& l = Logger::get_instance();
    const char* home = getenv("HOME");
    if (!home)
    {
        l.log("RESOURCE", "INIT", "failed to query home path");
        return;
    }

    std::filesystem::path base_path = home;
    base_path /= libshell_folder;
    l.log("RESOURCE", "INIT", base_path.c_str());
    paths[0] = base_path;
    std::filesystem::path audio_path = base_path / libshell_audio_folder;
    std::filesystem::path images_path = base_path / libshell_images_folder;
    std::filesystem::path text_path = base_path / libshell_text_folder;
    std::filesystem::path taskbar_path = base_path / libshell_taskbar_folder;
    paths[1] = audio_path;
    paths[2] = images_path;
    paths[3] = text_path;
    paths[4] = taskbar_path;
    //oh nop
    try {
        std::filesystem::create_directories(base_path);
        std::filesystem::create_directories(audio_path);
        std::filesystem::create_directories(images_path);
        std::filesystem::create_directories(text_path);
	std::filesystem::create_directories(taskbar_path);
    } catch (const std::filesystem::filesystem_error& e) {
        l.log("RESOURCE", "INIT", e.what());
        return;
    }
}

std::string Resource::extract_text(std::string path)
{
    char c[1000];
    FILE* f;
    if((f = fopen(path.c_str(), "r")) == NULL)
    {
        return "Error";
    }
    fscanf(f, "%[^\n]", c);
    fclose(f);
    return c;
}

std::string Resource::get_resource(std::string res)
{
    char c = res[0];
    if(c == 'A')
    {
        return paths[1] + "/" + res.substr(res.find("_")+1) + ".ogg";
    } else if(c == 'I')
    {
        return paths[2] + "/" + res.substr(res.find("_")+1) + ".jpeg";
    } else if (c == 'T')
    {
        return extract_text(paths[3] + "/" + res.substr(res.find("_")+1) + ".txt");
    } else if(c == 'D')
    {
        if(res.substr(res.find("_")+1) == "TASKBAR") //bad approach, needs fixing
        {
            return paths[4] + "/";
        } else
            return "invalid";
            
    } else{
        return "invalid";
    }
    return "invalid";
}
