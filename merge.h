typedef struct AsciiResultType {
	char **lines;
	int count;
} AsciiResult;

int merge(char *base_path, AsciiResult *ascii_result);
void merge_free(AsciiResult *ascii_result);