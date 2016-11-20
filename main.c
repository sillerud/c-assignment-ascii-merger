#include <stdio.h>
#include <stdlib.h>
#include "merge.h"

int main(int argc, char **argv) {
	if (argc <= 1) {
		printf("Please enter a path for the ascii art tiles.");
		return 0;
	}

	AsciiResult ascii_result;

	merge(argv[1], &ascii_result);

	for (int i = 0; i < ascii_result.count; i++) {
		printf("%s", ascii_result.lines[i]);
	}
	printf("\n");
	merge_free(&ascii_result);
}
