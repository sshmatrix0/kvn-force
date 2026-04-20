cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=C:\Qt\6.11.0\msvc2022_64 -S ..\service -B ..\build\service
cmake --build ..\build\service 

rmdir /s /q ..\build\Debug

xcopy ..\build\service\Debug\KVNForceService.exe ..\build\Debug\service\ /E /I /H /Y
C:\Qt\6.11.0\msvc2022_64\bin\windeployqt6.exe ..\build\Debug\service\KVNForceService.exe
xcopy  ..\build\service\bin ..\build\Debug\service\bin /E /I /H /Y
xcopy C:\Users\ksp\vcpkg\installed\x64-windows\bin\cpptrace.dll ..\build\Debug\service\ /Y


cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=C:\Qt\6.11.0\msvc2022_64 -S ..\app -B ..\build\app
cmake --build ..\build\app 

xcopy ..\build\app\Debug\KVNForce.* ..\build\Debug\app\ /E /I /H /Y
C:\Qt\6.11.0\msvc2022_64\bin\windeployqt6.exe --qmldir ..\app\src\gui_forms\  ..\build\Debug\app\KVNForce.exe
xcopy C:\Users\ksp\vcpkg\installed\x64-windows\bin\cpptrace.dll ..\build\Debug\app\ /Y