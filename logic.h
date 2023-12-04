#ifndef LOGIC_H
#define LOGIC_H
#include <QString>
#include <QBrush>
#include "Piece.h"
#include "mainwindow.h"
#include "figures.h"
class Logic
{
private:
    void SetPiece(Piece*, int, int);
    void CopyPiece(Piece, Piece*);
public:
    Logic(MainWindow*);
    void InitPiece();
    void ReloadPiece();
    bool PieceIsBlocked();
    void DropPiece();
    void Gameover();
    void RowFull();
    void BlockPiece();
    bool PieceCanGoRight();
    bool PieceCanGoLeft();
    bool PieceCanRotate(int);
    int GetPieceWidth(Piece);
    int GetPieceHeight(Piece);
    Figures figure;
    int Board[22][12];
    Piece CurrentPiece;
    Piece NextPiece;
    bool gameover;
    bool dropping;
    int rot;
    int piece;
    int next_rot;
    int next_piece;
    int score;
};




#endif // LOGIC_H
