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

    if (inicio_f < 0)                       inicio_f = 0;
    if (inicio_c < 0)                       inicio_c = 0;
    if (inicio_f > FILAS_MAPA - VENTANA_FILAS) inicio_f = FILAS_MAPA - VENTANA_FILAS;
    if (inicio_c > COLUMNAS_MAPA - VENTANA_COLS) inicio_c = COLUMNAS_MAPA - VENTANA_COLS;
    char (*mapa_real)[COLUMNAS_MAPA + 1] = (char (*)[COLUMNAS_MAPA + 1])mapa;

    for (int i = 0; i < VENTANA_FILAS; i++) {
        for (int j = 0; j < VENTANA_COLS; j++) {
            int r = inicio_f + i;
            int c = inicio_c + j;
            if (r == fila_jugador && c == col_jugador)
                putchar(SIM_JUGADOR);
            else
                putchar(mapa_real[r][c]);
        }
        putchar('\n');
    }
}

void imprimir_hud(const Jugador *jugador, const Nivel *nivel) {
    printf("========================================\n");
    printf("Nivel: %d  |  Salud: %d  |  Llave: %s\n",
        nivel->numero,
        jugador->salud,
        jugador->tiene_llave ? "SI" : "NO"
    );
    printf("Monedas: %d/%d  |  Pasos: %d\n",
        jugador->monedas,
        nivel->total_monedas,
        jugador->pasos
    );
    printf("========================================\n");
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
    printf("\n=================================\n");
    printf("Nivel %d completado\n", nivel->numero);
    printf("Monedas recolectadas: %d / %d\n", jugador->monedas, nivel->total_monedas);
    printf("Pasos realizados: %d\n", jugador->pasos);
    printf("Salud restante: %d\n", jugador->salud);
    printf("Puntaje del nivel: %d\n", puntaje);
    printf("=================================\n");
    printf("\nPresiona cualquier tecla para continuar...");
    _getch();
}

void mostrar_resumen_final(int monedas_totales, int pasos_totales, int niveles_completados, int puntaje_final) {
    printf("\n=================================\n");
    printf("Juego completado\n");
    printf("Monedas totales recolectadas: %d\n", monedas_totales);
    printf("Pasos totales: %d\n", pasos_totales);
    printf("Niveles completados: %d\n", niveles_completados);
    printf("Puntaje final: %d\n", puntaje_final);
    printf("=================================\n");
}

int preguntar_reintentar() {
    char respuesta;
    printf("\nQuieres reintentar el nivel? (s/n): ");
    respuesta = tolower(_getch());
    printf("%c\n", respuesta);
    return (respuesta == 's') ? 1 : 0;
}


int contar_caracteres(const char *mapa, int total_celdas, char objetivo) {
    int conteo = 0;
    for (int i = 0; i < total_celdas; i++) {
        if (mapa[i] == objetivo) {
            conteo++;
        }
    }
    return conteo;
}

int validar_movimiento(const char *mapa, int columnas, int fila, int columna) {
    char celda = mapa[fila * columnas + columna];
    return (celda != SIM_PARED);
}

int calcular_puntaje(int monedas, int pasos, int niveles_completados) {
    int puntaje = (monedas * 100) + (niveles_completados * 500) - pasos;
    return (puntaje < 0) ? 0 : puntaje;
}

int detectar_objeto(const char *mapa, int columnas, int fila, int columna, char objeto) {
    return (mapa[fila * columnas + columna] == objeto);
}

int contar_celdas_libres(const char *mapa, int total_celdas) {
    int libres = 0;
    for (int i = 0; i < total_celdas; i++) {
        if (mapa[i] == SIM_CAMINO) {
            libres++;
        }
    }
    return libres;
}
