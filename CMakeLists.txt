cmake_minimum_required(VERSION 3.10)
project(SenseGloveTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=0")

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(E1_TestDatos
    examples/E1_TestDatos.cpp
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