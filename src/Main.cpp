#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <unistd.h>
#include <libconfig.h++>
#include "Settings.hpp"
#include "App.hpp"
/*
//Synthesizer class to inherit Windows Audio.
class Synth : public WinAudio {
public:
	bool audio_reset;
	bool audio_pause;
	double volume;
	double play_x, play_y;
	double play_cx, play_cy;
	double play_nx, play_ny;
	double play_px, play_py;

	Synth(HWND hwnd) : WinAudio(hwnd, sample_rate) {
		audio_reset = true;
		audio_pause = false;
		volume = 8000.0;
		play_x = 0.0;
		play_y = 0.0;
		play_cx = 0.0;
		play_cy = 0.0;
		play_nx = 0.0;
		play_ny = 0.0;
		play_px = 0.0;
		play_py = 0.0;
	}

	void SetPoint(double x, double y) {
		play_nx = x;
		play_ny = y;
		audio_reset = true;
		audio_pause = false;
	}

	virtual bool onGetData(Chunk& data) override {
		//Setup the chunk info
		data.samples = m_samples;
		data.sampleCount = AUDIO_BUFF_SIZE;
		memset(m_samples, 0, sizeof(m_samples));

		//Check if audio needs to reset
		if (audio_reset) {
			m_audio_time = 0;
			play_cx = (jx < 1e8 ? jx : play_nx);
			play_cy = (jy < 1e8 ? jy : play_ny);
			play_x = play_nx;
			play_y = play_ny;
			play_px = play_nx;
			play_py = play_ny;
			mean_x = play_nx;
			mean_y = play_ny;
			volume = 8000.0;
			audio_reset = false;
		}

		//Check if paused
		if (audio_pause) {
			return true;
		}

		//Generate the tones
		const int steps = sample_rate / max_freq;
		for (int i = 0; i < AUDIO_BUFF_SIZE; i+=2) {
			const int j = m_audio_time % steps;
			if (j == 0) {
				play_px = play_x;
				play_py = play_y;
				fractal(play_x, play_y, play_cx, play_cy);
				if (play_x*play_x + play_y*play_y > escape_radius_sq) {
					audio_pause = true;
					return true;
				}

				if (normalized) {
					dpx = play_px - play_cx;
					dpy = play_py - play_cy;
					dx = play_x - play_cx;
					dy = play_y - play_cy;
					if (dx != 0.0 || dy != 0.0) {
						double dpmag = 1.0 / std::sqrt(1e-12 + dpx*dpx + dpy*dpy);
						double dmag = 1.0 / std::sqrt(1e-12 + dx*dx + dy*dy);
						dpx *= dpmag;
						dpy *= dpmag;
						dx *= dmag;
						dy *= dmag;
					}
				} else {
					//Point is relative to mean
					dx = play_x - mean_x;
					dy = play_y - mean_y;
					dpx = play_px - mean_x;
					dpy = play_py - mean_y;
				}

				//Update mean
				mean_x = mean_x*0.99 + play_x*0.01;
				mean_y = mean_y*0.99 + play_y*0.01;

				//Don't let the volume go to infinity, clamp.
				double m = dx*dx + dy*dy;
				if (m > 2.0) {
					dx *= 2.0 / m;
					dy *= 2.0 / m;
				}
				m = dpx*dpx + dpy*dpy;
				if (m > 2.0) {
					dpx *= 2.0 / m;
					dpy *= 2.0 / m;
				}

				//Lose volume over time unless in sustain mode
				if (!sustain) {
					volume *= 0.9992;
				}
			}

			//Cosine interpolation
			double t = double(j) / double(steps);
			t = 0.5 - 0.5*std::cos(t * 3.14159);
			double wx = t*dx + (1.0 - t)*dpx;
			double wy = t*dy + (1.0 - t)*dpy;

			//Save the audio to the 2 channels
			m_samples[i]   = (int16_t)std::min(std::max(wx * volume, -32000.0), 32000.0);
			m_samples[i+1] = (int16_t)std::min(std::max(wy * volume, -32000.0), 32000.0);
			m_audio_time += 1;
		}

		//Return the sound clip
		return !audio_reset;
	}

	int16_t m_samples[AUDIO_BUFF_SIZE];
	int32_t m_audio_time;
	double mean_x;
	double mean_y;
	double dx;
	double dy;
	double dpx;
	double dpy;
};
*/
//Main entry-

void print_settings(const Settings app_settings) {
	std::cout <<
	"FSE Settings (default.cfg):\n"
	"target_fps : " 		<< app_settings.target_fps		<< "\n"
	"sample_rate : " 		<< app_settings.sample_rate		<< "\n"
	"max_freq : "			<< app_settings.max_freq		<< "\n"
	"fullscreen : "			<< app_settings.fullscreen		<< "\n"
	"window_width : "		<< app_settings.window_width	<< "\n"
	"window_height : "		<< app_settings.window_height	<< "\n"
	"starting_fractal : "	<< app_settings.starting_fractal<< "\n"
	"max_iters : "			<< app_settings.max_iters		<< "\n"
	"escape_radius : "		<< app_settings.escape_radius	<< "\n"
	<< std::endl;
}

Settings LoadSettings(const std::string& path) {
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
	Settings app_settings;
	if (!(	
			app_settings_store.lookupValue("target_fps", app_settings.target_fps)
		&& 	app_settings_store.lookupValue("sample_rate", app_settings.sample_rate)
		&& 	app_settings_store.lookupValue("max_freq", app_settings.max_freq)
		&& 	app_settings_store.lookupValue("window_width", app_settings.window_width)
		&& 	app_settings_store.lookupValue("window_height", app_settings.window_height)
		&& 	app_settings_store.lookupValue("starting_fractal", app_settings.starting_fractal)
		&& 	app_settings_store.lookupValue("max_iters", app_settings.max_iters)
		&& 	app_settings_store.lookupValue("escape_radius", app_settings.escape_radius)
		&&	app_settings_store.lookupValue("fullscreen", app_settings.fullscreen)
	)) {
		std::cerr << "Settings are wrong !" << std::endl;
		std::getchar();
	}
	print_settings(app_settings);
	return app_settings;
}
#if _WIN32
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
#else
int main(int argc, char *argv[]) {
#endif
	FSE App;
	return App.Init(LoadSettings("default.cfg"));
}
