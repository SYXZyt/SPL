@echo off
python --version > NUL
if errorlevel 1 goto noPy

python clc.py
goto end rem We don't use `exit` to ensure that the console stays open after the Python has finished

:noPy
echo "ERROR: Python not found"

:end