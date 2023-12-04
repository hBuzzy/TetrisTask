#include "logic.h"
#include "mainwindow.h"
#include <ctime>
#include <iostream>
MainWindow* main_app;
int num_lines = 0;
Logic::Logic(MainWindow* context)
{
    gameover = false;
    dropping = false;
    NextPiece.PieceColor = -1;
    main_app = context;
    score = 0;
    for (int i = 0; i < 22; i++)
        for (int j = 0; j < 12; j++)
            Board[i][j] = 0;
    std::time_t result = std::time(0);
    srand(result);
    InitPiece();
}
void Logic::SetPiece(Piece* p, int piece_nr, int piece_rot)
{
    p->PieceColor = piece_nr + 1;
    p->X = 3;
    p->Y = 0;
    // int s = piece_nr * (16 * 4) + piece_rot * 16;
    p->PieceArray = figure.getFigure(piece_nr, piece_rot);
}
void Logic::CopyPiece(Piece source, Piece * dest)
{
    dest->PieceColor = source.PieceColor;
    dest->X = source.X;
    dest->Y = source.Y;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            dest->PieceArray[i][j] = source.PieceArray[i][j];
}
int Logic::GetPieceWidth(Piece p)
{
    int maxw = 0;
    for (int j=0; j<4; j++)
    {
        for (int i = 0; i<4; i++)
            if (p.PieceArray[i][j]) { maxw++; break; }
    }
    return maxw * 29;
}
int Logic::GetPieceHeight(Piece p)
{
    int maxh = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            if (p.PieceArray[i][j]) { maxh++; break; }
    }
    return maxh * 29;
}

void Logic::InitPiece()
{
    if (NextPiece.PieceColor == -1) {
        piece = rand() % 6 + 1;
        rot = rand() % 3 + 1;
        SetPiece(&CurrentPiece, piece, rot);
        next_piece = rand() % 7 + 1;
        next_rot = rand() % 4 + 1;
        SetPiece(&NextPiece, next_piece, next_rot);
    } else {
        CopyPiece(NextPiece, &CurrentPiece);
        piece = next_piece;
        rot = next_rot;
        next_piece = rand() % 7 + 1;
        next_rot = rand() % 4 + 1;
        SetPiece(&NextPiece, next_piece, next_rot);
    }
}
void Logic::ReloadPiece()
{
    CurrentPiece.PieceArray = figure.getFigure(piece, rot);
}
bool Logic::PieceIsBlocked()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            if (CurrentPiece.PieceArray[i][j] && Board[CurrentPiece.Y + i + 1][CurrentPiece.X + j] != 0)
                return true;
            if (CurrentPiece.PieceArray[i][j] && i + CurrentPiece.Y + 1 ==20) return true;
        }
   return false;
}
void Logic::DropPiece()
{
    if (!PieceIsBlocked() && !gameover)
        CurrentPiece.Y++;
    else if (gameover)
    {
    }
    else
    {
        BlockPiece();
        Gameover();
        RowFull();
        InitPiece();
    }
}

void Logic::Gameover()
{
    for (int i = 0; i < 10; i++)
        if (Board[0][i] != 0)
        {
            gameover = true;
            main_app->game_over();
            break;
        }
}
void Logic::RowFull()
{
    for (int k = 19; k >= 1; k--)
    {
        int c = 0;
        for (int i = 0; i < 10; i++)
        if (Board[k][i] != 0)
            c++;
    if (c == 10)
    {
        for (int i = k; i >= 1; i--)
            for (int j = 0; j < 10; j++)
                Board[i][j] = Board[i - 1][j];
        score += 100;
        k++;
        num_lines++;
        if (num_lines % 5 == 0)
        main_app->increase_speed();
        }
    }
}
void Logic::BlockPiece()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            if (CurrentPiece.PieceArray[i][j])
            Board[CurrentPiece.Y + i][CurrentPiece.X + j] = CurrentPiece.PieceColor;
        }
    if (dropping)
    {
        main_app->reset_speed();
        dropping = false;
    }
    score += 10;
}

bool Logic::PieceCanGoRight()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            if (CurrentPiece.PieceArray[i][j])
            {
            if (CurrentPiece.X + j + 1 == 10) return false;
                if (Board[CurrentPiece.Y + i][CurrentPiece.X + j + 1] != 0)
            return false;
            }
        }
    return true;
}
bool Logic::PieceCanGoLeft()
{
    if (CurrentPiece.X + 1 == 0) return false;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
            {
                if (CurrentPiece.PieceArray[i][j])
                {
                    if (CurrentPiece.X + j - 1 >= 0)
                    if (Board[CurrentPiece.Y + i][CurrentPiece.X + j - 1] != 0)
                return false;
                }
            }
    return true;
}
bool Logic::PieceCanRotate(int piece_rot)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            bool b = figure.getFigure(piece, piece_rot)[i][j] == 0 ? false : true;
            if (b)
            {
                if (i + CurrentPiece.Y > 19) return false;
                if (j + CurrentPiece.X > 9) return false;
                if (Board[i + CurrentPiece.Y][j + CurrentPiece.X] != 0) return
            false;
            }
        }
    return true;
}
