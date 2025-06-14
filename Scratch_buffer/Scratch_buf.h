/**
 * @file ScratchBuf.h
 * @author Misha Solodilov (mihsolodilov2015@gmail.com)
 * @brief Header file for the ScratchBuffer, a temporary buffer for efficient
 * string manipulation.
 *
 * This header defines the ScratchBuffer, a utility that helps in efficient
 * string manipulation in scenarios where temporary storage for strings is
 * required. It allows for operations such as initialization, clearing,
 * appending, and formatted string output in a buffer. The ScratchBuffer is
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

#include "../String/String.h"

/**
 * @brief Retrieve the raw ScratchBuffer.
 *
 * This function provides access to the current ScratchBuffer in an unsafe
 * manner. It returns a pointer to the underlying `String` object without
 * performing any checks, so it should be used with caution.
 *
 * @return Pointer to the current ScratchBuffer (`String*`).
 */
String* getScratchBuffer_UNSAFE_();

/**
 * @brief Initialize a ScratchBuffer with a specified capacity.
 *
 * This function initializes the ScratchBuffer with the provided capacity. The
 * ScratchBuffer is a temporary buffer that allows for efficient string
 * manipulations. The capacity defines the maximum size the buffer can grow to.
 *
 * @param capacity The desired size of the ScratchBuffer in bytes.
 *
 * @return An error code indicating success or failure.
 *
 * @see Error_code
 */
Error_code scratch_init(size_t capacity);

/**
 * @brief Destroys the ScratchBuffer.
 *
 * This function cleans up and deallocates the memory used by the ScratchBuffer.
 * After calling this function, the ScratchBuffer is no longer valid.
 */
void scratch_dtor();

/**
 * @brief Get the size of the ScratchBuffer.
 *
 * This function returns the current size of the ScratchBuffer.
 *
 * @return The current size of the ScratchBuffer in bytes.
 */
size_t scratch_get_size();

/**
 * @brief Get the raw data of the ScratchBuffer.
 *
 * This function returns a pointer to the raw character data stored in the
 * ScratchBuffer.
 *
 * @return A pointer to the raw data in the ScratchBuffer (`char*`).
 */
char* scratch_get();

/**
 * @brief Get the ScratchBuffer as a `Str` (slice).
 *
 * This function provides a `Str` (slice) representation of the ScratchBuffer,
 * which can be used for more efficient operations with string manipulations.
 *
 * @return A `Str` representing the current contents of the ScratchBuffer.
 *
 * @see Str
 */
Str scratch_get_str();

/**
 * @brief Copy the contents of the ScratchBuffer into a `String`.
 *
 * This function creates a new `String` object and copies the contents of the
 * ScratchBuffer into it.
 *
 * @return A `Result_String` containing either the copied string or an error
 * code indicating failure.
 *
 * @see String
 * @see Error_code
 */
Result_String scratch_copy_string();

/**
 * @brief Set a count of characters in the ScratchBuffer to `'\0'`.
 *
 * This function effectively truncates the ScratchBuffer by setting the first
 * `count` characters to `'\0'`, reducing the content size accordingly.
 *
 * @param count The number of characters to set to `'\0'`.
 */
void scratch_pop(size_t count);

/**
 * @brief Clears the contents of the ScratchBuffer.
 *
 * This function empties the ScratchBuffer by resetting its content without
 * deallocating its memory.
 */
void scratch_clear();

/**
 * @brief Print formatted output into the ScratchBuffer.
 *
 * This function formats the given string according to the provided `format`
 * string and appends the result to the ScratchBuffer.
 *
 * @param format The format string used for the printf-style formatting.
 *
 * @return An error code indicating success or failure.
 *
 * @see Error_code
 */
Error_code scratch_printf(const char* format, ...);

/**
 * @brief Print formatted output into the ScratchBuffer using a `va_list`.
 *
 * This function formats the string using a `va_list` and appends the result to
 * the ScratchBuffer. It is a variant of `scratch_printf` that allows for
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
 * @brief Append a C-style string to the ScratchBuffer.
 *
 * This function appends the provided null-terminated C-style string to the
 * ScratchBuffer.
 *
 * @param string The C-style string to append.
 *
 * @return An error code indicating success or failure.
 *
 * @see Error_code
 */
Error_code scratch_append(const char* string);

/**
 * @brief Append a `Str` (slice) to the ScratchBuffer.
 *
 * This function appends a `Str` slice (which is a part of a string) to the
 * ScratchBuffer.
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
 * @brief Append a `String` to the ScratchBuffer.
 *
 * This function appends the contents of a `String` object to the ScratchBuffer.
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
 * @brief Append a single character to the ScratchBuffer.
 *
 * This function appends a single character to the ScratchBuffer.
 *
 * @param c The character to append.
 *
 * @return An error code indicating success or failure.
 *
 * @see Error_code
 */
Error_code scratch_append_char(char c);

#endif // CMLIB_SCRATCH_BUF_H_
