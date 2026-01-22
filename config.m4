PHP_ARG_ENABLE(pghash, whether to enable pghash extension,
[  --enable-pghash   Enable pghash extension], no)

if test "$PHP_PGHASH" != "no"; then
  PHP_NEW_EXTENSION(pghash, pghash_ext.c pghash.c, $ext_shared)
fi
