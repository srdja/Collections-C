/*
 * common.h
 *
 *  Created on: Sep 3, 2014
 *      Author: srdja
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define NO_SUCH_INDEX (size_t) - 1

#if defined(_MSC_VER)

#define INLINE __inline
#define FORCE_INLINE __forceinline

#else

#define INLINE inline
#define FORCE_INLINE inline __attribute__((always_inline))

#endif /* _MSC_VER */

#endif /* COMMON_H_ */
