#!/bin/sh

set -x

DIE=0

# Detect [g]libtoolize
if libtoolize --version > /dev/null 2>&1; then
  LIBTOOLIZE=libtoolize
elif glibtoolize --version > /dev/null 2>&1; then
  LIBTOOLIZE=glibtoolize
else
  echo "**Error**: Missing 'libtoolize'"
  DIE=1
fi

if [ $DIE -eq 1 ]; then
  exit 1
fi

mkdir -p m4

aclocal -I m4/
autoheader --warnings=no-portability
${LIBTOOLIZE} --force
autoconf --warnings=no-portability
automake --add-missing
