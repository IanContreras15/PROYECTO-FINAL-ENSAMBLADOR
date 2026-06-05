#include "juego.h"
#include <windows.h>
#include <conio.h>

static void mostrar_menu() {     
    limpiar_pantalla();
    printf("============================================================\n");
    printf("                            LABERINTO                       \n");
    printf("============================================================\n");
    printf("\n");
    printf("  1. Jugar\n");
    printf("  2. Salir\n");
    printf("\n");
    printf("Selecciona una opcion: ");
}

static void mensaje_inicio() {
    limpiar_pantalla();
    printf("============================================================\n");
    printf("                   Iniciando Laberinto...                   \n");
    printf("============================================================\n");
    printf("\n");
    printf("  Controles:\n");
    printf("    W = Arriba\n");
    printf("    S = Abajo\n");
    printf("    A = Izquierda\n");
    printf("    D = Derecha\n");
    printf("    Q = Salir\n");
    printf("\n");
    printf("  Recoge la llave (K) para abrir la puerta (D)\n");
    printf("  Cuidado con las trampas (,) te quitan 10 de salud\n");
    printf("  Llega a la salida (E) para completar el nivel\n");
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
                printf("\nMoneda recolectada! +1\n");
                Sleep(500);
            } else if (resultado == 2) {
                limpiar_pantalla();
                imprimir_ventana(nivel.mapa, jugador->fila, jugador->columna);
                imprimir_hud(jugador, &nivel);
                printf("\nLlave recogida! Ahora puedes abrir la puerta.\n");
                Sleep(700);
            } else if (resultado == 3) {
                limpiar_pantalla();
                imprimir_ventana(nivel.mapa, jugador->fila, jugador->columna);
                imprimir_hud(jugador, &nivel);
                printf("\nPuerta abierta!\n");
                Sleep(700);
            } else if (resultado == -2) {
                limpiar_pantalla();
                imprimir_ventana(nivel.mapa, jugador->fila, jugador->columna);
                imprimir_hud(jugador, &nivel);
                printf("\nTrampa! -10 de salud. Salud actual: %d\n", jugador->salud);
                Sleep(700);

                if (jugador->salud <= 0) {
                    limpiar_pantalla();
                    printf("\n============================================================\n");
                    printf("                      GAME OVER                             \n");
                    printf("============================================================\n");
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