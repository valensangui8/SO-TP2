#include <lib.h>
#include <naiveConsole.h>
#include <stdint.h>

void *my_memset(void *destination, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t) c;
	char *dst = (char *) destination;

	while (length--)
		dst[length] = chr;

	return destination;
}

void *my_memcpy(void *destination, const void *source, uint64_t length) {
	/*
	 * memcpy does not support overlapping buffers, so always do it
	 * forwards. (Don't change this without adjusting memmove.)
	 *
	 * For speedy copying, optimize the common case where both pointers
	 * and the length are word-aligned, and copy word-at-a-time instead
	 * of byte-at-a-time. Otherwise, copy by bytes.
	 *
	 * The alignment logic below should be portable. We rely on
	 * the compiler to be reasonably intelligent about optimizing
	 * the divides and modulos out. Fortunately, it is.
	 */
	uint64_t i;

	if ((uint64_t) destination % sizeof(uint32_t) == 0 &&
		(uint64_t) source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0) {
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *) source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else {
		uint8_t *d = (uint8_t *) destination;
		const uint8_t *s = (const uint8_t *) source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

unsigned int my_strlen(const char *s) {
	unsigned int len = 0;
	while (s[len] != '\0') {
		len++;
	}
	return len;
}

void my_strncpy(char *dest, const char *src, unsigned int n) {
	unsigned int i;
	for (i = 0; i < n && src[i] != '\0'; i++) {
		dest[i] = src[i];
	}
	for (; i < n; i++) {
		dest[i] = '\0';
	}
}

unsigned int log(uint64_t n, int base) {
	unsigned int count = 0;
	while (n /= base)
		count++;
	return count;
}

void my_strcpy(char *dest, const char *src) {
	//char *aux = dest;
	while ((*dest++ = *src++))
		;
	return;
}

void my_strcat(char *dest, const char *src) {
	while (*dest)
		dest++;
	while ((*dest++ = *src++))
		;
}

uint64_t my_strcmp(const char *s1, const char *s2) {
	while (*s1 && (*s1 == *s2)) {
		s1++;
		s2++;
	}
	return *(const unsigned char *) s1 - *(const unsigned char *) s2;
}