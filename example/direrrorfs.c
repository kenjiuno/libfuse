/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.
*/

/** @file
 *
 * example filesystem always return EIO error on readdir.
 *
 * Compile with:
 *
 *     gcc -Wall direrrorfs.c `pkg-config fuse3 --cflags --libs` -o direrrorfs
 *
 * ## Source code ##
 * \include direrrorfs.c
 */


#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>

static void *direrrorfs_init(struct fuse_conn_info *conn,
                             struct fuse_config *cfg)
{
	(void) conn;
	cfg->kernel_cache = 1;
	return NULL;
}

static int direrrorfs_getattr(const char *path, struct stat *stbuf,
                              struct fuse_file_info *fi)
{
	(void) fi;
	int res = 0;

	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	}
	else {
		res = -ENOENT;
	}

	return res;
}

static int direrrorfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                              off_t offset, struct fuse_file_info *fi,
                              enum fuse_readdir_flags flags)
{
	(void) offset;
	(void) fi;
	(void) flags;
	(void) buf;
	(void) filler;

	if (strcmp(path, "/") != 0)
		return -ENOENT;

	return -EIO;
}

static int direrrorfs_open(const char *path, struct fuse_file_info *fi)
{
	(void) path;
	(void) fi;

	return -ENOENT;
}

static struct fuse_operations direrrorfs_oper = {
	.init           = direrrorfs_init,
	.getattr	= direrrorfs_getattr,
	.readdir	= direrrorfs_readdir,
	.open		= direrrorfs_open,
};

int main(int argc, char *argv[])
{
	return fuse_main(argc, argv, &direrrorfs_oper, NULL);
}

