#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <libgen.h>

#define SECTOR_SIZE 512
#define DISK_IMAGE_SIZE_IN_SECTORS 0xFF

#define FILETABLE_SIZE_IN_SECTORS 2
#define FILETABLE_SIZE (FILETABLE_SIZE_IN_SECTORS * SECTOR_SIZE)

#define MAX_ENTRIES ((FILETABLE_SIZE - 32) / sizeof(filetable_entry_t))
#define MAX_NAME_LENGTH 16

#define EVFS_SIGNATURE "evfs"
#define EVFS_SIGNATURE_LENGTH (sizeof(EVFS_SIGNATURE) - 1)

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

void usage(const char* program) {
	printf("%s <disk image> <bootsector> <files for filetable>\n", program);
}

int add_entry_to_filetable(filetable_t* filetable, uint8_t* disk_image, size_t* sector_count, const char* filename, size_t filename_length, FILE* file, size_t filesize) {
	if(filename_length > MAX_NAME_LENGTH) {
		fprintf(stderr, "Name of file %s is too long.\n", filename);
		return 1;
	}

	filetable_entry_t* entry = &filetable->entries[filetable->number_of_entries++];
	entry->starting_sector = *sector_count; // Sectors start at 1
	entry->size_in_sectors = filesize / SECTOR_SIZE;
	entry->name_length = filename_length;
	entry->is_free_space = 0;
	memcpy(entry->name, filename, filename_length);

	size_t read = fread(&disk_image[(entry->starting_sector - 1) * SECTOR_SIZE], SECTOR_SIZE, entry->size_in_sectors, file);
	if(read != entry->size_in_sectors) {
		fprintf(stderr, "Couldn't read %s\n", filename);
		return 1;
	}

	*sector_count += entry->size_in_sectors;
	return 0;
}

void set_filetable_checksum(filetable_t* filetable) {
	uint16_t checksum = 0;

	uint8_t* entries_bytes = (uint8_t*)filetable->entries;
	for(uint32_t i = 0; i < (filetable->number_of_entries * sizeof(filetable_entry_t)); i++) {
		checksum += entries_bytes[i];
	}

	filetable->checksum = checksum;
}

int main(int argc, char** argv) {
	if(argc < 4) {
		usage(argv[0]);
		fprintf(stderr, "ERROR: No files provided.\n");
		return 1;
	}

	filetable_t filetable;
	memset(&filetable, 0, sizeof(filetable));
	memcpy(filetable.signature, EVFS_SIGNATURE, EVFS_SIGNATURE_LENGTH);
	filetable.checksum = 0;
	filetable.number_of_entries = 0;
	filetable.total_sectors = DISK_IMAGE_SIZE_IN_SECTORS;

	size_t sector_count = 1;

	uint8_t* disk_image = (uint8_t*)malloc(DISK_IMAGE_SIZE_IN_SECTORS * SECTOR_SIZE);
	if(!disk_image) {
		fprintf(stderr, "Failed to allocate disk image.\n");
		return 1;
	}

	memset(disk_image, 0, DISK_IMAGE_SIZE_IN_SECTORS * SECTOR_SIZE);

	FILE* bootfile = fopen(argv[2], "rb");
	if(!bootfile) {
		fprintf(stderr, "Couldn't open %s: %s\n", argv[2], strerror(errno));
		return 1;
	}
	add_entry_to_filetable(&filetable, disk_image, &sector_count, "boot", 4, bootfile, 512);
	printf("diskimg: added boot\n");

	sector_count += FILETABLE_SIZE_IN_SECTORS;

	for(int i = 3; i < argc; i++) {
		char* filename = argv[i];

		FILE* file = fopen(filename, "rb");
		if(!file) {
			fprintf(stderr, "Couldn't open %s: %s\n", filename, strerror(errno));
			return 1;
		}

		fseek(file, 0, SEEK_END);
		size_t filesize = ftell(file);
		fseek(file, 0, SEEK_SET);
		
		if(filesize % SECTOR_SIZE != 0) {
			fprintf(stderr, "%s isn't aligned to sector size.\n", filename);
			return 1;
		}

		char* entry_filename = basename(filename);
		for(size_t j = 0; j < strlen(entry_filename); j++) {
			if(entry_filename[j] == '.') {
				entry_filename[j] = 0;
				break;
			}
		}

		int result = add_entry_to_filetable(&filetable, disk_image, &sector_count, entry_filename, strlen(entry_filename), file, filesize);
		if(result != 0) {
			fprintf(stderr, "Failed to add %s to disk image.\n", filename);
			return result;
		}

		printf("diskimg: added %s\n", entry_filename);
	}

	printf("diskimg: finalizing disk image creation...\n");
	set_filetable_checksum(&filetable);
	memcpy(&disk_image[1 * SECTOR_SIZE], &filetable, FILETABLE_SIZE);

	FILE* outfile = fopen(argv[1], "wb");
	if(!outfile) {
		fprintf(stderr, "Couldn't open %s: %s\n", argv[1], strerror(errno));
		return 1;
	}

	size_t written = fwrite(disk_image, SECTOR_SIZE, DISK_IMAGE_SIZE_IN_SECTORS, outfile);
	if(written != DISK_IMAGE_SIZE_IN_SECTORS) {
		fprintf(stderr, "Couldn't write %s\n", argv[1]);
		return 1;
	}

	fclose(outfile);
	free(disk_image);

	printf("diskimg: done\n");

	return 0;
}