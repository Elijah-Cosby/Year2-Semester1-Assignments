libmysort.so: mySort.c mySort.h
    gcc -O3 -shared -o libmysort.so -fPIC mySort.c

test: main.c mySort.c mySort.h
    gcc main.c mySort.c -o sortTest
    ./sortTest