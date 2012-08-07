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

void GameClass::Update () {
  timeLeft--;
  drawCount += 1;
  if (drawCount >= tileSize)
    drawCount = 0;
  if (numberKilled > (numberOfObjects - cInitialObjects + 1)*cDifficulty) {
    numberOfObjects++;
    if (numberOfObjects > cMaxObjects)
      numberOfObjects = cMaxObjects;
  }
  if (!swapping) {
    bool killed = false;
    if (!locked) {
      for (size_t i = 0; i < gridHeight; i++) {
        for (size_t j = 0; j < gridWidth; j++) {
          if (Verify(i, j))
            killed = true;
        }
      }
    } else {
      Fall();
    }
    justMoved = false;
  } else {
    swapCount += cSwapSpeed;
    if (swapCount > 255) {
      locked = false;
      swapping = false;
      int aux = grid[iFirst][jFirst];
      grid[iFirst][jFirst] = grid[iSecond][jSecond];
      grid[iSecond][jSecond] = aux;
    }
  }
}

void GameClass::Fall () {
  falling -= cFallingSpeed;
  if (falling <= 0) {
    falling = 0;
    locked = false;
    for (size_t i = 0; i < gridHeight; i++) {
      for (size_t j = 0; j < gridWidth; j++) {
        if (grid[i][j] == -1)
          Down(i, j);
      }
    }
    for (size_t i = 0; i < gridHeight; i++) {
      for (size_t j = 0; j < gridWidth; j++) {
        grid[i][j] = gridNew[i][j];
      }
    }
    for (size_t i = 0; i < gridHeight; i++) {
      for (size_t j = 0; j < gridWidth; j++) {
        if (grid[i][j] == -1) {
          locked = true;
          falling = tileSize;
          for (size_t k = 0; k < gridWidth; k++)
            next[k] = rand()%numberOfObjects;
          break;
        }
      }
      if (locked) break;
    }
  }
}

void GameClass::Down (size_t i, size_t j) {
  if (i == 0) {
    gridNew[i][j] = next[j];
    return;
  }
  gridNew[i][j] = grid[i-1][j];
  Down(i-1, j);
}

bool GameClass::Verify (size_t i, size_t j) {
  bool killed = false;
  if (grid[i][j] == -1) {
    locked = true;
    falling = tileSize;
  } else {
    size_t nh = HorizontalConsecutive(grid[i][j], i, j),
        nv = VerticalConsecutive(grid[i][j], i, j);

    bool horz = (nh > 2 ? true : false),
         vert = (nv > 2 ? true : false);

    if (horz) {
      RemoveH(i, j, nh);
      locked = true;
      falling = tileSize;
      score += cScoreReward[nh];
      timeLeft += cTimeReward[nh];
      numberKilled++;
    } else if (vert) {
      RemoveV(i, j, nv);
      locked = true;
      falling = tileSize;
      score += cScoreReward[nv];
      timeLeft += cTimeReward[nv];
      numberKilled++;
    }
  }

  if (locked) {
    for (size_t j = 0; j < gridWidth; j++) {
      for (size_t i = 0; i < gridHeight; i++) {
        gridNew[i][j] = grid[i][j];
      }
      next[j] = rand()%numberOfObjects;
    }
    killed = true;
  }

  return killed;
}

void GameClass::RemoveH (size_t i, size_t j, size_t n) {
  for (size_t jj = j; jj < j + n; jj++) {
    grid[i][jj] = -1;
  }
}

void GameClass::RemoveV (size_t i, size_t j, size_t n) {
  for (size_t ii = i; ii < i + n; ii++) {
    grid[ii][j] = -1;
  }
}

size_t GameClass::HorizontalConsecutive(int type, size_t i, size_t j) const {
  if (j == gridWidth)
    return 0;
  else if (grid[i][j] == type)
    return 1 + HorizontalConsecutive(type, i, j+1);
  else
    return 0;
}

size_t GameClass::VerticalConsecutive(int type, size_t i, size_t j) const {
  if (i == gridHeight)
    return 0;
  else if (grid[i][j] == type)
    return 1 + VerticalConsecutive(type, i+1, j);
  else
    return 0;
}

