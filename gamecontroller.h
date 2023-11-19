#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "game.h"

void Game::MoveBlockUp() {
    if(IsCollide(block_position_.pos_x,block_position_.pos_y,UP)){
        return;
    }

    BlockRotate(current_block_);

    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(block_position_.pos_y+i >=0 &&   game_area_[block_position_.pos_y+i][block_position_.pos_x+j] != 2){
                game_area_[block_position_.pos_y+i][block_position_.pos_x+j]=current_block_[i][j];
            }
        }
    }
    GetBorder(current_block_,current_border_);
}

void Game::MoveBlockDown() {
    if(block_position_.pos_y+current_border_.dbound==AREA_ROW-1)
    {
        game_state_ = Wait;

        ConvertStable(block_position_.pos_x,block_position_.pos_y);
        ResetBlock();

        game_state_ = FreeRun;

        return;
    }
    if(IsCollide(block_position_.pos_x,block_position_.pos_y,DOWN))
    {

        game_state_ = Wait;

        ConvertStable(block_position_.pos_x,block_position_.pos_y);
        ResetBlock();

        game_state_ = FreeRun;

        return;
    }
    for(int j=current_border_.lbound;j<=current_border_.rbound;j++){
        if(block_position_.pos_y >=0){
            game_area_[block_position_.pos_y][block_position_.pos_x+j]=0;
        }
    }
    block_position_.pos_y+=1;
    for(int i=0;i<4;i++){
        for(int j=current_border_.lbound;j<=current_border_.rbound;j++){
            if(block_position_.pos_y+i<=AREA_ROW-1&&game_area_[block_position_.pos_y+i][block_position_.pos_x+j]!=2){
                if(block_position_.pos_y+i >=0){
                    game_area_[block_position_.pos_y+i][block_position_.pos_x+j]=current_block_[i][j];
                }
            }
        }
    }
}

void Game::MoveBlockLeft() {
    if(block_position_.pos_x+current_border_.lbound==0||IsCollide(block_position_.pos_x,block_position_.pos_y,LEFT))
        return;
    for(int i=current_border_.ubound;i<=current_border_.dbound;i++){
        if(block_position_.pos_y+i >=0){
            game_area_[block_position_.pos_y+i][block_position_.pos_x+3]=0;
        }
    }
    block_position_.pos_x-=1;
    for(int i=current_border_.ubound;i<=current_border_.dbound;i++){
        for(int j=0;j<4;j++){
            if(block_position_.pos_x+j>=0&&game_area_[block_position_.pos_y+i][block_position_.pos_x+j]!=2){
                if(block_position_.pos_y+i >=0){
                    game_area_[block_position_.pos_y+i][block_position_.pos_x+j]=current_block_[i][j];
                }
            }
        }
    }
}

void Game::MoveBlockRight() {
    if(block_position_.pos_x+current_border_.rbound==AREA_COL-1||IsCollide(block_position_.pos_x,block_position_.pos_y,RIGHT))
        return;
    for(int i=current_border_.ubound;i<=current_border_.dbound;i++){
        if(block_position_.pos_y+i >=0){
            game_area_[block_position_.pos_y+i][block_position_.pos_x]=0;
        }
    }
    block_position_.pos_x+=1;
    for(int i=current_border_.ubound;i<=current_border_.dbound;i++){
        for(int j=0;j<4;j++){
            if(block_position_.pos_x+j<=AREA_COL-1&&game_area_[block_position_.pos_y+i][block_position_.pos_x+j]!=2){
                if(block_position_.pos_y+i >=0){
                    game_area_[block_position_.pos_y+i][block_position_.pos_x+j]=current_block_[i][j];
                }
            }
        }
    }
}

void Game::MoveBlockSpace() {
    game_state_= Wait;

    while(block_position_.pos_y+current_border_.dbound<AREA_ROW-1&&!IsCollide(block_position_.pos_x,block_position_.pos_y,DOWN))
    {
        for(int j=current_border_.lbound;j<=current_border_.rbound;j++){
            if(block_position_.pos_y >=0){
                game_area_[block_position_.pos_y][block_position_.pos_x+j]=0;
            }
        }
        block_position_.pos_y+=1;
        for(int i=0;i<4;i++){
            for(int j=current_border_.lbound;j<=current_border_.rbound;j++){
                if(block_position_.pos_y+i<=AREA_ROW-1&&game_area_[block_position_.pos_y+i][block_position_.pos_x+j]!=2){
                    if(block_position_.pos_y+i >=0){
                        game_area_[block_position_.pos_y+i][block_position_.pos_x+j]=current_block_[i][j];
                    }
                }
            }
        }
    }
    ConvertStable(block_position_.pos_x,block_position_.pos_y);
    ResetBlock();

    game_state_ = FreeRun;
}

#endif // GAMECONTROLLER_H
