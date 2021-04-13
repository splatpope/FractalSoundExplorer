#ifndef SYNTHBASE_INCLUDED
#define SYNTHBASE_INCLUDED
#include <math.h>
#include "Gamma/AudioIO.h"
#include "Gamma/Domain.h"
#include "Gamma/Oscillator.h"
#include "Fractals.hpp"
#include "Synth.hpp"

namespace FSE {

class SynthBase : public gam::AudioCallback {
public:

    inline SynthBase(double sampleRate) {
        audio_io.append(*this);
        initAudio(sampleRate);
    }

    inline void initAudio(double framesPerSecond, unsigned framesPerBuffer=128, unsigned outChans=2, unsigned inChans=0) {
        audio_io.framesPerSecond(framesPerSecond);
        audio_io.framesPerBuffer(framesPerBuffer);
        audio_io.channelsOut(outChans);
        audio_io.channelsIn(inChans);
        gam::sampleRate(framesPerSecond);
    }

    inline gam::AudioIO& audioIO() { return audio_io; }

    inline void start() {
        if (isRunning) {        
            audio_io.stop();
        }
        audio_io.start();
        isRunning = true;
    }

    inline void stop() {
        audio_io.stop();
        isRunning = false;
    }

private:

    bool isRunning {false};
    gam::AudioIO audio_io;
};

class FractalSynth : public SynthBase {
public:

    gam::Sine<> osc;
    gam::Accum<> timer;
    gam::Complex<double> orbit_start {0.0, 0.0};
    Fractal fractal;
    unsigned int max_freq;

    inline FractalSynth(double sampleRate, unsigned int max_freq) : 
    timer {2},
    SynthBase {sampleRate},
    max_freq {max_freq}
    { 
        osc.freq(max_freq);
    }

    inline void onAudio (gam::AudioIOData& io) {
        while (io()) {
            if (timer()) setFreqsFromFractal(fractal);
            float s = osc();
            s *= 0.2;

            io.out(0) = io.out(1) = s;
        }
    }

    template <typename T>
    inline void setFreqFromVec2 (gam::Sine<>& osc, T x, T y) {
        gam::Complex<double> cp{static_cast<double>(x), static_cast<double>(y)};
        osc.freq(max_freq * (cp.arg() / M_PI));
    }

    inline void setFreqsFromFractal(Fractal fractal) {
        gam::Complex<double> orbit = orbit_start;
        fractal(orbit, orbit_start);
        setFreqFromVec2(osc, orbit_start.r, orbit_start.i);
        orbit_start = orbit;
    }
};

}
#endif
