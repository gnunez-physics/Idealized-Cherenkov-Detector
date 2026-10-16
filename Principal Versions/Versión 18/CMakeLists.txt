# 1. Configuración básica del proyecto y versión mínima de CMake
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(Cherenkov)

# Configuración de compilación optimizada en modo Release
set(CMAKE_BUILD_TYPE Release)

# 2. Búsqueda e integración de Geant4 (obligatorio para compilar)
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

# 3. Directorios de encabezados (.hh)
include_directories(${PROJECT_SOURCE_DIR}/include)

# 4. Automatización de macros (.mac)
# Copia los archivos de macros a la carpeta de compilación para que Geant4 los encuentre
file(GLOB MACRO_FILES "macros/*.mac")
file(COPY ${MACRO_FILES} DESTINATION ${CMAKE_BINARY_DIR}/)




# Copio y pego los .dat que hay en la carpeta build, directamente el archivo, no la carpeta data que está en la madre (Versión 15)
file(GLOB DATA_FILES "data/*.dat")
file(COPY ${DATA_FILES} DESTINATION ${CMAKE_BINARY_DIR}/)



# 5. Detección automática de archivos fuente (.cc en la carpeta src)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

# 6. Definición del ejecutable principal ("muon")
add_executable(muon muon.cc ${sources})

# 7. Enlace con las librerías oficiales de Geant4
target_link_libraries(muon ${Geant4_LIBRARIES})

# 8. Creación de un acceso directo o alias global ("Cherenkov")
add_custom_target(Cherenkov DEPENDS muon)

# Copiar automáticamente el script de fusión ROOT a la carpeta output en el build
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/script/fusion_roots.C DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/output)