
/* AUTO-GENERATED by gen_isr_tables.py, do not edit! */

#include <toolchain.h>
#include <linker/sections.h>
#include <sw_isr_table.h>
#include <arch/cpu.h>

#if defined(CONFIG_GEN_SW_ISR_TABLE) && defined(CONFIG_GEN_IRQ_VECTOR_TABLE)
#define ISR_WRAPPER ((uintptr_t)&_isr_wrapper)
#else
#define ISR_WRAPPER NULL
#endif

typedef void (* ISR)(const void *);
uintptr_t __irq_vector_table _irq_vector_table[39] = {
	ISR_WRAPPER,
	117093,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	117037,
	ISR_WRAPPER,
	ISR_WRAPPER,
	117067,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
	ISR_WRAPPER,
};
struct _isr_table_entry __sw_isr_table _sw_isr_table[39] = {
	{(const void *)0x1c98f, (ISR)0x1c9af},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x1d5b4, (ISR)0x1c8b5},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)0x16e01},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x200024dc, (ISR)0x1749d},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)0x10f81},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)0x17fad},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
	{(const void *)0x0, (ISR)&z_irq_spurious},
};