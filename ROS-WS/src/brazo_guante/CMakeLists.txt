cmake_minimum_required(VERSION 3.8)
project(brazo_guante)

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
find_package(raptor_api_interfaces REQUIRED)

# =========================================================
# SenseGlove SDK
# =========================================================

if (WIN32)
  set(SGCORE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/win64/msvc143/release
  )

  add_library(SGCoreLib SHARED IMPORTED)

  set_target_properties(SGCoreLib PROPERTIES
    IMPORTED_IMPLIB ${SGCORE_PATH}/sgcore.lib
    IMPORTED_LOCATION ${SGCORE_PATH}/sgcore.dll
  )

  add_library(SGConnectLib SHARED IMPORTED)

  set_target_properties(SGConnectLib PROPERTIES
    IMPORTED_IMPLIB ${SGCORE_PATH}/sgconnect.lib
    IMPORTED_LOCATION ${SGCORE_PATH}/sgconnect.dll
  )

elseif(UNIX)
  set(SGCORE_PATH
    $CMAKE_CURRENT_SOURCE_DIR/lib/linux/x86-64
  )

  add_library(SGCoreLib SHARED IMPORTED)

  set_target_properties(SGCoreLib PROPERTIES
    IMPORTED_LOCATION ${SGCORE_PATH}/libsgcore.so
  )

  add_library(SGConnectLib SHARED IMPORTED)

  set_target_properties(SGConnectLib PROPERTIES
    IMPORTED_LOCATION ${SGCORE_PATH}/libsgconnect.so
  )

endif()

# =========================================================
# Internal library
# =========================================================

add_library(senseglove_core
  lib/ConjuntoGestos.cpp
  lib/Gesto.cpp
  lib/LectorEscritorGestos.cpp
  lib/ControlGuantes.cpp
)

target_include_directories(senseglove_core PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(senseglove_core
  SGCoreLib
  SGConnectLib
)

ament_target_dependencies(senseglove_core
  rclcpp
  rclcpp_action
  std_msgs
  geometry_msgs
)

# =========================================================
# Nodes
# =========================================================

add_executable(feedback_brazo_v1
  src/feedback_brazo_v1.cpp
)

target_link_libraries(feedback_brazo_v1
  senseglove_core
)

ament_target_dependencies(feedback_brazo_v1
  rclcpp
  rclcpp_action
  std_msgs
  geometry_msgs
  raptor_api_interfaces
)

add_executable(interprete_brazo_v1
  src/interprete_brazo_v1.cpp
)

target_link_libraries(interprete_brazo_v1
  senseglove_core
)

ament_target_dependencies(interprete_brazo_v1
  rclcpp
  rclcpp_action
  std_msgs
  geometry_msgs
  raptor_api_interfaces
)

# =========================================================
# Install
# =========================================================

install(TARGETS
  senseglove_core
  interprete_brazo_v1
  feedback_brazo_v1
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include/
)

ament_package()