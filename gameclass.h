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

    void Update ();
    bool Verify (size_t, size_t);
    size_t HorizontalConsecutive (int, size_t, size_t) const;
    size_t VerticalConsecutive (int, size_t, size_t) const;
    void RemoveH (size_t, size_t, size_t);
    void RemoveV (size_t, size_t, size_t);
    void Fall ();
    void Down (size_t, size_t);

    ALLEGRO_COLOR GetColor(int) const;
    void DrawGame () const;
    void DrawHud () const;
    void DrawObject (bool, int, float, float) const;
    void DrawObject (float, float, ALLEGRO_COLOR) const;
    void DrawObject (float, float, float, ALLEGRO_COLOR) const;

    // Game
    int **grid, **gridNew, *next;
    size_t gridWidth, gridHeight;
    bool firstChosen;
    size_t iFirst, jFirst, iSecond, jSecond;
    int redDiff, greenDiff, blueDiff;
    int swapCount;
    bool swapping, justMoved;
    int lives;
    int score, numberKilled;
    int numberOfObjects;

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

    int windowWidth, windowHeight, tileSize;
    
    bool done, locked;

    bool hasFailed;
    int errorValue;
    float falling;

    float hudWidth;
};

#endif
