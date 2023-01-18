// https://wiki.osdev.org/Serial_Ports

#include <serial/serial.h>
#include <ports/ports.h>

#define PORT 0x3F8
#define CHECK_BYTE 0xAE

static uint8_t did_receive() {
	return inb(PORT + 5) & 1;
}

static uint8_t is_transmit_empty() {
	return inb(PORT + 5) & 0x20;
}

int32_t serial_init() {
	outb(PORT + 1, 0x00);	// Disable interrupts
	outb(PORT + 3, 0x80);	// Enable DLAB
	outb(PORT + 0, 0x03);	// Set divisor to 3 -> 38400 baud
	outb(PORT + 1, 0x00);
	outb(PORT + 3, 0x03);	// 8 bits, no parity, one stop bit
	outb(PORT + 2, 0xC7);	// Enable FIFO, clear them, with 14-byte threshold
	outb(PORT + 4, 0x0B);	// IRQs enabled, RTS/DSR set
	outb(PORT + 4, 0x1E);	// Set in loopback mode, test the serial chip
	outb(PORT + 0, CHECK_BYTE);

	// If we didn't receive the check byte back, the serial is faulty
	if(inb(PORT) != CHECK_BYTE) {
		return 1;
	}

	// If it's all file, set it to normal mode
	outb(PORT + 4, 0x0F);
	return 0;
}

void serial_write(const char c) {
	while(is_transmit_empty() == 0);
	outb(PORT, c);
}

char serial_read() {
	while(did_receive() == 0);
	return inb(PORT);
}