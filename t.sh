# TODO: add conda cache volume

docker run -d -it \
    --name ca_dev_0 \
    -v ssh:/root/.ssh \
    -v repos:/repos \
    -w /repos \
    base:latest

micromamba install -y cmake ninja
micromamba install -y clangdev llvmdev lld
micromamba install -y spirv-headers spirv-tools lit
micromamba install -y zlib

################################################################################

cmake \
    --no-warn-unused-cli \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_INSTALL_PREFIX:STRING=$PWD/build/install \
    -DCA_LLVM_INSTALL_DIR:STRING=$CONDA_PREFIX \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/clang \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/clang++ \
    -DCA_RISCV_ENABLED:BOOL=TRUE \
    -DCA_ENABLE_API=cl \
    -S $PWD \
    -B $PWD/build \
    -G Ninja

cmake --build $PWD/build --config Debug --target all --
cmake --build $PWD/build --config Debug --target install --

# TODO: use clang from conda
# -DCMAKE_C_COMPILER:FILEPATH=$CONDA_PREFIX/bin/clang \
# -DCMAKE_CXX_COMPILER:FILEPATH=$CONDA_PREFIX/bin/clang++ \

################################################################################

# misc
muxc --list-devices
