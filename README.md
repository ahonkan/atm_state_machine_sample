# ATM Controller Sample Project
This sample project demonstrates cmake build environment & gtest test environment,
 using a toy project, an ATM controller.


## Code Layout
  - src/  : Path for all the source code
  - test/ : Pathf for all the test code
  - build/ : cmake build location to build the application

The src/ path contains following directories:
  - hal/   : library for hardware abstraction layer
  - auth/  : Authentication functions
  - user_info/ : User Account Access API functions
  - ui/ : User Interface functions
  - controller/ : Main controller code with state machine

## Building and testing

Before running test code, first run setup_gtest.sh to clone and install the gtest facility.

### Building the controller tests

1: cd to test/controller/build
2: enter:  cmake ..
3: enter:  make all
4: run the test

TODO: Currently the test fails


