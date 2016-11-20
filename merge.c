#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <regex.h>
#include <string.h>

#include "merge.h"

#define MAX_WIDTH_TILES 8
#define MAX_HEIGHT_TILES 8
#define PART_WIDTH 30
#define PART_HEIGHT 30

int merge(char *base_path, AsciiResult *ascii_result) {
	regex_t regex;
	if (regcomp(&regex, "^part_([0-9]+)_([0-9]+)_.+", REG_ICASE | REG_EXTENDED)) {
		printf("Failed to compile regex\n");
		return 1;
	}
	DIR *partsFolder = opendir(base_path);
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
			char *fullFileName = malloc(strlen(base_path) + strlen(fileName) + 2);
			strcpy(fullFileName, base_path);
			strcat(fullFileName, "/");
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

	ascii_result->lines = malloc(sizeof(char *) * height * PART_HEIGHT);
	ascii_result->count = height * PART_HEIGHT;

	for (int y = 0; y < height; y++) {
		for (int i = 0; i < PART_HEIGHT; i++) {
			char *line = malloc((PART_WIDTH * width + 1) * sizeof(char));
			for (int x = 0; x < width; x++) {
				fread(&line[x * PART_WIDTH], sizeof(char), PART_WIDTH, files[x][y]);
			}
			line[PART_WIDTH * width] = '\0';
			ascii_result->lines[i + y * PART_HEIGHT] = line;
		}
		for (int x = 0; x < width; x++) {
			fclose(files[x][y]);
		}
	}

	regfree(&regex);
	closedir(partsFolder);

	return 0;
}

void merge_free(AsciiResult *ascii_result) {
	for (int i = 0; i < ascii_result->count; i++) {
		free(ascii_result->lines[i]);
	}
	free(ascii_result->lines);
}
