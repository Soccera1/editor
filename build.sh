#!/usr/bin/env sh

CC="zig cc"
OS=$(uname -s)
ARCH=$(uname -m)
[ $ARCH = arm64 ] && ARCH=aarch64

if [ -n "$1" ] && [ "$1" != "install" ] && [ "$1" != "uninstall" ] && [ "$1" != "clean" ]; then
  echo "If an argument is used, the first must be install, uninstall or clean!" >&2
elif [ -n "$1" ] && [ -z "$2" ] && [ "$1" != "clean" ]; then
  if [ "$1" = "install" ]; then
    echo "You must provide a prefix to install to!" >&2
    exit 1
  elif [ "$1" = "uninstall" ]; then
    echo "You must provide a prefix to uninstall from!" >&2
    exit 1
  fi
fi

if [ "$1" = "clean" ]; then
  if [ -d bin ]; then
    rm -rv bin
  else
    echo "bin does not exist." >&2
  fi
  exit 0
fi

if [ ! -d bin ]; then
  mkdir bin
fi

if [ $OS = Darwin ]; then
  $CC editor.c -o bin/editor -target $ARCH-macos-none -Oz -static || exit 1
elif [ $OS = Linux ]; then
  $CC editor.c -o bin/editor -target $ARCH-linux-musl -Oz -static || exit 1
else
  echo "OS unsupported." >&2
  exit 1
fi

if [ "$1" = "install" ] && [ -n "$2" ]; then
  if [ ! -d $2/bin ]; then
    mkdir -p $2/bin || exit 1
  fi
  cp -v bin/editor $2/bin/editor
fi

if [ "$1" = "uninstall" ]; then
  rm $2/bin/editor
fi
