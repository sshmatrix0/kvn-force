set prefix_build=%~dp0\..\..\..\..\build
set prefix_app=%~dp0\..\..\..\..\app

@echo off
setlocal


:: Получить всю строку
for /f "usebackq delims=" %%i in (`powershell -Command "Select-String -Path '%prefix_app%\CMakeLists.txt' -Pattern 'project\(' | Select-Object -First 1 | ForEach-Object { $_.Line }"`) do set "found_line=%%i"


:: Получить только версию (если в формате project(NAME VERSION X.Y.Z)
for /f "usebackq delims=" %%i in (`powershell -Command "(Select-String -Path '%prefix_app%\CMakeLists.txt' -Pattern 'project\([^)]+VERSION\s+([0-9.]+)' | ForEach-Object { $_.Matches.Groups[1].Value }) -join ''"`) do set "version=%%i"

echo Version: %version%


rmdir /s /q %prefix_build%\windows_amd64\zip\
xcopy  %prefix_build%\Release\  %prefix_build%\windows_amd64\zip\XRayClientLight\ /E /I /H /Y
xcopy XRayClientLightService.*  %prefix_build%\windows_amd64\zip\XRayClientLight\service  /E /I /H /Y
xcopy X2RayClientLight.lnk %prefix_build%\windows_amd64\zip\XRayClientLight\ /E /I /H /Y
xcopy %~dp0\app_icon.ico %prefix_build%\windows_amd64\zip\XRayClientLight\ /E /I /H /Y
xcopy %~dp0\uninstall.bat %prefix_build%\windows_amd64\zip\XRayClientLight\ /E /I /H /Y
cd %prefix_build%\windows_amd64\zip\ & 7z a "X2RayClientLight_%version%.zip"
cd %prefix_build%\windows_amd64\zip\ & 7z a "X2RayClientLight_%version%.zip" %~dp0\install.bat