#include <stdio.h>
#include <stdlib.h>
#include "merge.h"

int main(int argc, char **argv) {
	if (argc <= 1) {
		printf("Please enter a path for the ascii art tiles.");
		return 0;
	}

	char **lines;
	int number_of_lines = 0;

	merge(argv[1], &lines, &number_of_lines);
	printf("%x\n", (void *) lines);

	for (int i = 0; i < number_of_lines; i++) {
		printf("%s", lines[i]);
		free(lines[i]);
	}
	printf("\n");

	free(lines);
}