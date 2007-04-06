if test -n "$DATRIE_BASEPATH"; then
  case `uname -a` in
	CYGWIN*)
	  DATRIE_BASEPATH=`echo $DATRIE_BASEPATH | sed -e 's/\"//g'`
	  DATRIE_BASEPATH=`cygpath $DATRIE_BASEPATH`;;
	MINGW32*)
	  DATRIE_BASEPATH=`echo $DATRIE_BASEPATH | sed -e 's/\"//g' -e 's/\\\\/\\//g' -e 's/^\\([a-zA-Z]\\):/\\/\\1/g'`
  esac
  export DATRIE_BASEPATH=$DATRIE_BASEPATH
  export PATH=$DATRIE_BASEPATH/bin:$PATH
#  export ACLOCAL_FLAGS="-I $DATRIE_BASEPATH/share/aclocal $ACLOCAL_FLAGS"

  if test "x$C_INCLUDE_PATH" = x; then
	APPEND=
  else
	APPEND=":$C_INCLUDE_PATH"
  fi
  export C_INCLUDE_PATH=$DATRIE_BASEPATH/include$APPEND

  if test "x$LIBRARY_PATH" = x; then
	APPEND=
  else
	APPEND=":$LIBRARY_PATH"
  fi
  export LIBRARY_PATH=$DATRIE_BASEPATH/lib:/lib/w32api$APPEND

  if test "x$PKG_CONFIG_PATH" = x; then
	APPEND=
  else
	APPEND=":$PKG_CONFIG_PATH"
  fi
  export PKG_CONFIG_PATH=$DATRIE_BASEPATH/lib/pkgconfig$APPEND

  if test "x$MANPATH" = x; then
	APPEND=
  else
	APPEND=":$MANPATH"
  fi
  export MANPATH=$DATRIE_BASEPATH/share/man$APPEND
fi
