######################################################################
# Automatically generated by qmake (2.01a) lun mag 19 18:38:13 2008
######################################################################

TEMPLATE = lib
TARGET = 
DEPENDPATH += .

DESTDIR = ../../../bin/plugins
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui
OBJECTS_DIR = build/obj

# including also the PluginInterface.h
INCLUDEPATH += . \
    ../../plugin_framework \
    ../../core

RESOURCES = resources/resources.qrc

# Input
HEADERS += Plugin.h
SOURCES += Plugin.cpp \
	../../plugin_framework/PluginProxy.cpp \
	../../core/VirtualMachine.cpp
