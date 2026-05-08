# Workspace ROS

Zona de desarrollo de los paquetes ROS para los simuladores

Sourcear ros2 (Underlay):
-Command Prompt
-cd C:\pixi_ws
-pixi shell
-call C:\pixi_ws\ros2-windows\local_setup.bat

Crear paquete:

-ros2 pkg create --build-type ament_cmake --license Apache-2.0 --node-name my_node my_package

Ejecutar paquete:

call install\setup.bat

Construir workspace:
IMPORTANTE: Utilizar terminal "x64 Native Tools Command Prompt for VS 2019"
 Press Windows key.
Release Windows key.
Type x64

colcon build --merge-install