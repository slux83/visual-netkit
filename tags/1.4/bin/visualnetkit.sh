#!/bin/bash

# the visual netkit plugin paths (separed by ":")
APP=`which $0`
APP_PATH=${APP/\/visualnetkit.sh/}

export VISUAL_NETKIT_PLUGINS="$APP_PATH/plugins:$HOME/.visualnetkit/plugins"

$APP_PATH/VisualNetkit
