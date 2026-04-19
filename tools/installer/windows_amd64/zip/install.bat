
@echo off
if not "%1"=="am_admin" (
    @powershell  "Start-Process -Verb RunAs -FilePath '%0' -WorkingDirectory '%~dp0' -ArgumentList 'am_admin','%~dp0'"
    exit /b
)

xcopy %~dp0\XRayClientLight\ "%ProgramFiles%\XRayClientLight" /E /I /H /Y
"%ProgramFiles%\XRayClientLight\service\XRayClientLightService.exe" install
"%ProgramFiles%\XRayClientLight\service\XRayClientLightService.exe" start

copy "%ProgramFiles%\XRayClientLight\X2RayClientLight.lnk" "C:\Users\%USERNAME%\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup\"
copy "%ProgramFiles%\XRayClientLight\X2RayClientLight.lnk" "C:\Users\%USERNAME%\Desktop\"
