cmake_minimum_required(VERSION 3.10)
set(TARGET_NAME auth)
project(${TARGET_NAME} VERSION 1.0.0 LANGUAGES CXX)

#
set(PROJECT_ROOT_DIR ../../)
include(${PROJECT_ROOT_DIR}/cmake/cmake_common_include)
#

set(INCS
)

set(INC_DIRS
  ${PROJECT_ROOT_DIR}/src/
)

set(EXPORT
  user_info.h
)

set(SRCS
  user_info.cc
)

set(LIBS
)

#------------------------------------------------------------------------------
# Include instructions to make target.  This must be the last line in the file
#
include(${PROJECT_ROOT_DIR}/cmake/cmake_shared_lib)

