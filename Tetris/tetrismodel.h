#ifndef TETRISMODEL_H
#define TETRISMODEL_H

#include "tetrisitem.h"
class TetrisModel {
  static const int minSpeed_ = 1;
  static const int maxSpeed_ = 25;
  static const int maxTouchCount = 1;

 public:
  TetrisModel(int widthBlocks, int heightBlocks);
  void DoStep();
  bool IsThisGameOver() const;
  int GetScore() const;
  void ResetGame();
  const TetrisItem &GetItem() const;
  const TetrisItem &GetNextItem() const;
  void RotateItem();
  void MoveItemLeft();
  void MoveItemRight();
  void StartDrop();
  void StopDrop();
  int GetWidthBlocks() const;
  int GetHeightBlocks() const;
  int GetWidthPoints() const;
  int GetHeightPoints() const;
  int GetBlockType(int xBlocks, int yBlocks) const;

 private:
  void MoveItemX(int offsetPoints);
  bool IsCollisions(const TetrisItem &item) const;
  bool IsCollisions(int xPoints, int yPoints) const;

  void Clean();
  void IncScore();

 private:
  int widthBlocks_;
  int heightBlocks_;
  std::vector<std::vector<int>> fieldMatrix_;
  int score_;
  int speed_;
  bool isDropEnabled_;
  int itemBottomTouchCounter_;

  TetrisItem activeItem_;
  TetrisItem nextItem_ = TetrisItem::generateRandom();
  bool isGameOver_;
};

#endif  // TETRISMODEL_H
