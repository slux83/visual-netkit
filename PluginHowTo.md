**NOTE: BEFORE READ THIS HOW-TO, PLEASE READ PluginArchitecture PAGE**

## This how-to is only for VisualNetkit (version 1.0 or less) ##


---


# Making a simple plugin for a Virtual Machine element #
In this section we try to make a Plugin for a Virtual Machine. The plugin is very simple. It's just export the PS1 environment variable and some aliases.

Our plugin will have only two properties that can have two values ("true" or "false"):
  * `use PS1`
  * `use aliases`

The plugin name is **BashRC** because the plugin offer only one template builded in base of the properties values.

## The plugin.ini ##
```
[global]
name=BashRC
type=vm
description="This plugin set PS1 env variable and/or some aliases in /root/.bashrc"
version=1.0
dependencies=""
author="Alessio Di Fazio"

[properties]
use PS1/p_default_value=true
use PS1/p_description="Set true if you want add a cool and coloured Bash promt inside the root .bashrc file, false otherwise"

use aliases/p_default_value=true
use aliases/p_description="Set true if you want add some useful aliases inside the root .bashrc file, false otherwise"
```

## The templates ##
Inside our plugin we have three templates used to build the .bashrc final file!
### .bashrc - main ###
```
#
# .bashrc file created by VisualNetkit BashRC plugin
#

umask 022
export TERM=xterm

# Bash completion
if [ -f /etc/bash_completion ]; then
    . /etc/bash_completion
fi
```

### .bashrc - aliases part ###
```
# Aliases
alias ls='ls --color'
alias grep='grep --color'
alias xorpsh='su xorp -p -c xorpsh'
alias ll='ls -lih --color'
alias la='ls -alh --color'
alias l='ls -ah --color'
alias ls='ls -h --color'
alias cd..='cd ..'
alias nano='nano -i -m -w -B -F -L'
```

### .bashrc - promt part ###
```
# Bash promt
export PS1='\n\[\033[1;30m\][\[\033[0;33m\]\D{%d/%m/%Y %T}\[\033[1;30m\]]\n\[\033[31;04m\]\u\[\033[37;00m\]@\[\033[36;01m\]\h\[\033[37;00m\][\w]\n~> '
```

These two last templates is appended to the main .bashrc file if the properties are setted to "true".

## Implementing a plugin for VisualNetkit ##
Sources:
  * [bashrc/bashrc.pro](http://code.google.com/p/visual-netkit/source/browse/trunk/src/plugin_dev/bashrc/bashrc.pro)
  * [bashrc/PluginBashRc.h](http://code.google.com/p/visual-netkit/source/browse/trunk/src/plugin_dev/bashrc/PluginBashRc.h)
  * [bashrc/PluginBashRc.cpp](http://code.google.com/p/visual-netkit/source/browse/trunk/src/plugin_dev/bashrc/PluginBashRc.cpp)
  * [bashrc/resources/resources.qrc](http://code.google.com/p/visual-netkit/source/browse/trunk/src/plugin_dev/bashrc/resources/resources.qrc)
  * [bashrc/resources/plugin.ini](http://code.google.com/p/visual-netkit/source/browse/trunk/src/plugin_dev/bashrc/resources/plugin.ini)
  * [bashrc/resources/bashrc](http://code.google.com/p/visual-netkit/source/browse/trunk/src/plugin_dev/bashrc/resources/bashrc)
  * [bashrc/resources/ps1](http://code.google.com/p/visual-netkit/source/browse/trunk/src/plugin_dev/bashrc/resources/ps1)
  * [bashrc/resources/aliases](http://code.google.com/p/visual-netkit/source/browse/trunk/src/plugin_dev/bashrc/resources/aliases)

### bashrc.pro - Qt project file ###
Project files contain all the information required by `qmake` to build your application, library, or plugin.

The most important variables are `TARGET`, `HEADERS` and `SOURCES`.
`TARGET` is the plugin (project) name. If you set `TARGET = foo` the plugin file will be named `libfoo.so`.

The `.pro` file for each plugin is quite standard; you have to change `HEADERS` and `SOURCES` but only adding plugin files such as PluginClass{.cpp .h} and other private classes. Do not touch the other files.

If you want more informations, please read the Qt-doc: http://doc.trolltech.com/4.4/qmake-project-files.html

### resources.qrc - Qt Resource system ###
The Qt resource system is a platform-independent mechanism for storing binary files in the application's executable. This is useful if your application always needs a certain set of files (icons, translation files, etc.) and you don't want to run the risk of losing the files.

The resources associated with an application are specified in a `.qrc` file, an **XML-based** file format that lists files on the disk and optionally assigns them a resource name that the application must use to access the resource.

Our resource file:
```
<RCC>
    <qresource prefix="/" >
        <file alias="bash_main" >bashrc</file>
        <file alias="bash_aliases" >aliases</file>
        <file alias="bash_ps1" >ps1</file>
        <file alias="ini_bashrc" >plugin.ini</file>
    </qresource>
</RCC>
```

**NOTE:** The most important thing that you don't forget is that the aliases inside the `.qrc` file must be unique for each plugins! I suggest to add the plugin name on `prefix` attribute (prefix="/bashrc"), or be careful do not mix resources of other plugins using the same alias for two or more plugin`s resources.

In our example we use four templates (text files): one of these is the plugin.ini described in PluginArchitecture wiki page.

If you want more informations, please read the Qt-doc: http://doc.trolltech.com/4.4/resources.html

### PluginBashRc.h - The plugin class header ###

The includes are quite standard for each plugin:
```
#include <QSettings>
#include "PluginInterface.h"
#include "PluginProxy.h"
#include "PluginProperty.h"
#include "VirtualMachine.h"
```

Our plugin must extend the PluginInterface.h (see PluginArchitecture to obtain more info) that is a pure C++ abstract class.
```
class PluginBashRc : public PluginInterface
```

The private stuff of our plugin class is showed:
```
private:
        QMap<QString, PluginProperty*> properties;
        QSettings *mySettings;
        PluginProxy *myProxy;
        QString myName;

        void fetchProperties();
        QString getTemplateLocation();
```
We use a `QMap` for store properties, a `QSetting*` to open and parse authomatically the `plugin.ini` file, the `PluginProxy*` required by interface, a `QString` that represent the plugin name readed inside the `.ini` file and two private functions.

The first is called by the constructor to fill the properties `QMap` reading these from the setting class.

The second is just for convenient; it return a `QString` that contain the template relative path (in our case: "VM\_NAME/root/.bashrc".

The public part of the class is the classic header of a C++ class that contains only function of the super class to implement.

On the end we have to do factory and destroy functions as explained in PluginArchitecture page.

### PluginBashRc.cpp - The plugin class implementation ###
The constructor is quite standard for each plugin.
```
/** 
 * Constructor 
 */
PluginBashRc::PluginBashRc() : PluginInterface()
{
        //get the ini file from resource
        mySettings = new QSettings(":/ini_bashrc", QSettings::NativeFormat);
        
        /* Save the name of this plugin for future uses */
        mySettings->beginGroup("global");
        myName = mySettings->value("name").toString();
        mySettings->endGroup();
        
        myProxy = NULL;
        fetchProperties();      //read properties from ini file
}
```
It init some private variables like settings, name (reading from plugin.ini), fetch the preperties and set `myProxy` to `NULL` because it's initialized and passed (`void setProxy(PluginProxy* p)`) by the system.

An important function is `saveProperty()`
```
/**
 * If pluginAlertMsg is empty, initializes the passed property propName to propValue. 
 */
bool PluginBashRc::saveProperty(QString propName, QString propValue, QString *pluginAlertMsg)
{
        /* Check the property name */
        if(!properties.contains(propName))
        {
                if(pluginAlertMsg != NULL) { pluginAlertMsg->append("Property not found."); }
                
                return false;
        }
        
        /* Validate the value of the property: only "true" or "false" */
        if(pluginAlertMsg != NULL && propValue != "true" && propValue != "false")
        {
                pluginAlertMsg->append("You can set only \"true\" or \"false\" values.");
                return false;
        }
        
        /* All is ok, store the property */
        properties.value(propName)->setValue(propValue);
        
        return true;
}
```

As described on PluginArchitecture page, this function allow the user to change a property passing the name, the new value and a pointer to a `QString`.

**The `QString *pluginAlertMsg` meaning:**
This last argument (default `NULL`) is a easy way to pass an alert message to the system. When the user want change a property, the `pluginAlertMsg` is allways passed (the first time), and you have to set a message in this string if the property value is not validated and return `FALSE`.

When the alert is showed to the user, a possible behaviour is that the user want ignore this alert and store the value of the property also if this is uncorrect. If so, the function is re-called by the system, but in this case the `pluginAlertMsg` is `NULL` and you must save the property as it is and return `TRUE`.

**The function `getTemplates()`:**
```
/**
 * Returns the plugin templates
 */
QMap<QString, QString> PluginBashRc::getTemplates()
{
        QMap<QString, QString> templates;
        QString mainBashContent;
        
        QFile data(":/bash_main");
        QFile aliases(":/bash_aliases");
        QFile ps1(":/bash_ps1");
        
        /* Read bash main */
        if (data.open(QFile::ReadOnly)) 
        {
                QTextStream in(&data);
                mainBashContent = in.readAll();
                data.close();
        }
        else
        {
                qWarning() << "The plugin getTemplate() failed:" << data.errorString();
        }
                
        /* append PS1 and/or asiases according to propertie values */
        if(properties["use aliases"]->getValue() == "true")
        {
                /* Read bash aliases */
                if (aliases.open(QFile::ReadOnly)) 
                {
                        QTextStream in(&aliases);
                        mainBashContent.append(in.readAll());   //append to bashrc content
                        aliases.close();
                }
                else
                {
                        qWarning() << "The plugin getTemplate() failed:" << aliases.errorString();
                }
        }
        
        if(properties["use PS1"]->getValue() == "true")
        {
                /* Read bash aliases */
                if (ps1.open(QFile::ReadOnly)) 
                {
                        QTextStream in(&ps1);
                        mainBashContent.append(in.readAll());   //append to bashrc content
                        ps1.close();
                }
                else
                {
                        qWarning() << "The plugin getTemplate() failed:" << ps1.errorString();
                }
        }
        
        //we have only one template file
        templates.insert(getTemplateLocation(), mainBashContent);
        
        return templates;
}
```
In this function you have to "build" your templates (your files) and return a `QMap<QString, QString>` where the `KEY` is the file location, and the `VALUE` is the content of the file.

Normally to create the file content you need to parse plugin properties and "build" it dinamically, or if you wish you can return a static template.

As sayd, the `KEY` of the map is a location; in particolar this is the relative path (stanting from lab root dir) of the file that you want fill your datas.

In our example we return a `QMap` of size 1 and the key is `VIRTUAL_MACHINE_NAME/root/.bashrc`, where `VIRTUAL_MACHINE_NAME` is offers by `getTemplateLocation()` private function.

**REMEMBER:**
If more than one plugin want write some content inside the same file, the system will be append the contents.

**The `init()` function** is normally more complex.
```
/* Do nothing on init. Just show a label "/root/.bashrc" */ 
bool PluginBashRc::init(QString laboratoryPath)
{
    Q_UNUSED(laboratoryPath)
    myProxy->changeGraphicsLabel("/root/.bashrc");
    return false;
}
```
As you can read in PluginArchitecture, the init() function is called by the system (later the plugin constructor) when the plugin il attached to an element on _Lab Loading_ time.

In this function you can do stuff like parsing filesystem files to fetch properties values like inside PluginIpv4 and PluginMac that parse `.startup` files to get netmask, broadcast, mac and ip address.

#### Why `init()` return a boolean??? ####
Thinking in future, in particolar to importing a lab created by hands (not standard) we cannot know which plugins attach to elements. Probably we can init all plugins for each element and if `init()` return `TRUE`, this mean that the plugin is able to parse (maybe not totally) files and the plugin can remain attached to the element. If the `init()` return `FALSE` at import time the plugin will be trashed.

At present the returned boolean is ignored by the system.

In our plugins we do nothing. Just show a label on graphics.


---


Other functions are simple and "standard".

Enjoy! :D

## The plugin at work!!! ##
![![](http://www.capponcino.it/alessio/./files/universita/tesi-visual-netkit/plugin_bashrc_at_work-thumb.png)](http://www.capponcino.it/alessio/./files/universita/tesi-visual-netkit/plugin_bashrc_at_work.png)