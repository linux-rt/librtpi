/* SPDX-License-Identifier: LGPL-2.1-only */
/* Copyright © 2018 VMware, Inc. All Rights Reserved. */

#ifndef RPTI_H_INTERNAL_H
#define RPTI_H_INTERNAL_H

#include <linux/futex.h>

/*
 * The __TIMESIZE macro identifies the native size of timestamps and
 * should always have the same value for any given target
 * architecture.  The (user-provided) value of _TIME_BITS determines
 * whether timestamps in the standard library are 32 or 64 bit.
 *
 * If undefined, then _TIME_BITS == __TIMESIZE is implied.
 * Else if __TIMESIZE == 32 && _TIME_BITS == 64 a special set of
 * syscalls has to be used to support 64 bit timestamps on a 32 bit
 * system.  If the Linux kernel has been configured without
 * CONFIG_COMPAT_32BIT_TIME then those platforms will only have the 64
 * bit replacement syscalls available.
 *
 * Even glibc doesn't support __TIMESIZE == 64 && _TIME_BITS == 32 so
 * don't worry about that combination.
 *
 */

#ifdef PI_EXPLICIT_TIME64
#    // take the user's word for it
#elif !defined(__TIMESIZE) || (__TIMESIZE != 32 && __TIMESIZE != 64)
#    error "Expected __TIMESIZE macro to be defined to either 32 or 64 bit"
#elif !defined(_TIME_BITS) || _TIME_BITS == __TIMESIZE
#    define PI_EXPLICIT_TIME64 0
#elif __TIMESIZE == 32 && _TIME_BITS == 64
#    define PI_EXPLICIT_TIME64 1
#else
#    error "Unexpected combination of __TIMESIZE and _TIME_BITS detected"
#endif


/*
 * PI Mutex
 */
union pi_mutex {
	struct {
		__u32	futex;
		__u32	flags;
	};
	__u8 pad[64];
} __attribute__ ((aligned(64)));

#ifndef __cplusplus
#define PI_MUTEX_INIT(f) { .futex = 0, .flags = f }
#else
inline constexpr pi_mutex PI_MUTEX_INIT(__u32 f) {
	return pi_mutex{{ 0, f }};
}
#endif

/*
 * PI Cond
 */
union pi_cond {
	struct {
		__u32		cond;
		__u32		flags;
		__u32		wake_id;
		__u32		state;
	};
	__u8 pad[128];
} __attribute__ ((aligned(64)));

#define RTPI_COND_STATE_READY 0x1

#ifndef __cplusplus
#define PI_COND_INIT(f) \
	{ .cond = 0 \
	, .flags = f \
	, .wake_id = 0 \
	, .state = RTPI_COND_STATE_READY }
#else
inline constexpr pi_cond PI_COND_INIT(__u32 f) {
	return pi_cond{{ 0, f, 0, RTPI_COND_STATE_READY }};
}
#endif

#endif // RPTI_H_INTERNAL_H
