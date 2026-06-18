# Workspace ROS

Zona de desarrollo de los paquetes ROS para los simuladores

## Sourcear ros2 (Underlay):
Command Prompt
cd C:\pixi_ws
pixi shell
call C:\pixi_ws\ros2-windows\local_setup.bat

## Sourcer ros2 (Overlay propietario del proyecto)

--Hacerlo en la carpeta ROS-WS 
call install\setup.bat

## Construir workspace:
IMPORTANTE: Utilizar terminal "x64 Native Tools Command Prompt for VS 2019"
Press Windows key.
Release Windows key.
Type x64

colcon build --merge-install 

colcon build --merge-install --packages-select "Paquete a construir 1" "paquete a construir 2" ..

## Crear paquete:

ros2 pkg create --build-type ament_cmake --license Apache-2.0 --node-name my_node my_package

## Ejecutar nodo:

ros2 run "paquete" "nodo"

#### senseglove_nova

ros2 run senseglove_nova captador_guantes

ros2 run senseglove_nova test_interprete

ros2 run senseglove_nova test_feedback

#### senseglove_nova

ros2 run senseglove_nova captador_guantes

ros2 run turtlesim_guante interprete_turtlesim

ros2 run turtlesim turtlesim_node

## Ejecutar archivo launch:

--Dentro de la carpeta launch
ros2 launch "Archivo launch"