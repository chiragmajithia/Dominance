#-------------------------------------------------
#
# Project created by QtCreator 2017-03-05T15:08:08
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Dominance
TEMPLATE = app


SOURCES += main.cpp\
        init.cpp \
    board.cpp \
    include/player.cpp \
    include/ai.cpp

HEADERS  += init.h \
    board.h \
    include/player.h \
    include/ai.h \
    include/point.h

FORMS    += init.ui \
    board.ui
