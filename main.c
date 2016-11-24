#include <stdio.h>
#include <stdlib.h>
#include "merge.h"

int contains_newline(char *value) {
	int i = 0;
	char c;
	while ((c = value[i++]) != '\0') {
		if (c == '\n')
			return 1;
	}
	return 0;
}

int main(int argc, char **argv) {
	char *output = NULL;
	if (argc <= 1) {
		printf("Please enter a path for the ascii art tiles.");
		return 0;
	}
	if (argc > 2) {
		output = argv[2];
	}

	AsciiResult ascii_result;

	merge(argv[1], &ascii_result);

	if (output == NULL) {
		for (int i = 0; i < ascii_result.count; i++) {
			printf(contains_newline(ascii_result.lines[i]) ? "%s" : "%s\n", ascii_result.lines[i]);
		}
		printf("\n");
	} else {
		FILE *outputFile = fopen(output, "w");
		for (int i = 0; i < ascii_result.count; i++) {
			fputs(ascii_result.lines[i], outputFile);
		}
		printf("Output written to %s\n", output);
	}
	merge_free(&ascii_result);
}
