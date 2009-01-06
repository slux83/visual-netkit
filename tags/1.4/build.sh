 #!/bin/bash

# VisualNetkit Builder script
# Copyright (C) 2008  Alessio Di Fazio, Paolo Minasi
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

QMAKE="qmake"
GDB="gdb"
VN_PLUGINS=(	"src/plugin_dev/test"
		"src/plugin_dev/ipv4" \
		"src/plugin_dev/mac"  \
		"src/plugin_dev/quagga/core" \
		"src/plugin_dev/quagga/bgp" \
		"src/plugin_dev/quagga/rip" \
)

VN_HOME=`pwd`
MAKE="make -j2"

usage()
{
	echo
	echo "Visual Netkit Builder script version 1.4"
	echo "USE ./build.sh [OPTION]"
	echo
	echo " 	OPTIONS:"
	echo "	-h		Display this help and exit"
	echo "	-b		Build plugins and Visual Netkit"
	echo "	-bg		Build all in Debug mode (using GDB)"
	echo "	-c		Clear plugins and Visual Netkit"
	echo "	-cb		Clear and Re-Build plugins and Visual Netkit"
	echo "	-r		Build Visual Netkit for release (Stripped)"
	echo	
}

release()
{
	compile r
	strip bin/VisualNetkit
	strip bin/plugins/*
	
	echo
	echo "######################"
	echo "# RELEASE BUILD DONE #"
	echo "######################"
	echo
	echo "All stuff (binary, startup script and plugins) is inside $VN_HOME/bin/"
}

compile()
{
	#compile plugins first
	for plugin in "${VN_PLUGINS[@]}"
	do
		if [ -d $plugin ]
		then

			echo
			echo "###################"
			echo "# BUILDING PLUGIN #	>> $plugin <<"
			echo "###################"
			echo
			
			cd "$plugin"
			`$QMAKE` || echo "### ERROR: Failed to build the plugin $plugin (qmake)"
			
			$MAKE || exit 1
						
			cd "$VN_HOME"
		else
			echo "### ERROR: plugin $plugin not found!"
			
		fi
	done

	echo
	echo "##########################"
	echo "# BUILDING VISUAL NETKIT #"
	echo "##########################"
	echo

	`$QMAKE` || echo "### ERROR: Failed to build visual netkit (qmake)"
	$MAKE || exit 1
	
	if [ "$1" = "g" ]; then
		echo
		echo ">>> DEBUG MODE STARTING ./bin/VisualNetkit <<<"
		echo
		$GDB ./bin/VisualNetkit
	fi
	if [ "$1" = "" ]; then
		echo
		echo ">>> STARTING ./bin/VisualNetkit <<<"
		echo
		cd bin
		./visualnetkit.sh
	fi
	
}

clean()
{
	#clean plugins first
	for plugin in "${VN_PLUGINS[@]}"
	do
		if [ -d $plugin ]
		then

			echo
			echo "###################"
			echo "# CLEANING PLUGIN #"
			echo "###################"
			echo
			
			cd "$plugin"
			$MAKE clean
			$MAKE distclean
			rm -fr Makefile*
			
			cd "$VN_HOME"
		else
			echo "### ERROR: plugin $plugin not found!"
			
		fi
	done

	echo
	echo "##########################"
	echo "# CLEANING VISUAL NETKIT #"
	echo "##########################"
	echo
	
	$MAKE clean
	$MAKE distclean
	rm -fr Makefile*
}

if [ "$#" != "0" ]; then
	case "$1" in
		-h)
			usage
		;;
		
		-b)
			compile
		;;
		-bg)
			compile g
		;;
		-c)
			clean
		;;
		-cb)
			clean
			compile
		;;
		-r)
			release
		;;
		*)
			usage
		;;
	esac
else
	compile
fi
