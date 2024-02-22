# shellcheck disable=SC2046
# shellcheck disable=SC2006
gcc -c `mysql_config --cflags` server.c &&  gcc -o server server.o `mysql_config --libs`  ./libreria/wrapper.c && ./server
