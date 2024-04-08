# micromamba install -y cmake ninja
# micromamba install -y clangdev llvmdev lld
# micromamba install -y spirv-headers spirv-tools lit
# micromamba install -y zlib

################################################################################

cmake \
    --no-warn-unused-cli \
    -DCMAKE_BUILD_TYPE:STRING=Release \
    -DCMAKE_INSTALL_PREFIX:STRING=$PWD/build/install \
    -DCA_LLVM_INSTALL_DIR:STRING=/Users/allen/micromamba/envs/mlir \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/clang \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/clang++ \
    -DCA_RISCV_ENABLED:BOOL=TRUE \
    -DCA_ENABLE_API=cl \
    -S $PWD -B $PWD/build -G Ninja

cmake --build $PWD/build --target all
cmake --build $PWD/build --target install

################################################################################

# misc
muxc --list-devices
