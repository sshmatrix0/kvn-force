@echo off
if not "%1"=="am_admin" (
    @powershell  "Start-Process -Verb RunAs -FilePath '%0' -WorkingDirectory '%~dp0' -ArgumentList 'am_admin','%~dp0'"
    exit /b
)

taskkill /im X2RayClientLight.exe

timeout /t 3s

"%ProgramFiles%\XRayClientLight\service\XRayClientLightService.exe" stop

timeout /t 1s

"%ProgramFiles%\XRayClientLight\service\XRayClientLightService.exe" uninstall

rmdir /s /q "%ProgramFiles%"\XRayClientLight\

del "C:\Users\%USERNAME%\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup\X2RayClientLight.lnk"
del "C:\Users\%USERNAME%\Desktop\X2RayClientLight.lnk"

pause
