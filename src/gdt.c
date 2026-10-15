#include "gdt.h"


static struct gdt_entry *const gdt = (struct gdt_entry *)GDT_ADDRESS;
static struct gdt_ptr gdt_pointer;

static void gdt_set_entry(int index, uint32_t base, uint32_t limit,
						  uint8_t access, uint8_t granularity)
{
	gdt[index].base_low = base & 0xFFFF;
	gdt[index].base_middle = (base >> 16) & 0xFF;
	gdt[index].base_high = (base >> 24) & 0xFF;

	gdt[index].limit_low = limit & 0xFFFF;
	gdt[index].granularity = ((limit >> 16) & 0x0F) | (granularity & 0xF0);

	gdt[index].access = access;
}

void gdt_init(void)
{
	gdt_set_entry(0, 0, 0, 0, 0);                                            // Null
	gdt_set_entry(1, 0, 0xFFFFF, ACCESS_KERNEL_CODE, GRANULARITY_32BIT);     // Kernel code
	gdt_set_entry(2, 0, 0xFFFFF, ACCESS_KERNEL_DATA, GRANULARITY_32BIT);     // Kernel data
	gdt_set_entry(3, 0, 0xFFFFF, ACCESS_KERNEL_DATA, GRANULARITY_32BIT);     // Kernel stack
	gdt_set_entry(4, 0, 0xFFFFF, ACCESS_USER_CODE, GRANULARITY_32BIT);       // User code
	gdt_set_entry(5, 0, 0xFFFFF, ACCESS_USER_DATA, GRANULARITY_32BIT);       // User data
	gdt_set_entry(6, 0, 0xFFFFF, ACCESS_USER_DATA, GRANULARITY_32BIT);       // User stack

	gdt_pointer.limit = sizeof(struct gdt_entry) * GDT_ENTRIES - 1;
	gdt_pointer.base = GDT_ADDRESS;

	gdt_flush((uint32_t)&gdt_pointer);
}
