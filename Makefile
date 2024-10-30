all:
     gcc toralize.c -o toralize.so -fPIC -shared -ldl -D_GNU_SOURCE #compiling as shared library