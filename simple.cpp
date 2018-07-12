#include <stdio.h>
#include <stdlib.h>

#define N 4096 /* size of dictionary */
#define F 18 /* maximum match length */
#define THRESHOLD 2 /* minimum match length */

void do_output(unsigned char *output, bool *output_literal, size_t &output_count) {
	unsigned char flags = 0;
	for (size_t i = 0; i < output_count; i++) {
		flags |= output_literal[i] << i;
	}
	putchar(flags);

	for (size_t i = 0; i < output_count; i++) {
		if (output_literal[i]) {
			putchar(output[i * 2]);
		} else {
			putchar(output[i * 2]);
			putchar(output[i * 2 + 1]);
		}
	}

	output_count = 0;
}

int main(int argc, char **argv) {
	// Each pair of bytes is a raw byte or an encoded offset and length
	unsigned char output[16];
	// True if a byte, false if an offset+length
	bool output_literal[8];
	// Number of output buffer positions used
	size_t output_count = 0;

	// Start with a dictionary of spaces
	unsigned char dictionary[N];
	size_t dictionary_where = N - F;
	for (size_t i = 0; i < N; i++) {
		dictionary[i] = ' ';
	}

	// Read a few bytes ahead
	unsigned char lookahead[F];
	size_t ahead_count = 0;
	size_t ahead_start = 0;
	while (ahead_count < F) {
		int c = getchar();
		if (c == EOF) {
			break;
		}
		lookahead[ahead_count % F] = c;
		ahead_count++;
	}

	// Loop through the rest of the file
	while (ahead_count > 0) {
		// If there is space left in the lookahead, fill it up
		while (ahead_count < F) {
			int c = getchar();
			if (c == EOF) {
				break;
			}
			lookahead[(ahead_start + ahead_count) % F] = c;
			ahead_count++;
		}

		size_t best = 0;
		size_t bestlen = 0;

		// Look through the dictionary for a substring match
		for (size_t i = 0; i < N; i++) {
			size_t j;
			for (j = 0; j < ahead_count; j++) {
				if (dictionary[(i + j) % N] != lookahead[(ahead_start + j) % F]) {
					break;
				}
			}

			if (j > bestlen) {
				best = i;
				bestlen = j;
			}
		}

		if (bestlen > THRESHOLD) {
			// Found a usable match
			output[output_count * 2] = best & 0xFF;
			output[output_count * 2 + 1] = (bestlen - THRESHOLD - 1) | ((best & 0xFFFF0000) >> 4);
			output_literal[output_count] = false;
			output_count++;

			if (output_count == 8) {
				do_output(output, output_literal, output_count);
			}

			// Put whatever was used in the dictionary again
			for (size_t i = 0; i < bestlen; i++) {
				dictionary[dictionary_where % N] = lookahead[(ahead_start + i) % F];
				dictionary_where = (dictionary_where + 1) % N;
			}

			// Remove used text from lookahead
			ahead_start = (ahead_start + bestlen) % F;
			ahead_count -= bestlen;
		} else {
			// No usable match, so output one byte literally
			output[output_count * 2] = lookahead[ahead_start % F];
			output_literal[output_count] = true;
			output_count++;

			if (output_count == 8) {
				do_output(output, output_literal, output_count);
			}

			// Put that byte into the dictionary for future use
			dictionary[dictionary_where % N] = lookahead[ahead_start % F];
			dictionary_where = (dictionary_where + 1) % N;

			// Remove used text from lookahead
			ahead_start = (ahead_start + 1) % F;
			ahead_count -= 1;
		}

		if (ahead_count == 0) {
			int c = getchar();
			if (c == EOF) {
				break;
			}
			lookahead[(ahead_start + ahead_count) % F] = c;
			ahead_count++;
		}
	}

	if (output_count > 0) {
		do_output(output, output_literal, output_count);
	}
}
