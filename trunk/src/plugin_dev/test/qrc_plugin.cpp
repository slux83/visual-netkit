/****************************************************************************
** Resource object code
**
** Created: Mon May 19 21:14:35 2008
**      by: The Resource Compiler for Qt version 4.3.3
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtCore/qglobal.h>

static const unsigned char qt_resource_data[] = {
  // /home/paolo/svn/visual-netkit/trunk/src/plugin_dev/test/resources/plugin.ini
  0x0,0x0,0x0,0x6c,
  0x5b,
  0x47,0x65,0x6e,0x65,0x72,0x61,0x6c,0x5d,0xa,0x6e,0x61,0x6d,0x65,0x3d,0x54,0x65,
  0x73,0x74,0x50,0x6c,0x75,0x67,0x69,0x6e,0xa,0x74,0x79,0x70,0x65,0x3d,0x76,0x6d,
  0xa,0x64,0x65,0x73,0x63,0x72,0x69,0x70,0x74,0x69,0x6f,0x6e,0x3d,0x54,0x65,0x73,
  0x74,0x20,0x50,0x6c,0x75,0x67,0x69,0x6e,0xa,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,
  0x3d,0x31,0x2e,0x30,0xa,0x64,0x65,0x70,0x65,0x6e,0x64,0x65,0x6e,0x63,0x69,0x65,
  0x73,0x3d,0x70,0x69,0x70,0x70,0x6f,0xa,0x61,0x75,0x74,0x68,0x6f,0x72,0x3d,0x50,
  0x61,0x6f,0x6c,0x6f,0x20,0x4d,0x69,0x6e,0x61,0x73,0x69,
  
};

static const unsigned char qt_resource_name[] = {
  // ini
  0x0,0x3,
  0x0,0x0,0x70,0x49,
  0x0,0x69,
  0x0,0x6e,0x0,0x69,
  
};

static const unsigned char qt_resource_struct[] = {
  // :
  0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x1,
  // :/ini
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x0,

};

int qInitResources_plugin()
{
    extern bool qRegisterResourceData(int, const unsigned char *, const unsigned char *, const unsigned char *);
    qRegisterResourceData(0x01, qt_resource_struct, qt_resource_name, qt_resource_data);
    return 1;
}
Q_CONSTRUCTOR_FUNCTION(qInitResources_plugin)
int qCleanupResources_plugin()
{
    extern bool qUnregisterResourceData(int, const unsigned char *, const unsigned char *, const unsigned char *);
    qUnregisterResourceData(0x01, qt_resource_struct, qt_resource_name, qt_resource_data);
    return 1;
}
Q_DESTRUCTOR_FUNCTION(qCleanupResources_plugin)
