#!/bin/sh
(libtoolize || glibtoolize) && \
aclocal && \
echo "Ignore any messages about adding aclocal.m4 && \
autoheader && \
automake -a && \
autoconf && \
echo "Now run ./configure --prefix=$HOME && make install" && \
echo "Add --disable-shared to the configure line if building on Mac OS X"

