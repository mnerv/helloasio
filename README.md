# helloasio

Learning asio with C++20 coroutines feature.

## Requirements

  - [cmake](https://cmake.org/)
  - [vcpkg](https://vcpkg.io/)

## Build

```
cmake -S . -Bbuild -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake -GNinja
```

`${VCPKG_ROOT}`: Points to where vcpkg is installed.
