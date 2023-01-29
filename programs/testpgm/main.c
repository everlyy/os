#include <evprogram.h>

void evmain(void) {
	__asm__ __volatile__("mov $0xDEADBEEF, %EAX");
	__asm__ __volatile__("cli; hlt");
}