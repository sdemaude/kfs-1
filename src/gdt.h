#ifndef GDT_H
# define GDT_H

# include <stdint.h>

/* the GDT must live at this physical address. */
# define GDT_ADDRESS  0x00000800
# define GDT_ENTRIES  7

/*
 * Access byte:
 *   P(1) DPL(2) S(1) E(1) DC(1) RW(1) A(1)
 *   0x9A kernel code: present, ring 0, code, readable
 *   0x92 kernel data: present, ring 0, data, writable
 *   0xFA user code:   present, ring 3, code, readable
 *   0xF2 user data:   present, ring 3, data, writable
 *
 * Granularity 0xCF: 4 KiB granularity, 32-bit protected mode, limit[19:16] = 0xF
 */
#define ACCESS_KERNEL_CODE 0x9A
#define ACCESS_KERNEL_DATA 0x92
#define ACCESS_USER_CODE   0xFA
#define ACCESS_USER_DATA   0xF2
#define GRANULARITY_32BIT  0xCF

struct gdt_entry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t  base_middle;
	uint8_t  access;
	uint8_t  granularity;
	uint8_t  base_high;
} __attribute__((packed));

struct gdt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

void gdt_init(void);
void gdt_flush(uint32_t gdt_ptr_address);

#endif