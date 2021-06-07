


Very basic commands to compile a binary and run example gtest. Requires cmake 3.14 or better.


```
cmake -S . -B build

cmake --build build

./build/example_test
```

or

```
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug

cmake --build build-debug

gdb ./build-debug/example_test
```