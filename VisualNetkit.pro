TEMPLATE = app
TARGET = VisualNetkit
DEPENDPATH += . \
    src \
    ui
INCLUDEPATH += . \
    src
FORMS += ui/visual_netkit_main.ui
QT += core \
    gui \
    network
DESTDIR = bin
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui
OBJECTS_DIR = build/obj
RESOURCES = 
CONFIG += debug
HEADERS = src/core/NewLabFacadeController.h \
    src/gui/NewLabHandler.h \
    src/gui/MainWindow.h \
    src/core/VirtualMachine.h \
    src/core/HardwareInterface.h \
    src/core/CollisionDomain.h \
    src/core/Subnet.h \
    src/core/SuperGroup.h \
    src/core/Laboratory.h
SOURCES = src/core/NewLabFacadeController.cpp \
    src/gui/NewLabHandler.cpp \
    src/main.cpp \
    src/gui/MainWindow.cpp \
    src/core/VirtualMachine.cpp \
    src/core/HardwareInterface.cpp \
    src/core/SuperGroup.cpp \
    src/core/CollisionDomain.cpp \
    src/core/Subnet.cpp \
    src/core/Laboratory.cpp
