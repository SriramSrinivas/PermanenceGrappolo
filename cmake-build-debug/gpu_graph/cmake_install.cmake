# Install script for directory: /home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/gpu_graph

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gpu_graph" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gpu_graph")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gpu_graph"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/cmake-build-debug/gpu_graph/gpu_graph")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gpu_graph" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gpu_graph")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gpu_graph")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/gpu_graph/commonconstants.h"
    "/home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/gpu_graph/communityGPU.h"
    "/home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/gpu_graph/devconstants.h"
    "/home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/gpu_graph/graphGPU.h"
    "/home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/gpu_graph/graphHOST.h"
    "/home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/gpu_graph/hashitem.h"
    "/home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/gpu_graph/hostconstants.h"
    "/home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/gpu_graph/myutility.h"
    "/home/sriramsrinivas@unomaha.edu/CLionProjects/grappolo/gpu_graph/openaddressing.h"
    )
endif()

