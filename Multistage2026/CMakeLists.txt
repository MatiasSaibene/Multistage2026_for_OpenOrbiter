cmake_minimum_required(VERSION 3.16)
project(Multistage2026)
cmake_policy(SET CMP0002 NEW)  # evitar duplicación de targets
cmake_policy(SET CMP0079 NEW)  # permitir link externo a targets
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (Debug or Release)" FORCE)
endif()

# Ruta base del SDK
set(ORBITER_ROOT "C:/Users/Matias_Saibene/Desktop/OpenOrbiter2024")

# Submódulos
## set(MULTISTAGE2015_MFD_PATH ${ORBITER_ROOT}/Orbitersdk/samples/Multistage2015_MFD)
set(STAGE2010_PATH ${ORBITER_ROOT}/Orbitersdk/samples/Stage_dll_2010)

# Salidas
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Multistage2026_BIN)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

foreach(output_config ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${output_config} OUTPUTCONFIG)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
endforeach()

# Subdirectorios internos
add_subdirectory(Multistage2026_Core)
add_subdirectory(EmptyModule)
add_subdirectory(Crawler)
add_subdirectory(MS_Hangar)
add_subdirectory(MS_Camera)
add_subdirectory(boom)

# Subdirectorios externos (con carpetas de build distintas)
## add_subdirectory(${MULTISTAGE2015_MFD_PATH} "${CMAKE_BINARY_DIR}/build_MFD")
add_subdirectory(${STAGE2010_PATH} "${CMAKE_BINARY_DIR}/build_stage2010")
