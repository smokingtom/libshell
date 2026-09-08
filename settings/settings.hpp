#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "ini.h"

class Settings 
{
    public:
        static Settings& get_instance();
        void set_value(const char* area, const char* key, const char* value);
        std::string get_value(const char* area, const char* key);
    protected:
        Settings();
        ~Settings() = default;
        Settings(const Settings&) = delete;
        Settings& operator=(const Settings&) = delete;
        void init();
        std::string settings_file;
        const char* settings_file_name = "SYSTEM.ini";
        std::unique_ptr<mINI::INIFile> reader;  
        mINI::INIStructure ini;

};

#endif