#ifndef GAMEMODEWINDOW_H
#define GAMEMODEWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>

enum class GameMode {
    AI,
    PLAYER
};

class GameModeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameModeWindow(const QString& username, QWidget *parent = nullptr);

signals:
    void gameModeSelected(const QString& username, GameMode mode);

private slots:
    void selectAIMode();
    void selectPlayerMode();
    void goBack();

private:
    void setupUI();
    void applyStyles();

    QString currentUsername;
    QPushButton *aiButton;
    QPushButton *playerButton;
    QPushButton *backButton;
    QLabel *titleLabel;
    QLabel *welcomeLabel;
    QFrame *mainFrame;
};

#endif // GAMEMODEWINDOW_H
