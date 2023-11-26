#ifndef TETRISCONTROLLER_H
#define TETRISCONTROLLER_H

#include <QObject>
#include <QTimer>
class TetrisModel;
class TetrisView;

class TetrisController : QObject {
  Q_OBJECT
  static const int startSpeed = 10;

 public:
  TetrisController(TetrisModel *model, TetrisView *view, QObject *parent = 0);

 public slots:

  void Start();
  void Pause();
  void Resume();
  void OnStep();

  void MoveLeft();
  void MoveRight();
  void Rotate();

  void DropEnabled(bool enabled);

  void TogglePause();

 private:
  void InAction(void (TetrisModel::*action)());

 private:
  TetrisModel *model_;
  TetrisView *view_;
  QTimer timer_;
};

#endif  // TETRISCONTROLLER_H
