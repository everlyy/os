#pragma once

#include <types.h>

enum {
	ATA_STATUS_GENERIC_ERROR = -1,
	ATA_STATUS_SUCCESS = 0,
};

int32_t ata_read(const uint16_t starting_sector, const uint16_t size_in_sectors, uint8_t* buffer);
int32_t ata_write(const uint16_t starting_sector, const uint16_t size_in_sectors, const uint8_t* buffer);