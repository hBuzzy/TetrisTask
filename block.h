#ifndef BLOCK_H
#define BLOCK_H

#include "game.h"
#include "items.h"

inline void block_cpy(int dblock[4][4],int sblock[4][4])
{
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            dblock[i][j]=sblock[i][j];
        }
    }
}

void Game::CopyCurrentBlockToCurrent()
{
    block_cpy(current_block_, next_block_);
}

void Game::GetCurrentBlockBorder()
{
    GetBorder(current_block_, current_border_);
}

void Game::CreateNextBlock()
{
    int block_id = rand() % 7;
    CreateBlock(next_block_, block_id);
}

void Game::GetBorder(int block[4][4],Border &border)
{
    //Calculate border of UP, DOWN, LEFT, RIGHT
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(block[i][j]==1){
                border.dbound=i;
                break;
            }
        }
    }

    for(int i=3;i>=0;i--){
        for(int j=0;j<4;j++){
            if(block[i][j]==1){
                border.ubound=i;
                break;
            }
        }
    }

    for(int j=0;j<4;j++){
        for(int i=0;i<4;i++){
            if(block[i][j]==1){
                border.rbound=j;
                break;
            }
        }
    }

    for(int j=3;j>=0;j--){
        for(int i=0;i<4;i++){
            if(block[i][j]==1){
                border.lbound=j;
                break;
            }
        }
    }
}

void Game::DrawBackground(QPainter &painter) {
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawRect(MARGIN, MARGIN, AREA_COL * BLOCK_SIZE, AREA_ROW * BLOCK_SIZE);
}

void Game::DrawNextBlock(QPainter &painter) {
    painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (next_block_[i][j] == 1) {
                painter.drawRect(MARGIN * 3 + AREA_COL * BLOCK_SIZE + j * BLOCK_SIZE,
                                 MARGIN + i * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
            }
        }
    }
}

void Game::DrawScore(QPainter &painter) {
    QFontDatabase::addApplicationFont(":/src/font/pixel.ttf");
    painter.setPen(Qt::black);
    painter.setFont(QFont(default_font_family, default_font_size));

    painter.drawText(QRect(MARGIN * 3 + AREA_COL * BLOCK_SIZE, MARGIN * 2 + 4 * BLOCK_SIZE,
                           BLOCK_SIZE * 4, BLOCK_SIZE * 4),
                     Qt::AlignCenter, "SCORE: " + QString::number(score_));
}

void Game::DrawGameArea(QPainter &painter) {
    for (int i = 0; i < AREA_ROW; i++) {
        for (int j = 0; j < AREA_COL; j++) {
            if (game_area_[i][j] == 1) {
                DrawBlock(painter, Qt::red, j * BLOCK_SIZE + MARGIN, i * BLOCK_SIZE + MARGIN);
            } else if (game_area_[i][j] == 2) {
                DrawBlock(painter, Qt::green, j * BLOCK_SIZE + MARGIN, i * BLOCK_SIZE + MARGIN);
            }
        }
    }
}

void Game::DrawBlock(QPainter &painter, const QColor &color, int x, int y) {
    painter.setBrush(QBrush(color, Qt::SolidPattern));
    painter.drawRect(x, y, BLOCK_SIZE, BLOCK_SIZE);
}

void Game::CreateBlock(int block[4][4],int block_id)
{
    switch (block_id)
    {
    case 0:
        block_cpy(block,item1);
        break;
    case 1:
        block_cpy(block,item2);
        break;
    case 2:
        block_cpy(block,item3);
        break;
    case 3:
        block_cpy(block,item4);
        break;
    case 4:
        block_cpy(block,item5);
        break;
    case 5:
        block_cpy(block,item6);
        break;
    case 6:
        block_cpy(block,item7);
        break;
    default:
        break;
    }
}

#endif // BLOCK_H
