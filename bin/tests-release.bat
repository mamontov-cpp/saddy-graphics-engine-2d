echo Starting tests
REM tests-animations-release.exe || goto :error
REM tests-db-release.exe || goto :error
REM tests-sad-release.exe || goto :error
REM tests-dukpp-03-release.exe || goto :error
tests-freetype-release.exe || goto :error
REM tests-hfsm-release.exe || goto :error
REM tests-input-release.exe || goto :error
REM tests-p2d-release.exe || goto :error
REM tests-pipeline-release.exe || goto :error
REM tests-resource-release.exe || goto :error
REM tests-sad-release.exe || goto :error
REM tests-layouts-release.exe || goto :error
goto :EOF

:error
echo Failed with error #%errorlevel%.
exit /b %errorlevel%