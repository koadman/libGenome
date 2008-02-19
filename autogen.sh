#!/bin/sh
(libtoolize || glibtoolize) && \
aclocal  -I m4 && \
echo "Ignore any messages about adding libtool.m4 to aclocal.m4" && \
autoheader && \
automake -a && \
autoconf && \
echo "Now run ./configure --prefix=$HOME && make install" && \
echo "Add --disable-shared to the configure line if building on Mac OS X"

