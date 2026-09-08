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

