cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=C:\Qt\6.11.0\msvc2022_64 -S ..\service -B ..\build\service
cmake --build ..\build\service 

rmdir /s /q ..\build\Debug

xcopy ..\build\service\Debug\xray_client_light_service.exe ..\build\Debug\service\ /E /I /H /Y
C:\Qt\6.11.0\msvc2022_64\bin\windeployqt6.exe ..\build\Debug\service\xray_client_light_service.exe
xcopy  ..\build\service\bin ..\build\Debug\service\bin /E /I /H /Y
xcopy C:\Users\ksp\vcpkg\installed\x64-windows\bin\cpptrace.dll ..\build\Debug\service\ /Y


cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=C:\Qt\6.11.0\msvc2022_64 -S ..\app -B ..\build\app
cmake --build ..\build\app 

xcopy ..\build\app\Debug\X2RayClientLight.* ..\build\Debug\ /E /I /H /Y
C:\Qt\6.11.0\msvc2022_64\bin\windeployqt6.exe --qmldir ..\app\src\gui_forms\  ..\build\Debug\X2RayClientLight.exe
xcopy C:\Users\ksp\vcpkg\installed\x64-windows\bin\cpptrace.dll ..\build\Debug\ /Y