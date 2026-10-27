@echo off
setlocal
set "DASHBOARD_URL=http://127.0.0.1:5000"

curl.exe --silent --fail "%DASHBOARD_URL%/" >nul 2>&1
if not errorlevel 1 (
  start "" "%DASHBOARD_URL%"
  exit /b 0
)

start "MOSFET Flask Dashboard" /D "%~dp0" cmd /k python app.py

for /L %%i in (1,1,30) do (
  curl.exe --silent --fail "%DASHBOARD_URL%/" >nul 2>&1
  if not errorlevel 1 (
    start "" "%DASHBOARD_URL%"
    exit /b 0
  )
  timeout /t 1 /nobreak >nul
)

echo Flask did not start. Check the dashboard window for an error.
exit /b 1
