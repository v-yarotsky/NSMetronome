#ifndef METRONOME_H
#define METRONOME_H

#include "portaudio.h"

struct MetronomeState {
  unsigned int tempo;
  unsigned int beatsPerBar;
  int sampleRate;
  long long framesCount;
  float *samples;
  long long pos;
};

class Metronome {
public:
    Metronome(const char *samplePath);
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
};

#endif // METRONOME_H
