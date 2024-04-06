#include "debugger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h> //va_*
#include <errno.h> //errno
#include <string.h> //strerror

/* print errno & its message */
void printf_error(void) {
  fprintf(stderr, "ERROR: %s(%d)\n", strerror(errno), errno);
	exit(1);
}

/* print message if define DEBUG */
void printf_debug(const char* format, ...) {
#ifdef DEBUG
  va_list va;
	va_start(va, format);
	vprintf(format, va);
	va_end(va);
#endif
}


/* PRIVATE: get printable-charactor of ascii */
int _get_printable_char(char c) {
	return (0x20 <= c && c <= 0x7e) ? c : ' ';
}

/* print message if define DEBUG */
void _printf_string_debug(const char* str, const size_t len) {
#ifdef DEBUG
	int i;
	printf("byte: ");
	for (i = 0; i < len; i++) {
		char c = *(str + i);
		printf("%02X(%c) ", c, _get_printable_char(c));
	}
	printf("\n");
#endif
}
void printf_string_debug(const char* str) {
#ifdef DEBUG
	size_t len = strlen(str);
	_printf_string_debug(str, len);
#endif
}
void printf_char_debug(const int it, const char c) {
#ifdef DEBUG
	printf_debug("%05d:%c\n", it % 100000, _get_printable_char(c));
#endif
}
