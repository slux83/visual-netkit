TEMPLATE = app
TARGET = VisualNetkit
DEPENDPATH += . \
    src \
    ui
INCLUDEPATH += . \
    src
FORMS += ui/addCd.ui \
    ui/addVm.ui \
    ui/addLink.ui \
    ui/main.ui \
    ui/fileEdit.ui
QT += core \
    gui \
    network \
    svg \
    xml
DESTDIR = bin
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui
OBJECTS_DIR = build/obj
RESOURCES = resources/VisualNetkit.qrc
CONFIG += debug
HEADERS = src/gui/LineItemPrivate.h \
    src/gui/handles/LinkHandler.h \
    src/gui/LinkItem.h \
    src/persistence/XMLExpert.h \
    src/persistence/XMLSaver.h \
    src/gui/handles/LinkMapper.h \
    src/gui/handles/CdHandler.h \
    src/gui/handles/CdMapper.h \
    src/gui/AddCdForm.h \
    src/gui/CollisionDomainItem.h \
    src/gui/LabelItemPrivate.h \
    src/gui/SvgItemPrivate.h \
    src/common/CommonConfigs.h \
    src/gui/handles/VmMapper.h \
    src/core/handles/VmFacadeController.h \
    src/gui/undoCommands/UndoCommands.h \
    src/gui/UndoStack.h \
    src/common/Errors.h \
    src/common/NetworkAddress.h \
    src/common/Types.h \
    src/core/handles/LabFacadeController.h \
    src/gui/handles/LabHandler.h \
    src/gui/handles/LabPropertyController.h \
    src/gui/handles/VmHandler.h \
    src/gui/AddVmForm.h \
    src/gui/AddLinkForm.h \
    src/gui/VirtualMachineItem.h \
    src/gui/GraphicsView.h \
    src/gui/LabScene.h \
    src/core/DaemonManager.h \
    src/gui/MainWindow.h \
    src/core/VirtualMachine.h \
    src/core/HardwareInterface.h \
    src/core/CollisionDomain.h \
    src/core/SuperGroup.h \
    src/core/Laboratory.h
SOURCES = src/gui/LineItemPrivate.cpp \
    src/gui/handles/LinkHandler.cpp \
    src/persistence/XMLExpert.cpp \
    src/persistence/XMLSaver.cpp \
    src/gui/LinkItem.cpp \
    src/gui/handles/LinkMapper.cpp \
    src/gui/handles/CdHandler.cpp \
    src/gui/handles/CdMapper.cpp \
    src/gui/AddCdForm.cpp \
    src/gui/CollisionDomainItem.cpp \
    src/gui/LabelItemPrivate.cpp \
    src/gui/SvgItemPrivate.cpp \
    src/gui/handles/VmMapper.cpp \
    src/core/handles/VmFacadeController.cpp \
    src/gui/undoCommands/UndoCommands.cpp \
    src/gui/UndoStack.cpp \
    src/common/NetworkAddress.cpp \
    src/core/handles/LabFacadeController.cpp \
    src/gui/handles/LabHandler.cpp \
    src/gui/handles/LabPropertyController.cpp \
    src/gui/handles/VmHandler.cpp \
    src/gui/AddVmForm.cpp \
    src/gui/AddLinkForm.cpp \
    src/gui/VirtualMachineItem.cpp \
    src/gui/GraphicsView.cpp \
    src/gui/LabScene.cpp \
    src/core/DaemonManager.cpp \
    src/main.cpp \
    src/gui/MainWindow.cpp \
    src/core/VirtualMachine.cpp \
    src/core/HardwareInterface.cpp \
    src/core/SuperGroup.cpp \
    src/core/CollisionDomain.cpp \
    src/core/Laboratory.cpp
