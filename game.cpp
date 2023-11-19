#include "game.h"
#include "block.h"
#include "ui_game.h"
#include "gamecontroller.h"
#include "mainwindow.h"

Game::Game(QWidget *parent):
    QWidget(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);
    startMusicPlayer();
    InitGame();
    StartGame();
}

Game::~Game()
{
    delete ui;
}

inline GameState operator|(GameState a, GameState b)
{return static_cast<GameState>(static_cast<int>(a) | static_cast<int>(b));}

void Debug(Game *obj){
    int temp_block1[4][4];
    block_cpy(temp_block1, item7);
    Border temp_border1;
    obj->GetBorder(temp_block1,temp_border1);
}

void Game::startMusicPlayer() {
    music_player_  = new QMediaPlayer;
    music_player_ ->setMedia(QUrl("qrc:/resources/audio/tet.mp3"));
    music_player_ ->setVolume(50);

    connect(music_player_ , &QMediaPlayer::stateChanged, this, &Game::MusicStateChanged);

    toggleMusic();
}

void Game::toggleMusic() {
    if (music_player_->state() == QMediaPlayer::PlayingState) {
        music_player_->pause();
    } else {
        music_player_->play();
    }
}

void Game::MusicStateChanged(QMediaPlayer::State state) {
    if (state == QMediaPlayer::StoppedState) {
        QMediaPlayer* player = qobject_cast<QMediaPlayer*>(sender());
        if (player) {
            player->play();
        }
    }
}

void Game::InitGame()
{
    for(int i=0;i<AREA_ROW;i++){
        for(int j=0;j<AREA_COL;j++){
            game_area_[i][j]=0;
        }
    }

    speed_ms_=800;
    refresh_ms_=3;
    srand(time(0));
    score_=0;
}

void Game::StartGame()
{
    game_state_ = FreeRun;

    game_timer_=startTimer(speed_ms_);
    paint_timer_=startTimer(refresh_ms_);
    int block_id=rand()%7;
    CreateBlock(next_block_,block_id);
    ResetBlock();
    Debug(this);
}

void Game::GameOver()
{
    game_state_ = Game_Over;

    killTimer(game_timer_);
    killTimer(paint_timer_);
    toggleMusic();
    std::string str1 = "Game Over, score: ";
    std::string str2 = std::to_string(score_);
    str1.append(str2);
    const char* charArray = str1.c_str();
    QMessageBox::information(this, "Failed", charArray);
    close();
    handleGameOver();
}

void Game::handleGameOver() {
    MainWindow *MW = new MainWindow();
    MW->show();
}

void Game::SetStartPositionForNewBlock()
{
    block_point start_point;
    start_point.pos_x = AREA_COL / 2 - 2;
    start_point.pos_y = -4;
    block_position_ = start_point;
}

void Game::ResetBlock()
{
    CopyCurrentBlockToCurrent();
    GetCurrentBlockBorder();

    CreateNextBlock();
    SetStartPositionForNewBlock();
}

void Game::BlockMove(Direction dir)
{
    switch (dir) {
    case UP:
    MoveBlockUp();
        break;
    case DOWN:
    MoveBlockDown();
        break;
    case LEFT:
    MoveBlockLeft();
        break;
    case RIGHT:
    MoveBlockRight();
        break;
    case SPACE:
    MoveBlockSpace();
        break;
    default:
        break;
    }


    int i=AREA_ROW-1;
    int line_count=0;
    while(i>=1)
    {
        bool is_line_full=true;
        for(int j=0;j<AREA_COL;j++)
            if(game_area_[i][j]==0)
            {
                is_line_full=false;
                i--;
                break;
            }
        if(is_line_full)
        {
            for(int k=i;k>=1;k--){
                for(int j=0;j<AREA_COL;j++){
                    game_area_[k][j]=game_area_[k-1][j];
                }
            }
            line_count++;
        }
    }

    score_+=line_count*10;
    for(int j=0;j<AREA_COL;j++)
        if(game_area_[0][j]==2){
            GameOver();
        }
}

void Game::BlockRotate(int block[4][4])
{
    int temp_block[4][4];

    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            temp_block[3-j][i]=block[i][j];
        }
    }

    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            block[i][j]=temp_block[i][j];
        }
    }
}

void Game::ConvertStable(int x,int y)
{
    for(int i=current_border_.ubound;i<=current_border_.dbound;i++){
        for(int j=current_border_.lbound;j<=current_border_.rbound;j++){
            if(y+i >= 0 && current_block_[i][j]==1){
                game_area_[y+i][x+j]=2; //x and y is reverse
            }
        }
    }
}

bool Game::IsCollide(int x,int y,Direction dir)
{
    int temp_block[4][4];
    block_cpy(temp_block,current_block_);
    Border temp_border;
    GetBorder(temp_block,temp_border);

    switch(dir)
    {
    case UP:
        BlockRotate(temp_block);
        GetBorder(temp_block,temp_border); //Recompute border atfer rotate.
        break;
    case DOWN:
        y+=1;
        break;
    case LEFT:
        x-=1;
        break;
    case RIGHT:
        x+=1;
        break;
    default:
        break;
    }

    for(int i=temp_border.ubound;i<=temp_border.dbound;i++){
        for(int j=temp_border.lbound;j<=temp_border.rbound;j++){
            if(game_area_[y+i][x+j]==2 && temp_block[i][j]==1
                    ||x+temp_border.lbound<0
                    ||x+temp_border.rbound>AREA_COL-1){
                return true;}
        }
    }

    return false;
}

void Game::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    DrawBackground(painter);
    DrawNextBlock(painter);
    DrawScore(painter);
    DrawGameArea(painter);
}

void Game::timerEvent(QTimerEvent *event)
{
    if(event->timerId()==game_timer_ && game_state_ != Game_Over){
        BlockMove(DOWN);
    }
    if(event->timerId()==paint_timer_){
        update();
    }
}

void Game::keyPressEvent(QKeyEvent *event)
{
    if( game_state_ == FreeRun){

        switch(event->key())
        {
        case Qt::Key_Up:
            BlockMove(UP);
            break;
        case Qt::Key_Down:
            BlockMove(DOWN);
            break;
        case Qt::Key_Left:
            BlockMove(LEFT);
            break;
        case Qt::Key_Right:
            BlockMove(RIGHT);
            break;
        case Qt::Key_Space:
            BlockMove(SPACE);
            break;
        default:
            break;
        }

    }
}
