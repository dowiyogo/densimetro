trigger
=======

Cálculo de la probabilidad de detección de un sensor dada la activación del "trigger", para un material dado.

*Compilación y Ejecución*

Este proyecto usa la versión 9.6.4 de Geant4.

Para compilar, se debe crear una carpeta en la cual se escriban los archivos necesarios. También es necesario 
tener instalado cmake. En dicha carpeta (en este caso 'build') se ejecuta la siguiente línea:

$ cmake /path_to_CMakeList.txt

Luego de esto, si se cumple con las dependencias, se generará un Makefile. Con esto, se procede a compilar:

$ make

Lo que genera el ejecutable 'trigger'. Para correr se debe ejecutar como sigue:

$ ./trigger

Opcionalmente, se puede ejecutar usando el script de ejecución:

$ ./trigger run.mac

Al tertminar la ejecución, si todo funcionó bien, el programam genera un archivo llamado
'output.txt' el cual contiene los resultados de los eventos guardados.

Finalmente, para limpiar se puede ejecutar 

$ make clean

# Si tiene menos de 10 [keV], no considerar partícula


==========================
Especificaciones de Tamaño
==========================

-Largo del Sensor:			50   [mm]
-Largo del Colimador:			 7   [mm]
-Diámetro apertura colimador:		 3   [mm]
-Diámetro exterior colimador:		25.7 [mm]
-Distancia 'x' a la fuente:		10   [mm]