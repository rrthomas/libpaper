# Skip this test if we're not using glibc
( ldd --version | grep "GLIBC" ) || exit 77
no_user_papersize
