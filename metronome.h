#ifndef METRONOME_H
#define METRONOME_H

#include "portaudio.h"
#include <string>

const int SAMPLE_RATE = 44100;

struct Sample {
  long long framesCount;
  float *frames;
};

struct MetronomeState {
  unsigned int tempo;
  unsigned int beatsPerBar;
  unsigned int barBeatNumber;
  const Sample *sample;
  const Sample *accentSample;
  long long pos;
  bool resetting;
};

class Metronome {
public:
    Metronome(std::string samplePath, std::string accentSamplePath);
    ~Metronome();

    unsigned int tempo();
    void setTempo(unsigned int newTempo);

    unsigned int beatsPerBar();
    void setBeatsPerBar(unsigned int newBeatsPerBar);

    void start();
    void stop();

private:
    PaStream *stream;
    MetronomeState *state;

    const Sample * loadSample(std::string path);
};

#endif // METRONOME_H
