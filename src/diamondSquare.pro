TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(defaults.pri)

HEADERS += \
    diamondSquare.h \
    lib.h

SOURCES += \
    main.cpp \
    diamondSquare.cpp \
    lib.cpp
