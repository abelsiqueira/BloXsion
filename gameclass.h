#ifndef gameclass_h
#define gameclass_h

#include <list>
#include <string>
#include <cassert>
#include <sstream>
#include <fstream>
#include <iostream>
#include "definitions.h"
#include "allegro_includes.h"

class GameClass {
  public:
    GameClass ();
    ~GameClass ();

    void Run ();
    bool HasFailed () const { return hasFailed; }
  private:
    int AllegroInitialization ();

    void DrawGame () const;

    // Allegro
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *eventQueue;
    ALLEGRO_TIMER *timer;
    ALLEGRO_FONT *smallFont;
    ALLEGRO_FONT *normalFont;
    ALLEGRO_FONT *bigFont;
    ALLEGRO_FONT *hugeFont;
    ALLEGRO_BITMAP *level;
    ALLEGRO_AUDIO_STREAM *music;
    
    bool done;

    bool hasFailed;
    int errorValue;
};

#endif
