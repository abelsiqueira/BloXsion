#include "gameclass.h"
#include <cmath>

void GameClass::DrawGame () const {
  for (size_t i = 0; i < gridHeight; i++) {
    for (size_t j = 0; j < gridWidth; j++) {
      DrawBall(grid[i][j], j*cTileSize, i*cTileSize);
    }
  }
}

void GameClass::DrawBall (int b, float x, float y) const {
  ALLEGRO_COLOR color;

  switch (b) {
    case 0:
      color = al_map_rgb(50,0,0);
      break;
    case 1:
      color = al_map_rgb(0,50,0);
      break;
    case 2:
      color = al_map_rgb(0,0,50);
      break;
    case 3:
      color = al_map_rgb(50,50,0);
      break;
    case 4:
      color = al_map_rgb(50,0,50);
      break;
    case 5:
      color = al_map_rgb(0,50,50);
      break;
    default:
      return;
      break;
  }

  static float count = 0;
  count += ALLEGRO_PI/5000;
  if (count > 2*ALLEGRO_PI)
    count = 0;

  float cx = x + cTileSize/2, cy = y + cTileSize/2, radius = cTileSize*2/5;
  float mx = cos(count), my = sin(count);

  al_draw_filled_circle(cx, cy, radius, color);
//  al_draw_filled_circle(x + cTileSize/2, y + cTileSize/2, cTileSize*2/5, color);
  color.r *= 2;
  color.g *= 2;
  color.b *= 2;
  al_draw_filled_circle(cx + mx, cy + my, radius - 4, color);
//  al_draw_filled_circle(x + cTileSize/2+1, y + cTileSize/2+1, 0.95*cTileSize*2/5, color);
  color.r *= 2;
  color.g *= 2;
  color.b *= 2;
  al_draw_filled_circle(cx + 2*mx, cy + 2*my, radius - 10, color);
//  al_draw_filled_circle(x + cTileSize/2+3, y + cTileSize/2+3, 0.8*cTileSize*2/5, color);
}
