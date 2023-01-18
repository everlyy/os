SECTIONS
{
	.text 0x50000 :
	{
		*(kernel_entry);
		*(.text*);
	}

	.data :
	{
		*(.data*)
	}

	.rodata :
	{
		*(.rodata*);
	}

	.bss :
	{
		*(.bss*);
	}
}