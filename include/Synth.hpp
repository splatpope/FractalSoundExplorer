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

    inline void initAudio(double framesPerSecond, unsigned framesPerBuffer=128, unsigned outChans=2, unsigned inChans=0) {
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
    Complex<> orbit_start {0.0, 0.0};
    Fractal fractal;
    unsigned int max_freq;

    inline FractalSynth(double sampleRate, unsigned int max_freq) : 
    timer {2},
    SynthBase {sampleRate},
    max_freq {max_freq}
    {
        env.attack(0.2);
        env.decay(0.5);
        timer.phaseMax();
        osc.freq(max_freq);
    }

    inline void onAudio (gam::AudioIOData& io) {
        while (io()) {
            if (timer()) {
                nextFreq(fractal);
                env.reset();
            }

            io.out(0) = io.out(1) = osc() * env() * 0.2f;
        }
    }

    inline void setFreqFromCpx (Complex<> cp) {
        gam::real freq = max_freq * std::abs((std::arg(cp) / M_PI));
        freq = std::max(std::min(static_cast<float>(max_freq), freq), 1.0f);
        osc.freq(freq);
    }

    inline void nextFreq(Fractal fractal) {
        Complex<> orbit = orbit_start;
        fractal(orbit, orbit_start);
        setFreqFromCpx(orbit_start);
        orbit_start = orbit;
    }

    inline void start() {
        if (!isRunning) {
            SynthBase::start();
        } else {
            nextFreq(fractal);
        }
    }
};

}
#endif
