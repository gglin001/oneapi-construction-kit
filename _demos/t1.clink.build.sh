pushd clik

# TODO: not work

cmake --preset clik_linux
# or
cmake --preset clik_macos

cmake --build $PWD/build --target all

popd
