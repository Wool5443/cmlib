#ifndef CMLIB_COMMON_H
#define CMLIB_COMMON_H

#if defined(__GNUC__) || defined(__clang__)
    #define INLINE inline __attribute__((unused))
    #define UNUSED [[maybe_unused]]
#else
    #define INLINE inline
    #define UNUSED
#endif

#define ARRAY_SIZE(array) sizeof(array) / sizeof(*(array))

#define FIRST(x, ...) x
#define SECOND(_, x, ...) x
#define IS_EMPTY(...) SECOND(__VA_OPT__(,) 0, 1)
#define EXPAND_BUT_FIRST(_, ...) __VA_ARGS__
#define VA_OPT_BUT_FIRST(_, ...) __VA_OPT__(,)

#define IF(condition, true_case, false_case) _IF(condition)(true_case, false_case)
#define _IF(condition) _IF_##condition
#define _IF_1(true_case, false_case) true_case
#define _IF_0(true_case, false_case) false_case

#define SWITCH_EMPTY(empty_case, non_empty_case, ...) \
    IF(IS_EMPTY(__VA_ARGS__), empty_case, non_empty_case)

#ifdef __cplusplus
#define CMLIB_CAST(type, value) type(value)
#else
#define CMLIB_CAST(type, value) (type)(value)
#endif

#endif // CMLIB_COMMON_H
