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