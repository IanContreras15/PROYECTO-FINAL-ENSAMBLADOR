## Autor

README y pdf elaborado por Ximena Ramos Campos.

# Bitquest : Explorador de Matrices con C y NASM

## Proyecto Final

*Profesor:* Irving Cardona Camacho

### Integrantes
- Ian Slash Contreras García
- Angel Emmanuel Ortega Sánchez
- Ximena Montserrat Ramos Campos
- Landon Enrique Reyes de Lira

---

## Descripción

Es un juego de laberinto desarrollado en lenguaje C y NASM como proyecto final de la materia. El objetivo del jugador es recorrer distintos niveles, recolectar monedas, encontrar llaves, evitar trampas y llegar a la salida para completar cada escenario.

---

## Requisitos

- NASM instalado.
- GCC (MinGW) instalado.

---

## Ejecución mediante build.bat

La forma más fácil de ejecutar el proyecto es utilizando el archivo build.bat.

En lugar de escribir comandos uno por uno, únicamente debes ejecutarlo y él se encarga de realizar el proceso de compilación.

### Pasos

1. Descargar o descomprimir el proyecto.
2. Abrir la carpeta donde se encuentra el juego.
3. Buscar el archivo llamado build.bat.
4. Dar doble clic sobre él.
5. Esperar unos segundos mientras se realiza el proceso.
6. Una vez terminado, se generará el archivo ejecutable del juego.
7. Abrir el ejecutable para comenzar a jugar.

Esta es la opción recomendada porque es la más rápida y sencilla.

---

## Compilación manual

La compilación manual realiza exactamente lo mismo que el archivo build.bat, pero en lugar de hacerlo automáticamente, debes escribir los comandos por tu cuenta.

Primero se procesa el archivo escrito en lenguaje ensamblador (NASM). Después se unen todos los archivos del proyecto para generar el programa final. Finalmente se ejecuta el juego.

bash
nasm -f win64 rutinas.asm -o rutinas.obj
gcc main.c juego.c mapas.c rutinas.obj -o BitQuest.exe
BitQuest.exe


En resumen, la compilación manual permite realizar paso por paso lo mismo que hace automáticamente el archivo build.bat.

---

## Controles

| Tecla | Acción |
|--------|--------|
| W | Mover arriba |
| A | Mover izquierda |
| S | Mover abajo |
| D | Mover derecha |

---

## Conclusiones

- El proyecto integra funciones desarrolladas tanto en C como en NASM.
- Se recomienda ejecutar el juego desde consola.
- Verificar que NASM y GCC estén configurados correctamente.

---

