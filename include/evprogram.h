#pragma once

void evmain(void);

__attribute__((section("program_entry"))) void evos_program_entry(void) {
	evmain();
}