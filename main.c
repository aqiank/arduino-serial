#include <stdio.h>
#include <time.h>
#include <errno.h>

#define BUF_LEN 256

int main(int argc, char *argv[])
{
	FILE *file;
	char buf[BUF_LEN];
	size_t nread;
	clock_t initclk;

	if (argc != 2) {
		fprintf(stderr, "Usage: PROG [DEVICE]\n");
		return 1;
	}

	file = fopen(argv[1], "rb+");
	if (!file) {
		perror(argv[1]);
		return 2;
	}

	/* wait 1 second for arduino to be ready */
	initclk = clock();
	while (((clock() - initclk) / CLOCKS_PER_SEC) < 1)
		nread = fread(buf, 1, BUF_LEN, file); /* clear the serial buffer while waiting */

	/* start the actual reading */
	for (;;) {
		nread = fread(buf, 1, BUF_LEN, file);
		if (nread > 0) {
			buf[nread] = '\0';
			fprintf(stdout, "%s", buf);
		}
	}

	fclose(file);
	return 0;
} 
