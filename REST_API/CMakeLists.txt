cmake_minimum_required(VERSION 3.10)

project(REST_API)

# Try to find fmt
find_package(fmt CONFIG QUIET)
if(NOT fmt_FOUND)
    message(STATUS "fmt not found via find_package")
endif()

# Manually configure PostgreSQL paths for MSYS64
# Try to locate libpq in MSYS64
find_file(LIBPQ_HEADER NAMES libpq-fe.h 
    PATHS C:/msys64/ucrt64/include
          C:/msys64/mingw64/include
    NO_DEFAULT_PATH)

find_library(LIBPQ_LIBRARY NAMES pq libpq
    PATHS C:/msys64/ucrt64/lib
          C:/msys64/mingw64/lib
    NO_DEFAULT_PATH)

if(LIBPQ_HEADER AND LIBPQ_LIBRARY)
    message(STATUS "Found libpq: ${LIBPQ_LIBRARY}")
    get_filename_component(PostgreSQL_INCLUDE_DIR ${LIBPQ_HEADER} DIRECTORY)
else()
    message(WARNING "libpq not found in MSYS64. Please install: pacman -S mingw-w64-ucrt64-postgresql")
    set(PostgreSQL_INCLUDE_DIR "C:/msys64/ucrt64/include")
    set(LIBPQ_LIBRARY "C:/msys64/ucrt64/lib/libpq.dll.a")
endif()

add_executable(REST_API main.c utils.c headers/utils.h)

if(fmt_FOUND)
    target_link_libraries(REST_API PRIVATE fmt::fmt ${LIBPQ_LIBRARY})
else()
    target_link_libraries(REST_API PRIVATE ${LIBPQ_LIBRARY})
endif()

target_include_directories(REST_API PRIVATE ${PostgreSQL_INCLUDE_DIR})