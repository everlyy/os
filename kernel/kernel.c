#include <types.h>
#include <serial/serial.h>
#include <serial/debug.h>
#include <evfs/evfs.h>
#include <string.h>

__attribute__((section("kernel_entry"))) void kernel_main(void) {
	serial_init();

	int32_t status = evfs_verify_filetable();
	if(status < EVFS_STATUS_SUCCESS) {
		debug_printf("Failed to verify filetable: %d\n", status);
		while(1);
	}

	filetable_entry_t* entry;
	status = evfs_get_entry_by_name(6, "kernel", &entry);
	if(status < EVFS_STATUS_SUCCESS) {
		debug_printf("Failed to get entry: %d\n", status);
		while(1);
	}

	debug_printf(
		"Found entry: starting_sector=%d size_in_sectors=%d name_length=%d is_free_space=%x\n", 
		entry->starting_sector,
		entry->size_in_sectors,
		entry->name_length,
		entry->is_free_space
	);

	while(1);
}