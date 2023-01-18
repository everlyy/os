#include <evfs/evfs.h>
#include <ata/ata.h>
#include <string.h>

static int32_t verify_signature() {
	const char* signature = EVFS_SIGNATURE;

	if(memcmp(filetable->signature, signature, EVFS_SIGNATURE_LENGTH) != 0)
		return EVFS_STATUS_INVALID_SIGNATURE;

	return EVFS_STATUS_SUCCESS;
}

static uint16_t calculate_checksum() {
	// Not really a checksum but it will do for now
	uint16_t checksum = 0;

	uint8_t* entries_bytes = (uint8_t*)filetable->entries;
	for(uint32_t i = 0; i < (filetable->number_of_entries * sizeof(filetable_entry_t)); i++) {
		checksum += entries_bytes[i];
	}

	return checksum;
}

static void update_checksum() {
	filetable->checksum = calculate_checksum();
}

static int32_t verify_checksum() {
	uint16_t checksum = calculate_checksum();

	if(checksum != filetable->checksum)
		return EVFS_STATUS_INVALID_CHECKSUM;

	return EVFS_STATUS_SUCCESS;
}

int32_t evfs_write_filetable() {
	return ata_write(2, FILETABLE_SIZE_IN_SECTORS, (uint8_t*)filetable);
}

int32_t evfs_verify_filetable() {
	int32_t status;

	status = verify_signature();
	if(status < EVFS_STATUS_SUCCESS)
		return status;

	status = verify_checksum();
	if(status < EVFS_STATUS_SUCCESS)
		return status;

	return EVFS_STATUS_SUCCESS;
}

int32_t evfs_create_file(const uint8_t name_length, const char* name, filetable_entry_t** entry) {
	// TODO: Add creating files
	(void)name_length;
	(void)name;
	(void)entry;
	return EVFS_STATUS_GENERIC_ERROR;
}

int32_t evfs_get_entry_by_name(const uint8_t name_length, const char* name, filetable_entry_t** entry) {
	for(uint8_t i = 0; i < filetable->number_of_entries; i++) {
		filetable_entry_t* temp = &filetable->entries[i];

		if(temp->name_length != name_length)
			continue;

		if(memcmp(temp->name, name, name_length) == 0) {
			*entry = temp;
			return EVFS_STATUS_SUCCESS;
		}
	}

	return EVFS_STATUS_ENTRY_NOT_FOUND;
}

int32_t evfs_delete_entry(filetable_entry_t* entry) {
	// Set `is_free_space` byte in entry
	entry->is_free_space = 1;

	update_checksum();
	return EVFS_STATUS_SUCCESS;
}

int32_t evfs_rename_entry(filetable_entry_t* entry, const uint8_t name_length, const char* new_name) {
	if(name_length > MAX_NAME_LENGTH)
		return EVFS_STATUS_NAME_TOO_LONG;

	memcpy(entry->name, new_name, name_length);

	update_checksum();
	return EVFS_STATUS_SUCCESS;
}

int32_t evfs_read_file(const filetable_entry_t* entry, uint8_t* buffer) {
	return ata_read(entry->starting_sector, entry->size_in_sectors, buffer);
}

int32_t evfs_write_file(filetable_entry_t* entry, const uint8_t* buffer, const uint16_t new_size_in_sectors) {
	// TODO: Add writing to files
	(void)entry;
	(void)buffer;
	(void)new_size_in_sectors;
	return EVFS_STATUS_GENERIC_ERROR;
}