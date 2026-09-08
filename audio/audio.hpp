#ifndef AUDIO_HPP
#define AUDIO_HPP
#include <string>

class Audio 
{
    public:
        static Audio& get_instance();
        void play(std::string f);
    protected:
        Audio();
        ~Audio() = default;
        Audio(const Audio&) = delete;
        Audio& operator=(const Audio&) = delete;
        void init();
};

#endif