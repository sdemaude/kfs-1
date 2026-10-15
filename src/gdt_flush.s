BITS 32

%define KERNEL_CODE  0x08
%define KERNEL_DATA  0x10
%define KERNEL_STACK 0x18

section .text
global gdt_flush

; void gdt_flush(uint32_t gdt_ptr_address)
gdt_flush:
	mov eax, [esp + 4]
	lgdt [eax]

	mov ax, KERNEL_DATA
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ax, KERNEL_STACK
	mov ss, ax

	; Far jump to reload CS with the kernel code selector
	jmp KERNEL_CODE:.reload_cs
.reload_cs:
	ret
