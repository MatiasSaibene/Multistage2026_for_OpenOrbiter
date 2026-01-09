cmake_minimum_required(VERSION 3.10)

# Establecer políticas
cmake_policy(SET CMP0081 NEW)

project(Multistage2026 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Variables de ruta
set(ORBITER_ROOT "C:/Users/Matias_Saibene/Desktop/OpenOrbiter2024")
set(ORBITER_SDK_INCLUDE "${ORBITER_ROOT}/Orbitersdk/include")
#set(ORBITER_SDK_LIB_RELEASE "${ORBITER_ROOT}/Orbitersdk/lib/*.lib")
#set(ORBITER_SDK_LIB_DEBUG "${ORBITER_ROOT}/Orbitersdk/lib_debug/*.lib")
set(ORBITER_COMMON_DIR "${ORBITER_ROOT}/Orbitersdk/samples/Common/Dialog")
set(ORBITER_MODULES "${ORBITER_ROOT}/Modules")

file(GLOB ORBITER_SDK_LIBS "${ORBITER_ROOT}/Orbitersdk/lib_debug/*.lib")

# Archivos fuente y encabezados
set(SOURCES
    DevDlg.cpp
    DevModeCtrl.cpp
    Graph.cpp
    Guidance.cpp
    Multistage2026.cpp
    parser.cpp
    Particles.cpp
    peg.cpp
    WriteIni.cpp
    Resources.rc    
    ${ORBITER_COMMON_DIR}/TabDlg.cpp
)

set(HEADERS
    DevDlg.hpp
    DevModeCtrl.hpp
    Graph.hpp
    Multistage2026.hpp
    resource.h
    WriteIni.hpp
    ${ORBITER_COMMON_DIR}/TabDlg.h
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
   ${ORBITER_SDK_LIBS}
    uxtheme.lib
    comctl32.lib
    user32.lib
    gdi32.lib
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:/MDd>
        $<$<CONFIG:Release>:/MD>
    )
endif()