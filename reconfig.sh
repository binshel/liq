#!/bin/sh 
set -x

# autoscan  # 生成 configure.scan 修改为 configure.ac，初始化时才需要

aclocal         # 根据 configure.ac 生成 aclocal.m4 文件，该文件主要处理各种宏定义
autoconf        # 将 configure.ac 中的宏展开，生成 configure 脚本，这过程中可能会用到 aclocal.m4
autoheader      # 生成 config.h.in 文件，如果有 "acconfig.h” 文件，从中复制用户附加的符号定义
libtoolize --automake --copy --force    # 安装初始动态库文件的一个工具，被automake需要

touch NEWS README AUTHORS ChangeLog     # 创建必要的文件
automake --add-missing      # 根据 configure.in 中的参量把 Makefile.am 转换成 Makefile.in 文件


# 根据 Makefile.in 生成 Makefile
INSTALL_DST=${HOME}/frame_exec
./configure CFLAGS='-Wall -Werror -g3 -O0 -Wno-long-long' CXXFLAGS='-Wall -Werror -g3 -O0 -Wno-long-long' --prefix=${INSTALL_DST}
