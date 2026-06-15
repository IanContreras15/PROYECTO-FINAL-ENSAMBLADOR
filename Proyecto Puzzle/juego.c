#include "juego.h"
#include <conio.h>
#include <string.h>

extern char mapa1[FILAS_MAPA][COLUMNAS_MAPA + 1];
extern char mapa2[FILAS_MAPA][COLUMNAS_MAPA + 1];
extern char mapa3[FILAS_MAPA][COLUMNAS_MAPA + 1];

void limpiar_pantalla() {
    system("cls");
}

void iniciar_jugador(Jugador *jugador, int fila_ini, int col_ini) {
    jugador->fila            = fila_ini;
    jugador->columna         = col_ini;
    jugador->salud           = SALUD_INICIAL;
    jugador->tiene_llave     = 0;
    jugador->monedas         = 0;
    jugador->pasos           = 0;
}

void iniciar_nivel(Nivel *nivel, int numero_nivel) {
    nivel->numero = numero_nivel;

    if (numero_nivel == 1) {
        nivel->mapa        = (char (*)[COLUMNAS_MAPA])mapa1;
        nivel->fila_inicio = NIVEL1_INI_FILA;
        nivel->col_inicio  = NIVEL1_INI_COL;
    } else if (numero_nivel == 2) {
        nivel->mapa        = (char (*)[COLUMNAS_MAPA])mapa2;
        nivel->fila_inicio = NIVEL2_INI_FILA;
        nivel->col_inicio  = NIVEL2_INI_COL;
    } else {
        nivel->mapa        = (char (*)[COLUMNAS_MAPA])mapa3;
        nivel->fila_inicio = NIVEL3_INI_FILA;
        nivel->col_inicio  = NIVEL3_INI_COL;
    }

    nivel->total_monedas = contar_caracteres(
        (const char*)nivel->mapa,
        FILAS_MAPA * (COLUMNAS_MAPA + 1),
        SIM_MONEDA
    );

    nivel->celdas_libres = contar_celdas_libres(
        (const char*)nivel->mapa,
        FILAS_MAPA * (COLUMNAS_MAPA + 1)
    );
}

void imprimir_ventana(char mapa[FILAS_MAPA][COLUMNAS_MAPA], int fila_jugador, int col_jugador) {
    int mitad_f = VENTANA_FILAS / 2;
    int mitad_c = VENTANA_COLS  / 2;

    int inicio_f = fila_jugador - mitad_f;
    int inicio_c = col_jugador  - mitad_c;

    if (inicio_f < 0)                            inicio_f = 0;
    if (inicio_c < 0)                            inicio_c = 0;
    if (inicio_f > FILAS_MAPA - VENTANA_FILAS)   inicio_f = FILAS_MAPA - VENTANA_FILAS;
    if (inicio_c > COLUMNAS_MAPA - VENTANA_COLS) inicio_c = COLUMNAS_MAPA - VENTANA_COLS;

    char (*mapa_real)[COLUMNAS_MAPA + 1] = (char (*)[COLUMNAS_MAPA + 1])mapa;

    for (int i = 0; i < VENTANA_FILAS; i++) {
        for (int j = 0; j < VENTANA_COLS; j++) {
            int  r   = inicio_f + i;
            int  c   = inicio_c + j;
            char cel;

            if (r == fila_jugador && c == col_jugador)
                cel = SIM_JUGADOR;
            else
                cel = mapa_real[r][c];

            /* Color segun simbolo — usando rutina ASM establecer_color */
            switch (cel) {
                case SIM_JUGADOR:  establecer_color(COL_CIAN_CLARO);    break;
                case '#':          establecer_color(COL_GRIS);           break;
                case SIM_MONEDA:   establecer_color(COL_AMARILLO_CLARO); break;
                case SIM_LLAVE:    establecer_color(COL_AMARILLO);       break;
                case SIM_PUERTA:   establecer_color(COL_MAGENTA);        break;
                case SIM_SALIDA:   establecer_color(COL_VERDE_CLARO);    break;
                case SIM_TRAMPA:   establecer_color(COL_ROJO_CLARO);     break;
                case '.':          establecer_color(COL_GRIS);           break;
                default:           establecer_color(COL_BLANCO);         break;
            }

            putchar(cel);
            reset_color();
        }
        putchar('\n');
    }
}

void imprimir_hud(const Jugador *jugador, const Nivel *nivel) {
    set_color(COL_CIAN);
    printf("========================================\n");
    reset_color();

    /* Nivel */
    printf("Nivel: ");
    set_color(COL_AMARILLO_CLARO); printf("%d", nivel->numero); reset_color();

    printf("  |  Salud: ");
    if (jugador->salud > 60)      set_color(COL_VERDE_CLARO);
    else if (jugador->salud > 30) set_color(COL_AMARILLO_CLARO);
    else                          set_color(COL_ROJO_CLARO);
    printf("%d", jugador->salud);
    reset_color();

    printf("  |  Llave: ");
    if (jugador->tiene_llave) { set_color(COL_AMARILLO); printf("SI"); }
    else                      { set_color(COL_GRIS);     printf("NO"); }
    reset_color();
    printf("\n");

    printf("Monedas: ");
    set_color(COL_AMARILLO_CLARO);
    printf("%d/%d", jugador->monedas, nivel->total_monedas);
    reset_color();
    printf("  |  Pasos: ");
    set_color(COL_CIAN_CLARO);
    printf("%d", jugador->pasos);
    reset_color();
    printf("\n");

    set_color(COL_CIAN);
    printf("========================================\n");
    reset_color();
}

int mover_jugador(Jugador *jugador, Nivel *nivel, char direccion) {
    int nueva_fila = jugador->fila;
    int nueva_col  = jugador->columna;

    direccion = tolower(direccion);

    if      (direccion == 'w') nueva_fila--;
    else if (direccion == 's') nueva_fila++;
    else if (direccion == 'a') nueva_col--;
    else if (direccion == 'd') nueva_col++;
    else return -1;

    if (nueva_fila < 0 || nueva_fila >= FILAS_MAPA ||
        nueva_col  < 0 || nueva_col  >= COLUMNAS_MAPA)
        return -1;

    char (*mapa_real)[COLUMNAS_MAPA + 1] = (char (*)[COLUMNAS_MAPA + 1])nivel->mapa;

    if (!validar_movimiento((const char*)nivel->mapa, COLUMNAS_MAPA + 1, nueva_fila, nueva_col))
        return -1;

    char celda = mapa_real[nueva_fila][nueva_col];

    if (celda == SIM_PUERTA) {
        if (!jugador->tiene_llave)
            return -1;
        mapa_real[nueva_fila][nueva_col] = SIM_CAMINO;
        jugador->fila    = nueva_fila;
        jugador->columna = nueva_col;
        jugador->pasos++;
        return 3;
    }

    jugador->fila    = nueva_fila;
    jugador->columna = nueva_col;
    jugador->pasos++;

    if (celda == SIM_MONEDA) {
        mapa_real[nueva_fila][nueva_col] = SIM_CAMINO;
        jugador->monedas++;
        return 1;
    }

    if (celda == SIM_LLAVE) {
        mapa_real[nueva_fila][nueva_col] = SIM_CAMINO;
        jugador->tiene_llave = 1;
        return 2;
    }

    if (celda == SIM_SALIDA) {
        return 4;
    }

    if (celda == SIM_TRAMPA) {
        jugador->salud -= DANO_TRAMPA;
        if (jugador->salud < 0) jugador->salud = 0;
        return -2;
    }

    return 0;
}

void mostrar_resumen_nivel(const Jugador *jugador, const Nivel *nivel, int puntaje) {
    set_color(COL_CIAN);
    printf("\n=================================\n");
    set_color(COL_VERDE_CLARO);
    printf("Nivel %d completado\n", nivel->numero);
    reset_color();
    printf("Monedas recolectadas: ");
    set_color(COL_AMARILLO_CLARO); printf("%d / %d\n", jugador->monedas, nivel->total_monedas); reset_color();
    printf("Pasos realizados: ");
    set_color(COL_CIAN_CLARO);     printf("%d\n", jugador->pasos); reset_color();
    printf("Salud restante: ");
    if (jugador->salud > 60)       set_color(COL_VERDE_CLARO);
    else if (jugador->salud > 30)  set_color(COL_AMARILLO_CLARO);
    else                           set_color(COL_ROJO_CLARO);
    printf("%d\n", jugador->salud); reset_color();
    printf("Puntaje del nivel: ");
    set_color(COL_AMARILLO_CLARO); printf("%d\n", puntaje); reset_color();
    set_color(COL_CIAN);
    printf("=================================\n");
    reset_color();
    printf("\nPresiona cualquier tecla para continuar...");
    _getch();
}

void mostrar_resumen_final(int monedas_totales, int pasos_totales, int niveles_completados, int puntaje_final) {
    set_color(COL_AMARILLO_CLARO);
    printf("\n=================================\n");
    printf("Juego completado\n");
    reset_color();
    printf("Monedas totales recolectadas: ");
    set_color(COL_AMARILLO_CLARO); printf("%d\n", monedas_totales); reset_color();
    printf("Pasos totales: ");
    set_color(COL_CIAN_CLARO);     printf("%d\n", pasos_totales); reset_color();
    printf("Niveles completados: ");
    set_color(COL_VERDE_CLARO);    printf("%d\n", niveles_completados); reset_color();
    printf("Puntaje final: ");
    set_color(COL_AMARILLO_CLARO); printf("%d\n", puntaje_final); reset_color();
    set_color(COL_AMARILLO_CLARO);
    printf("=================================\n");
    reset_color();
}

int preguntar_reintentar() {
    char respuesta;
    printf("\nQuieres reintentar el nivel? (s/n): ");
    respuesta = tolower(_getch());
    printf("%c\n", respuesta);
    return (respuesta == 's') ? 1 : 0;
}