######################################################################
# Generated by Jens Krueger
######################################################################

TEMPLATE          = app
CONFIG           += link_prl static warn_on stl exceptions x86 ppc
TARGET            = Build/UVFConverter
QTPLUGIN         += qjpeg qtiff qgif
DEPENDPATH       += .
INCLUDEPATH      += .
LIBS              = -L../Tuvok/Build -lTuvok
unix:QMAKE_CXXFLAGS += -fno-strict-aliasing
unix:QMAKE_CFLAGS += -fno-strict-aliasing

# Input
HEADERS += DebugOut/HRConsoleOut.h


SOURCES += DebugOut/HRConsoleOut.cpp \
           main.cpp
