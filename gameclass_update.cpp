#include "gameclass.h"

void GameClass::Update () {
  if (!locked) {
    for (size_t i = 0; i < gridHeight; i++) {
      for (size_t j = 0; j < gridWidth; j++) {
        Verify(i, j);
      }
    }
  } else {
    Fall();
  }
}

void GameClass::Fall () {
  falling -= fallingSpeed;
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
          falling = cTileSize;
          break;
        }
      }
      if (locked) break;
    }
  }
}

void GameClass::Down (size_t i, size_t j) {
  if (i == 0) {
    gridNew[i][j] = rand()%cMaxBalls;
    return;
  }
  gridNew[i][j] = grid[i-1][j];
  Down(i-1, j);
}

void GameClass::Verify (size_t i, size_t j) {
  if (grid[i][j] == -1) {
    locked = true;
    falling = cTileSize;
  } else {
    size_t nh = HorizontalConsecutive(grid[i][j], i, j),
        nv = VerticalConsecutive(grid[i][j], i, j);

    bool horz = (nh > 2 ? true : false),
         vert = (nv > 2 ? true : false);

    if (horz) {
      RemoveH(i, j, nh);
      locked = true;
      falling = cTileSize;
    } else if (vert) {
      RemoveV(i, j, nv);
      locked = true;
      falling = cTileSize;
    }
  }

  if (locked) {
    for (size_t i = 0; i < gridHeight; i++) {
      for (size_t j = 0; j < gridWidth; j++) {
        gridNew[i][j] = grid[i][j];
      }
    }
  }
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

