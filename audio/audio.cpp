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
    