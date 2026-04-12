cmake_minimum_required(VERSION 3.10)
project(TFG-SENSEGLOVE-ROS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=0")

set(CMAKE_CXX_STANDARD 17)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

set(HEADERS ${PROJECT_SOURCE_DIR}/include/Aplicacion/)
set(IMPLEMENTS ${PROJECT_SOURCE_DIR}/lib/Aplicacion/)

file( GLOB_RECURSE HPPS "${HEADERS}/*.h" "${HEADERS}/*.hpp" )
file( GLOB_RECURSE CPPS "${IMPLEMENTS}/*.c" "${IMPLEMENTS}/*.cpp" )

add_executable(E3_ReconocimientoGestos
    src/E3_ReconocimientoGestos.cpp 
    ${HPPS}
    ${CPPS}
)

add_executable(E2_GuardadoGestos
    src/E2_GuardadoGestos.cpp
)

add_executable(E1_TestDatos
    src/E1_TestDatos.cpp
)

add_executable(sgcore-client
    src/sgcore-client.cpp
)

# Paths
set(SGCORE_PATH ${PROJECT_SOURCE_DIR}/lib/win64/msvc143/release)

add_library(SGCoreLib SHARED IMPORTED)
add_library(SGConnectLib SHARED IMPORTED)

# DLL (runtime)
set_target_properties(SGCoreLib PROPERTIES
    IMPORTED_LOCATION ${SGCORE_PATH}/sgcore.dll
    IMPORTED_IMPLIB ${SGCORE_PATH}/sgcore.lib
)

set_target_properties(SGConnectLib PROPERTIES
    IMPORTED_LOCATION ${SGCORE_PATH}/sgconnect.dll
    IMPORTED_IMPLIB ${SGCORE_PATH}/sgconnect.lib
)

# Link
target_link_libraries(E1_TestDatos
    SGCoreLib
    SGConnectLib
)

target_link_libraries(sgcore-client
    SGCoreLib
    SGConnectLib
)

target_link_libraries(E2_GuardadoGestos
    SGCoreLib
    SGConnectLib
)

target_link_libraries(E3_ReconocimientoGestos
    SGCoreLib
    SGConnectLib
)
