# shark 编译
## DEBUG编译
```
rm -rf build
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_BUILD_RPATH=/opt/PTIDE/Utils ..
make
```

## RELEASE编译
```
rm -rf build
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_BUILD_RPATH=/opt/PTIDE/Utils ..
make
```
