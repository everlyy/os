#include <keyboard/keyboard.h>
#include <ports/ports.h>
#include <stdbool.h>

#define PORT_DATA_REGISTER 0x60
#define PORT_STATUS_REGISTER 0x64
#define PORT_COMMAND_REGISTER 0x64

#define STATUS_OUTPUT_BUFFER (1 << 0)

#define COMMAND_GET_CONTROLLER_CONFIG 0x20
#define COMMAND_SET_CONTROLLER_CONFIG 0x60
#define COMMAND_TEST_CONTROLLER 0xAA
#define COMMAND_TEST_PORT1 0xAB
#define COMMAND_ENABLE_PORT1 0xAE
#define COMMAND_DISABLE_PORT1 0xAD
#define COMMAND_DISABLE_PORT2 0xA7
#define COMMAND_READ_CONTROLLER_OUTPUT 0xD0
#define COMMAND_WRITE_CONTROLLER_OUTPUT 0xD1

static bool shift = false;
static bool ctrl = false;
static bool e0 = false;

static const char scancode_to_ascii[] =
	"\0"
	"\0" "1234567890-=" "\b"
	"\t" "qwertyuiop[]" "\n"
	"\0" "asdfghjkl;'`" "\0"
	"\\" "zxcvbnm,./" "\0\0\0"
	" "
;

static const char scancode_to_ascii_shift[] =
	"\0"
	"\0" "!@#$%^&*()_+" "\b"
	"\t" "QWERTYUIOP{}" "\n"
	"\0" "ASDFGHJKL:\"~" "\0"
	"|" "ZXCVBNM<>?" "\0\0\0"
	" "
;

int32_t keyboard_init() {
	// Disable first and second PS/2 ports
	outb(PORT_COMMAND_REGISTER, COMMAND_DISABLE_PORT1);
	outb(PORT_COMMAND_REGISTER, COMMAND_DISABLE_PORT2);

	// Flush the output buffer
	while(inb(PORT_STATUS_REGISTER) & STATUS_OUTPUT_BUFFER)
		inb(PORT_DATA_REGISTER);

	// Set the Controller Configuration Byte 
	outb(PORT_COMMAND_REGISTER, COMMAND_GET_CONTROLLER_CONFIG);
	uint8_t controller_configuration = inb(PORT_DATA_REGISTER);

	controller_configuration &= ~0b01000011; // You want to disable all IRQs and disable translation (clear bits 0, 1 and 6)

	outb(PORT_COMMAND_REGISTER, COMMAND_SET_CONTROLLER_CONFIG);
	outb(PORT_DATA_REGISTER, controller_configuration);

	// Perform Controller Self Test 
	outb(PORT_COMMAND_REGISTER,COMMAND_TEST_CONTROLLER);
	uint8_t test_result = inb(PORT_DATA_REGISTER);
	// Check if test passed
	if(test_result != 0x55)
		return 1;

	// Restore controller configuration (test can reset controller on some hardware)
	outb(PORT_COMMAND_REGISTER, COMMAND_SET_CONTROLLER_CONFIG);
	outb(PORT_DATA_REGISTER, controller_configuration);

	// Perform Interface Tests
	outb(PORT_COMMAND_REGISTER, COMMAND_TEST_PORT1);
	test_result = inb(PORT_DATA_REGISTER);
	if(test_result != 0x00)
		return 1;

	// Enable Devices
	outb(PORT_COMMAND_REGISTER, COMMAND_ENABLE_PORT1);

	// Reset Devices
	outb(PORT_DATA_REGISTER, 0xFF);

	// Set scan code set to 1
	outb(PORT_DATA_REGISTER, 0xF0);
	outb(PORT_DATA_REGISTER, 1);

	return 0;
}

uint8_t keyboard_get_scancode() {
	// Wait until there's data
	while(!(inb(PORT_STATUS_REGISTER) & STATUS_OUTPUT_BUFFER));
	return inb(PORT_DATA_REGISTER);
}

uint8_t keyboard_get_ascii() {
	char ascii = 0x00;
	uint8_t scancode = keyboard_get_scancode();

	enum {
		LSHIFT_DOWN = 0x2A,
		LSHIFT_UP = 0xAA,
		RSHIFT_DOWN = 0x36,
		RSHIFT_UP = 0xB6,
		LCTRL_DOWN = 0x14,
		LCTRL_UP = 0x9D,
	};

	if(scancode == LSHIFT_DOWN || scancode == RSHIFT_DOWN) {
		shift = true;
	} else if(scancode == LSHIFT_UP || scancode == RSHIFT_UP) {
		shift = false;
	} else if(scancode == LCTRL_DOWN) {
		ctrl = true;
	} else if(scancode == LCTRL_UP) {
		ctrl = false;
	} else if(scancode == 0xE0) {
		e0 = true;
	} else {
		if(!(scancode & 0x80) && !e0) {
			if(shift)
				ascii = scancode_to_ascii_shift[scancode];
			else
				ascii = scancode_to_ascii[scancode];
		}
		if(e0)
			e0 = false;
	}

	return ascii;
}