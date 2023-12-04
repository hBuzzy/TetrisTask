#ifndef PIECE_H
#define PIECE_H
#include <QBrush>
#include <vector>
class Piece
{
public:
    std::vector<std::vector<int> > PieceArray;
    int PieceColor;
    int X, Y;
};
#endif // PIECE_H
