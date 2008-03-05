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
HEADERS = src/VirtualMachine.h \
    src/HardwareInterface.h \
    src/CollisionDomain.h \
    src/Subnet.h \
    src/SuperGroup.h \
    src/Laboratory.h
SOURCES = src/VirtualMachine.cpp \
    src/HardwareInterface.cpp \
    src/SuperGroup.cpp \
    src/CollisionDomain.cpp \
    src/Subnet.cpp \
    src/Laboratory.cpp
