cmake \
  --no-warn-unused-cli \
  -DCMAKE_BUILD_TYPE:STRING=Debug \
  -DCMAKE_INSTALL_PREFIX:STRING=$PWD/build/install \
  -DCA_LLVM_INSTALL_DIR:STRING=$CONDA_PREFIX \
  -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
  -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/clang \
  -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/clang++ \
  -S $PWD \
  -B $PWD/build \
  -G Ninja

cmake --build $PWD/build --config Debug --target all --
