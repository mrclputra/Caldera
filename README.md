# Caldera
:))

```bash
pacman -S mingw-w64-clang-x86_64-clang \
          mingw-w64-clang-x86_64-cmake \
          mingw-w64-clang-x86_64-ninja \
          mingw-w64-clang-x86_64-qt6-base \
          mingw-w64-clang-x86_64-qt6-tools \
          mingw-w64-clang-x86_64-eigen3 \
          mingw-w64-clang-x86_64-boost \
          mingw-w64-clang-x86_64-flann \
          mingw-w64-clang-x86_64-qhull
```

CLion CMake options:

`-G Ninja -DCMAKE_PREFIX_PATH=D:/tools/msys64/clang64`
or where-ever your llvm compiler is

implement SLAM interface? todo: read on opencv documentation
