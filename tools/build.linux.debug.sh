#!/bin/bash
cmake  -S ../service -B ../build/service
cmake --build ../build/service


rm -r ../build/Debug || 0
mkdir ../build/Debug
mkdir ../build/Debug/service/
mkdir ../build/Debug/service/bin

cp ../build/service/KVNForceService ../build/Debug/service/
cp  -R ../build/service/bin/linux_amd64/. ../build/Debug/service/bin/linux_amd64/


cmake -S ../app -B ../build/app
cmake --build ../build/app

cp ../build/app/KVNForce ../build/Debug/ 
cp ../app/res/app_icon.png ../build/Debug/ 
