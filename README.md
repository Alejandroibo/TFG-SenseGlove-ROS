# TFG-SenseGlove-ROS
TFG del desarrollo de una aplicacion para el control de brazo robot simulado con ROS utilizando el guante SenseGlove Nova

Esta carpeta incluye principalmente informacion de gestos general (Carpeta Data), librerias (Carpeta Include), y programas de ejemplo y utilidad (Carpeta src).

Para compilar los archivos de src, hacer uso del CMAKE.

Aparte, esta la carpeta ROS-WS, la cual contiene varios paquetes ROS con nodos para las distintas simulaciones.
Estos paquetes tienen sus propias formas de compilarse y ejecutarse, al igual que su propia seleccion de headers e informacion de gesto,
segun la necesidad de la simulacion

Finalmente, la carpeta SenseCom contiene la herramienta SenseCom para la conexion y calibracion del guante Nova 1. Antes de ejecutar cualquier programa que haga uso de los guantes es necesario ejecutar este programa, asegurarse de que los guantes se conectan, y, preferiblemente, calibrar los guantes.