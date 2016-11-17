#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <regex.h>
#include <string.h>

#define MAX_WIDTH_TILES 8
#define MAX_HEIGHT_TILES 8
#define PART_WIDTH 30
#define PART_HEIGHT 30

int main(int argc, char **argv) {
	if (argc <= 1) {
		return 0;
	}
	regex_t regex;
	if (regcomp(&regex, "^part_([0-9]+)_([0-9]+)_.+", REG_ICASE | REG_EXTENDED)) {
		printf("Failed to compile regex\n");
		return 0;
	}
	DIR *partsFolder = opendir(argv[1]);
	struct dirent *partFile;
	char *fileNames[MAX_WIDTH_TILES][MAX_HEIGHT_TILES];
	int width = 0;
	int height = 0;
	while ((partFile = readdir(partsFolder)) != NULL) {
		regmatch_t pmatch[3];
		if (!regexec(&regex, partFile->d_name, 3, pmatch, 0)) {
			char *fileName = partFile->d_name;
			int xStrSize = pmatch[1].rm_eo - pmatch[1].rm_so;
			int yStrSize = pmatch[2].rm_eo - pmatch[2].rm_so;
			char xStr[xStrSize + 1];
			char yStr[yStrSize + 1];
			xStr[xStrSize] = '\0';
			yStr[yStrSize] = '\0';
			strncpy(xStr, &fileName[pmatch[1].rm_so], xStrSize);
			strncpy(yStr, &fileName[pmatch[2].rm_so], yStrSize);
			int x = strtol(xStr, NULL, 10);
			int y = strtol(yStr, NULL, 10);
			if (width < x) {
				width = x;
			}
			if (height < y) {
				height = y;
			}
			char *fullFileName = malloc(strlen(argv[1]) + strlen(fileName) + 2);
			strcpy(fullFileName, argv[1]);
			strcat(fullFileName, fileName);
			fileNames[x][y] = fullFileName;
		}
	}

	width += 1;
	height += 1;

	FILE *files[width][height];

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			FILE *file = fopen(fileNames[x][y], "r");
			files[x][y] = file;
			free(fileNames[x][y]);
		}
	}

	for (int y = 0; y < height; y++) {
		for (int i = 0; i < PART_HEIGHT; i++) {
			char *line = malloc(PART_WIDTH * width + 1);
			for (int x = 0; x < width; x++) {
				fread(&line[x * PART_WIDTH], 1, PART_WIDTH, files[x][y]);
			}
			line[PART_WIDTH * width] = '\0';
			printf("%s", line);
			free(line);
		}
		for (int x = 0; x < width; x++) {
			fclose(files[x][y]);
		}
	}

	regfree(&regex);
	closedir(partsFolder);

	return 0;
}