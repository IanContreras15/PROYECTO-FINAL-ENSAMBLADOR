#ifndef COLORES_H
#define COLORES_H

#include <windows.h>

/* ── Codigos de color para SetConsoleTextAttribute ── */
#define COL_NEGRO        0
#define COL_AZUL         1
#define COL_VERDE        2
#define COL_CIAN         3
#define COL_ROJO         4
#define COL_MAGENTA      5
#define COL_AMARILLO     6
#define COL_BLANCO       7
#define COL_GRIS         8
#define COL_AZUL_CLARO   9
#define COL_VERDE_CLARO  10
#define COL_CIAN_CLARO   11
#define COL_ROJO_CLARO   12
#define COL_MAGENTA_CLARO 13
#define COL_AMARILLO_CLARO 14
#define COL_BLANCO_CLARO  15

/* Fondo negro por defecto (bits altos = 0) */
static inline void set_color(int color) {
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hCon, (WORD)color);
}

static inline void reset_color() {
    set_color(COL_BLANCO);
}

#endif /* COLORES_H */
