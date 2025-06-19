#include "RegisterWindow.h"
#include "GameModeWindow.h"
#include "LoginWindow.h"
#include "StartupWindow.h"
#include <QTimer>

RegisterWindow::RegisterWindow(QWidget *parent) : QWidget(parent), auth("users.db")
{
    setWindowTitle("Tic Tac Toe - Register");
    setFixedSize(400, 600);
    setupUI();
    applyStyles();
}

RegisterWindow::~RegisterWindow() {}

void RegisterWindow::setupUI()
{
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Main frame for styling
    mainFrame = new QFrame();
    mainFrame->setObjectName("mainFrame");

    QVBoxLayout *frameLayout = new QVBoxLayout(mainFrame);
    frameLayout->setSpacing(20);
    frameLayout->setContentsMargins(40, 50, 40, 50);

    // Title
    titleLabel = new QLabel("REGISTER");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);

    // Input fields
    QLabel* userLabel = new QLabel("Username:");
    userLabel->setObjectName("inputLabel");

    usernameLineEdit = new QLineEdit;
    usernameLineEdit->setObjectName("inputField");
    usernameLineEdit->setPlaceholderText("Choose a username");
    usernameLineEdit->setFixedHeight(45);

    QLabel* passLabel = new QLabel("Password:");
    passLabel->setObjectName("inputLabel");

    passwordLineEdit = new QLineEdit;
    passwordLineEdit->setObjectName("inputField");
    passwordLineEdit->setPlaceholderText("Choose a password");
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setFixedHeight(45);

    QLabel* confirmLabel = new QLabel("Confirm Password:");
    confirmLabel->setObjectName("inputLabel");

    confirmPasswordLineEdit = new QLineEdit;
    confirmPasswordLineEdit->setObjectName("inputField");
    confirmPasswordLineEdit->setPlaceholderText("Confirm your password");
    confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordLineEdit->setFixedHeight(45);

    // Buttons
    registerButton = new QPushButton("REGISTER");
    registerButton->setObjectName("primaryButton");
    registerButton->setFixedHeight(50);

    loginButton = new QPushButton("Already have an account? Login");
    loginButton->setObjectName("linkButton");
    loginButton->setFixedHeight(35);

    backButton = new QPushButton("← BACK");
    backButton->setObjectName("backButton");
    backButton->setFixedHeight(40);

    // Status label
    statusLabel = new QLabel();
    statusLabel->setObjectName("statusLabel");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setWordWrap(true);

    // Add to layout
    frameLayout->addStretch();
    frameLayout->addWidget(titleLabel);
    frameLayout->addSpacing(20);
    frameLayout->addWidget(userLabel);
    frameLayout->addWidget(usernameLineEdit);
    frameLayout->addWidget(passLabel);
    frameLayout->addWidget(passwordLineEdit);
    frameLayout->addWidget(confirmLabel);
    frameLayout->addWidget(confirmPasswordLineEdit);
    frameLayout->addSpacing(20);
    frameLayout->addWidget(registerButton);
    frameLayout->addWidget(loginButton);
    frameLayout->addWidget(statusLabel);
    frameLayout->addStretch();
    frameLayout->addWidget(backButton);

    mainLayout->addWidget(mainFrame);

    // Connect signals
    connect(registerButton, &QPushButton::clicked, this, &RegisterWindow::on_registerButton_clicked);
    connect(loginButton, &QPushButton::clicked, this, &RegisterWindow::goToLogin);
    connect(backButton, &QPushButton::clicked, this, &RegisterWindow::goBack);

    // Enter key support
    connect(usernameLineEdit, &QLineEdit::returnPressed, this, &RegisterWindow::on_registerButton_clicked);
    connect(passwordLineEdit, &QLineEdit::returnPressed, this, &RegisterWindow::on_registerButton_clicked);
    connect(confirmPasswordLineEdit, &QLineEdit::returnPressed, this, &RegisterWindow::on_registerButton_clicked);
}

void RegisterWindow::applyStyles()
{
    setStyleSheet(R"(
        QWidget {
            font-family: 'Segoe UI', Arial, sans-serif;
        }

        #mainFrame {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #a8edea, stop:1 #fed6e3);
            border-radius: 15px;
        }

        #titleLabel {
            font-size: 28px;
            font-weight: bold;
            color: #2c3e50;
            letter-spacing: 2px;
        }

        #inputLabel {
            color: #2c3e50;
            font-size: 14px;
            font-weight: 500;
            margin-bottom: 5px;
        }

        #inputField {
            background: rgba(255, 255, 255, 0.8);
            border: 2px solid rgba(44, 62, 80, 0.2);
            border-radius: 22px;
            padding: 0 15px;
            font-size: 14px;
            color: #2c3e50;
        }

        #inputField:focus {
            border: 2px solid rgba(44, 62, 80, 0.5);
            background: white;
        }

        #primaryButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #00b894, stop:1 #00a085);
            color: white;
            border: none;
            border-radius: 25px;
            font-size: 16px;
            font-weight: bold;
            letter-spacing: 1px;
        }

        #primaryButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #00a085, stop:1 #008f75);
        }

        #primaryButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #008f75, stop:1 #007d65);
        }

        #linkButton {
            background: none;
            color: rgba(44, 62, 80, 0.7);
            border: none;
            font-size: 12px;
            text-decoration: underline;
        }

        #linkButton:hover {
            color: #2c3e50;
        }

        #backButton {
            background: rgba(44, 62, 80, 0.1);
            color: #2c3e50;
            border: 1px solid rgba(44, 62, 80, 0.2);
            border-radius: 20px;
            font-size: 14px;
            font-weight: bold;
        }

        #backButton:hover {
            background: rgba(44, 62, 80, 0.2);
        }

        #statusLabel {
            color: #2c3e50;
            font-size: 12px;
            min-height: 20px;
        }
    )");
}

void RegisterWindow::on_registerButton_clicked()
{
    QString username = usernameLineEdit->text().trimmed();
    QString password = passwordLineEdit->text();
    QString confirmPassword = confirmPasswordLineEdit->text();

    if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        showError("Please fill in all fields!");
        return;
    }

    if (username.length() < 3) {
        showError("Username must be at least 3 characters long!");
        return;
    }

    if (password.length() < 6) {
        showError("Password must be at least 6 characters long!");
        return;
    }

    if (password != confirmPassword) {
        showError("Passwords do not match!");
        return;
    }

    if (auth.registerUser(username.toStdString(), password.toStdString())) {
        showSuccess("Registration successful! Redirecting...");

        // Delay to show success message, then switch windows
        QTimer::singleShot(1500, [this, username]() {
            // Create the new window first
            GameModeWindow *gameModeWindow = new GameModeWindow(username);
            gameModeWindow->setAttribute(Qt::WA_DeleteOnClose);
            gameModeWindow->show();

            // Emit signal before closing
            emit registrationComplete(username);

            // Close this window after the new one is shown
            this->deleteLater();
        });
    } else {
        showError("Username already exists! Please choose another.");
    }
}

void RegisterWindow::goToLogin()
{
    // Create login window first
    LoginWindow *login = new LoginWindow();
    login->setAttribute(Qt::WA_DeleteOnClose);
    login->show();

    // Close this window
    this->deleteLater();
}

void RegisterWindow::goBack()
{
    // Create startup window first
    StartupWindow *startup = new StartupWindow();
    startup->setAttribute(Qt::WA_DeleteOnClose);
    startup->show();

    // Close this window
    this->deleteLater();
}

void RegisterWindow::showError(const QString& message) {
    statusLabel->setText("❌ " + message);
    statusLabel->setStyleSheet("color: #e74c3c; font-weight: bold;");

    // Clear after 3 seconds
    QTimer::singleShot(3000, [this]() {
        statusLabel->clear();
    });
}

void RegisterWindow::showSuccess(const QString& message) {
    statusLabel->setText("✅ " + message);
    statusLabel->setStyleSheet("color: #00b894; font-weight: bold;");
}
