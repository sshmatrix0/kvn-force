cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=C:\Qt\6.11.0\msvc2022_64 -S ..\service -B ..\build\service
cmake --build ..\build\service --config Release

rmdir /s /q ..\build\Release

xcopy ..\build\service\Release\xray_client_light_service.exe ..\build\Release\service\ /E /I /H /Y
C:\Qt\6.11.0\msvc2022_64\bin\windeployqt6.exe ..\build\Release\service\xray_client_light_service.exe
xcopy  ..\build\service\bin ..\build\Release\service\bin /E /I /H /Y
xcopy C:\Users\ksp\vcpkg\installed\x64-windows\bin\cpptrace.dll ..\build\Release\service\ /Y


cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=C:\Qt\6.11.0\msvc2022_64 -S ..\app -B ..\build\app
cmake --build ..\build\app --config Release

xcopy ..\build\app\Release\X2RayClientLight.* ..\build\Release\ /E /I /H /Y
C:\Qt\6.11.0\msvc2022_64\bin\windeployqt6.exe --qmldir ..\app\src\gui_forms\  ..\build\Release\X2RayClientLight.exe
xcopy C:\Users\ksp\vcpkg\installed\x64-windows\bin\cpptrace.dll ..\build\Release\ /Y