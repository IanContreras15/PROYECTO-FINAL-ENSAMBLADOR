@echo off
echo Compilando rutinas en NASM...
nasm -f win64 rutinas.asm -o rutinas.obj
if %errorlevel% neq 0 (
    echo Error al ensamblar rutinas.asm
    pause
    exit /b 1
)

echo Compilando proyecto C + ASM...
gcc main.c juego.c mapas.c rutinas.obj -o output\JuegoPuzzle.exe -lkernel32
if %errorlevel% == 0 (
    echo.
    echo Compilacion exitosa! Iniciando juego...
    output\JuegoPuzzle.exe
) else (
    echo Error al compilar
    pause
)
