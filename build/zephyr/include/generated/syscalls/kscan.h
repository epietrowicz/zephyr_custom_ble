
/* auto-generated by gen_syscalls.py, don't edit */
#ifndef Z_INCLUDE_SYSCALLS_KSCAN_H
#define Z_INCLUDE_SYSCALLS_KSCAN_H


#ifndef _ASMLANGUAGE

#include <syscall_list.h>
#include <syscall.h>

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic push
#endif

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#if !defined(__XCC__)
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern int z_impl_kscan_config(const struct device * dev, kscan_callback_t callback);
static inline int kscan_config(const struct device * dev, kscan_callback_t callback)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		/* coverity[OVERRUN] */
		return (int) arch_syscall_invoke2(*(uintptr_t *)&dev, *(uintptr_t *)&callback, K_SYSCALL_KSCAN_CONFIG);
	}
#endif
	compiler_barrier();
	return z_impl_kscan_config(dev, callback);
}


extern int z_impl_kscan_enable_callback(const struct device * dev);
static inline int kscan_enable_callback(const struct device * dev)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		/* coverity[OVERRUN] */
		return (int) arch_syscall_invoke1(*(uintptr_t *)&dev, K_SYSCALL_KSCAN_ENABLE_CALLBACK);
	}
#endif
	compiler_barrier();
	return z_impl_kscan_enable_callback(dev);
}


extern int z_impl_kscan_disable_callback(const struct device * dev);
static inline int kscan_disable_callback(const struct device * dev)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		/* coverity[OVERRUN] */
		return (int) arch_syscall_invoke1(*(uintptr_t *)&dev, K_SYSCALL_KSCAN_DISABLE_CALLBACK);
	}
#endif
	compiler_barrier();
	return z_impl_kscan_disable_callback(dev);
}


#ifdef __cplusplus
}
#endif

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic pop
#endif

#endif
#endif /* include guard */
