@echo off
nasm -f win64 rutinas.asm -o rutinas.obj
gcc main.c juego.c mapas.c rutinas.obj -o output\JuegoPuzzle.exe
if %errorlevel% == 0 (
    echo Compilacion exitosa
    output\JuegoPuzzle.exe
) else (
    echo Error al compilar
)