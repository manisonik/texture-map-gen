set PATH=C:\msys64;%PATH%
msys2_shell.cmd -use-full-path
unset TMP
unset TEMP
./build-cairo-windows.sh
pause