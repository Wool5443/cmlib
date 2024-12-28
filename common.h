#ifndef CMLIB_COMMON_H
#define CMLIB_COMMON_H

#if defined(__GNUC__) || defined(__clang__)
    #define INLINE static __attribute__((unused))
    #define UNUSED [[maybe_unused]]
#else
    #define INLINE static
    #define UNUSED
#endif

#define FIRST(x, ...) x
#define SECOND(_, x, ...) x
#define HAS_COMMA(...) SECOND(__VA_OPT__(,) 0, 1)
#define IS_EMPTY(...) HAS_COMMA(__VA_ARGS__)
#define EXPAND_BUT_FIRST(_, ...) __VA_ARGS__
#define VA_OPT_BUT_FIRST(_, ...) __VA_OPT__(,)

#define IF(condition, true_case, false_case) _IF(condition)(true_case, false_case)
#define _IF(condition) _IF_##condition
#define _IF_1(true_case, false_case) true_case
#define _IF_0(true_case, false_case) false_case

#define SWITCH_EMPTY(empty_case, non_empty_case, ...) \
    IF(IS_EMPTY(__VA_ARGS__), empty_case, non_empty_case)

#endif // CMLIB_COMMON_H
