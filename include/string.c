#include <string.h>

int32_t memcmp(const void* s1, const void* s2, uint32_t n) {
	const uint8_t* a = (const uint8_t*)s1;
	const uint8_t* b = (const uint8_t*)s2;

	for(uint32_t i = 0; i < n; i++) {
		if(a[i] < b[i])
			return -1;
		else if(b[i] < a[i])
			return 1;
	}

	return 0;
}

void* memcpy(void* dest, const void* src, uint32_t n) {
	uint8_t* a = (uint8_t*)dest;
	const uint8_t* b = (const uint8_t*)src;

	for(uint32_t i = 0; i < n; i++)
		a[i] = b[i];

	return dest;
}

uint32_t strlen(const char* str) {
	uint32_t len = 0;
	while(*str++)
		len++;
	return len;
}