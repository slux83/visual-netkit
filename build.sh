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
VN_PLUGINS=( "src/plugin_dev/test" "src/plugin_dev/ipv4" )
VN_HOME=`pwd`

usage()
{
	echo
	echo "Visual Netkit Builder script version 1.0"
	echo "USE ./build.sh [OPTION]"
	echo
	echo " 	OPTIONS:"
	echo "	-h		Display this help and exit"
	echo "	-b		Build plugins and Visual Netkit"
	echo "	-c		Clear plugins and Visual Netkit"
	echo "	-cb		Clear and Re-Build plugins and Visual Netkit"
	echo	
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
			
			make || exit 1
						
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
	make || exit 1
	
	echo
	echo ">>> STARTING ./bin/VisualNetkit <<<"
	echo
	./bin/VisualNetkit
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
			make clean || echo "### ERROR: Failed to clean the plugin $plugin"
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
	
	make clean || echo "### ERROR: Failed to clean Visual Netkit"
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
		-c)
			clean
		;;
		-cb)
			clean
			compile
		;;
		*)
			usage
		;;
	esac
else
	compile
fi
