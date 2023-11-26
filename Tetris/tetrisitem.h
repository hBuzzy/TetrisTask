#ifndef TETRISITEM_H
#define TETRISITEM_H
#include <QString>
#include <vector>
static const int blockSize = 12;
static const int halfBlockSize = blockSize / 2;

int blocksToPoints(int x);

class TetrisItem {
 public:
  TetrisItem(int xPoints = 0, int yPoints = 0);
  static TetrisItem generateRandom();

  void SetName(QString name);
  bool IsNull() const;

  void Rotate();

  void SetPosition(int xPoints, int yPoints);
  int GetXPoints() const;
  int GetYPoints() const;

  int GetSizeBlocks() const;
  int GetBlockType(int innerXBlocks, int innerYBlocks) const;
  int GetBlockXPoints(int innerXBlocks) const;
  int GetBlockYPoints(int innerYBlocks) const;

 private:
  explicit TetrisItem(const std::vector<std::vector<int>> &matrix);

 private:
  QString figureName_;
  int xPoints_;
  int yPoints_;
  std::vector<std::vector<int>> figureMatrix_;
};
#endif  // TETRISITEM_H
