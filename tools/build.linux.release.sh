#!/bin/bash
cmake  -S ../service -B ../build/service
cmake --build ../build/service --config Release


rm -r ../build/Release || 0
mkdir ../build/Release
mkdir ../build/Release/service/
mkdir ../build/Release/app/
mkdir ../build/Release/service/bin

python3 ./linuxdeployqt6.py-0.2/linuxdeployqt6.py -force -qtdir ~/Qt/6.*.*/gcc_64/ -out-lib-dir ../build/Release/service/lib -out-dir ../build/Release/service/  ../build/service/KVNForceService
#cp ../build/service/KVNForceService ../build/Release/service/
cp  -R ../build/service/bin/linux_amd64/. ../build/Release/service/bin/linux_amd64/


cmake -S ../app -B ../build/app
cmake --build ../build/app --config Release

python3 ./linuxdeployqt6.py-0.2/linuxdeployqt6.py -force -qtdir ~/Qt/6.*.*/gcc_64/ -out-lib-dir ../build/Release/app/lib -out-dir ../build/Release/app/   -qmlscandir ../app/src/gui_forms/ ../build/app/KVNForce
cp ../app/res/app_icon.png ../build/Release/app/ 
