cmake_minimum_required(VERSION 3.8)
project(turtlesim_guante)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 17)

# =========================================================
# Dependencies
# =========================================================

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(turtlesim REQUIRED)

# =========================================================
# Internal library
# =========================================================

add_library(senseglove_core
  lib/ConjuntoGestos.cpp
  lib/Gesto.cpp
  lib/LectorEscritorGestos.cpp
)

target_include_directories(senseglove_core PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(senseglove_core
  rclcpp
  rclcpp_action
  std_msgs
  geometry_msgs
  turtlesim
)

# =========================================================
# Nodes
# =========================================================

add_executable(interprete_turtlesim
  src/interprete_turtlesim.cpp
)

target_link_libraries(interprete_turtlesim
  senseglove_core
)

ament_target_dependencies(interprete_turtlesim
  rclcpp
  rclcpp_action
  std_msgs
  geometry_msgs
  turtlesim
)

# =========================================================
# Install
# =========================================================

install(TARGETS
  senseglove_core
  interprete_turtlesim
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include/
)

ament_package()