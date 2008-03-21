TEMPLATE = app
TARGET = VisualNetkit
DEPENDPATH += . \
    src \
    ui
INCLUDEPATH += . \
    src
FORMS += ui/fileEdit.ui \
    ui/main.ui
QT += core \
    gui \
    network \
    svg
DESTDIR = bin
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui
OBJECTS_DIR = build/obj
RESOURCES = resources/VisualNetkit.qrc
CONFIG += debug
HEADERS = src/gui/VirtualMachineItem.h \
    src/gui/VmController.h \
    src/gui/VmHandler.h \
    src/core/Types.h \
    src/gui/GraphicsView.h \
    src/gui/LabScene.h \
    src/gui/LabPropertyController.h \
    src/gui/LabHandler.h \
    src/core/LabFacadeController.h \
    src/core/DaemonManager.h \
    src/core/VirtualMachineFactory.h \
    src/gui/MainWindow.h \
    src/core/VirtualMachine.h \
    src/core/HardwareInterface.h \
    src/core/CollisionDomain.h \
    src/core/Subnet.h \
    src/core/SuperGroup.h \
    src/core/Laboratory.h
SOURCES = src/gui/GraphicsView.cpp \
    src/gui/LabScene.cpp \
    src/gui/VirtualMachineItem.cpp \
    src/gui/VmController.cpp \
    src/gui/VmHandler.cpp \
    src/gui/VirtualMachineItem.cpp \
    src/gui/VmController.cpp \
    src/gui/VmHandler.cpp \
    src/gui/GraphicsView.cpp \
    src/gui/LabScene.cpp \
    src/gui/LabPropertyController.cpp \
    src/gui/LabHandler.cpp \
    src/core/LabFacadeController.cpp \
    src/core/DaemonManager.cpp \
    src/core/VirtualMachineFactory.cpp \
    src/main.cpp \
    src/gui/MainWindow.cpp \
    src/core/VirtualMachine.cpp \
    src/core/HardwareInterface.cpp \
    src/core/SuperGroup.cpp \
    src/core/CollisionDomain.cpp \
    src/core/Subnet.cpp \
    src/core/Laboratory.cpp
