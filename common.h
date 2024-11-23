#ifndef CMLIB_COMMON_H
#define CMLIB_COMMON_H

#if defined(__GNUC__) || defined(__clang__)
    #define INLINE static inline __attribute__((unused))
#else
    #define INLINE static inline
#endif


#endif // CMLIB_COMMON_H
