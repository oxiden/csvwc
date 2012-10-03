#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen

#include "debugger.h"

//#define DEBUG
#define BUFFERSIZE (16)
#define TRUE (1)
#define FALSE (0)
#define LF '\n'
#define QUOTE '"'

#define TARGET_FILES (100)
#define OPTION_NONE (0x00)
#define OPTION_WORD (0x01)
#define OPTION_COLUMN (0x02)
#define OPTION_LINE (0x04)

/* LF/CR/CRLF char? */
int is_eol(const char const* p) {
	if (*p == LF) {
		printf_debug("detect LF\n");
		return TRUE;
	}
	return FALSE;
}

/* count lines */
long countCSV(FILE** pfp) {
	char* pbuf = (char*)malloc(BUFFERSIZE * sizeof(char));

	long count = 0L;
	int quote_in = FALSE;

	char* p;
	while (fgets(pbuf, BUFFERSIZE, *pfp) != NULL) {
		int it;
		size_t len = strlen(pbuf);
		printf_string_debug(pbuf);
		for (it = 0; it < len; it++) {
			p = pbuf + it;
			printf_debug("%2d:%c\n", it, *p);
			if (is_eol(p)) {
				if (quote_in) {
					/* skip count */
				} else {
					count++;
					printf_debug("count up! %ld\n", count);
				}
			}
			if (*p == QUOTE) {
				if (quote_in) {
					quote_in = FALSE;
					printf_debug("quote out!\n");
				} else {
					quote_in = TRUE;
					printf_debug("quote in!\n");
					//return -1; /* TODO:case of nest quoting */
				}
			}
		}
	}
	if (!is_eol(p)) {
		count++;
	}
	free(pbuf);
	return count;
}

long openCSV(const char* filename) {
	printf_debug("file=[%s]\n", filename);
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		return -1;
	}
	long result = countCSV(&fp);
	fclose(fp);
	return result;
}

void print_usage(const char const* self) {
	printf("Usage: %s [options] file [file...]\n", self);
	printf("    -w, --word                       count words\n");
	printf("    -c, --column                     count columns\n");
	printf("    -l, --line                       count lines\n");
	printf("    -h, --help                       print this message\n");
}

int main(int argc, char *argv[]) {
	int options = OPTION_NONE;
	int target_file_no[TARGET_FILES];
	int target_file_count = 0;
  int double_hyphen = FALSE;

	int data_from_stdin = FALSE;

	int i;
	/* parse comand line */
	for(i=1; i<argc; i++){
		printf_debug("[%d]: %s\n", i, argv[i]);
		if (!double_hyphen && strncmp(argv[i], "-", 1) == 0) {
			if (strncmp(argv[i] + 1, "-", 1) == 0) {
				/* long option */
				if (strcmp(argv[i] + 2, "word") == 0) {
					options |= OPTION_WORD;
				}
				if (strcmp(argv[i] + 2, "column") == 0) {
					options |= OPTION_COLUMN;
				}
				if (strcmp(argv[i] + 2, "line") == 0) {
					options |= OPTION_LINE;
				}
				if (strcmp(argv[i] + 2, "help") == 0) {
					print_usage(argv[0]);
					exit(0);
				}
				if (strcmp(argv[i] + 2, "") == 0) {
					double_hyphen = TRUE;
					printf_debug("detect double-pyphen\n");
				}
			} else {
				/* short option */
				if (strcmp(argv[i] + 1, "w") == 0) {
					options |= OPTION_WORD;
				}
				if (strcmp(argv[i] + 1, "c") == 0) {
					options |= OPTION_COLUMN;
				}
				if (strcmp(argv[i] + 1, "l") == 0) {
					options |= OPTION_LINE;
				}
				if (strcmp(argv[i] + 1, "h") == 0) {
					print_usage(argv[0]);
					exit(0);
				}
			}
			if (!double_hyphen && options == OPTION_NONE) {
				print_usage(argv[0]);
				exit(0);
			}
		} else {
			printf_debug("target(%d)=[%s]\n", target_file_count+1, argv[i]);
			target_file_no[target_file_count] = i;
			target_file_count++;
		}
	}
	if (options == OPTION_NONE) {
		options = (OPTION_WORD | OPTION_COLUMN | OPTION_LINE);
	}
	printf_debug("options=%02x\n", options);

	/* main procedure */
	if (target_file_count == 0) {
		/* get payload from stdin */
		data_from_stdin = TRUE;
		target_file_count = 1;
	}
	for (i = 0; i < target_file_count; i++) {
		if (options & OPTION_WORD) {
			printf("%ld ", 0L); /* TODO: word count is not imprelemeted yet. */
		}
		if (options & OPTION_COLUMN) {
			printf("%ld ", 0L); /* TODO: column count is not imprelemeted yet */
		}
		if (options & OPTION_LINE) {
			long result;
			if (data_from_stdin) {
				result = countCSV(&stdin);
			} else {
				result = openCSV(argv[target_file_no[i]]);
			}
			printf("%ld ", result);
		}
		if (options) {
			printf("%s\n", argv[target_file_no[i]]);
		}
	}
	return 0;
}
