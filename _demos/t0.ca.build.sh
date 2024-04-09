# cmake \
#     -DCMAKE_BUILD_TYPE:STRING=Release \
#     -DCMAKE_INSTALL_PREFIX:STRING=$PWD/build/install \
#     -DCA_LLVM_INSTALL_DIR:STRING=$CONDA_PREFIX \
#     -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
#     -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/clang \
#     -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/clang++ \
#     -DCMAKE_INSTALL_RPATH_USE_LINK_PATH=ON \
#     -DGIT_CLANG_FORMAT_INCLUDED=ON \
#     -Dargs_CLANG_FORMAT_EXECUTABLE=clang-format \
#     -DCA_ENABLE_DOCUMENTATION=OFF \
#     -DCA_ENABLE_TESTS=OFF \
#     -DCA_RISCV_ENABLED=ON \
#     -DCA_ENABLE_API=cl \
#     -S $PWD -B $PWD/build -G Ninja
# cmake --build $PWD/build --target all
# cmake --build $PWD/build --target install

################################################################################

# cmake --preset cl

cmake --preset cl_linux
# or
# no riscv enabled
cmake --preset cl_macos

cmake --build $PWD/build --target all

################################################################################

muxc --list-devices
