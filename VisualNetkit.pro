TARGET = VisualNetkit
DEPENDPATH += . \
    src \
    ui
INCLUDEPATH += . \
    src
FORMS += ui/about.ui \
    ui/openLab.ui \
    ui/addCd.ui \
    ui/addVm.ui \
    ui/addLink.ui \
    ui/main.ui \
    ui/fileEdit.ui \
    ui/initPluginsProperties.ui
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
HEADERS = src/persistence/FilesystemExpert.h \
    src/gui/FsTreeView.h \
    src/gui/handles/FsManager.h \
    src/gui/TreeView.h \
    src/gui/handles/AbstractPropertyHandler.h \
    src/plugin_framework/PropertyExpert.h \
    src/gui/tree_model/TreeModel.h \
    src/gui/tree_model/TreeItem.h \
    src/gui/handles/AreaController.h \
    src/gui/AreaItem.h \
    src/gui/AboutDialog.h \
    src/core/handles/SettingsFacadeController.h \
    src/gui/handles/SettingsHandler.h \
    src/persistence/SettingsManager.h \
    src/gui/SyntaxHighLighter.h \
    src/gui/TabWidget.h \
    src/gui/handles/TabController.h \
    src/gui/FileEditor.h \
    src/gui/OpenLabForm.h \
    src/persistence/LabOpener.h \
    src/gui/handles/LinkPropertyController.h \
    src/gui/handles/VmPropertyController.h \
    src/gui/InitPluginsPropertiesDialog.h \
    src/plugin_framework/PluginProperty.h \
    src/gui/PluginsSharedArea.h \
    src/plugin_framework/PluginRegistry.h \
    src/plugin_framework/PluginInterface.h \
    src/plugin_framework/PluginProxy.h \
    src/plugin_framework/PluginLoaderFactory.h \
    src/gui/handles/SceneTreeMapper.h \
    src/common/BugDumper.h \
    src/gui/handles/CdPropertyController.h \
    src/persistence/TemplateExpert.h \
    src/persistence/LabSaver.h \
    src/persistence/XMLParser.h \
    src/gui/LineItemPrivate.h \
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
    src/gui/MainWindow.h \
    src/core/VirtualMachine.h \
    src/core/HardwareInterface.h \
    src/core/CollisionDomain.h \
    src/core/Laboratory.h
SOURCES = src/persistence/FilesystemExpert.cpp \
    src/gui/FsTreeView.cpp \
    src/gui/handles/FsManager.cpp \
    src/gui/TreeView.cpp \
    src/gui/handles/AreaController.cpp \
    src/plugin_framework/PropertyExpert.cpp \
    src/gui/tree_model/TreeItem.cpp \
    src/gui/tree_model/TreeModel.cpp \
    src/gui/AreaItem.cpp \
    src/gui/AboutDialog.cpp \
    src/core/handles/SettingsFacadeController.cpp \
    src/gui/handles/SettingsHandler.cpp \
    src/persistence/SettingsManager.cpp \
    src/gui/SyntaxHighLighter.cpp \
    src/gui/TabWidget.cpp \
    src/gui/handles/TabController.cpp \
    src/gui/FileEditor.cpp \
    src/gui/OpenLabForm.cpp \
    src/persistence/LabOpener.cpp \
    src/gui/handles/LinkPropertyController.cpp \
    src/gui/handles/VmPropertyController.cpp \
    src/gui/InitPluginsPropertiesDialog.cpp \
    src/plugin_framework/PluginProperty.cpp \
    src/gui/PluginsSharedArea.cpp \
    src/plugin_framework/PluginRegistry.cpp \
    src/plugin_framework/PluginProxy.cpp \
    src/plugin_framework/PluginLoaderFactory.cpp \
    src/gui/handles/SceneTreeMapper.cpp \
    src/common/BugDumper.cpp \
    src/gui/handles/CdPropertyController.cpp \
    src/persistence/TemplateExpert.cpp \
    src/persistence/LabSaver.cpp \
    src/persistence/XMLParser.cpp \
    src/gui/LineItemPrivate.cpp \
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
    src/core/handles/LabFacadeController.cpp \
    src/gui/handles/LabHandler.cpp \
    src/gui/handles/LabPropertyController.cpp \
    src/gui/handles/VmHandler.cpp \
    src/gui/AddVmForm.cpp \
    src/gui/AddLinkForm.cpp \
    src/gui/VirtualMachineItem.cpp \
    src/gui/GraphicsView.cpp \
    src/gui/LabScene.cpp \
    src/main.cpp \
    src/gui/MainWindow.cpp \
    src/core/VirtualMachine.cpp \
    src/core/HardwareInterface.cpp \
    src/core/CollisionDomain.cpp \
    src/core/Laboratory.cpp
