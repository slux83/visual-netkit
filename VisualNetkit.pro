TEMPLATE = app
TARGET = VisualNetkit
DEPENDPATH += . \
    src \
    ui
INCLUDEPATH += . \
    src
FORMS += ui/visual_netkit_main.ui
QT += core \
    gui
DESTDIR = bin
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui
OBJECTS_DIR = build/obj
RESOURCES = 
CONFIG += debug
HEADERS = src/Subnet.h \
    src/Lan.h \
    src/SuperGroup.h \
    src/Laboratory.h
SOURCES = src/Subnet.cpp \
    src/Lan.cpp \
    src/Laboratory.cpp
