#!/bin/bash
set -x
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
BUILD_DIR=$SCRIPT_DIR/../../../../build
BUILD_DIR_LINUX=$BUILD_DIR/linux_amd64
BUILD_DIR_QTIF=$BUILD_DIR_LINUX/qtif
APP_DIR=$SCRIPT_DIR/../../../../app

rm -rf $BUILD_DIR_QTIF || 0
mkdir -p $BUILD_DIR_QTIF
mkdir -p $BUILD_DIR_QTIF/packages
cp -R $SCRIPT_DIR/config/. $BUILD_DIR_QTIF/config/

APP_VERSION=`cat $APP_DIR/CMakeLists.txt | grep project\(kvn_force | awk '{print $3}'`

sed -i 's/%VERSION%/'${APP_VERSION}'/' $BUILD_DIR_QTIF/config/config.xml

sed -i 's/%DIST_PATH%/\/opt\/KVNForce/' $BUILD_DIR_QTIF/config/config.xml

mkdir -p $BUILD_DIR_QTIF/packages/org.sshmatrix/data
mkdir -p $BUILD_DIR_QTIF/packages/org.sshmatrix/meta

cp -R $BUILD_DIR/Release/. $BUILD_DIR_QTIF/packages/org.sshmatrix/data/
cp -R $SCRIPT_DIR/meta/. $BUILD_DIR_QTIF/packages/org.sshmatrix/meta/
cp -R $SCRIPT_DIR/KVNForce.service $BUILD_DIR_QTIF/packages/org.sshmatrix/data/

RELEASE_DATE=`git --git-dir ${APP_DIR}/../.git log -1 --pretty='%ad' --date=format:'%Y-%m-%d'`


sed -i 's/%VERSION%/'${APP_VERSION}'/' $BUILD_DIR_QTIF/packages/org.sshmatrix/meta/package.xml
sed -i 's/%RELEASE_DATE%/'${RELEASE_DATE}'/' $BUILD_DIR_QTIF/packages/org.sshmatrix/meta/package.xml

cd $BUILD_DIR_QTIF
~/Qt/Tools/QtInstallerFramework/4.*/bin/binarycreator -c config/config.xml -p packages KVNForceInstaller_${APP_VERSION}