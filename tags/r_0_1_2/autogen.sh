#!/bin/sh

set -x
autoheader
libtoolize --force
aclocal
automake --add-missing
autoconf

