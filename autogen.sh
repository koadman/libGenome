#!/bin/sh
(libtoolize || glibtoolize) && \
aclocal && \
autoheader && \
automake -a && \
autoconf && \
echo "Now run ./configure --prefix=$HOME ; make install" && \
echo "Add --disable-shared to the configure line if building on Mac OS X"

