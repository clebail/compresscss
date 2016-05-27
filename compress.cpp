#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE				1024

long int getFileSize(FILE *);
size_t readFile(FILE *, char *);
void dropSequence(char *, long int, long int, long unsigned);
long unsigned dropSpaceBefore(char *, long unsigned);
long unsigned dropSpaceAfter(char *, long unsigned);

int main(int argc, char **argv) {
	FILE *input, *output;
	char *inputBuffer;
	long int inputSize, gain;
	long unsigned idx = 0, idxStartCmtr = 0;
	bool inDebutLigne = true, inCommentaire = false, inQuote = false;
	char quote;

	if (argc < 3) {
		printf("USAGE: %s <input file> <output file>\n", argv[0]);
		return 1;
	}

	if ((input = fopen(argv[1], "r")) == 0) {
		printf("Fichier non trouvé : %s\n", argv[1]);
		return 1;
	}

	if ((output = fopen(argv[2], "w")) == 0) {
		printf("Impossible de créer : %s\n", argv[2]);

		fclose(input);

		return 1;
	}

	inputSize = getFileSize(input) + 1;
	inputBuffer = (char *) malloc(inputSize * sizeof(char));
	memset(inputBuffer, 0, inputSize);
	readFile(input, inputBuffer);

	do {
		char c = inputBuffer[idx];

		if (!inQuote) {
			if (inCommentaire) {
				if (c == '*' && inputBuffer[idx + 1] == '/') {
					dropSequence(inputBuffer, idxStartCmtr, idx - idxStartCmtr + 2, strlen(inputBuffer));

					inCommentaire = false;
					idx = idxStartCmtr;
				} else {
					idx++;
				}
			} else if (c == '/' && inputBuffer[idx + 1] == '*') {
				idxStartCmtr = idx;
				inCommentaire = true;
				idx += 2;
			} else if ((c == ' ' || c == '\t') && inDebutLigne) {
				dropSequence(inputBuffer, idx, 1, strlen(inputBuffer));
			} else if (c == ':' || c == ';' || c == ',' || c == '+' || c == '~' || c == '>' || c == '{' || c == '!') {
				idx = dropSpaceBefore(inputBuffer, idx);
				idx = dropSpaceAfter(inputBuffer, idx) + 1;
			} else if (c == '}') {
				idx = dropSpaceBefore(inputBuffer, idx);
				if (inputBuffer[idx - 1] == ';') {
					dropSequence(inputBuffer, --idx, 1, strlen(inputBuffer));
				}
				if (inputBuffer[idx - 1] == '{') {
					long unsigned i = idx - 1;
					int len = 0;

					while (inputBuffer[i - 1] != '}' && i - 1) {
						i--;
						len++;
					}
					if (len != 0) {
						dropSequence(inputBuffer, i, len + 2,
								strlen(inputBuffer));
						idx = i;
					}
				} else {
					idx = dropSpaceAfter(inputBuffer, idx) + 1;
				}
			} else if (c == '\r') {
				dropSequence(inputBuffer, idx, inputBuffer[idx + 1] == '\n' ? 2 : 1, strlen(inputBuffer));
				inDebutLigne = true;
			} else if (c == '\n') {
				dropSequence(inputBuffer, idx, 1, strlen(inputBuffer));
				inDebutLigne = true;
			}else if(c == '\'' || c == '"') {
				inQuote = true;
				quote = c;
				idx++;
			} else {
				inDebutLigne = false;
				idx++;
			}
		} else if (c == quote && inputBuffer[idx-1] != '\\') {
			inQuote = false;
			idx++;
		} else {
			idx++;
		}
	} while (idx < strlen(inputBuffer));

	fputs(inputBuffer, output);

	gain = inputSize - strlen(inputBuffer) - 1;

	free(inputBuffer);
	fclose(input);
	fclose(output);

	printf("Gain: %lu octets\n", gain);

	return 0;
}

long int getFileSize(FILE *file) {
	long int pos = ftell(file);
	long int size;

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, pos, SEEK_SET);

	return size;
}

size_t readFile(FILE *file, char *result) {
	char buffer[BUFFER_SIZE];
	size_t totalRead = 0, nbRead;
	long int pos = 0;

	do {
		nbRead = fread(buffer, 1, BUFFER_SIZE, file);

		strncpy(&result[pos], buffer, nbRead);

		totalRead += nbRead;
		pos += nbRead;
	} while (!feof(file));

	return totalRead;
}

void dropSequence(char *buffer, long int start, long int length,
		long unsigned bufferLength) {
	char *temp = (char *) malloc((bufferLength - start - length + 1) * sizeof(char));
	strcpy(temp, &buffer[start + length]);

	strcpy(&buffer[start], temp);

	free(temp);
}

long unsigned dropSpaceBefore(char *buffer, long unsigned idx) {
	int len = 0;
	unsigned long i = idx;

	while (buffer[i - 1] == ' ' || buffer[i - 1] == '\t') {
		len++;
		i--;
	}

	if (len != 0) {
		dropSequence(buffer, i, len, strlen(buffer));
		idx = i;
	}

	return idx;
}

long unsigned dropSpaceAfter(char *buffer, long unsigned idx) {
	int len = 0;
	unsigned long i = idx;

	while (buffer[i + 1] == ' ' || buffer[i + 1] == '\t') {
		len++;
		i++;
	}

	if (len != 0) {
		dropSequence(buffer, idx + 1, len, strlen(buffer));
	}

	return idx;
}
