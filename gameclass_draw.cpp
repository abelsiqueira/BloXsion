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
  
  if (swapping) {
    float xFirst  =  jFirst*cTileSize, yFirst  =  iFirst*cTileSize,
          xSecond = jSecond*cTileSize, ySecond = iSecond*cTileSize;

    ALLEGRO_COLOR colorFirst = GetColor(grid[iFirst][jFirst]),
                  colorSecond = GetColor(grid[iSecond][jSecond]);

    colorFirst.r -= (redDiff*swapCount*cColorIntensity)/(255.0*255.0);
    colorFirst.g -= (greenDiff*swapCount*cColorIntensity)/(255.0*255.0);
    colorFirst.b -= (blueDiff*swapCount*cColorIntensity)/(255.0*255.0);
    colorSecond.r += (redDiff*swapCount*cColorIntensity)/(255.0*255.0);
    colorSecond.g += (greenDiff*swapCount*cColorIntensity)/(255.0*255.0);
    colorSecond.b += (blueDiff*swapCount*cColorIntensity)/(255.0*255.0);


    float s = fabs(255 - 2*swapCount)/255.0;
    al_draw_filled_rectangle(xFirst, yFirst, xFirst + cTileSize, yFirst + cTileSize,
        al_map_rgb(0,0,0));
    al_draw_filled_rectangle(xSecond, ySecond, xSecond + cTileSize, ySecond + cTileSize,
        al_map_rgb(0,0,0));
    DrawObject(xFirst, yFirst, s, colorFirst);
    DrawObject(xSecond, ySecond, s, colorSecond);
  }

  DrawHud();
}

void GameClass::DrawHud() const {
  float x = cWindowWidth - hudWidth, y = 0,
         xf = cWindowWidth, yf = cWindowHeight;
  float d1 = 5;

  al_draw_filled_rectangle(x, y, xf, yf, al_map_rgb(250,250,25));
  x += d1; y += d1; xf -= d1; yf -= d1;
  al_draw_filled_rounded_rectangle(x, y, xf, yf, 50, 50, al_map_rgb(0,10,50));

  al_draw_text(bigFont, al_map_rgb(250,250,0), (x + xf)/2, y + 50, 
      ALLEGRO_ALIGN_CENTRE, "Vidas:");
  std::stringstream aux;
  aux << lives;
  al_draw_text(bigFont, al_map_rgb(250,250,0), (x + xf)/2, y + 100, 
      ALLEGRO_ALIGN_CENTRE, aux.str().c_str());
}

ALLEGRO_COLOR GameClass::GetColor (int b) const {
  ALLEGRO_COLOR color;

  switch (b) {
    case 0:
      color = al_map_rgb(cColorIntensity,0,0);
      break;
    case 1:
      color = al_map_rgb(0,cColorIntensity,0);
      break;
    case 2:
      color = al_map_rgb(0,0,cColorIntensity);
      break;
    case 3:
      color = al_map_rgb(cColorIntensity,cColorIntensity,0);
      break;
    case 4:
      color = al_map_rgb(cColorIntensity,0,cColorIntensity);
      break;
    case 5:
      color = al_map_rgb(0,cColorIntensity,cColorIntensity);
      break;
    default:
      return color;
      break;
  }
  return color;
}

void GameClass::DrawObject (bool fall, int b, float x, float y) const {
  ALLEGRO_COLOR color = GetColor(b);

  if (fall)
    y += cTileSize - falling;
  DrawObject(x, y, color);
}

void GameClass::DrawObject (float x, float y, ALLEGRO_COLOR color) const {
  float xf = x + cTileSize, yf = y + cTileSize, 
        d1 = 2*cTileSize/100.0, d2 = 5*cTileSize/100.0, d3 = 15*cTileSize/100.0;

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

void GameClass::DrawObject (float x, float y, float s, ALLEGRO_COLOR color) const {
  if (s < 0 || s > 1) s = 1.0;
  float e = 30*(1 - s);
  float xf = x + cTileSize, yf = y + cTileSize, 
        d1 = 2+e, d2 = 5+e, d3 = 15+e;
  d1 *= cTileSize/100.0;
  d2 *= cTileSize/100.0;
  d3 *= cTileSize/100.0;

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
