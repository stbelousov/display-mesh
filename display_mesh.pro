QT       += core gui

TARGET = mesh_visualizer
TEMPLATE = app

SOURCES += main.cpp\
        widget.cpp \
        viewer.cpp \
        facet.cpp

HEADERS  += widget.h \
            viewer.h \
            facet.h

FORMS    += widget.ui \
            viewer.ui

QMAKE_CXXFLAGS += -O2
