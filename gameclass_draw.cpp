/* Copyright 2012 - Abel Soares Siqueira
 * 
 * This file is part of BloXsion.
 * 
 * BloXsion is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BloXsion is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BloXsion.  If not, see <http://www.gnu.org/licenses/>.
 */
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
        DrawObject(fall, type, j*tileSize, i*tileSize);
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
      DrawObject(fall, next[j], j*tileSize, -tileSize);
  }

  if (firstChosen) {
    float x = jFirst*tileSize, y = iFirst*tileSize,
          xf = x + tileSize, yf = y + tileSize;
    al_draw_rectangle(x, y, xf, yf, al_map_rgb(255,255,255), 2);
  }
  
  if (swapping) {
    float xFirst  =  jFirst*tileSize, yFirst  =  iFirst*tileSize,
          xSecond = jSecond*tileSize, ySecond = iSecond*tileSize;

    ALLEGRO_COLOR colorFirst = GetColor(grid[iFirst][jFirst]),
                  colorSecond = GetColor(grid[iSecond][jSecond]);

    float alpha = swapCount/255.0;
    float r1 = colorFirst.r, r2 = colorSecond.r;
    float g1 = colorFirst.g, g2 = colorSecond.g;
    float b1 = colorFirst.b, b2 = colorSecond.b;

    colorFirst.r = alpha*r2 + (1 - alpha)*r1;
    colorFirst.g = alpha*g2 + (1 - alpha)*g1;
    colorFirst.b = alpha*b2 + (1 - alpha)*b1;
    colorSecond.r = alpha*r1 + (1 - alpha)*r2;
    colorSecond.g = alpha*g1 + (1 - alpha)*g2;
    colorSecond.b = alpha*b1 + (1 - alpha)*b2;


    float s = fabs(255.0 - 2*swapCount)/255.0;
    al_draw_filled_rectangle(xFirst, yFirst, xFirst + tileSize, yFirst + tileSize,
        al_map_rgb(0,0,0));
    al_draw_filled_rectangle(xSecond, ySecond, xSecond + tileSize, ySecond + tileSize,
        al_map_rgb(0,0,0));
    DrawObject(xFirst, yFirst, s, colorFirst);
    DrawObject(xSecond, ySecond, s, colorSecond);
  }

  DrawHud();
}

void GameClass::DrawHud() const {
  float x = windowWidth - hudWidth, y = 0,
         xf = windowWidth, yf = windowHeight;
  float d1 = 5;
  float ydiv = (windowHeight - 3*d1)/3;

  al_draw_filled_rectangle(x, y, xf, yf, al_map_rgb(250,250,25));
  x += d1; y += d1; xf -= d1; yf -= d1;
  al_draw_filled_rounded_rectangle(x, y, xf, y+ydiv-d1, 10, 10, al_map_rgb(0,0,0));
  al_draw_filled_rounded_rectangle(x, y+ydiv+d1, xf, y+2*ydiv-d1, 10, 10, al_map_rgb(0,0,0));
  al_draw_filled_rounded_rectangle(x, y+2*ydiv+d1, xf, y+3*ydiv-d1, 10, 10, al_map_rgb(0,0,0));

  al_draw_text(bigFont, al_map_rgb(250,250,0), (x + xf)/2, y + 3*d1, 
      ALLEGRO_ALIGN_CENTRE, "Tempo:");
  std::stringstream aux;

  aux.width(4);
  aux.fill('0');
  aux.precision(1);
  aux.setf(std::ios::fixed,std::ios::floatfield);
  aux << (float)timeLeft/cFps;
  al_draw_text(bigFont, al_map_rgb(250,250,0), (x + xf)/2, y + 3*d1 + ydiv/2, 
      ALLEGRO_ALIGN_CENTRE, aux.str().c_str());

  std::stringstream aux2;
  al_draw_text(bigFont, al_map_rgb(250,250,0), (x + xf)/2, y + 3*d1 + ydiv,
      ALLEGRO_ALIGN_CENTRE, "Pontos:");
  aux2 << score;
  al_draw_text(bigFont, al_map_rgb(250,250,0), (x + xf)/2, y + 3*d1 + 3*ydiv/2,
      ALLEGRO_ALIGN_CENTRE, aux2.str().c_str());

  std::stringstream aux3;
  al_draw_text(bigFont, al_map_rgb(250,250,0), (x + xf)/2, y + 3*d1 + 2*ydiv,
      ALLEGRO_ALIGN_CENTRE, "Objetos:");
  aux3 << numberOfObjects;
  al_draw_text(bigFont, al_map_rgb(250,250,0), (x + xf)/2, y + 3*d1 + 5*ydiv/2,
      ALLEGRO_ALIGN_CENTRE, aux3.str().c_str());
}

ALLEGRO_COLOR GameClass::GetColor (int b) const {
  ALLEGRO_COLOR color;

  float colorOptions[cMaxObjects][3] = {
    { 1, 0, 0 },
    { 0, 1, 0 },
    { 0, 0, 1 },
    { 1, 1, 0 },
    { 1, 0, 1 },
    { 0, 1, 1 },
    { 1, 1, 1 },
    { 0.3, 0.3, 0.3 },
    { 0.8, 0.5, 0.2 },
    { 0.2, 0.8, 0.5 },
    { 0.5, 0.2, 0.8 },
    { 0.2, 0.5, 0.8 },
    { 0.8, 0.2, 0.5 },
    { 0.5, 0.8, 0.2 }
  };

  color = al_map_rgb(cColorIntensity*colorOptions[b][0],
                     cColorIntensity*colorOptions[b][1],
                     cColorIntensity*colorOptions[b][2]);
  return color;
}

void GameClass::DrawObject (bool fall, int b, float x, float y) const {
  ALLEGRO_COLOR color = GetColor(b);

  if (fall)
    y += tileSize - falling;
  DrawObject(x, y, color);
}

void GameClass::DrawObject (float x, float y, ALLEGRO_COLOR color) const {
  DrawObject(x, y, 1, color);
}

void GameClass::DrawObject (float x, float y, float s, ALLEGRO_COLOR color) const {
  if (s < 0 || s > 1) s = 1.0;
  float e = 30*(1 - s);
  float xf = x + tileSize, yf = y + tileSize, 
        d1 = 2+e, d2 = 5+e, d3 = 15+e;
  d1 *= tileSize/100.0;
  d2 *= tileSize/100.0;
  d3 *= tileSize/100.0;

  al_draw_filled_rectangle(x + d1, y + d1, xf - d1, yf - d1, color);
  color.r *= 2;
  color.g *= 2;
  color.b *= 2;
  al_draw_filled_rectangle(x + d2, y + d2, xf - d2, yf - d2, color);
  color.r *= 2;
  color.g *= 2;
  color.b *= 2;
  al_draw_filled_rectangle(x + d3, y + d3, xf - d3, yf - d3, color);

#ifdef TRUE_COLORS
  ALLEGRO_COLOR lineColor = color;

  float porc = drawCount/((float)tileSize);
  lineColor.r *= 4*porc*(1-porc);
  lineColor.g *= 4*porc*(1-porc);
  lineColor.b *= 4*porc*(1-porc);

  DrawLines(x, y, porc, lineColor);
  porc = (drawCount + tileSize/4)%tileSize/((float)tileSize);
  DrawLines(x, y, porc, lineColor);
  porc = (drawCount + 2*tileSize/4)%tileSize/((float)tileSize);
  DrawLines(x, y, porc, lineColor);
  porc = (drawCount + 3*tileSize/4)%tileSize/((float)tileSize);
  DrawLines(x, y, porc, lineColor);
#endif
}

void GameClass::DrawLines (float x, float y, float p, ALLEGRO_COLOR lineColor) const {

  float xf = x + tileSize, yf = y + tileSize,
        xt = p*x + (1-p)*xf,
        xb = p*xf + (1-p)*x,
        yl = p*yf + (1-p)*y,
        yr = p*y + (1-p)*yf;


  al_draw_line(x, yl, xt, y, lineColor, 2);
  al_draw_line(xt, y, xf, yr, lineColor, 2);
  al_draw_line(xf, yr, xb, yf, lineColor, 2);
  al_draw_line(xb, yf, x, yl, lineColor, 2);

  al_draw_line(x, yl, xt, y, lineColor, 2);
  al_draw_line(xt, y, xf, yr, lineColor, 2);
  al_draw_line(xf, yr, xb, yf, lineColor, 2);
  al_draw_line(xb, yf, x, yl, lineColor, 2);
}
