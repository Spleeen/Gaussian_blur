#Vérification de la version (Modern CMake (3.1+)
#Pour l'optimisation inter-procédurale
cmake_minimum_required(VERSION 3.9)

if(${CMAKE_VERSION} VERSION_LESS 3.9)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

#Nom du projet
set(PROJ_NAME "Gaussian_Blur" )

#Type de build
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BUILD_TYPE})

#Recherche de module
set(CMAKE_MODULE_PATH "../cmake")

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
    endif()

#Déclaration du projet (la 1ère chose à mettre)
project(
    ${PROJ_NAME} 
    VERSION 1.0
    DESCRIPTION "Projet EPSI"
    )

#Inclusion de la bibliothèque SDL
find_package(SDL REQUIRED  >= 1.2.0)
find_package(SDL_image REQUIRED)
find_package(SDL_ttf REQUIRED)
find_package(SDL_gfx REQUIRED)

#Génération de la liste des fichiers sources / headers
#! Variante GLOB_RECURSE cherche dans les sous-dossiers
file(
    GLOB_RECURSE
    SOURCES
    ../src/*.cpp
    )

file(
    GLOB_RECURSE
    HEADERS
    ../src/*.h
    ../include/*.h
    )

#Déclaration de l'exécutable
add_executable(
	${PROJ_NAME} 
	${SOURCES}
	${HEADERS}
    )

#Activer C++17
target_compile_features(
    ${PROJ_NAME}  
    PUBLIC 
    cxx_std_17
    )

set_target_properties(
    ${PROJ_NAME}  
    PROPERTIES 
    CXX_EXTENSIONS OFF
    )

#Optimisation inter-procédurale
include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
    set_target_properties(
        ${PROJ_NAME} 
        PROPERTIES 
        INTERPROCEDURAL_OPTIMIZATION 
        TRUE)
endif()

target_include_directories (
    ${PROJ_NAME}
    PRIVATE
    ${SDL_INCLUDE_DIRS}
    ${SDL_IMAGE_INCLUDE_DIR}
    ${SDL_TTF_INCLUDE_DIR}   
    ${SDL_GFX_INCLUDE_DIR}
    ) 

#Linker
target_link_libraries(
    ${PROJ_NAME} ${SDL_LIBRARY} 
    ${SDL_IMAGE_LIBRARY} 
    ${SDL_TTF_LIBRARY}
    ${SDL_GFX_LIBRARY}
    )


