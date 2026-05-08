cmake_minimum_required(VERSION 3.8)
project(senseglove_nova)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(captador_guantes src/captador_guantes.cpp)
ament_target_dependencies(captador_guantes rclcpp std_msgs)
target_include_directories(captador_guantes PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
target_compile_features(captador_guantes PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

add_executable(test_datos src/test_datos.cpp)
ament_target_dependencies(test_datos rclcpp std_msgs)
target_include_directories(test_datos PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
target_compile_features(test_datos PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

add_executable(test_interprete src/test_interprete.cpp)
ament_target_dependencies(test_interprete rclcpp std_msgs)
target_include_directories(test_interprete PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
target_compile_features(test_interprete PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

install(TARGETS 
  captador_guantes
  test_datos
  test_interprete
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
