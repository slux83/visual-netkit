TEMPLATE = app
TARGET = 
DEPENDPATH += . \
    src
INCLUDEPATH += . \
    src
FORMS += window.ui
QT += svg \
    gui \
    core
MOC_DIR = build
RCC_DIR = build
UI_DIR = build
OBJECTS_DIR = build
SOURCES += src/SvgItemLink.cpp \
    main.cpp \
    src/Scene.cpp \
    src/SvgItemNode.cpp \
    src/ViewWindow.cpp
HEADERS += src/SvgItemLink.h \
    src/Scene.h \
    src/SvgItemNode.h \
    src/ViewWindow.h
