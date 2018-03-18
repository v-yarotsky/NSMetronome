#ifndef METRONOME_H
#define METRONOME_H

#include "portaudio.h"
#include <string>
#include <list>
#include <chrono>

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

const int MIN_TEMPO = 20;
const int MAX_TEMPO = 350;

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
    void tapTempo();

private:
    PaStream *stream;
    MetronomeState *state;
    std::list<std::chrono::system_clock::time_point> m_tapTempoTimestamps;

    const Sample * loadSample(std::string path);
};

#endif // METRONOME_H
