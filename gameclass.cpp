#include "gameclass.h"
#include <cmath>
#include <algorithm>

GameClass::GameClass () {
  srand(time(0));

  gridHeight = cWindowHeight/cTileSize;
  gridWidth = gridHeight;
  hudWidth = cWindowWidth - cWindowHeight;

  errorValue = (AllegroInitialization() == 0 ? false : true);
  if (errorValue == 0) {
    hasFailed = false;
  } else {
    hasFailed = true;
    switch (errorValue) {
      case 1:
        std::cout << "Allegro error. Something failed to initialize" << std::endl;
        break;
      default:
        std::cout << "Unidentified error." << std::endl;
        break;
    }
    return;
  }
  
  locked = false;
  firstChosen = false;
  swapCount = 0;
  swapping = false;
  lives = 3;
  justMoved = false;
  score = 0;
  numberKilled = 0;
  numberOfObjects = cInitialObjects;

  grid = new int*[gridHeight];
  gridNew = new int*[gridHeight];
  for (size_t i = 0; i < gridHeight; i++) {
    grid[i] = new int[gridWidth];
    gridNew[i] = new int[gridWidth];
    for (size_t j = 0; j < gridWidth; j++) {
      int obj = (i + j)%numberOfObjects;
      grid[i][j] = obj;
      gridNew[i][j] = grid[i][j];
    }
  }
  next = new int[gridWidth];
  for (size_t j = 0; j < gridWidth; j++)
    next[j] = rand()%numberOfObjects;
}

int GameClass::AllegroInitialization () {
  al_init();
  display = al_create_display(cWindowWidth, cWindowHeight);
  if (!display) 
    return 1;
  al_set_window_title(display, "test");
  eventQueue = al_create_event_queue();
  if (!eventQueue)
    return 1;
  timer = al_create_timer(1.0/cFps);
  if (!timer)
    return 1;

  al_init_font_addon();
  al_init_ttf_addon();
  al_init_primitives_addon();
  if (!al_install_keyboard() ||
      !al_install_mouse() ||
      !al_install_audio() ||
      !al_init_image_addon() ||
      !al_init_acodec_addon() ||
      !al_reserve_samples(1))
    return 1;

  al_register_event_source(eventQueue, al_get_display_event_source(display));
  al_register_event_source(eventQueue, al_get_timer_event_source(timer));
  al_register_event_source(eventQueue, al_get_keyboard_event_source());
  al_register_event_source(eventQueue, al_get_mouse_event_source());

  int minSize = hudWidth/25;

  hugeFont = al_load_font("DejaVuSans.ttf", 8*minSize, 0);
  bigFont = al_load_font("DejaVuSans.ttf", 4*minSize, 0);
  normalFont = al_load_font("DejaVuSans.ttf", 2*minSize, 0);
  smallFont = al_load_font("DejaVuSans.ttf", minSize, 0);

  if (!hugeFont || !bigFont || !normalFont || !smallFont)
    return 2;
  
/*   music = al_load_audio_stream("Music/background.ogg", 4, 1024);
 *   if (!music)
 *     return 4;
 *   al_attach_audio_stream_to_mixer(music, al_get_default_mixer());
 *   al_set_audio_stream_gain(music, 0.5);
 *   al_set_audio_stream_playing(music, true);
 *   al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_LOOP);
 */

  return 0;
}

GameClass::~GameClass () {
  for (size_t i = 0; i < gridHeight; i++) {
    delete []grid[i];
    delete []gridNew[i];
  }
  delete []grid;
  delete []gridNew;

  al_destroy_font(bigFont);
  al_destroy_font(normalFont);
  al_destroy_font(smallFont);
  al_destroy_timer(timer);
  al_destroy_event_queue(eventQueue);
  al_destroy_display(display);
}

void GameClass::Run () {
  bool redraw = false;
  done = false;

  al_start_timer(timer);

  while (!done) {
    ALLEGRO_EVENT ev;
    al_wait_for_event(eventQueue, &ev);

    if (ev.type == ALLEGRO_EVENT_TIMER) {
      Update();
      if (lives <= 0)
        done = true;
      redraw = true;
    } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      done = true;
    } else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES ||
               ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
    } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
      if (!locked) {
        int i = ev.mouse.y/cTileSize, j = ev.mouse.x/cTileSize;
        if (firstChosen) {
          if ( fabs(i - (int)iFirst) + fabs(j - (int)jFirst) != 1) {
            firstChosen = false;
          } else {
            iSecond = i;
            jSecond = j;
            firstChosen = false;
            locked = true;
            swapCount = 0;
            swapping = true;
            ALLEGRO_COLOR color1 = GetColor(grid[iFirst][jFirst]),
                          color2 = GetColor(grid[iSecond][jSecond]);
            redDiff   = (color1.r - color2.r)*255/cColorIntensity;
            greenDiff = (color1.g - color2.g)*255/cColorIntensity;
            blueDiff  = (color1.b - color2.b)*255/cColorIntensity;
            justMoved = true;
          }
        } else {
          iFirst = i;
          jFirst = j;
          firstChosen = true;
        }
      } else {
      }
    } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
      switch (ev.keyboard.keycode) {
        case ALLEGRO_KEY_ESCAPE:
        case ALLEGRO_KEY_Q:
          done = true;
          break;
        default:
          break;
      }
    }

    if (redraw && al_is_event_queue_empty(eventQueue)) {
      redraw = false;
      al_clear_to_color(al_map_rgb(0,0,0));

      DrawGame();

      al_flip_display();
    }
  }
}

