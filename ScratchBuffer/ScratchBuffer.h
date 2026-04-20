/**
 * @file Scratch_buf.h
 * @brief Process-global temporary string buffer.
 *
 * Scratch buffer APIs operate on a single internal `String` instance.
 */

#ifndef CMLIB_SCRATCH_BUF_H_
#define CMLIB_SCRATCH_BUF_H_

#include "String.h"

/**
 * @brief Initializes scratch storage.
 * @param memory_resource Resource pointer; must be non-NULL.
 * @param capacity Initial capacity excluding trailing null.
 * @return `EVERYTHING_FINE` or error code.
 */
Error_code scratch_ctor(void* memory_resource, size_t capacity);

/**
 * @brief Releases scratch storage and resets state.
 */
void scratch_dtor();

/**
 * @brief Returns current scratch size.
 */
size_t scratch_get_size();

/**
 * @brief Returns mutable scratch data pointer.
 */
char* scratch_get();

/**
 * @brief Returns borrowed slice of current scratch contents.
 */
Str scratch_get_str();

/**
 * @brief Returns deep copy of scratch contents as owned `String`.
 * @param memory_resource Resource pointer for returned string; must be
 * non-NULL.
 * @return Owned string copy, or error.
 */
Result_String scratch_copy_string(void* memory_resource);

/**
 * @brief Removes last `count` bytes from scratch.
 * @param count Number of bytes to remove.
 * When `count > size`, function clears the buffer.
 */
void scratch_pop(size_t count);

/**
 * @brief Clears scratch content while preserving capacity.
 */
void scratch_clear();

/**
 * @brief Appends printf-formatted text to scratch buffer.
 */
Error_code scratch_printf(const char* format, ...);

/**
 * @brief Appends printf-formatted text from a `va_list`.
 */
Error_code scratch_vprintf(const char* format, va_list args);

/**
 * @brief Appends null-terminated string to scratch buffer.
 */
Error_code scratch_append(const char* string);

/**
 * @brief Appends borrowed slice bytes to scratch buffer.
 */
Error_code scratch_append_str(Str slice);

/**
 * @brief Appends owned string contents to scratch buffer.
 */
Error_code scratch_append_string(const String string);

/**
 * @brief Appends single byte to scratch buffer.
 */
Error_code scratch_append_char(char c);

#endif // CMLIB_SCRATCH_BUF_H_
