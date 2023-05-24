:: Copyright(c) LeeEndl; License Apache License 2.0

@echo off

if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" (
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
devenv "neko.sln" /build Release
) else ( 
@color 4
@echo Visual Studio 2022 internal build tools not found.
)

@Pause>nul