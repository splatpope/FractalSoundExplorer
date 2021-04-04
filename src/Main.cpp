#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <unistd.h>
#include "Settings.hpp"
#include "App.hpp"

/*  Kept as reference for future changes
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
		} // -> this means that the first data is always the clicked point

		//Check if paused
		if (audio_pause) {
			return true;
		}

		//Generate the tones
		const int steps = sample_rate / max_freq;		// how many samples at worst does it take to represent a cycle of tones ?
		// unit : [ sample/s / cycle/s] -> [sample/cycle] // default : 48000 / 4000 = 12
		for (int i = 0; i < AUDIO_BUFF_SIZE; i+=2) {	// iterate over each sample couple

			const int j = m_audio_time % steps;			// sample cursor inside $steps samples
			// unit : [samples]
			if (j == 0) {								// have we generated $steps samples ? -> next orbit point

				play_px = play_x;						// clearly we use the previously generated point for tone gen
				play_py = play_y;
				fractal(play_x, play_y, play_cx, play_cy); // which means we sort of pregen the next orbit point, neat
				// in all cases -> play = current point, play_p = previous point

				// reset if we clicked outside the set, obviously (first points might still be inside the escape radius)
				if (play_x*play_x + play_y*play_y > escape_radius_sq) {
					audio_pause = true;
					return true;
				}

				if (normalized) { // at least for mandelbrot
					
					dpx = play_px - play_cx;			// by definition, c is constant and acts as an offset
					dpy = play_py - play_cy;			// so we get the distance from the offset
					// -> offset delta of previous point
					dx = play_x - play_cx;
					dy = play_y - play_cy;
					// -> offset delta of current point

					// actual normalization of both deltas
					if (dx != 0.0 || dy != 0.0) { // even tho the denom is never 0, its still better not to mess with 0
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
				// how do you justify this ?

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
				// more like divide distance by 2 (4) if it's higher than sqrt 2 (2)
				// clamp happens afterwards

				//Lose volume over time unless in sustain mode
				if (!sustain) {
					volume *= 0.9992;
				}
			}

			// so up to now, we have a distance to c or something close

			//Cosine interpolation
			double t = double(j) / double(steps);		// time inside the step window : [0, #steps-1] -> [0, #steps-1 / #steps]
			// unit : [samples]
			t = 0.5 - 0.5*std::cos(t * 3.14159);		// cosine of t, in [0, 1] (more accurately [1, 0])

			// before considering the fractal :
			// every step, we have $steps samples of a cosine of freq = #steps/2 (over the sample rate)
			// which means we get half a cosine sample over #steps samples

			// for the duration needed to represent the maximum freq
			double wx = t*dx + (1.0 - t)*dpx; // or dpx + t*(dx - dpx)
			double wy = t*dy + (1.0 - t)*dpy;

			//Save the audio to the 2 channels
			m_samples[i]   = (int16_t)std::min(std::max(wx * volume, -32000.0), 32000.0); // actual clamp
			m_samples[i+1] = (int16_t)std::min(std::max(wy * volume, -32000.0), 32000.0);
			m_audio_time += 1;

			// result : the buffer holds interleaved cosine values scaled so their frequency depends on distances between orbit point distances
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

#if _WIN32
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
#else
int main(int argc, char *argv[]) {
#endif
	FSE::App fse{FSE::Settings {"default.cfg"}};
	return fse.Start();
}
