#include "tetrisitem.h"

#include <stdexcept>

#include "tetrismodel.h"

TetrisItem::TetrisItem(int xPoints, int yPoints)
    : xPoints_(xPoints), yPoints_(yPoints) {}

TetrisItem TetrisItem::generateRandom() {
  static std::vector<TetrisItem> ITEMS = {
      TetrisItem({
          {1, 1, 1, 0},
          {1, 0, 1, 0},
          {1, 0, 1, 0},
          {1, 0, 1, 0},

      }),
      TetrisItem({
          {2, 2, 2, 0},
          {2, 0, 2, 0},
          {2, 0, 2, 0},
          {2, 2, 2, 0},
      }),
      TetrisItem({
          {0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 3, 3, 3, 0},
          {0, 0, 0, 3, 0, 0, 0},
          {0, 0, 0, 3, 0, 0, 0},
          {0, 0, 0, 3, 3, 3, 0},
          {0, 0, 0, 0, 0, 0, 0},

      }),
      TetrisItem({
          {0, 4, 4, 4, 0},
          {0, 0, 4, 0, 0},
          {0, 0, 4, 0, 0},
          {0, 0, 4, 0, 0},
      }),
      TetrisItem({
          {0, 0, 5, 0, 0},
          {0, 5, 0, 5, 0},
          {0, 5, 5, 5, 0},
          {0, 5, 0, 5, 0},
      }),
      TetrisItem({
          {0, 6, 6, 6, 0},
          {0, 6, 0, 6, 0},
          {0, 6, 6, 6, 0},
          {0, 6, 0, 6, 0},
          {0, 6, 6, 6, 0},

      }),
      TetrisItem({
          {0, 7, 0, 0, 0},
          {0, 7, 0, 0, 0},
          {0, 7, 7, 7, 0},
          {0, 7, 0, 7, 0},
          {0, 7, 7, 7, 0},
      }),
      TetrisItem({
          {0, 8, 8, 8, 0},
          {0, 0, 8, 0, 0},
          {0, 0, 8, 0, 0},
          {0, 0, 8, 0, 0},
      }),
      TetrisItem({
          {0, 9, 9, 9, 0},
          {0, 9, 0, 0, 0},
          {0, 9, 9, 9, 0},
          {0, 9, 0, 0, 0},
          {0, 9, 9, 9, 0},
      }),
      TetrisItem({
          {0, 10, 10, 10, 0},
          {0, 10, 0, 0, 0},
          {0, 10, 10, 10, 0},
          {0, 0, 0, 10, 0},
          {0, 10, 10, 10, 0},
      }),
  };

  int type = rand() % ITEMS.size();
  return ITEMS[type];
}

bool TetrisItem::IsNull() const { return figureMatrix_.empty(); }

void TetrisItem::Rotate() {
  std::vector<std::vector<int>> rotatedMatrix(GetSizeBlocks());
  for (int i = 0; i < GetSizeBlocks(); ++i) {
    rotatedMatrix[i].resize(GetSizeBlocks());
    for (int j = 0; j < GetSizeBlocks(); ++j) {
      rotatedMatrix[i][j] = figureMatrix_[j][GetSizeBlocks() - 1 - i];
    }
  }

  figureMatrix_ = rotatedMatrix;
}

void TetrisItem::SetPosition(int xPoints, int yPoints) {
  xPoints_ = xPoints;
  yPoints_ = yPoints;
}

int TetrisItem::GetXPoints() const { return xPoints_; }

int TetrisItem::GetYPoints() const { return yPoints_; }

int TetrisItem::GetSizeBlocks() const { return figureMatrix_.size(); }

int TetrisItem::GetBlockType(int innerXBlocks, int innerYBlocks) const {
  if (innerXBlocks < 0 || GetSizeBlocks() <= innerXBlocks || innerYBlocks < 0 ||
      GetSizeBlocks() <= innerYBlocks) {
    return 0;
  }

  return figureMatrix_[innerYBlocks][innerXBlocks];
}

int TetrisItem::GetBlockXPoints(int innerXBlocks) const {
  int innerXPoints = blocksToPoints(innerXBlocks) + halfBlockSize;
  int innerXCenterPoints = blocksToPoints(GetSizeBlocks()) / 2;
  return xPoints_ - innerXCenterPoints + innerXPoints;
}

int TetrisItem::GetBlockYPoints(int innerYBlocks) const {
  int innerYPoints = blocksToPoints(innerYBlocks) + halfBlockSize;
  int innerYCenterPoints = blocksToPoints(GetSizeBlocks()) / 2;
  return yPoints_ - innerYCenterPoints + innerYPoints;
}

TetrisItem::TetrisItem(const std::vector<std::vector<int>> &matrix)
    : TetrisItem() {
  figureMatrix_ = matrix;
}
