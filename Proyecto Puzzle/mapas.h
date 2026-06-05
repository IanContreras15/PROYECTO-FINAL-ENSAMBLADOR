#ifndef MAPAS_H
#define MAPAS_H

#define FILAS_MAPA    60
#define COLUMNAS_MAPA 60
#define VENTANA_FILAS 20
#define VENTANA_COLS  20

#define SIM_PARED    '#'
#define SIM_TRAMPA   ','
#define SIM_CAMINO   '.'
#define SIM_JUGADOR  'P'
#define SIM_MONEDA   'M'
#define SIM_LLAVE    'K'
#define SIM_PUERTA   'D'
#define SIM_SALIDA   'E'

extern char mapa1[FILAS_MAPA][COLUMNAS_MAPA + 1];
extern char mapa2[FILAS_MAPA][COLUMNAS_MAPA + 1];
extern char mapa3[FILAS_MAPA][COLUMNAS_MAPA + 1];

#define NIVEL1_INI_FILA 1
#define NIVEL1_INI_COL  1

#define NIVEL2_INI_FILA 1
#define NIVEL2_INI_COL  1

#define NIVEL3_INI_FILA 1
#define NIVEL3_INI_COL  1

#endif
