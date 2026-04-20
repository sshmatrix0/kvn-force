set SCRIPT_DIR=%~dp0
set BUILD_DIR=%SCRIPT_DIR%..\..\build
set BUILD_DIR_WINWOWS=%BUILD_DIR%\windows_amd64
set BUILD_DIR_QTIF=%BUILD_DIR_WINWOWS%\qtif
set APP_DIR=%SCRIPT_DIR%..\..\app
set SERVICE_DIR=%SCRIPT_DIR%..\..\service

::@echo off
setlocal enabledelayedexpansion
for /f "usebackq delims=" %%i in (`powershell -Command "Select-String -Path '%APP_DIR%\CMakeLists.txt' -Pattern 'project\(' | Select-Object -First 1 | ForEach-Object { $_.Line }"`) do set "found_line=%%i"
for /f "usebackq delims=" %%i in (`powershell -Command "(Select-String -Path '%APP_DIR%\CMakeLists.txt' -Pattern 'project\([^)]+VERSION\s+([0-9.]+)' | ForEach-Object { $_.Matches.Groups[1].Value }) -join ''"`) do set "version=%%i"

echo Version: %version%

rmdir /s /q %BUILD_DIR_QTIF%

xcopy  %SCRIPT_DIR%\config\  %BUILD_DIR_QTIF%\config\ /E /I /H /Y

powershell -Command "(Get-Content '%BUILD_DIR_QTIF%\config\config.xml') -replace '%%VERSION%%', '%version%' | Set-Content '%BUILD_DIR_QTIF%\config\config.xml'"
powershell -Command "(Get-Content '%BUILD_DIR_QTIF%\config\config.xml') -replace '%%DIST_PATH%%', 'C:\Program Files\KVNForce' | Set-Content '%BUILD_DIR_QTIF%\config\config.xml'"


xcopy  %BUILD_DIR%\Release\  %BUILD_DIR_QTIF%\packages\org.sshmatrix\data\ /E /I /H /Y
xcopy  %SCRIPT_DIR%\icons\app_icon.ico  %BUILD_DIR_QTIF%\packages\org.sshmatrix\data\app\ /E /I /H /Y
xcopy  %SCRIPT_DIR%\meta\  %BUILD_DIR_QTIF%\packages\org.sshmatrix\meta\ /E /I /H /Y
rename "%BUILD_DIR_QTIF%\packages\org.sshmatrix\data\service\KVNForceService.exe" "KVNForceServiceOrigin.exe"
xcopy %SCRIPT_DIR%\service.windows\ %BUILD_DIR_QTIF%\packages\org.sshmatrix\data\service\ /E /I /H /Y

set GIT_CMD=git --git-dir %SCRIPT_DIR%\..\..\.git log -1 --pretty^^="%%ad" --date=format:"%%Y-%%m-%%d"
for /f "delims=" %%i in ('%GIT_CMD%') do set RELEASE_DATE=%%i

echo %RELEASE_DATE%

::sed -i 's/%VERSION%/'${APP_VERSION}'/' $BUILD_DIR_QTIF/packages/org.sshmatrix/meta/package.xml
::sed -i 's/%RELEASE_DATE%/'${RELEASE_DATE}'/' $BUILD_DIR_QTIF/packages/org.sshmatrix/meta/package.xml

powershell -Command "(Get-Content '%BUILD_DIR_QTIF%\packages\org.sshmatrix\meta\package.xml') -replace '%%VERSION%%', '%version%' | Set-Content '%BUILD_DIR_QTIF%\packages\org.sshmatrix\meta\package.xml'"
powershell -Command "(Get-Content '%BUILD_DIR_QTIF%\packages\org.sshmatrix\meta\package.xml') -replace '%%RELEASE_DATE%%', '%RELEASE_DATE%' | Set-Content '%BUILD_DIR_QTIF%\packages\org.sshmatrix\meta\package.xml'"


cd %BUILD_DIR_QTIF%
C:\Qt\Tools\QtInstallerFramework\4.11\bin\binarycreator -c config\config.xml -p packages KVNForceInstaller_%version%.exe
