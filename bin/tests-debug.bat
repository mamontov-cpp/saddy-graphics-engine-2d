echo Starting tests
tests-animations-debug.exe || goto :error
tests-db-debug.exe || goto :error
tests-sad-debug.exe || goto :error
tests-dukpp-03-debug.exe || goto :error
tests-freetype-debug.exe || goto :error
tests-hfsm-debug.exe || goto :error
tests-input-debug.exe || goto :error
tests-p2d-debug.exe || goto :error
tests-pipeline-debug.exe || goto :error
tests-resource-debug.exe || goto :error
tests-sad-debug.exe || goto :error
tests-layouts-debug.exe || goto :error
goto :EOF

:error
echo Failed with error #%errorlevel%.
exit /b %errorlevel%