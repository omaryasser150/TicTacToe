#ifndef STARTUPWINDOW_H
#define STARTUPWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>

class StartupWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StartupWindow(QWidget *parent = nullptr);

private slots:
    void openLogin();
    void openRegister();

private:
    void setupUI();
    void applyStyles();

    QPushButton *loginButton;
    QPushButton *registerButton;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QFrame *mainFrame;
};

#endif // STARTUPWINDOW_H
