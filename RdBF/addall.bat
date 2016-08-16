@echo off
rem Put DB file in this folder to use.

for %%i in ("WEB\*.TXT") do (
  echo Processing: "%%~nxi"
  rdbf otdb.db "%%i"
)
pause
