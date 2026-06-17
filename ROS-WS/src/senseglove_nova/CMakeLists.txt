cmake_minimum_required(VERSION 3.8)
project(senseglove_nova)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 17)

# =========================================================
# Dependencies
# =========================================================

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

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
  std_msgs
)

# =========================================================
# Nodes
# =========================================================

add_executable(captador_guantes
  src/captador_guantes.cpp
)

target_link_libraries(captador_guantes
  senseglove_core
)

ament_target_dependencies(captador_guantes
  rclcpp
  std_msgs
)

# ---------------------------------------------------------

add_executable(test_datos
  src/test_datos.cpp
)

target_link_libraries(test_datos
  senseglove_core
)

ament_target_dependencies(test_datos
  rclcpp
  std_msgs
)

# ---------------------------------------------------------

add_executable(test_interprete
  src/test_interprete.cpp
)

target_link_libraries(test_interprete
  senseglove_core
)

ament_target_dependencies(test_interprete
  rclcpp
  std_msgs
)

# ---------------------------------------------------------

add_executable(test_feedback
  src/test_feedback.cpp
)

target_link_libraries(test_feedback
  senseglove_core
)

ament_target_dependencies(test_feedback
  rclcpp
  std_msgs
)

# =========================================================
# Install
# =========================================================

install(TARGETS
  senseglove_core
  captador_guantes
  test_datos
  test_interprete
  test_feedback
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include/
)

ament_package()