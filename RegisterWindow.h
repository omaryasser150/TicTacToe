#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include "Auth.h"

class RegisterWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = nullptr);
    ~RegisterWindow();

signals:
    void registrationComplete(const QString& username);

private slots:
    void on_registerButton_clicked();
    void goToLogin();
    void goBack();

private:
    void setupUI();
    void applyStyles();
    void showError(const QString& message);
    void showSuccess(const QString& message);

    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QLineEdit *confirmPasswordLineEdit;
    QPushButton *registerButton;
    QPushButton *loginButton;
    QPushButton *backButton;
    QLabel *titleLabel;
    QLabel *statusLabel;
    QFrame *mainFrame;
    Auth auth;
};

#endif // REGISTERWINDOW_H
