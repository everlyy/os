#include <io/io.h>

void io_wait() {
	outb(0x80, 0);
}