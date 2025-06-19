QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += qt gui


# INCLUDEPATH += C:/Program Files/OpenSSL-Win64/include
# LIBS += -LC:/Program Files/OpenSSL-Win64/lib -llibssl -llibcrypto
SOURCES += main.cpp \
    GameModeWindow.cpp \
    LoginWindow.cpp \
    MainWindow.cpp \
    Auth.cpp \
    Game.cpp \
    History.cpp \
    AI.cpp \
    PlayerVsPlayerWindow.cpp \
    RegisterWindow.cpp \
    StartupWindow.cpp
HEADERS += LoginWindow.h \
    GameModeWindow.h \
    MainWindow.h \
    Auth.h \
    Game.h \
    History.h \
    AI.h \
    PlayerVsPlayerWindow.h \
    RegisterWindow.h \
    StartupWindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
