#ifndef JUEGO_H
#define JUEGO_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "mapas.h"
#include "colores.h"

#define SALUD_INICIAL  100
#define DANO_TRAMPA     10
#define TOTAL_NIVELES    3

typedef struct {
    int fila;
    int columna;
    int salud;
    int tiene_llave;
    int monedas;
    int pasos;
    int monedas_totales;
    int pasos_totales;
} Jugador;

typedef struct {
    char (*mapa)[COLUMNAS_MAPA];
    int  numero;
    int  total_monedas;
    int  celdas_libres;
    int  fila_inicio;
    int  col_inicio;
} Nivel;

void iniciar_jugador(Jugador *jugador, int fila_ini, int col_ini);
void iniciar_nivel(Nivel *nivel, int numero_nivel);
void imprimir_ventana(char mapa[FILAS_MAPA][COLUMNAS_MAPA], int fila_jugador, int col_jugador);
void imprimir_hud(const Jugador *jugador, const Nivel *nivel);
int  mover_jugador(Jugador *jugador, Nivel *nivel, char direccion);
void mostrar_resumen_nivel(const Jugador *jugador, const Nivel *nivel, int puntaje);
void mostrar_resumen_final(int monedas_totales, int pasos_totales, int niveles_completados, int puntaje_final);
int  preguntar_reintentar();
void limpiar_pantalla();

int contar_caracteres(const char *mapa, int total_celdas, char objetivo);
int validar_movimiento(const char *mapa, int columnas, int fila, int columna);
int calcular_puntaje(int monedas, int pasos, int niveles_completados);
int detectar_objeto(const char *mapa, int columnas, int fila, int columna, char objeto);
int contar_celdas_libres(const char *mapa, int total_celdas);

/* Rutina ASM: establece color de consola (0-15) */
void establecer_color(int color);

#endif
