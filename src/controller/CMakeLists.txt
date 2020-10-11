cmake_minimum_required(VERSION 3.10)
set(TARGET_NAME controller)
project(${TARGET_NAME} VERSION 1.0.0 LANGUAGES CXX)

#
set(PROJECT_ROOT_DIR ../../)
include(${PROJECT_ROOT_DIR}/cmake/cmake_common_include)
#

set(INCS
)

set(INC_DIRS
  ${CMAKE_INCLUDE_PATH}
)

set(LIB_DIRS
  ${CMAKE_LIBRARY_PATH}
)

set(SRCS
  controller.cc
)

set(LIBS
)

#------------------------------------------------------------------------------
# Additional Packages etc
#


#------------------------------------------------------------------------------
# Include instructions to make target.  This must be the last line in the file
#
include(${PROJECT_ROOT_DIR}/cmake/cmake_executable)

