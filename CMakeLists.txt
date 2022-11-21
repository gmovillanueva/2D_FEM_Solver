# |========================================================== Projects Settings
cmake_minimum_required(VERSION 3.1...3.14)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

find_program(GNU_CXX_COMPILER NAMES gcc)
message(STATUS ${GNU_COMPILER})

if(UNIX AND NOT APPLE)
    add_definitions(-DUNIX_ENV)
elseif(APPLE)
    add_definitions(-DAPPLE_ENV)
    add_definitions(-DUNIX_ENV)
else()
    add_definitions(-DWINDOWS_ENV)
endif()

set( CMAKE_BUILD_TYPE Release ... FORCE )
set(TARGET poly_fem)
project(${TARGET} VERSION 0.0.3
                 DESCRIPTION "FEM Solver Test"
                 LANGUAGES C CXX)


# |========================================================== Projects Variables
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake_Modules")
set(PROJ_SOURCE "${PROJECT_SOURCE_DIR}/Source")


# |========================================================== Include Modules
include(GNUInstallDirs)
include(Filter)


# |========================================================== Projects Variables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
# set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
# set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_CXX_COMPILER ${GNU_COMPILER})


# |========================================================== Custom Dependencies
find_package(PETSc REQUIRED)



# |========================================================== Dependencies


# |========================================================== Compiler Flags
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")


# |========================================================== Directory Includes
include_directories(${PETSC_INCLUDES})
add_definitions (${PETSC_DEFINITIONS})

# |========================================================== Library Directories


# |========================================================== Setting Libraries
set(LIBS ${PETSC_LIBRARIES})
set(LIBS ${LIBS})


# |========================================================== Compile
add_executable(${TARGET})


# |========================================================== Source File Locations
add_subdirectory(Source)


# |========================================================== Linking Everything
target_link_libraries(${TARGET} ${LIBS})
