#pragma once

#include <types.h>

#define SECTOR_SIZE 512

#define FILETABLE_MEMORY_ADDRESS 0x2000
#define FILETABLE_SIZE_IN_SECTORS 2
#define FILETABLE_SIZE (FILETABLE_SIZE_IN_SECTORS * SECTOR_SIZE)

#define MAX_ENTRIES ((FILETABLE_SIZE - 32) / sizeof(filetable_entry_t))
#define MAX_NAME_LENGTH 16

#define EVFS_SIGNATURE "evfs"
#define EVFS_SIGNATURE_LENGTH (sizeof(EVFS_SIGNATURE) - 1)

enum {
	EVFS_STATUS_ENTRY_NOT_FOUND = -4,
	EVFS_STATUS_INVALID_CHECKSUM = -3,
	EVFS_STATUS_INVALID_SIGNATURE = -2,
	EVFS_STATUS_GENERIC_ERROR = -1,
	EVFS_STATUS_SUCCESS = 0,
};

struct filetable_entry_s {
	uint16_t starting_sector;
	uint16_t size_in_sectors;
	uint8_t name_length;
	uint8_t is_free_space;
	uint8_t ununsed[10];
	char name[MAX_NAME_LENGTH];
};
typedef struct filetable_entry_s filetable_entry_t;

struct filetable_s {
	char signature[EVFS_SIGNATURE_LENGTH];
	uint16_t checksum;
	uint16_t number_of_entries;
	uint16_t total_sectors;
	uint8_t ununsed[21];
	filetable_entry_t entries[MAX_ENTRIES];
};
typedef struct filetable_s filetable_t;

filetable_t* filetable = (filetable_t*)FILETABLE_MEMORY_ADDRESS;

int32_t evfs_verify_filetable();
int32_t evfs_get_entry_by_name(const uint8_t name_length, const char* name, filetable_entry_t** entry);