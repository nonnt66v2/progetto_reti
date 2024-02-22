# shellcheck disable=SC2046
# shellcheck disable=SC2006
gcc -c `mysql_config --cflags` segreteria.c &&  gcc -o segreteria segreteria.o `mysql_config --libs` ./libreria/wrapper.c && ./segreteria 127.0.0.1 && rm sergreteria.o sergreteria
