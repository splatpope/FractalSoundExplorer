#ifndef SETTINGS_INCLUDED
#define SETTINGS_INCLUDED
#include <iostream>
#include <libconfig.h++>
namespace FSE {
    struct Settings {
        int target_fps;
        int sample_rate;
        int max_freq;
        int window_width;
        int window_height;
        int starting_fractal;
        int max_iters;
        int escape_radius;
        bool fullscreen;
        std::string app_name = "Fractal Sound Explorer";

        Settings (std::string path) {
            libconfig::Config cfg;
            try
            {
                {cfg.readFile(path.c_str());}
            }
            catch(const libconfig::FileIOException &fioex)
            {
                std::cerr << "Error while reading settings file : " << fioex.what() << '\n';
                std::getchar();
                exit(EXIT_FAILURE);
            }
            catch(const libconfig::ParseException &pex)
            {
                std::cerr << "Config parsing error at " << pex.getFile() << ":" << pex.getLine()
                                    << " - " << pex.getError() << std::endl;
                std::getchar();
                exit(EXIT_FAILURE);
            }
            libconfig::Setting& root = cfg.getRoot();
            libconfig::Setting& app_settings_store = root["settings"];
            if (!(	
                    app_settings_store.lookupValue("target_fps", target_fps)
                && 	app_settings_store.lookupValue("sample_rate", sample_rate)
                && 	app_settings_store.lookupValue("max_freq", max_freq)
                && 	app_settings_store.lookupValue("window_width", window_width)
                && 	app_settings_store.lookupValue("window_height", window_height)
                && 	app_settings_store.lookupValue("starting_fractal", starting_fractal)
                && 	app_settings_store.lookupValue("max_iters", max_iters)
                && 	app_settings_store.lookupValue("escape_radius", escape_radius)
                &&	app_settings_store.lookupValue("fullscreen", fullscreen)
            )) {
                std::cerr << "Settings are wrong !" << std::endl;
                std::getchar();
            }
            Print();
        }


        void Print() {
            std::cout <<
            "FSE Settings (default.cfg):\n"
            "target_fps : " 		<< target_fps		<< "\n"
            "sample_rate : " 		<< sample_rate		<< "\n"
            "max_freq : "			<< max_freq		    << "\n"
            "fullscreen : "			<< fullscreen		<< "\n"
            "window_width : "		<< window_width	    << "\n"
            "window_height : "		<< window_height	<< "\n"
            "starting_fractal : "	<< starting_fractal << "\n"
            "max_iters : "			<< max_iters		<< "\n"
            "escape_radius : "		<< escape_radius	<< "\n"
            << std::endl;
        }
    };
}
#endif
