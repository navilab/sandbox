/** Module rsb2_assert - Interface.
 * @file rsb2_assert.h
 * @author jp.tranvouez@navilab.com
 * @defgroup rsb2_assert Assertion checker
 * @ingroup rsb2_libcore
 * @{
 */
#ifndef RSB2_ASSERT_H
#define RSB2_ASSERT_H

#include <assert.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Check that a condition is true.
 * @param c condition to check
 */
#define RSB2_ASSERT(c) \
		assert((c) &&  #c " is false.")

/** Check that a condition is false.
 * @param c condition to check
 */
#define RSB2_ASSERT_FALSE(c) \
		assert(!(c) &&  #c " is true.")

/** Check that a pointer is NULL.
 * @param p pointer to check
 */
#define RSB2_ASSERT_NULL(p) \
		assert((p) == NULL && "Not a null pointer.")

/** Check that a pointer is not NULL.
 * @param p pointer to check
 */
#define RSB2_ASSERT_NOTNULL(p) \
		assert((p) != NULL && "Null pointer.")

/** Check that an integer is zero.
 * @param i integer to check
 */
#define RSB2_ASSERT_ZERO(i) \
		assert((i) == 0 && "Not a zero value.")

/** Check that an integer is not zero.
 * @param i integer to check
 */
#define RSB2_ASSERT_NOTZERO(i) \
		assert((i) != 0 && "Zero value.")

/** Check that an integer is positive.
 * @param i integer to check
 */
#define RSB2_ASSERT_POSINT(i) \
		assert((i) > 0 && "Not a positive integer.")

/** Check that an integer is not positive.
 * @param i integer to check
 */
#define RSB2_ASSERT_NOTPOSINT(i) \
		assert((i) <= 0 && "Positive integer.")

/** Check that an integer is negative.
 * @param i integer to check
 */
#define RSB2_ASSERT_NEGINT(i) \
		assert((i) < 0 && "Not a negative integer.")

/** Check that an integer is not negative.
 * @param i integer to check
 */
#define RSB2_ASSERT_NOTNEGINT(i) \
		assert((i) >= 0 && "Negative integer.")

#ifdef __cplusplus
}
#endif

#endif /*@} RSB2_ASSERT_H */
