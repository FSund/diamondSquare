TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(defaults.pri)

HEADERS += \
    src/diamondSquare.h \
    src/random.h

SOURCES += \
    src/main.cpp \
    src/random.cpp \
    src/diamondSquare.cpp
