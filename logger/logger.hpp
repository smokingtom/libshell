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