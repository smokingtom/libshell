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
#ifndef LOGGER_H
#define LOGGER_H
#define DEBUG

#include <fstream>
#include <string>

class Logger {
    public:
        void log(const char* src,
                 const char* func,
                 const char* msg);
        static Logger& get_instance()
        {
            static Logger obj;
            return obj;
        }
        const char* query_logfile();
        void stop();

    protected:
        const char* logfile_name = ".libshell_log.txt";
        std::string logfile_path;
        std::ofstream logfile;
        int is_logging = 0;

    private:
        Logger() {}
};

#endif