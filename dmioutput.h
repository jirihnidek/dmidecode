/*
 * Generic output functions
 * This file is part of the dmidecode project.
 *
 *   Copyright (C) 2020 Jean Delvare <jdelvare@suse.de>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include <json-c/json.h>
#include "dmidecode.h"

#define TEXT_FORMAT     1
#define JSON_FORMAT     2

void pr_set_json_format(void);
void pr_printf(const char *format, ...);
void pr_comment(const char *format, ...);
void pr_info(const char *format, ...);
void pr_handle(json_object *item, const struct dmi_header *h);
void pr_handle_name(json_object *item, const char *format, ...);
void pr_attr(json_object *entry, const char *name, const char *format, ...);
void pr_subattr(json_object *entry, const char *name, const char *format, ...);
json_object *pr_list_start(json_object *entry, const char *name, const char *format, ...);
void pr_list_item(json_object *list, const char *format, ...);
void pr_list_end(void);
void pr_sep(void);
void pr_struct_err(const char *format, ...);
