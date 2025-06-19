#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>
#include "Auth.h"

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    LoginWindow(QWidget* parent = nullptr);
    QString getUsername() const;

signals:
    void loginSuccessful(const QString& username);

private slots:
    void handleLogin();
    void handleRegister();
    void goBack();

private:
    void setupUI();
    void applyStyles();
    void showError(const QString& message);
    void showSuccess(const QString& message);

    QLineEdit* userEdit;
    QLineEdit* passEdit;
    QPushButton* loginBtn;
    QPushButton* registerBtn;
    QPushButton* backBtn;
    QLabel* statusLabel;
    QLabel* titleLabel;
    QFrame* mainFrame;
    Auth auth;
};

#endif
