#include <string>
#include <fstream>

#define A_STARTUP "A_STARTUP"
#define A_SHUTDOWN "A_SHUTDOWN"
#define A_CHIMES "A_CHIMES"
//#define A_CHORD "A_CHORD" 
#define A_DING "A_DING"
#define A_TADA "A_TADA"

#define I_LOGO "I_LOGO"
#define I_CAT "I_CAT"

#define T_STARTBUTTON "T_STARTBUTTON"

#define D_TASKBAR "D_TASKBAR"
class Resource
{
    public:
        std::string get_resource(std::string res);
	std::string get_dir(std::string d);
        static Resource& get_instance()
        {
            static Resource instance;
            return instance;
        }
        void init();
    protected:
        std::ifstream file;
        std::string libshell_folder = ".libshell";
        std::string libshell_audio_folder = "audio";
        std::string libshell_images_folder = "images";
        std::string libshell_text_folder = "text";
	std::string libshell_taskbar_folder = "taskbar";
        std::string extract_text(std::string path);
        std::string paths[5] = {};
        Resource(){}
        ~Resource() = default;
        Resource(const Resource&) = delete;
        Resource& operator=(const Resource&) = delete;
};


