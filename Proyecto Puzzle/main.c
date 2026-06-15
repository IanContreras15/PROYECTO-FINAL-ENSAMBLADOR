#include "juego.h"
#include <windows.h>
#include <conio.h>

/* ============================================================
 *  Pantalla de inicio (mejora opcional)
 *  Se muestra una sola vez al arrancar el programa, antes
 *  de llegar al menu principal.
 * ============================================================ */
static void mostrar_pantalla_inicio() {
    limpiar_pantalla();
    set_color(COL_CIAN);
    printf("============================================================\n");
    printf("============================================================\n");
    printf("\n");
    set_color(COL_AMARILLO_CLARO);
    printf("                       L A B E R I N T O                       \n");
    set_color(COL_BLANCO_CLARO);
    printf("            Explorador de Matrices con C y NASM              \n");
    printf("\n");
    set_color(COL_CIAN);
    printf("============================================================\n");
    printf("============================================================\n");
    reset_color();
    printf("\n");
    set_color(COL_VERDE_CLARO);
    printf("  Recolecta monedas, encuentra la llave (K),\n");
    printf("  abre la puerta (D) y llega a la salida (E)\n");
    printf("  para avanzar de nivel.\n");
    reset_color();
    printf("\n");
    set_color(COL_AMARILLO_CLARO);
    printf("  Presiona cualquier tecla para continuar...");
    reset_color();
    _getch();
}

/* ============================================================
 *  Pantalla de victoria (mejora opcional)
 *  Se muestra cuando el jugador completa los TOTAL_NIVELES
 *  niveles, justo antes del resumen final.
 * ============================================================ */
static void mostrar_pantalla_victoria() {
    limpiar_pantalla();
    set_color(COL_VERDE_CLARO);
    printf("============================================================\n");
    printf("============================================================\n");
    printf("\n");
    set_color(COL_AMARILLO_CLARO);
    printf("                     * * VICTORIA * *                       \n");
    printf("\n");
    set_color(COL_VERDE_CLARO);
    printf("       Has completado los %d niveles de BitQuest!\n", TOTAL_NIVELES);
    printf("\n");
    set_color(COL_VERDE_CLARO);
    printf("============================================================\n");
    printf("============================================================\n");
    reset_color();
    printf("\nPresiona cualquier tecla para ver tu resumen final...");
    _getch();
}

static void mostrar_menu() {     
    limpiar_pantalla();
    set_color(COL_CIAN);
    printf("============================================================\n");
    set_color(COL_AMARILLO_CLARO);
    printf("                            LABERINTO                       \n");
    set_color(COL_CIAN);
    printf("============================================================\n");
    reset_color();
    printf("\n");
    set_color(COL_VERDE_CLARO);  printf("  1. Jugar\n");
    set_color(COL_ROJO_CLARO);   printf("  2. Salir\n");
    reset_color();
    printf("\n");
    printf("Selecciona una opcion: ");
}

static void mensaje_inicio() {
    limpiar_pantalla();
    set_color(COL_CIAN);
    printf("============================================================\n");
    set_color(COL_AMARILLO_CLARO);
    printf("                   Iniciando Laberinto...                   \n");
    set_color(COL_CIAN);
    printf("============================================================\n");
    reset_color();
    printf("\n");
    set_color(COL_BLANCO_CLARO); printf("  Controles:\n"); reset_color();
    set_color(COL_VERDE_CLARO);
    printf("    W = Arriba\n");
    printf("    S = Abajo\n");
    printf("    A = Izquierda\n");
    printf("    D = Derecha\n");
    set_color(COL_ROJO_CLARO);
    printf("    Q = Salir\n");
    reset_color();
    printf("\n");
    set_color(COL_AMARILLO);  printf("  Recoge la llave (K)");
    reset_color();            printf(" para abrir la ");
    set_color(COL_MAGENTA);   printf("puerta (D)\n");
    reset_color();
    set_color(COL_ROJO_CLARO); printf("  Cuidado con las trampas (,)");
    reset_color();             printf(" te quitan 10 de salud\n");
    set_color(COL_VERDE_CLARO); printf("  Llega a la salida (E)");
    reset_color();              printf(" para completar el nivel\n");
    printf("\n");
    printf("Presiona cualquier tecla para comenzar...");
    _getch();
}

static void jugar_nivel(int numero_nivel, Jugador *jugador, int *niveles_completados) {
    Nivel nivel;
    int   resultado;
    int   jugando = 1;
    char  tecla;

    while (jugando) {
        iniciar_nivel(&nivel, numero_nivel);
        iniciar_jugador(jugador, nivel.fila_inicio, nivel.col_inicio);
        nivel.mapa[nivel.fila_inicio][nivel.col_inicio] = SIM_CAMINO;

        int nivel_activo = 1;
        while (nivel_activo) {
            limpiar_pantalla();
            imprimir_ventana(nivel.mapa, jugador->fila, jugador->columna);
            imprimir_hud(jugador, &nivel);

            tecla = _getch();
            tecla = tolower(tecla);

            if (tecla == 'q') {
                printf("\nSaliendo del juego...\n");
                exit(0);
            }

            if (tecla != 'w' && tecla != 'a' && tecla != 's' && tecla != 'd')
                continue;

            resultado = mover_jugador(jugador, &nivel, tecla);

            if (resultado == -1)
                continue;

            if (resultado == 1) {
                limpiar_pantalla();
                imprimir_ventana(nivel.mapa, jugador->fila, jugador->columna);
                imprimir_hud(jugador, &nivel);
                set_color(COL_AMARILLO_CLARO);
                printf("\nMoneda recolectada! +1\n");
                reset_color();
                Sleep(500);
            } else if (resultado == 2) {
                limpiar_pantalla();
                imprimir_ventana(nivel.mapa, jugador->fila, jugador->columna);
                imprimir_hud(jugador, &nivel);
                set_color(COL_AMARILLO);
                printf("\nLlave recogida! Ahora puedes abrir la puerta.\n");
                reset_color();
                Sleep(700);
            } else if (resultado == 3) {
                limpiar_pantalla();
                imprimir_ventana(nivel.mapa, jugador->fila, jugador->columna);
                imprimir_hud(jugador, &nivel);
                set_color(COL_MAGENTA_CLARO);
                printf("\nPuerta abierta!\n");
                reset_color();
                Sleep(700);
            } else if (resultado == -2) {
                limpiar_pantalla();
                imprimir_ventana(nivel.mapa, jugador->fila, jugador->columna);
                imprimir_hud(jugador, &nivel);
                set_color(COL_ROJO_CLARO);
                printf("\nTrampa! -10 de salud. Salud actual: %d\n", jugador->salud);
                reset_color();
                Sleep(700);

                if (jugador->salud <= 0) {
                    limpiar_pantalla();
                    set_color(COL_ROJO_CLARO);
                    printf("\n============================================================\n");
                    printf("                      GAME OVER                             \n");
                    printf("============================================================\n");
                    reset_color();
                    printf("\nTe quedaste sin salud en el nivel %d\n", numero_nivel);

                    if (preguntar_reintentar()) {
                        nivel_activo = 0;
                    } else {
                        jugando     = 0;
                        nivel_activo = 0;
                        *niveles_completados = -1;
                    }
                }
            } else if (resultado == 4) {
                int puntaje_nivel = calcular_puntaje(
                    jugador->monedas,
                    jugador->pasos,
                    numero_nivel
                );
                jugador->monedas_totales += jugador->monedas;
                jugador->pasos_totales   += jugador->pasos;

                limpiar_pantalla();
                mostrar_resumen_nivel(jugador, &nivel, puntaje_nivel);

                (*niveles_completados)++;
                jugando      = 0;
                nivel_activo = 0;
            }
        }
    }
}

int main() {
    char opcion;
    int  salir = 0;

    /* Carga los mapas desde mapa1.txt, mapa2.txt y mapa3.txt si existen */
    cargar_mapas();

    /* Pantalla de inicio (mejora opcional) */
    mostrar_pantalla_inicio();

    while (!salir) {
        mostrar_menu();
        opcion = _getch();

        if (opcion == '1') {
            mensaje_inicio();

            Jugador jugador;
            jugador.monedas_totales = 0;
            jugador.pasos_totales   = 0;

            int niveles_completados = 0;

            for (int nivel = 1; nivel <= TOTAL_NIVELES; nivel++) {
                jugar_nivel(nivel, &jugador, &niveles_completados);

                if (niveles_completados == -1)
                    break;
            }

            if (niveles_completados == TOTAL_NIVELES) {
                int puntaje_final = calcular_puntaje(
                    jugador.monedas_totales,
                    jugador.pasos_totales,
                    niveles_completados
                );

                /* Pantalla de victoria (mejora opcional) */
                mostrar_pantalla_victoria();

                limpiar_pantalla();
                mostrar_resumen_final(
                    jugador.monedas_totales,
                    jugador.pasos_totales,
                    niveles_completados,
                    puntaje_final
                );
                printf("\nPresiona cualquier tecla para volver al menu...");
                _getch();
            }

        } else if (opcion == '2') {
            limpiar_pantalla();
            printf("Hasta luego!\n");
            salir = 1;
        }
    }

    return 0;
}