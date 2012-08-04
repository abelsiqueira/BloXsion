#include "gameclass.h"

void GameClass::Update () {
  for (size_t i = 0; i < gridHeight; i++) {
    for (size_t j = 0; j < gridWidth; j++) {
      Verify(i, j);
    }
  }
}

void GameClass::Verify (size_t i, size_t j) {
  if (grid[i][j] == -1)
    return;

  size_t nh = HorizontalConsecutive(grid[i][j], i, j),
      nv = VerticalConsecutive(grid[i][j], i, j);

  bool horz = (nh > 2 ? true : false),
       vert = (nv > 2 ? true : false);

  if (horz) {
    RemoveH(i, j, nh);
  } else if (vert) {
    RemoveV(i, j, nv);
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
  if (j == gridWidth)
    return 0;
  else if (grid[i][j] == type)
    return 1 + HorizontalConsecutive(type, i, j+1);
  else
    return 0;
}

