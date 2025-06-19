#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QMenuBar>
#include <QStatusBar>
#include "Game.h"
#include "AI.h"
#include "History.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(const QString& username, QWidget* parent = nullptr);

private slots:
    void handleCellClick();
    void updateBoard();
    void newGame();
    void showHistory();
    void showAbout();
    void goBack();

private:
    void setupUI();
    void applyStyles();
    void logResult(const QString& result);
    void showGameResult(const QString& result);
    void enableBoard(bool enabled);
    void handleGameEnd();

    QPushButton* cells[3][3];
    QLabel* statusLabel;
    QLabel* scoreLabel;
    QLabel* playerLabel;
    QPushButton* newGameBtn;
    QPushButton* historyBtn;
    QPushButton* backBtn;
    QFrame* gameFrame;
    QFrame* controlFrame;

    Game game;
    AI ai;
    QString user;
    History history;

    int playerWins;
    int aiWins;
    int draws;
};

#endif
