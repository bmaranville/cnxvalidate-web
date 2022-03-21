cmake_minimum_required(VERSION 3.10)
include(FetchContent)

# pre-cache the linker flags
set(SRC_VERSION "v2.9.12" CACHE STRING "version to build")

set(SRC_HASH_v2.9.12 8a4ddd706419c210b30b8978a51388937fd9362c34fc9a3d69e4fcc6f8055ee0)

FetchContent_Declare(
  libxml-src
  URL https://github.com/GNOME/libxml2/archive/refs/tags/${SRC_VERSION}.tar.gz
  URL_HASH SHA256=${SRC_HASH_${SRC_VERSION}}
)
FetchContent_Populate(libxml-src)

# set the project name
project(libxml2-wasm)

option(LIBXML2_WITH_PYTHON "Build python support" OFF)
option(LIBXML2_WITH_HTTP "Build http support" OFF)
option(LIBXML2_WITH_FTP "Build ftp support" OFF)
option(LIBXML2_WITH_LZMA "Build lzma support" OFF)
# option(LIBXML2_WITH_THREADS "Build threads support" OFF)
option(BUILD_SHARED_LIBS "Build shared libs" OFF)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/dist)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -sUSE_ZLIB=1")
add_subdirectory(${libxml-src_SOURCE_DIR} build)