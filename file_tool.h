/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright 2020, Josh Chen
 */

#ifndef __FILE_TOOL_H__
#define __FILE_TOOL_H__

#include <stdbool.h>
#include <sys/stat.h>

/***** File Tool Define *****/
enum FT_RETURN {
	FT_OK_SUCCESS		= 0,
	FT_OUT_OF_MEM_ERR	= -1,
	FT_ARGUMENT_ERR		= -2,
	FT_FILE_ERR			= -3,
	FT_NOT_FOUND		= -4,
};

bool ft_is_file_exist(const char *filename);

size_t ft_get_file_size(const char *filename);

int ft_copy_file(const char *src, const char *dest);

int ft_create_path(const char *path, mode_t mode);

#endif //__FILE_TOOL_H__
