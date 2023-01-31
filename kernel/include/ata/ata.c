// https://wiki.osdev.org/ATA_PIO_Mode

#include <ata/ata.h>
#include <io/ports.h>

#define IO_PORT_BASE 0x1F0
#define PORT_DATA_REGISTER			(IO_PORT_BASE + 0)
#define PORT_ERROR_REGISTER			(IO_PORT_BASE + 1)
#define PORT_FEATURES_REGISTER		(IO_PORT_BASE + 1)
#define PORT_SECTOR_COUNT_REGISTER	(IO_PORT_BASE + 2)
#define PORT_SECTOR_NUMBER_REGISTER	(IO_PORT_BASE + 3)
#define PORT_CYLINDER_LOW_REGISTER	(IO_PORT_BASE + 4)
#define PORT_CYLINDER_HIGH_REGISTER	(IO_PORT_BASE + 5)
#define PORT_DRIVE_HEAD_REGISTER	(IO_PORT_BASE + 6)
#define PORT_STATUS_REGISTER		(IO_PORT_BASE + 7)
#define PORT_COMMAND_REGISTER		(IO_PORT_BASE + 7)

#define CONTROL_PORT_BASE 0x3F6
#define PORT_ALTERNATE_STATUS_REGISTER	(CONTROL_PORT_BASE + 0)
#define PORT_DEVICE_CONTROL_REGISTER	(CONTROL_PORT_BASE + 0)
#define PORT_DRIVE_ADDRESS_REGISTER		(CONTROL_PORT_BASE + 1)

#define STATUS_ERR  (1 << 0)
#define STATUS_IDX  (1 << 1)
#define STATUS_CORR (1 << 2)
#define STATUS_DRQ  (1 << 3)
#define STATUS_SRV  (1 << 4)
#define STATUS_DF   (1 << 5)
#define STATUS_RDY  (1 << 6)
#define STATUS_BSY  (1 << 7)

#define COMMAND_READ_SECTORS 0x20
#define COMMAND_WRITE_SECTORS 0x30
#define COMMAND_FLUSH_CACHE 0xE7

#define DRIVE_MASTER 0xE0
#define DRIVE_SLAVE 0xF0

static void send_command(const uint16_t starting_sector, const uint16_t size_in_sectors, const uint8_t command) {
	outb(PORT_DRIVE_HEAD_REGISTER, 0xE0 | ((starting_sector >> 24) & 0x0F));
	outb(PORT_SECTOR_COUNT_REGISTER, size_in_sectors);
	outb(PORT_SECTOR_NUMBER_REGISTER, starting_sector & 0xFF);
	outb(PORT_CYLINDER_LOW_REGISTER, (starting_sector >> 8) & 0xFF);
	outb(PORT_CYLINDER_HIGH_REGISTER, (starting_sector >> 16) & 0xFF);
	outb(PORT_COMMAND_REGISTER, command);
}

int32_t ata_read(const uint16_t starting_sector, const uint16_t size_in_sectors, uint8_t* buffer) {
	// Remove 1 from starting sector because sectors are supposed to be 1-based, but that isn't the case here for some reason
	send_command(starting_sector - 1, size_in_sectors, COMMAND_READ_SECTORS);

	uint16_t* bufptr = (uint16_t*)buffer;
	for(uint16_t i = 0; i < size_in_sectors; i++) {
		// Wait until drive isn't busy
		while(inb(PORT_STATUS_REGISTER) & STATUS_BSY);

		// Read 256 words (512 bytes, 1 sector) from data port
		for(uint32_t j = 0; j < 256; j++)
			*bufptr++ = inw(PORT_DATA_REGISTER);

		// Read alternate status register for 400ns delay
		for(uint8_t j = 0; j < 4; j++)
			inb(PORT_ALTERNATE_STATUS_REGISTER);
	}

	// FIXME: Actually handle errors
	return ATA_STATUS_SUCCESS;
}

int32_t ata_write(const uint16_t starting_sector, const uint16_t size_in_sectors, const uint8_t* buffer) {
	send_command(starting_sector - 1, size_in_sectors, COMMAND_WRITE_SECTORS);

	uint16_t* bufptr = (uint16_t*)buffer;
	for(uint16_t i = 0; i < size_in_sectors; i++) {
		while(inb(PORT_STATUS_REGISTER) & STATUS_BSY);

		// Write 256 words (512 bytes, 1 sector) to data port
		for(uint32_t j = 0; j < 256; j++)
			outw(PORT_DATA_REGISTER, *bufptr++);

		// Send cache flush command
		outb(PORT_COMMAND_REGISTER, COMMAND_FLUSH_CACHE);
		while(inb(PORT_STATUS_REGISTER) & STATUS_BSY);
	}

	// FIXME: Actually handle errors
	return ATA_STATUS_SUCCESS;
}