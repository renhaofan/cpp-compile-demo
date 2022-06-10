[TOC]

# Introduce

Author: steve

Email: yqykrhf@163.com

Date: 2022-6-10



This is a concise tutorial to illustrate the process of compiling C program. The platform is Deepin 20.6, 

community, a Debian based Linux distribution.  Complier utilize g++.

```
$ g++ --version
>>> g++ (Uos 8.3.0.3-3+rebuild) 8.3.0
Copyright (C) 2018 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

# 1. Single compile process

```c++
// @file: test.c
#include <iostream>
using namespace std;

#define PI 3.14
#ifdef USING_FLOAT64
typedef double scalar;
#else
typedef float scalar;
#endif

int main() {
  scalar n;
  cout << "Hello world!" << endl;
  return 0;
}
```

Directly compile and generate executable file by command:

```
$ g++ test.c -o test
```

option `-o` means the output executable file name. If you don't specify it, the executable file name 
is `a.out` by default.

Then run the executable file, 

```
$ ./test
>>> Hello world!
```

We hope to dive into the details when run command `g++ test.c -o main` to figure out the process when compile program.

Actually, `g++ test.c` contains 4 stages:

* Preprocess
* Compile
* Assemble
* Link

## 1.1 Preprocess

This stage generate `test.i` from `test.c`, which consists of  4 steps. 

1. expanding header file
2. macro replacement
3. uncomment
4. condition compile

```
$ g++ test.c -E -o test.i
```

you could open both files to distinguish difference. 

```
$ wc -l test.c
>>> 16 test.c
$ wc -l test.i
>>> 28279 test.i
```

Most part of `test.i` is include `iostream` file,  whose last few lines are shown as follows:

```C++
using namespace std;





typedef float scalar;


int main() {
  scalar n;
  cout << "Hello world!" << endl;
  return 0;
}
```

## 1.2 Compile

Grammar check happened in this stage. At the same time, generate assembly code `test.s`

```
$ gcc test.c -S -o test.s
```

## 1.3 Assemble

Convert assembly code to machine code.

```
$ g++ test.s -c -o test.o
```

## 1.4 Link

This stage is to generate binary executable file from machine code. If you use the library, such as OpenCV, PCL, etc, the concerned library will be linked to `test.o` files.

```
$ g++ test.o -o test
```

## *static and dynamic library

At the stage of link, two different kinds of libraries are supplied. 

Both of them exist in the form of files, no math which platform you choose. **Essentially both of them is a binary format of executable code that can be loaded into memory for execution. Both dynamic libraries and static link libraries provide variables, functions and classes to their callers.**

The static library will be embedded into the executable file so that normally executable file size is large than dynamic library.  In other word, the static library becomes the part of the executable file.  On the contrary, dynamic library only embed some description information (redirection info normally) into executable file. Only the program is loaded into memory, the program will link the concerned libraries.

As for a single file ,  the program linked by static library can run anywhere. The dynamic library and executable file should keep all together in case that program run successfully.

However, if multiple file load the same library, it is more worthy to make use of dynamic library. For example, program A and B use the same function alignment() in XXX library. We only need to load once in memory rather than twice  if dynamic library suppied. 

---

Win32 platform static library file extension is `.lib`, dynamic file extension is `.dll`

Linux platform static library file extension is `.a`, dynamic file extension is `.so`

This image show the naming rule about library.

![so](./images/so.png)

# 2. Generate library file

Before start to learn how to generate library file, we just make a summary about the compile options:

* `-shared` Generate dynamic library file

* `-static` Generate static library file

* `-fPIC` Position Independent Code
* `-g` gdb option for DEBUG
* `-c`  Only generate .o file from .c
* `-I` include folder
* `-L` link library

## 2.1 Static library

If we have two files, `add.c` , `add.h`

```
// @file add.h
int add(int x, int y);
int sub(int x, int y);
```

```
// @file add.c
#pragma once
#include "add.h"

int add(int x, int y) {
  return x + y;
}

int sub(int x, int y) {
  return x - y;
}
```

```
$ g++ -c add.c
$ ls
>>> add.c add.h add.o
```

Generate static library by command ``

```
$ ar -rc libadd.a add.o
$ ls
>>> add.c add.h add.o libadd.a
```

the `libadd.a` can placed in any location only if the complier find it when linking. Later we'll discuss the locations the complier search for.  For convenience, we touch a new file `main.c` located in the same folder with `add.h`

```
$ g++ main.c -o main libadd.a
$ ./main
>>> add(5,4) is 9
sub(5,4) is 1
```

Let's see `libadd.a` ,`add.h` and  `main.c` are not in the same place, see source in folder `add_sub_a_folder`

```
$ g++ main.c -o main -I ./include ./lib/libadd.a
```

or

```
$ g++ main.c -o main -I ./include -L ./lib -ladd
```

remove `-I` , `-L` see what happened.

## 2.2 Dynamic library

folder `add_sub_so`

generate dynamic library by two commands

```shell
$ g++ -fPIC -c add.c -o libadd.o
$ g++ -shared -o libadd.so libadd.o
```

or single one command:

```shell
$ g++ -fPIC -shared -o libadd.so add.c
```

link

```shell
$ g++ main.c -o main libadd.so
```

If we move the `libadd.so` in another place,  what will happened?

```shell
$ mv libadd.so ../
$ ./main
>>> /main: error while loading shared libraries: libadd.so: cannot open shared object file: No such file or directory
```

## 2.3 How to install dynamic library?

### 1. copy *.so to /lib or /usr/lib

```sh
$ sudo cp -d ../libadd.so /lib
```

or 

```shell
$ sudo cp -d ../libadd.so /usr/lib
```

Actually `/lib` is a soft link of `/usr/lib`

```shell
$ ll /lib
>>> ll /lib lrwxrwxrwx 1 root root 7 Mar 31 15:11 /lib -> usr/lib
```

Then 

```
$ ./main
>>> add(5,4) is 9
sub(5,4) is 1
```

**Note: `/usr/local/lib` not work**

### 2. Macro LD_LIBRARY_PATH

```
export LD_LIBRARY_PATH=/home/*****/*****/
```

Permanently enable

```
vim /etc/profile
export LD_LIBRARY_PATH=/usr/local/cuda/lib64:$LD_LIBRARY_PATH
source /etc/profile
```

or ~/.bashrc

### 3. /etc/ld.so.conf

```shell
sudo vim /etc/ld.so.conf
```

add /usr/local/lib

copy *.so to /usr/local/lib 

```shell
sudo ldconfig
```



# CMake version 

notice `main.c:3:10: fatal error: iostream: No such file or directory`

change `main.c` to `main.cpp` could solve this problem. 

I just simply `#include<stdio.h>` to solve it. 



# Tips

```
cmake -DCMAKE_INSTALL_PREFIX=~/.local
export LD_LIBRARY_PATH=~/.local:/$LD_LIBRARY_PATH
```

```
make -j16 # 执行编译操作，我的CPU有16个线程，所以我写16以加快编译速度
sudo make install # 安装 PCL
cd lib/ # 该文件夹在build文件夹下
sudo cp -d libpcl_* /usr/lib/x86_64-linux-gnu/ # 将这些so文件复制到之前清理过的这个目录，否则编译自己的程序时找不到so文件，-d参数是保持文件之间的软链接关系
```

[花了50大洋买回来的教训](https://www.icode9.com/content-4-1134365.html)

