TEMPLATE = lib
TARGET = quagga-rip

# prevent linking error on some system (like ubuntu)
QMAKE_LFLAGS = -Wl,-rpath,$$[QT_INSTALL_LIBS] \
    $$[QMAKE_LFLAGS_SHLIB]
DEPENDPATH += .
DESTDIR = ../../../../bin/plugins
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui
OBJECTS_DIR = build/obj
VERSION = 1.0
QT += network
QT += xml
INCLUDEPATH += . \
    ../../../plugin_framework \
    ../../../core
RESOURCES = resources/resources.qrc

# Input
HEADERS += PluginQuaggaRip.h \
    ../../../plugin_framework/PluginProxy.h
SOURCES += PluginQuaggaRip.cpp \
    ../../../plugin_framework/PluginProxy.cpp \
    ../../../core/VirtualMachine.cpp \
    ../../../core/HardwareInterface.cpp \
    ../../../core/CollisionDomain.cpp \
    ../../../plugin_framework/PluginProperty.cpp \
    ../../../plugin_framework/PluginRegistry.cpp \
    ../../../plugin_framework/PropertyExpert.cpp
