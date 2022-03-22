## Process this file with cmake
#==============================================================================
#  NeXus - Neutron & X-ray Common Data Format
#
#  CMakeLists for building the NeXus file validation library
#  and command line validator nxvalidate
#
#  Copyright (C) 2015 Mark Koennecke
#
#  This library is free software; you can redistribute it and/or modify it under
#  the terms of the GNU Lesser General Public License as published by the Free
#  Software Foundation; either version 2 of the License, or (at your option) any
#  later version.
#
#  This library is distributed in the hope that it will be useful, but WITHOUT
#  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
#  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
#  details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with this library; if not, write to the Free Software Foundation, Inc.,
#  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#  For further information, see <http://www.nexusformat.org>
#
#
#==============================================================================
cmake_minimum_required(VERSION 3.10.0)
include(FetchContent)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

#The name of our project
project (cnxvalidate-web)

#------------------------------------------------------------------------------
# set the default installation prefix
#------------------------------------------------------------------------------
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/dist")
#load module for GNU/Linux default installation directories
include(GNUInstallDirs)

#-----------------------------------------------------------------------------
# define package version
#-----------------------------------------------------------------------------
set (cnxvalidate-web_VERSION_MAJOR 0)
set (cnxvalidate-web_VERSION_MINOR 1)
set (NXVERSINFO '1:0:0')

#-----------------------------------------------------------------------------
# include some general purpose modules
#-----------------------------------------------------------------------------
#include(Utilities)
#include(CompilerChecks)

include(FindPkgConfig)

FetchContent_Declare(
  libhdf5
  URL https://github.com/usnistgov/libhdf5-wasm/releases/download/v0.1.1/libhdf5-1_12_1-wasm.tar.gz
  URL_HASH SHA256=e9bb11d89c4f26fa79b9cf1dab6159640c7b184ebf00dc97b098cd4f6de49bfe
)
FetchContent_MakeAvailable(libhdf5)

FetchContent_Declare(
  cnxvalidate-src
  GIT_REPOSITORY https://github.com/nexusformat/cnxvalidate.git
  GIT_TAG        fedfaea6fa0c470f77be2436959200c6df0b92cb # release-1.10.0
)
FetchContent_Populate(cnxvalidate-src)

# add libxml2 dependency that was previously built:
set (LIBXML2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libxml2/dist/include/libxml2)
set (LIBXML2_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libxml2/dist/lib/libxml2.a)
#----------------------------------------------------------------
# Dependencies
#----------------------------------------------------------------
# find_package(HDF5 REQUIRED COMPONENTS HL)
find_package(LibXml2 REQUIRED)

#--------------------------------------------------------------
# Our own stuff, really....
#-------------------------------------------------------------

include_directories(${cnxvalidate-src_SOURCE_DIR}/include ${libhdf5_SOURCE_DIR}/include ${LIBXML2_INCLUDE_DIR})
add_subdirectory(${cnxvalidate-src_SOURCE_DIR}/include)
add_subdirectory(${cnxvalidate-src_SOURCE_DIR}/src)

link_directories(${HDF5_LIBRARY_DIRS} )

add_link_options(-sFORCE_FILESYSTEM=1 -sENVIRONMENT=web -sEXPORT_ES6=1)

add_executable(webvalidate web-validate.cpp)
target_link_options(webvalidate PUBLIC "-sFORCE_FILESYSTEM=1 -sENVIRONMENT=web -s EXPORT_ES6=1")
target_link_options(webvalidate PUBLIC --bind)
target_link_options(webvalidate PUBLIC -sALLOW_MEMORY_GROWTH=1)
target_link_options(webvalidate PUBLIC --embed-file=application_defs)
target_link_options(webvalidate PUBLIC -sEXPORTED_RUNTIME_METHODS=['FS'])
target_link_libraries(webvalidate hdf5-wasm ${LIBXML2_LIBRARIES} NXV_Static_Library z)
