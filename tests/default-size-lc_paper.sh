# Skip this test if we're not using glibc
test "$HAVE_GLIBC" = "yes" || exit 77
no_user_papersize
