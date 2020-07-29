// SPDX-License-Identifier: BSD-3-Clause
// Copyright 2020, Josh Chen

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "file_tool.h"

#define FT_LINE_SIZE	512

#define FT_DBG			printf
#define FT_ZALLOC(t, n)	((t *) calloc((n), sizeof(t)))
#define FT_FREE(p)		{ free((void *)(p)); (p) = NULL; }
#define FT_FCLOSE(f)	{ fclose((f)); (f) = NULL; }

bool ft_is_file_exist(const char *filename)
{
	struct stat buf = { 0 };

	if ((filename == NULL) || (strlen(filename) == 0))
		return false;

	return (stat(filename, &buf) == 0);
}

size_t ft_get_file_size(const char *filename)
{
	int64_t size = 0;
	FILE *fp = NULL;

	if ((filename == NULL) || (strlen(filename) == 0)) {
		FT_DBG("filename = %p\n", filename);
		return FT_ARGUMENT_ERR;
	}

	fp = fopen(filename, "rb");
	if (fp == NULL) {
		FT_DBG("fp = %p\n", fp);
		goto exit_ft_get_file_size;
	}

	if (fseek(fp, 0, SEEK_END) != 0) {
		FT_DBG("fseek end fail\n");
		goto exit_ft_get_file_size;
	}

	size = ftell(fp);
	if (size == -1L)
		size = 0;

exit_ft_get_file_size:
	if (fp != NULL)
		FT_FCLOSE(fp);

	return (size_t) size;
}

int ft_copy_file(const char *src, const char *dest)
{
	FILE *src_fp = NULL, *dest_fp = NULL;
	int c = 0;
	int rtn = FT_OK_SUCCESS;

	if ((src == NULL) || (dest == NULL)) {
		FT_DBG("src = %p, dest = %p\n", src, dest);
		rtn = FT_ARGUMENT_ERR;
		goto exit_ft_copy_file;
	}

	if ((strlen(src) == 0) || (strlen(dest) == 0)) {
		FT_DBG("src = %s, dest = %s\n", src, dest);
		rtn = FT_ARGUMENT_ERR;
		goto exit_ft_copy_file;
	}

	src_fp = fopen(src, "rb");
	if (src_fp == NULL) {
		FT_DBG("fopen %s errno = %d\n", src, errno);
		rtn = FT_ARGUMENT_ERR;
		goto exit_ft_copy_file;
	}

	dest_fp = fopen(dest, "wb");
	if (dest_fp == NULL) {
		FT_DBG("fopen %s errno = %d\n", dest, errno);
		rtn = FT_ARGUMENT_ERR;
		goto exit_ft_copy_file;
	}

	while ((c = fgetc(src_fp)) != EOF)
		fputc(c, dest_fp);

exit_ft_copy_file:
	if (src_fp != NULL)
		FT_FCLOSE(src_fp);

	if (dest_fp != NULL)
		FT_FCLOSE(dest_fp);

	return rtn;
}

int ft_create_path(const char *path, mode_t mode)
{
	struct stat sb = { 0 };
	mode_t init_mask = 0;
	int rtn = FT_OK_SUCCESS;

	if ((path == NULL) || (strlen(path) == 0)) {
		FT_DBG("path = %p\n", path);
		return FT_ARGUMENT_ERR;
	}

	if ((stat(path, &sb) == 0) && (S_ISDIR(sb.st_mode) != 0))
		return rtn;

	if (mode == 0)
		mode = S_IRWXU | S_IRWXG | S_IRWXO;

	init_mask = umask(0);
	if (mkdir(path, mode) < 0) {
		FT_DBG("mkdir errno = %d\n", errno);
		rtn = FT_ARGUMENT_ERR;
	}
	umask(init_mask);

	return rtn;
}

