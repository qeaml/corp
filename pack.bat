@echo off

if exist target\pkg (rmdir /s/q target\pkg)
mkdir target\pkg

for %%L in (target\*.lib) do (
  mkdir target\pkg\%%~nL
  copy target\%%~nL.lib target\pkg\%%~nL >NUL
  copy target\%%~nL.dll target\pkg\%%~nL >NUL
)

for %%E in (target\*.exe) do (
  copy %%E target\pkg >NUL
)
