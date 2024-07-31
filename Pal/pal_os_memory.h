/**
 * SPDX-FileCopyrightText: 2021-2024 Infineon Technologies AG
 * SPDX-License-Identifier: MIT
 *
 * \author Infineon Technologies AG
 *
 * \file pal_os_memory.h
 *
 * \brief   This file provides the prototype declarations of PAL OS MEMORY.
 *
 * \ingroup  grPAL
 *
 * @{
 */

#ifndef _PAL_OS_MEMORY_H_
#define _PAL_OS_MEMORY_H_

#ifdef __cplusplus
extern "C" {
#endif


  /**
   * \brief Allocates a block of memory.
   *
   * \details
   * Allocates a block of memory.
   *
   * \pre
   * - None
   *
   * \note
   * - None
   *
   * \param[in] block_size   Size of memory block to be allocated
   */
  #define pal_os_malloc(block_size) malloc(block_size)

  /**
   * \brief Allocates a block of memory and clean the contents.
   *
   * \details
   * Allocates a block of memory and clean the contents.
   *
   * \pre
   * - None
   *
   * \note
   * - None
   *
   * \param[in] no_of_blocks   Number of blocks to be allocated
   * \param[in] block_size     Size of memory block to be allocated
   */
  #define pal_os_calloc(no_of_blocks, block_size) calloc(no_of_blocks, block_size)

  /**
   * \brief Free's a block of allocated memory.
   *
   * \details
   * Free's a block of allocated memory.
   *
   * \pre
   * - None
   *
   * \note
   * - None
   *
   * \param[in] p_block   Pointer to the memory block to be freed
   */
  #define pal_os_free(p_block) free(p_block)

  /**
   * \brief Copies the data from source to destination.
   *
   * \details
   * Copies the data from source to destination.
   *
   * \pre
   * - None
   *
   * \note
   * - None
   *
   * \param[in] p_destination   Pointer to destination buffer
   * \param[in] p_source        Pointer to source buffer
   * \param[in] size            Size in bytes to be copied
   */
  #define pal_os_memcpy(p_destination, p_source, size)    memcpy(p_destination, p_source, size)

  /**
   * \brief Sets the memory buffer to the data supplied.
   *
   * \details
   * Sets the memory buffer with the specified size to the data supplied.
   *
   * \pre
   * - None
   *
   * \note
   * - None
   *
   * \param[in] p_buffer        Pointer to buffer
   * \param[in] value           Value to be written in the buffer
   * \param[in] size            Size in bytes to be written with the specified value
   */
  #define pal_os_memset(p_buffer, value, size)    memset(p_buffer, value, size)

  #ifdef __cplusplus
  }
  #endif

  #endif /* _PAL_MEMORY_MGMT_H_*/

  /**
  * @}
  */
