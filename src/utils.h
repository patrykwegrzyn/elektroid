/*
 *   utils.h
 *   Copyright (C) 2019 David García Goñi <dagargo@gmail.com>
 *
 *   This file is part of Elektroid.
 *
 *   Elektroid is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Elektroid is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Elektroid. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <glib.h>

#ifndef UTILS_H
#define UTILS_H

#define LABEL_MAX 128

#define debug_print(level, format, ...) if (level <= debug_level) fprintf(stderr, "DEBUG:" __FILE__ ":%d:(%s): " format, __LINE__, __FUNCTION__, ## __VA_ARGS__)
#define error_print(format, ...) fprintf(stderr, "\x1b[31mERROR:" __FILE__ ":%d:(%s): " format "\x1b[m", __LINE__, __FUNCTION__, ## __VA_ARGS__)

enum item_type
{
  ELEKTROID_NONE = 0,
  ELEKTROID_FILE = 'F',
  ELEKTROID_DIR = 'D'
};

struct item_iterator;

typedef guint (*iterator_next) (struct item_iterator *);

typedef void (*iterator_free) (void *);

typedef gint (*iterator_copy) (struct item_iterator *,
			       struct item_iterator *);

struct item
{
  gchar *name;
  guint32 size;
  gint32 index;
  enum item_type type;
};

struct item_iterator
{
  iterator_next next;
  iterator_free free;
  iterator_copy copy;
  void *data;
  struct item item;
};

typedef void (*job_control_callback) (gdouble);

struct job_control
{
  gboolean active;
  GMutex mutex;
  job_control_callback callback;
};

typedef gint (*fs_init_iter_func) (struct item_iterator *, const gchar *,
				   void *);

typedef gint (*fs_path_func) (const gchar *, void *);

typedef gint (*fs_src_dst_func) (const gchar *, const gchar *, void *);

typedef gint (*fs_remote_file_op) (const gchar *, GByteArray *,
				   struct job_control *, void *);

typedef gchar *(*fs_get_item_id) (struct item *);

typedef gint (*fs_local_file_op) (const gchar *, GByteArray *,
				  struct job_control *);

struct fs_operations
{
  gint fs;
  fs_init_iter_func readdir;
  fs_path_func mkdir;
  fs_path_func delete;
  fs_src_dst_func rename;
  fs_src_dst_func move;
  fs_src_dst_func copy;
  fs_path_func clear;
  fs_src_dst_func swap;
  fs_remote_file_op download;
  fs_remote_file_op upload;
  fs_get_item_id getid;
  fs_local_file_op save;
  fs_local_file_op load;
  const gchar *extension;
};

extern int debug_level;

gchar *debug_get_hex_data (gint, guint8 *, guint);

gchar *debug_get_hex_msg (const GByteArray *);

gchar *chain_path (const gchar *, const gchar *);

void remove_ext (gchar *);

const gchar *get_ext (const gchar *);

gchar get_type_from_inventory_icon (const gchar *);

gchar *get_local_startup_path (const gchar *);

void free_msg (gpointer);

gchar *get_item_name (struct item *);

gchar *get_item_index (struct item *);

guint next_item_iterator (struct item_iterator *);

void free_item_iterator (struct item_iterator *);

gint copy_item_iterator (struct item_iterator *, struct item_iterator *);

gint load_file (const char *, GByteArray *, struct job_control *);

gint save_file (const char *, GByteArray *, struct job_control *);

gchar *get_human_size (guint, gboolean);

#endif
