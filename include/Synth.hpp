#ifndef SYNTHBASE_INCLUDED
#define SYNTHBASE_INCLUDED
#include <math.h>
#include "Gamma/AudioIO.h"
#include "Gamma/Envelope.h"
#include "Gamma/Oscillator.h"
#include "Gamma/Filter.h"
#include "Fractals.hpp"
#include "Synth.hpp"

namespace FSE {

class SynthBase : public gam::AudioCallback {
public:

    inline SynthBase(double sampleRate) {
        audio_io.append(*this);
        initAudio(sampleRate);
    }

    inline void initAudio(double framesPerSecond, unsigned framesPerBuffer=4096, unsigned outChans=2, unsigned inChans=0) {
        audio_io.framesPerSecond(framesPerSecond);
        audio_io.framesPerBuffer(framesPerBuffer);
        audio_io.channelsOut(outChans);
        audio_io.channelsIn(inChans);
        gam::sampleRate(framesPerSecond);
    }

    inline gam::AudioIO& audioIO() { return audio_io; }

    inline void start() {
        audio_io.start();
        isRunning = true;
    }

    inline void stop() {
        audio_io.stop();
        isRunning = false;
    }

    protected : bool isRunning {false};
    private : gam::AudioIO audio_io;
};

class FractalSynth : public SynthBase {
public:

    gam::Sine<> osc;
    gam::Accum<> timer;
    gam::AD<> env;
    Complex<> orbit_start {0.0f, 0.0f};
    Complex<> orbit {0.0f, 0.0f};
    Fractal fractal;
    unsigned int max_freq;
    float vol;

    inline FractalSynth(double sampleRate, unsigned int max_freq) : 
    timer {5},
    SynthBase {sampleRate},
    max_freq {max_freq},
    vol {1.0f}
    {
        env.attack(0.1);
        env.decay(0.09);
        timer.phaseMax();
        osc.freq(max_freq);
    }

    inline void onAudio (gam::AudioIOData& io) {
        while (io()) {
            if (timer()) {
                nextPoint(fractal);
                env.reset();
            }

            io.out(0) = io.out(1) = osc() * 0.2f * vol * env();
        }
    }

    inline void setFreqFromCpx (Complex<> cp) {
        gam::real freq = max_freq * std::abs((std::arg(cp) / M_PI));
        // turns out clamping the freqs is good
        freq = std::max(std::min(static_cast<float>(max_freq), freq), 44.0f);
        osc.freq(freq);
    }

    inline void setVolFromCpx (Complex<> cp) {
        vol = 1.0f / std::abs(cp) - 0.5f;
        vol = std::min(vol, 1.0f);
        //printf("\n%f\n", vol);
    }

    inline void nextPoint(Fractal fractal) {
        //Complex<> orbit = orbit_start;
        fractal(orbit, orbit_start);
        setFreqFromCpx(orbit);
        setVolFromCpx(orbit);
        //orbit_start = orbit;
    }

    inline void start() {
        if (!isRunning) {
            SynthBase::start();
        } else {
            nextPoint(fractal);
        }
    }
};

}
#endif
