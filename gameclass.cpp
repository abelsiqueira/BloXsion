#include "gameclass.h"
#include <algorithm>

GameClass::GameClass () {
  srand(time(0));

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

  gridWidth = cWindowWidth/cTileSize;
  gridHeight = cWindowHeight/cTileSize;
  grid = new int*[gridHeight];
  for (size_t i = 0; i < gridHeight; i++) {
    grid[i] = new int[gridWidth];
    for (size_t j = 0; j < gridWidth; j++) {
      grid[i][j] = rand()%cMaxBalls;
    }
  }

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

  hugeFont = al_load_font("DejaVuSans.ttf", 80, 0);
  bigFont = al_load_font("DejaVuSans.ttf", 40, 0);
  normalFont = al_load_font("DejaVuSans.ttf", 20, 0);
  smallFont = al_load_font("DejaVuSans.ttf", 10, 0);

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
  for (size_t i = 0; i < gridHeight; i++)
    delete []grid[i];
  delete []grid;

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
      redraw = true;
    } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      done = true;
    } else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES ||
               ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
    } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {

    } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
      switch (ev.keyboard.keycode) {
        case ALLEGRO_KEY_ESCAPE:
        case ALLEGRO_KEY_Q:
          done = true;
          break;
        default:
          Update();
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

