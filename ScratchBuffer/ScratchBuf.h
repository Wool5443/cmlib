#ifndef SCRATCH_BUF_H
#define SCRATCH_BUF_H

#include "String.h"

String* GetScratchBuffer_UNSAFE_();

/**
 * @brief Initialize a ScatchBuffer with capacity
 *
 * @param [in] capacity
 *
 * @return ErrorCode
 *
 * @see ErrorCode
 */
ErrorCode ScratchInit(size_t capacity);

/**
 * @brief Destroys the ScratchBuffer
 */
void ScratchDtor();

/**
 * @brief Get ScratchBuffer size
 *
 * @return size
 */
size_t ScratchGetSize();

/**
 * @brief Get ScratchBuffer raw data
 *
 * @return char*
 */
char* ScratchGet();

/**
 * @brief Get ScratchBuffer as Str
 *
 * @return Str
 *
 * @see Str
 */
Str ScratchGetStr();

/**
 * @brief Copy ScratchBuffer to a String
 *
 * @return ResultString
 *
 * @see String
 * @see ErrorCode
 */
ResultString ScratchCopyString();

/**
 * @brief Sets count characters to '\0' from ScratchBuffer
 *
 * @param [in] count
 */
void ScratchPop(size_t count);

/**
 * @brief Cleans the buffer
 */
void ScratchClean();

/**
 * @brief Append c-style string to ScratchBuffer
 *
 * @param [in] c-style string
 *
 * @return ErrorCode
 *
 * @see ErrorCode
 */
ErrorCode ScratchAppend(const char* string);

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
ErrorCode ScratchAppendStr(Str string);

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
ErrorCode ScratchAppendString(const String string);

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
ErrorCode ScratchAppendChar(char c);

#endif
