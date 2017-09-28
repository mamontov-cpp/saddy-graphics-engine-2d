echo Starting tests
tests-animations-release.exe || goto :error
tests-db-release.exe || goto :error
tests-sad-release.exe || goto :error
tests-dukpp-03-release.exe || goto :error
tests-freetype-release.exe || goto :error
tests-hfsm-release.exe || goto :error
tests-input-release.exe || goto :error
tests-p2d-release.exe || goto :error
tests-pipeline-release.exe || goto :error
tests-resource-release.exe || goto :error
tests-sad-release.exe || goto :error
tests-layouts-release.exe || goto :error
goto :EOF

:error
echo Failed with error #%errorlevel%.
exit /b %errorlevel%