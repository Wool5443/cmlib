#ifndef CMLIB_SCRATCH_BUF_H_
#define CMLIB_SCRATCH_BUF_H_

#include "../String/String.h"

String* getScratchBuffer_UNSAFE_();

/**
 * @brief Initialize a ScatchBuffer with capacity
 *
 * @param [in] capacity
 *
 * @return ErrorCode
 *
 * @see ErrorCode
 */
Error_code scratch_init(size_t capacity);

/**
 * @brief Destroys the ScratchBuffer
 */
void scratch_dtor();

/**
 * @brief Get ScratchBuffer size
 *
 * @return size
 */
size_t scratch_get_size();

/**
 * @brief Get ScratchBuffer raw data
 *
 * @return char*
 */
char* scratch_get();

/**
 * @brief Get ScratchBuffer as Str
 *
 * @return Str
 *
 * @see Str
 */
Str scratch_get_str();

/**
 * @brief Copy ScratchBuffer to a String
 *
 * @return Result_String
 *
 * @see String
 * @see ErrorCode
 */
Result_String scratch_copy_string();

/**
 * @brief Sets count characters to '\0' from ScratchBuffer
 *
 * @param [in] count
 */
void scratch_pop(size_t count);

/**
 * @brief Cleans the buffer
 */
void scratch_clear();


/**
 * @brief Printf in Scratch buffer
 *
 * @param [in] format
 *
 * @return ErrorCode
 *
 * @see ErrorCode
 */
Error_code scratch_printf(const char* format, ...);

/**
 * @brief Printf in Scratch buffer
 *
 * @param [in] format
 * @param [in] args
 *
 * @return ErrorCode
 *
 * @see ErrorCode
 */
Error_code scratch_vprintf(const char* format, va_list args);

/**
 * @brief Append c-style string to ScratchBuffer
 *
 * @param [in] c-style string
 *
 * @return ErrorCode
 *
 * @see ErrorCode
 */
Error_code scratch_append(const char* string);

/**
 * @brief Append Str to ScratchBuffer
 *
 * @param [in] slice
 *
 * @return ErrorCode
 *
 * @see Str
 * @see ErrorCode
 */
Error_code scratch_append_str(Str string);

/**
 * @brief Append String to ScratchBuffer
 *
 * @param [in] string
 *
 * @return ErrorCode
 *
 * @see String
 * @see ErrorCode
 */
Error_code scratch_append_string(const String string);

/**
 * @brief Append char to ScratchBuffer
 *
 * @param [in] char c
 *
 * @return ErrorCode
 *
 * @see Str
 * @see ErrorCode
 */
Error_code scratch_append_char(char c);

#endif // CMLIB_SCRATCH_BUF_H_
