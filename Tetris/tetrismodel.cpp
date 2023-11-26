#include "tetrismodel.h"

#include <QDebug>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <stdexcept>

int blocksToPoints(int x) { return x * blockSize; }

TetrisModel::TetrisModel(int widthBlocks, int heightBlocks)
    : widthBlocks_(widthBlocks),
      heightBlocks_(heightBlocks),
      isDropEnabled_(false) {
  srand(time(0));
  ResetGame();
}

const TetrisItem &TetrisModel::GetItem() const { return activeItem_; }

const TetrisItem &TetrisModel::GetNextItem() const { return nextItem_; }

void TetrisModel::DoStep() {
  if (activeItem_.IsNull()) {
    itemBottomTouchCounter_ = 0;
    activeItem_ = nextItem_;
    nextItem_ = TetrisItem::generateRandom();
    int xPoints = blocksToPoints(GetWidthBlocks() / 2);
    if (activeItem_.GetSizeBlocks() % 2 == 1) {
      xPoints += halfBlockSize;
    }
    activeItem_.SetPosition(xPoints, 0);
    if (IsCollisions(activeItem_)) {
      isGameOver_ = true;
    }
  }

  if (IsThisGameOver()) {
    return;
  }

  int speed = isDropEnabled_ ? maxSpeed_ : speed_;
  TetrisItem item = activeItem_;
  item.SetPosition(activeItem_.GetXPoints(), activeItem_.GetYPoints() + speed);

  if (!IsCollisions(item)) {
    activeItem_ = item;
    itemBottomTouchCounter_ = 0;
  } else {
    while (IsCollisions(item)) {
      item.SetPosition(item.GetXPoints(), item.GetYPoints() - 1);
    }

    if (maxTouchCount < itemBottomTouchCounter_) {
      activeItem_ = TetrisItem();
      for (int xBlocks = 0; xBlocks < item.GetSizeBlocks(); ++xBlocks) {
        for (int yBlocks = 0; yBlocks < item.GetSizeBlocks(); ++yBlocks) {
          int blockType = item.GetBlockType(xBlocks, yBlocks);
          if (blockType != 0) {
            int xPoints = item.GetBlockXPoints(xBlocks);
            int yPoints = item.GetBlockYPoints(yBlocks);
            fieldMatrix_[yPoints / blockSize][xPoints / blockSize] = blockType;
          }
        }
      }
      Clean();
    } else {
      activeItem_ = item;
      ++itemBottomTouchCounter_;
    }
  }
}

bool TetrisModel::IsThisGameOver() const { return isGameOver_; }

int TetrisModel::GetScore() const { return score_; }

void TetrisModel::ResetGame() {
  activeItem_ = TetrisItem();
  fieldMatrix_.clear();
  fieldMatrix_.resize(GetHeightBlocks());
  for (std::vector<int> &row : fieldMatrix_) {
    row.resize(GetWidthBlocks());
  }
  speed_ = minSpeed_;
  score_ = 0;
  isGameOver_ = false;
}

void TetrisModel::RotateItem() {
  TetrisItem item = activeItem_;
  item.Rotate();
  if (!IsCollisions(item)) {
    activeItem_ = item;
    return;
  }

  item.SetPosition(item.GetXPoints() + blockSize, item.GetYPoints());
  if (!IsCollisions(item)) {
    activeItem_ = item;
    return;
  }

  item.SetPosition(item.GetXPoints() - blocksToPoints(2), item.GetYPoints());
  if (!IsCollisions(item)) {
    activeItem_ = item;
    return;
  }
}

void TetrisModel::MoveItemLeft() { MoveItemX(-blockSize); }
void TetrisModel::MoveItemRight() { MoveItemX(blockSize); }
void TetrisModel::MoveItemX(int offsetPoints) {
  TetrisItem item = activeItem_;
  item.SetPosition(item.GetXPoints() + offsetPoints, item.GetYPoints());
  if (!IsCollisions(item)) {
    activeItem_ = item;
  }
}

void TetrisModel::StartDrop() { isDropEnabled_ = true; }
void TetrisModel::StopDrop() { isDropEnabled_ = false; }
int TetrisModel::GetWidthBlocks() const { return widthBlocks_; }
int TetrisModel::GetHeightBlocks() const { return heightBlocks_; }
int TetrisModel::GetWidthPoints() const {
  return blocksToPoints(GetWidthBlocks());
}

int TetrisModel::GetHeightPoints() const {
  return blocksToPoints(GetHeightBlocks());
}

int TetrisModel::GetBlockType(int xBlocks, int yBlocks) const {
  static const int BORDER_BLOCK_TYPE = -1;

  if (xBlocks < 0 || GetWidthBlocks() <= xBlocks ||
      GetHeightBlocks() <= yBlocks) {
    return BORDER_BLOCK_TYPE;
  } else if (yBlocks < 0) {
    return 0;
  }

  return fieldMatrix_[yBlocks][xBlocks];
}

bool TetrisModel::IsCollisions(const TetrisItem &item) const {
  for (int xBlocks = 0; xBlocks < item.GetSizeBlocks(); ++xBlocks) {
    for (int yBlocks = 0; yBlocks < item.GetSizeBlocks(); ++yBlocks) {
      if (item.GetBlockType(xBlocks, yBlocks) > 0 &&
          IsCollisions(item.GetBlockXPoints(xBlocks),
                       item.GetBlockYPoints(yBlocks))) {
        return true;
      }
    }
  }

  return false;
}

bool TetrisModel::IsCollisions(int xPoints, int yPoints) const {
  int xBlocks = (xPoints < 0) ? -1 : xPoints / blockSize;
  int yTopBlocks = yPoints - halfBlockSize;
  if (GetBlockType(xBlocks, yTopBlocks / blockSize)) {
    return true;
  }
  int yBottomBlocks = yPoints + halfBlockSize;
  if (yTopBlocks % blockSize != 0 &&
      GetBlockType(xBlocks, yBottomBlocks / blockSize)) {
    return true;
  }

  return false;
}

void TetrisModel::Clean() {
  for (int i = heightBlocks_ - 1; i >= 0; --i) {
    int counter =
        std::accumulate(fieldMatrix_[i].begin(), fieldMatrix_[i].end(), 0,
                        [](int a, int b) { return (b == 0) ? a : a + 1; });

    if (counter == 0) {
      return;
    } else if (counter == GetWidthBlocks()) {
      fieldMatrix_.erase(fieldMatrix_.begin() + i);
      std::vector<int> v(GetWidthBlocks());
      fieldMatrix_.insert(fieldMatrix_.begin(), v);
      IncScore();
      ++i;
    }
  }
}

void TetrisModel::IncScore() { ++score_; }
