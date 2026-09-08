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

#include "logger.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <filesystem>

const char* Logger::query_logfile()
{
    char* c = getenv("HOME");
    if(c == NULL)
    {
        #ifdef DEBUG
            std::cout << "LOGGER: getenv failed\n";
        #endif
        return NULL;
    }
    logfile_path = c;
    logfile_path = logfile_path + "/" + logfile_name;
    #ifdef DEBUG
        std::cout << "LOGGER: path is: " << logfile_path << "\n";
    #endif
    
    if(!std::filesystem::exists(logfile_path))
    {
        #ifdef DEBUG
            std::cout << "LOGGER: creating logfile\n"; 
        #endif
    } 

    logfile.open(logfile_path, std::ios::app);
    if(!logfile.is_open())
    {
        #ifdef DEBUG
            std::cout << "LOGGER: Could not open logfile while attempting to write initial message\n";
        #endif
        return NULL;
    }

    /*time thingy for the initial thingy (from https://stackoverflow.com/questions/5141960/get-the-current-time-in-c)*/
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    logfile << "\nlibshell LOGGER initialised at " << asctime(timeinfo) << "\n";
    logfile.flush();

    #ifdef DEBUG
        std::cout << "LOGGER: All went well while querying the logfile\n" << std::endl;
    #endif
    is_logging = 1;
    return logfile_path.c_str();
}

void Logger::log(const char* src, const char* func, const char* msg)
{
    if(!is_logging)
    {
        query_logfile();
    }

    if(!logfile.is_open())
    {
        return;
    }

    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    std::string timestamp = asctime(timeinfo);
    timestamp.pop_back();
    logfile << "[" << timestamp << "] " << src << " :: " << func << " :: " << msg << "\n";
    logfile.flush();
}

void Logger::stop()
{
    if(logfile.is_open())
    {
        logfile.close();
        is_logging = 0;
        #ifdef DEBUG
            std::cout << "LOGGER: Logfile closed";
        #endif
    }
}

/*
int main()
{
    Logger& l = Logger::get_instance();
    
    l.log("LOGGER.CPP", "MAIN", "logger test ");
    l.log("LOGGER.CPP", "MAIN", "yes indeed");

    return 0;

}
    */