set(TARGET_NAME controller)
project(${TARGET_NAME} VERSION 1.0.0 LANGUAGES CXX)

#
set(PROJECT_ROOT_DIR ../../)
include(${PROJECT_ROOT_DIR}/cmake/cmake_common_include)
#

set(INCS
)

set(INC_DIRS
)

set(LIB_DIRS
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

