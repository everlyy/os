#include <io/ports.h>

#define ASM __asm__ __volatile__

void outb(uint16_t port, uint8_t value) {
	ASM("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port) {
	uint8_t value;
	ASM("inb %1, %0" : "=a"(value) : "Nd"(port));
	return value;
}

void outw(uint16_t port, uint16_t value) {
	ASM("outw %0, %1" : : "a"(value), "Nd"(port));
}

uint16_t inw(uint16_t port) {
	uint16_t value;
	ASM("inw %1, %0" : "=a"(value) : "Nd"(port));
	return value;
}