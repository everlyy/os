#include <types.h>
#include <serial/serial.h>
#include <serial/debug.h>
#include <evfs/evfs.h>
#include <ata/ata.h>
#include <string.h>

__attribute__((section("kernel_entry"))) void kernel_main(void) {
	serial_init();

	filetable_entry_t* entry = NULL;
	evfs_get_entry_by_name(6, "kernel", &entry);
	evfs_rename_entry(entry, 6, "everly");
	evfs_write_filetable();

	while(1);
}