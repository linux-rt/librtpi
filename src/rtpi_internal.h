/* SPDX-License-Identifier: LGPL-2.1-only */
/* Copyright © 2018 VMware, Inc. All Rights Reserved. */

#ifndef RPTI_H_INTERNAL_H
#define RPTI_H_INTERNAL_H

#include <linux/futex.h>

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
	return pi_mutex{ 0, f };
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
	return pi_cond{ 0, f, 0, RTPI_COND_STATE_READY };
}
#endif

#endif // RPTI_H_INTERNAL_H
