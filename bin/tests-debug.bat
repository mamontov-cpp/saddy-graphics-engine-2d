echo Starting tests
tests-sad-debug.exe || goto :error
goto :EOF

:error
echo Failed with error #%errorlevel%.
exit /b %errorlevel%