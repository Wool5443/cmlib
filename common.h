/**
 * @file common.h
 * @brief Shared macro utilities used across cmlib modules.
 *
 * This header provides compile-time and expression macros. Many macros evaluate
 * their arguments exactly once using statement expressions, but they are
 * GCC/Clang extensions and are not portable to strict ISO C compilers.
 */

#ifndef CMLIB_COMMON_H
#define CMLIB_COMMON_H

#if defined(__GNUC__) || defined(__clang__)
/** @brief Marks inline helpers as potentially unused to avoid warning noise. */
#define INLINE inline __attribute__((unused))
/** @brief Marks a variable or parameter as intentionally unused. */
#define UNUSED [[maybe_unused]]
#else
#define INLINE inline
#define UNUSED
#endif

/** @brief Returns number of elements in a fixed-size array expression. */
#define ARRAY_SIZE(array) sizeof(array) / sizeof(*(array))

/** @brief Returns the first variadic argument. */
#define FIRST(x, ...) x
/** @brief Returns the second variadic argument. */
#define SECOND(ignored, x, ...) x
/** @brief Expands to 1 when `__VA_ARGS__` is empty, otherwise 0. */
#define IS_EMPTY(...) SECOND(__VA_OPT__(, ) 0, 1)
/** @brief Expands variadic arguments except the first one. */
#define EXPAND_BUT_FIRST(ignored, ...) __VA_ARGS__
/** @brief Produces ", __VA_ARGS__" when arguments are present. */
#define VA_OPT_BUT_FIRST(ignored, ...) __VA_OPT__(, )

/** @brief Selects one of two macro branches from a 0/1 condition literal. */
#define IF(condition, true_case, false_case)                                   \
    _IF(condition)(true_case, false_case)
#define _IF(condition) _IF_##condition
#define _IF_1(true_case, false_case) true_case
#define _IF_0(true_case, false_case) false_case

/** @brief Selects one of two expansions depending on variadic emptiness. */
#define SWITCH_EMPTY(empty_case, non_empty_case, ...)                          \
    IF(IS_EMPTY(__VA_ARGS__), empty_case, non_empty_case)

/** @brief Stringifies token sequence without prior macro expansion. */
#define STRINGIFY(x) #x
/** @brief Expands argument then stringifies it. */
#define STRINGIFY_VALUE(x) STRINGIFY(x)

/**
 * @brief Returns the smaller of two expressions.
 *
 * @warning Uses statement expressions and `auto` (GNU extension).
 */
#define MIN(x, y)                                                              \
    ({                                                                         \
        auto x_temp = (x);                                                     \
        auto y_temp = (y);                                                     \
        x_temp < y_temp ? x_temp : y_temp;                                     \
    })

/**
 * @brief Returns the larger of two expressions.
 *
 * @warning Uses statement expressions and `auto` (GNU extension).
 */
#define MAX(x, y)                                                              \
    ({                                                                         \
        auto x_temp = (x);                                                     \
        auto y_temp = (y);                                                     \
        x_temp > y_temp ? x_temp : y_temp;                                     \
    })

#endif // CMLIB_COMMON_H
