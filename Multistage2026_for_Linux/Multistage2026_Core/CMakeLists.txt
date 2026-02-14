cmake_minimum_required(VERSION 3.10)

# Establecer políticas
cmake_policy(SET CMP0081 NEW)

project(Multistage2026 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Variables de ruta
set(ORBITER_ROOT "$ENV{HOME}/orbiter_test/Orbiter")
set(ORBITER_SDK_INCLUDE "${ORBITER_ROOT}/Orbitersdk/include")
set(ORBITER_SDK_LIB "${ORBITER_ROOT}/Orbitersdk/lib/")
set(ORBITER_MODULES "${ORBITER_ROOT}/Modules")

file(GLOB ORBITER_LIBS_DEBUG "${ORBITER_SDK_LIB}/*.a")

# Archivos fuente y encabezados
set(SOURCES
    Guidance.cpp
    Multistage2026.cpp
    parser.cpp
    Particles.cpp
    peg.cpp
    WriteIni.cpp
    Resources.rc
)

set(HEADERS
    Multistage2026.hpp
    resource.h
    WriteIni.hpp
)

# Crear el objetivo como una librería compartida (DLL)
add_library(${PROJECT_NAME} SHARED 
    ${SOURCES}
    ${HEADERS}
)

# Rutas de inclusión
target_include_directories(${PROJECT_NAME} PRIVATE
    ${ORBITER_SDK_INCLUDE}
    ${ORBITER_SOURCE}
    ${ORBITER_COMMON_DIR}
)

# Enlace a librerías: versiones específicas por configuración
target_link_libraries(${PROJECT_NAME}
   ${ORBITER_LIBS_DEBUG}
)