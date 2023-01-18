#include <types.h>
#include <serial/serial.h>
#include <serial/debug.h>
#include <evfs/evfs.h>
#include <ata/ata.h>
#include <string.h>

__attribute__((section("kernel_entry"))) void kernel_main(void) {
	serial_init();

	uint8_t buffer[512];
	ata_read(1, 1, buffer);
	buffer[0] = 'e';
	buffer[1] = 'v';
	ata_write(1, 1, buffer);

	while(1);
}