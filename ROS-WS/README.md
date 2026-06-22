# Workspace ROS

Zona de desarrollo de los paquetes ROS para los simuladores.

### brazo_guante
Paquete con nodos para el sistema del brazo robot UR5

### raptor_api_interfaces
Paquete con nodos para el sistema del brazo robot UR5

### senseglove_nova
Paquete con el nodo captador y nodos de ejemplo

### turtlesim_guante
Paquete con nodos para el sistema de turtlesim

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

#### turtlesim_guante

ros2 run senseglove_nova captador_guantes

ros2 run turtlesim_guante interprete_turtlesim

ros2 run turtlesim turtlesim_node

#### brazo_robot

ros2 run senseglove_nova captador_guantes

ros2 run brazo_guante interprete_brazo_v1

ros2 run turtlesim feedback_brazo_v1

## Obtencion rosbag

ros2 bag record -o prueba_turtlesim --compression-mode message --compression-format zstd /guante_estado /turtle1/cmd_vel

ros2 bag record -o prueba_brazo --compression-mode message --compression-format zstd /guante_estado /delta_twist_cmds /in_gripper_status /grip_detected