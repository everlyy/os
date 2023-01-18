#include <evfs/evfs.h>
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

static int32_t verify_checksum() {
	uint16_t checksum = calculate_checksum();

	if(checksum != filetable->checksum)
		return EVFS_STATUS_INVALID_CHECKSUM;

	return EVFS_STATUS_SUCCESS;
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