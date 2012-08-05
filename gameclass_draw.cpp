#include "gameclass.h"
#include <cmath>

void GameClass::DrawGame () const {
  for (size_t j = 0; j < gridWidth; j++) {
    for (size_t i = 0; i < gridHeight; i++) {
      int type = grid[i][j];
      if (type != -1) {
        bool fall = false;
        for (size_t k = i+1; k < gridHeight; k++) {
          if (grid[k][j] == -1) {
            fall = true;
            break;
          }
        }
        DrawObject(fall, type, j*cTileSize, i*cTileSize);
      }
    }
    bool fall = false;
    for (size_t k = 0; k < gridHeight; k++) {
      if (grid[k][j] == -1) {
        fall = true;
        break;
      }
    }
    if (fall)
      DrawObject(fall, next[j], j*cTileSize, -cTileSize);
  }

  if (firstChosen) {
    float x = jFirst*cTileSize, y = iFirst*cTileSize,
          xf = x + cTileSize, yf = y + cTileSize;
    al_draw_rectangle(x, y, xf, yf, al_map_rgb(255,255,255), 2);
  }
}

void GameClass::DrawObject (bool fall, int b, float x, float y) const {
  ALLEGRO_COLOR color;

  int intensity = 48;

  switch (b) {
    case 0:
      color = al_map_rgb(intensity,0,0);
      break;
    case 1:
      color = al_map_rgb(0,intensity,0);
      break;
    case 2:
      color = al_map_rgb(0,0,intensity);
      break;
    case 3:
      color = al_map_rgb(intensity,intensity,0);
      break;
    case 4:
      color = al_map_rgb(intensity,0,intensity);
      break;
    case 5:
      color = al_map_rgb(0,intensity,intensity);
      break;
    default:
      return;
      break;
  }

  if (fall)
    y += cTileSize - falling;
  float xf = x + cTileSize, yf = y + cTileSize,
        d1 = 2, d2 = 5, d3 = 15;

  al_draw_filled_rectangle(x + d1, y + d1, xf - d1, yf - d1, color);
  color.r *= 2;
  color.g *= 2;
  color.b *= 2;
  al_draw_filled_rectangle(x + d2, y + d2, xf - d2, yf - d2, color);
  color.r *= 2;
  color.g *= 2;
  color.b *= 2;
  al_draw_filled_rectangle(x + d3, y + d3, xf - d3, yf - d3, color);
}
