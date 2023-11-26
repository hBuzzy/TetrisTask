#include "tetrisview.h"

#include <QDebug>
#include <QGridLayout>
#include <QGroupBox>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QtMath>

static const int pointToPixel = 2;

int modelPointsToPixels(int x) { return x * pointToPixel; }

static const int blockSizePixels = modelPointsToPixels(blockSize);
static const int halfBlockSizePixel = blockSizePixels / 2;

void TetrisView::SetCells() {
  qDebug() << "Количество строчек равняется" << rowsNumber_
           << " Число столбцов равняется " << columnsNumber_;
}

TetrisView::TetrisView(QWidget *parent) : QWidget(parent) {
  SetColumnsNumber(15);
  SetRowsNumber(25);
  model_ = new TetrisModel(columnsNumber_, rowsNumber_);
  width_ = modelPointsToPixels(model_->GetWidthPoints());
  height_ = modelPointsToPixels(model_->GetHeightPoints());
  width_extension_ = 350;

  connect(this, &TetrisView::InitialisationStarted, this, &TetrisView::SetCells,
          Qt::QueuedConnection);
  emit InitialisationStarted();

  controller_ = new TetrisController(model_, this, this);
  controller_->Start();
  resize(width_ + width_extension_, height_);
  QMenuBar *menuBar = new QMenuBar(this);
  QMenu *menu = menuBar->addMenu("Меню");
  menu->setStyleSheet("background-color: rgb(255, 209, 220); color:grey");
  QAction *start = new QAction("Новая игра");
  connect(start, &QAction::triggered, this, [=]() { model_->ResetGame(); });
  menu->addAction(start);
  connect(start, &QAction::triggered, this, [=]() {
    QMessageBox aboutMsgBox;
    aboutMsgBox.setStyleSheet(
        "background-color: rgb(255, 209, 220); color:grey");
    aboutMsgBox.setWindowTitle("Новая игра");
    aboutMsgBox.setText("Жаль, что вы не смогли достойно проиграть");
    aboutMsgBox.exec();
  });
  QAction *exit = new QAction("Выход");

  connect(exit, &QAction::triggered, this, &QApplication::quit);
  menu->addAction(exit);
  menuBar->addMenu(menu);

  QAction *aboutGame = new QAction("Подсказки для игры");
  connect(aboutGame, &QAction::triggered, this, [=]() {
    QMessageBox aboutMsgBox;
    aboutMsgBox.setStyleSheet(
        "background-color: rgb(255, 209, 220); color:grey");
    aboutMsgBox.setWindowTitle("Подсказки для игры");
    aboutMsgBox.setText(
        "Стрелка вверх: поворот фигурки\n"
        "Стрелки влево и вправо: перемещение фигурки\n"
        "Стрелка вниз: ускорение падения фигурки\n"
        "Пробел: пауза/продолжить");
    aboutMsgBox.exec();
  });
  menuBar->addAction(aboutGame);

  QAction *developerSuggestion = new QAction("От разработчика");
  connect(developerSuggestion, &QAction::triggered, this, [=]() {
    QMessageBox aboutMsgBox;
    aboutMsgBox.setStyleSheet(
        "background-color: rgb(255, 209, 220); color:grey");
    aboutMsgBox.setWindowTitle("Выгодное предложение");
    aboutMsgBox.setText(
        "А может быть зачтем это как половинку три в ряд(или хотя бы как "
        "четвертинку), цвета то у фигурок разные (и очень красивые)\n");
    aboutMsgBox.exec();
  });
  menuBar->addAction(developerSuggestion);

  QGroupBox *scoreGB = new QGroupBox(this);
  scoreGB->setGeometry(0, 20, 300, 60);
  scoreGB->setStyleSheet("border:none");

  QGridLayout *scoreLayout = new QGridLayout;
  scoreLayout->setSpacing(0);
  scoreGB->setLayout(scoreLayout);

  QLabel *scoreTitle = new QLabel;
  scoreTitle->setText("Счёт:");
  scoreLayout->addWidget(scoreTitle, 1, 1);

  scoreNumber_ = new QLCDNumber;
  scoreNumber_->display(model_->GetScore());
  scoreLayout->addWidget(scoreNumber_, 1, 2);

  QPushButton *newGameBtn = new QPushButton("Новая игра", this);
  connect(newGameBtn, &QPushButton::clicked, this,
          [=]() { model_->ResetGame(); });

  QPushButton *exitBtn = new QPushButton("Выйти", this);
  connect(exitBtn, &QPushButton::clicked, this, &QApplication::quit);

  menu->addAction(exit);
  newGameBtn->setFocusPolicy(Qt::NoFocus);
  exitBtn->setFocusPolicy(Qt::NoFocus);
  exitBtn->setStyleSheet("background-color: rgb(255, 209, 220); color:grey");
  newGameBtn->setGeometry(10, 70, 100, 30);
  exitBtn->setGeometry(110, 70, 130, 30);
  newGameBtn->setStyleSheet("background-color:rgb(255, 209, 220); color:grey");
  QGroupBox *nextFigureGB = new QGroupBox(this);
  nextFigureGB->setTitle("Следующая фигура");
  nextFigureGB->setGeometry(50, 120, 250, 200);

  QGridLayout *nextFigureLayout = new QGridLayout;
  nextFigureGB->setLayout(nextFigureLayout);
  nextFigureLayout->setContentsMargins(40, 125, 0, 0);

  nextFigureName_ = new QLabel;
  nextFigureLayout->addWidget(nextFigureName_);
}

TetrisView::~TetrisView() {}

void TetrisView::SetNextFigureName(int nameType) {
  QString name;
  switch (nameType) {
    case 1:
      name = "П";
      break;
    case 2:
      name = "О";
      break;
    case 3:
      name = "С";
      break;
    case 4:
      name = "Т";
      break;
    case 5:
      name = "А";
      break;
    case 6:
      name = "В";
      break;
    case 7:
      name = "Ь";
      break;
    case 8:
      name = "Т";
      break;
    case 9:
      name = "Е";
      break;
    case 10:
      name = "5";
      break;
    default:
      name = "Пожалуйста(";
  };
  nextFigureName_->setText(name);
}

void TetrisView::Refresh() { repaint(); }

uint TetrisView::GetRowsNumber() const { return rowsNumber_; }

void TetrisView::SetRowsNumber(uint rowsNumber) {
  if (rowsNumber_ == rowsNumber) return;
  rowsNumber_ = rowsNumber;
  emit RowsNumberChanged();
}

uint TetrisView::GetColumnsNumber() const { return columnsNumber_; }

void TetrisView::SetColumnsNumber(uint columnsCount) {
  if (columnsNumber_ == columnsCount) return;
  columnsNumber_ = columnsCount;
  emit ColumnsNumberChanged();
}

void TetrisView::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.fillRect(width_extension_, 0, width_, height_, Qt::gray);
  scoreNumber_->display(model_->GetScore());
  painter.setPen(QColor(255, 209, 220));
  for (int x = blockSizePixels; x < width_; x += blockSizePixels) {
    painter.drawLine(x + width_extension_, 0, x + width_extension_, height_);
  }
  for (int y = blockSizePixels; y < height_; y += blockSizePixels) {
    painter.drawLine(width_extension_, y, width_ + width_extension_, y);
  }
  for (int x = 0; x < model_->GetWidthBlocks(); ++x) {
    for (int y = 0; y < model_->GetHeightBlocks(); ++y) {
      DrawBlock(blocksToPoints(x) + halfBlockSize,
                blocksToPoints(y) + halfBlockSize, model_->GetBlockType(x, y),
                &painter);
    }
  }

  const TetrisItem &nextItem = model_->GetNextItem();
  SetNextFigureName(nextItem.GetBlockType(qCeil(nextItem.GetSizeBlocks() / 2),
                                          qCeil(nextItem.GetSizeBlocks() / 2)));
  for (int x = 0; x < nextItem.GetSizeBlocks(); ++x) {
    for (int y = 0; y < nextItem.GetSizeBlocks(); ++y) {
      DrawBlock(nextItem.GetBlockXPoints(x) - 85,
                nextItem.GetBlockYPoints(y) + 110, nextItem.GetBlockType(x, y),
                &painter);
    }
  }

  const TetrisItem &item = model_->GetItem();

  for (int x = 0; x < item.GetSizeBlocks(); ++x) {
    for (int y = 0; y < item.GetSizeBlocks(); ++y) {
      DrawBlock(item.GetBlockXPoints(x), item.GetBlockYPoints(y),
                item.GetBlockType(x, y), &painter);
    }
  }
}

void TetrisView::keyPressEvent(QKeyEvent *pressedKey) {
  switch (pressedKey->key()) {
    case Qt::Key_Left:
      controller_->MoveLeft();
      break;
    case Qt::Key_Right:
      controller_->MoveRight();
      break;
    case Qt::Key_Up:
      controller_->Rotate();
      break;
    case Qt::Key_Down:
      controller_->DropEnabled(true);
      break;
    case Qt::Key_Space:
      controller_->TogglePause();
    default:
      QWidget::keyPressEvent(pressedKey);
  }
}

void TetrisView::keyReleaseEvent(QKeyEvent *e) {
  switch (e->key()) {
    case Qt::Key_Down:
      controller_->DropEnabled(false);
      break;
    default:
      QWidget::keyPressEvent(e);
  }
}

void TetrisView::DrawBlock(int xPoints, int yPoints, int type,
                           QPainter *painter) {
  static const std::vector<QColor> colorTable = {
      QColor(255, 209, 220), QColor(252, 15, 192),  QColor(255, 225, 245),
      QColor(255, 240, 245), QColor(255, 245, 238), QColor(255, 192, 203),
      QColor(255, 0, 153),   QColor(255, 20, 147)};
  if (type <= 0) {
    return;
  }
  int color_numb = rand() % colorTable.size();
  int xPixels = modelPointsToPixels(xPoints) - halfBlockSizePixel;
  int yPixels = modelPointsToPixels(yPoints) - halfBlockSizePixel;
  painter->fillRect(xPixels + width_extension_, yPixels, blockSizePixels,
                    blockSizePixels, colorTable[color_numb]);
}
