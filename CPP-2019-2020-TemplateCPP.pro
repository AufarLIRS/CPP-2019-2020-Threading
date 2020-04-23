TEMPLATE = app
CONFIG += console c++11

QMAKE_CXXFLAGS += -std=c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        task1.cpp \
        task2.cpp \
        task3.cpp
