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
