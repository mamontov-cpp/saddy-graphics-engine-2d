@echo off
SET EXPAND="expand-gnu"
for /f "usebackq delims=|" %%a in (`dir *.cpp /s/b`) do (
        echo %%a
		%EXPAND%  --initial -t 4 "%%a" > "%%a.txt"
		cat "%%a.txt" > "%%a"
		del "%%a.txt"
)

for /f "usebackq delims=|" %%a in (`dir *.h /s/b`) do (
        echo %%a
		%EXPAND%  --initial -t 4 "%%a" > "%%a.txt"
		cat "%%a.txt" > "%%a"
		del "%%a.txt"
)