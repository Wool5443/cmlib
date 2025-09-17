/**
 * @file Scratch_buf.h
 * @author Misha Solodilov (mihsolodilov2015@gmail.com)
 * @brief Header file for the Scratch buffer, a temporary buffer for efficient
 * string manipulation.
 *
 * This header defines the Scratch buffer, a utility that helps in efficient
 * string manipulation in scenarios where temporary storage for strings is
 * required. It allows for operations such as initialization, clearing,
 * appending, and formatted string output in a buffer. The Scratch buffer is
 * useful in cases where the program requires dynamic memory management for
 * temporary data without constantly allocating and deallocating memory.
 *
 * @version 1.0
 * @date 12.06.2025
 *
 * @copyright Copyright (c) 2025
 */

#ifndef CMLIB_SCRATCH_BUF_H_
#define CMLIB_SCRATCH_BUF_H_

#include "String.h"

/**
 * @brief Retrieve the raw Scratch buffer.
 *
 * This function provides access to the current Scratch buffer in an unsafe
 * manner. It returns a pointer to the underlying `String` object without
 * performing any checks, so it should be used with caution.
 *
 * @return Pointer to the current Scratch buffer (`String*`).
 */
String* get_scratch_buffer_unsafe_();

/**
 * @brief Initialize a Scratch buffer with a specified capacity.
 *
 * This function initializes the Scratch buffer with the provided capacity. The
 * Scratch buffer is a temporary buffer that allows for efficient string
 * manipulations. The capacity defines the maximum size the buffer can grow to.
 *
 * @param capacity The desired size of the Scratch buffer in bytes.
 *
 * @return An error code indicating success or failure.
 *
 * @see Error_code
 */
Error_code scratch_init(size_t capacity);

/**
 * @brief Destroys the Scratch buffer.
 *
 * This function cleans up and deallocates the memory used by the Scratch buffer.
 * After calling this function, the Scratch buffer is no longer valid.
 */
void scratch_dtor();

/**
 * @brief Get the size of the Scratch buffer.
 *
 * This function returns the current size of the Scratch buffer.
 *
 * @return The current size of the Scratch buffer in bytes.
 */
size_t scratch_get_size();

/**
 * @brief Get the raw data of the Scratch buffer.
 *
 * This function returns a pointer to the raw character data stored in the
 * Scratch buffer.
 *
 * @return A pointer to the raw data in the Scratch buffer (`char*`).
 */
char* scratch_get();

/**
 * @brief Get the Scratch buffer as a `Str` (slice).
 *
 * This function provides a `Str` (slice) representation of the Scratch buffer,
 * which can be used for more efficient operations with string manipulations.
 *
 * @return A `Str` representing the current contents of the Scratch buffer.
 *
 * @see Str
 */
Str scratch_get_str();

/**
 * @brief Copy the contents of the Scratch buffer into a `String`.
 *
 * This function creates a new `String` object and copies the contents of the
 * Scratch buffer into it.
 *
 * @return A `Result_String` containing either the copied string or an error
 * code indicating failure.
 *
 * @see String
 * @see Error_code
 */
Result_String scratch_copy_string();

/**
 * @brief Set a count of characters in the Scratch buffer to `'\0'`.
 *
 * This function effectively truncates the Scratch buffer by setting the first
 * `count` characters to `'\0'`, reducing the content size accordingly.
 *
 * @param count The number of characters to set to `'\0'`.
 */
void scratch_pop(size_t count);

/**
 * @brief Clears the contents of the Scratch buffer.
 *
 * This function empties the Scratch buffer by resetting its content without
 * deallocating its memory.
 */
void scratch_clear();

/**
 * @brief Print formatted output into the Scratch buffer.
 *
 * This function formats the given string according to the provided `format`
 * string and appends the result to the Scratch buffer.
 *
 * @param format The format string used for the printf-style formatting.
 *
 * @return An error code indicating success or failure.
 *
 * @see Error_code
 */
Error_code scratch_printf(const char* format, ...);

/**
 * @brief Print formatted output into the Scratch buffer using a `va_list`.
 *
 * This function formats the string using a `va_list` and appends the result to
 * the Scratch buffer. It is a variant of `scratch_printf` that allows for
 * argument lists to be passed in.
 *
 * @param format The format string used for the printf-style formatting.
 * @param args The list of arguments to format.
 *
 * @return An error code indicating success or failure.
 *
 * @see Error_code
 */
Error_code scratch_vprintf(const char* format, va_list args);

/**
 * @brief Append a C-style string to the Scratch buffer.
 *
 * This function appends the provided null-terminated C-style string to the
 * Scratch buffer.
 *
 * @param string The C-style string to append.
 *
 * @return An error code indicating success or failure.
 *
 * @see Error_code
 */
Error_code scratch_append(const char* string);

/**
 * @brief Append a `Str` (slice) to the Scratch buffer.
 *
 * This function appends a `Str` slice (which is a part of a string) to the
 * Scratch buffer.
 *
 * @param slice The `Str` to append.
 *
 * @return An error code indicating success or failure.
 *
 * @see Str
 * @see Error_code
 */
Error_code scratch_append_str(Str slice);

/**
 * @brief Append a `String` to the Scratch buffer.
 *
 * This function appends the contents of a `String` object to the Scratch buffer.
 *
 * @param string The `String` to append.
 *
 * @return An error code indicating success or failure.
 *
 * @see String
 * @see Error_code
 */
Error_code scratch_append_string(const String string);

/**
 * @brief Append a single character to the Scratch buffer.
 *
 * This function appends a single character to the Scratch buffer.
 *
 * @param c The character to append.
 *
 * @return An error code indicating success or failure.
 *
 * @see Error_code
 */
Error_code scratch_append_char(char c);

#endif // CMLIB_SCRATCH_BUF_H_
