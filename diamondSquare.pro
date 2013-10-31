TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    src/diamondSquare/diamondSquare.h \
    src/random/random.h

SOURCES += \
    src/main.cpp \
    src/random/random.cpp \
    src/diamondSquare/diamondSquare.cpp

include(defaults.pri)
