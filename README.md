
# Experiments with Clang AST Matchers

# Using Arch linux as it has rolling package updates so we can test latest tools easily

```
sudo docker build -t refactor .
sudo docker run -v $PWD/src:/src -it refactor /bin/bash
```

```
cd src
mkdir build
cd build
cmake -GNinja ..
ninja
./refactor ../test.cpp -- 
```
