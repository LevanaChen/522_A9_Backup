#!/bin/bash
MYVAR="HI there!"
echo $MYVAR
current_dir=$(pwd)
sep="/"

WINNAME="cygwin"
if [ "$OSTYPE" = "$WINNAME" ] ; then
	current_dir=`cygpath -w $current_dir`
	sep="\\"
fi


echo "current PYTHONPATH=$PYTHONPATH"
echo "current MAYA_SCRIPT_PATH=$MAYA_SCRIPT_PATH"
echo "current MAYA_SHELF_PATH=$MAYA_SHELF_PATH"
echo "current MAYA_DIR=$MAYA_DIR"


export PYENGINE_WORKSPACE_DIR=${current_dir}${sep}..

export PYENGINE_WORKSPACE_DIR_UNIX=$(pwd)/..

export PYTHONPATH=${PYENGINE_WORKSPACE_DIR}${sep}Tools${sep}XParser

export MAYA_SCRIPT_PATH=${PYENGINE_WORKSPACE_DIR}${sep}Tools${sep}LevelTools

export MAYA_SHELF_PATH=${PYENGINE_WORKSPACE_DIR}${sep}Tools${sep}XParser



if [ "$OSTYPE" = "$WINNAME" ] ; then
	
	export PREMAKE_PATH=${PYENGINE_WORKSPACE_DIR}${sep}External${sep}CompiledSource${sep}"premake-stable"${sep}src
	export PATH=$PATH:$PYENGINE_WORKSPACE_DIR/External/CompiledSource/"bin-win"/"premake-stable"/debug
	alias premake=premake4-stable
	#pepython.bat is a script that runs xbox environment setup and then launches python
	alias pepython="cygstart pepython.bat"
	export MAYA_DIR="C:\\Program Files\\Autodesk\\Maya2014"
    export VSDIR="C:\\Program Files (x86)\\Microsoft Visual Studio 10.0\\Common7\\IDE"
else
	export PREMAKE_PATH=${PYENGINE_WORKSPACE_DIR}${sep}External${sep}CompiledSource${sep}"premake-dev-iphone-xcode4-master"${sep}src
	export PATH=$PATH:${PYENGINE_WORKSPACE_DIR}${sep}External${sep}CompiledSource${sep}"bin-osx"${sep}"premake-dev-iphone-xcode-master"${sep}debug
    export MAYA_DIR="${sep}Applications${sep}Autodesk${sep}maya2014"
	alias premake=premake-dev-iphone-xcode-master
    alias pepython=python
   
fi

echo "exported PYENGINE_WORKSPACE_DIR=$PYENGINE_WORKSPACE_DIR"
echo "exported PYTHONPATH=$PYTHONPATH"
echo "exported MAYA_SCRIPT_PATH=$MAYA_SCRIPT_PATH"
echo "exported MAYA_SHELF_PATH=$MAYA_SHELF_PATH"
echo "exported MAYA_DIR=$MAYA_DIR"
echo "exported PREMAKE_PATH=$PREMAKE_PATH"
echo "exported PATH=$PATH"

