@echo off

for %%i in ("WEB\*.TXT") do (
  echo Processing: "%%~nxi"
  rdbf otdb.db "%%i"
)
pause
