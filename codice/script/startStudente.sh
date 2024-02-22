# shellcheck disable=SC2046
# shellcheck disable=SC2006
gcc -c studente.c &&  gcc -o studente studente.o && ./studente 127.0.0.1
