rm -rf googletest
git clone git@github.com:google/googletest.git
mkdir googletest/build
cd googletest/build
cmake ..
make all

