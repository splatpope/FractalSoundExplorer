#ifndef SYNTHBASE_INCLUDED
#define SYNTHBASE_INCLUDED
#include <math.h>
#include "Gamma/AudioIO.h"
#include "Gamma/Domain.h"
#include "Gamma/Oscillator.h"

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
    }

    inline void stop() {
        audio_io.stop();
    }

private:

    gam::AudioIO audio_io;
};

class FractalSynth : public SynthBase {
public:

    gam::Sine<> osc;
    gam::Accum<> timer;
    unsigned int max_freq;

    inline FractalSynth(double sampleRate, unsigned int max_freq) : 
    SynthBase {sampleRate},
    max_freq {max_freq}
    { 
        osc.freq(max_freq);
    }

    inline void onAudio (gam::AudioIOData& io) {
        while (io()) {
            float s = osc();
            s *= 0.2;

            io.out(0) = io.out(1) = s;
        }
    }

    template <typename T>
    inline void setFreqFromVec2 (T x, T y) {
        gam::Complex<double> cp{static_cast<double>(x), static_cast<double>(y)};
        osc.freq(max_freq * (cp.arg() / M_PI));
    }
};

}
#endif
