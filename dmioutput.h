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

#include "dmidecode.h"

void pr_comment(const char *format, ...);
void pr_info(const char *format, ...);
void pr_handle(const struct dmi_header *h);
void pr_handle_name(const char *format, ...);
void pr_attr(const char *name, const char *format, ...);
void pr_subattr(const char *name, const char *format, ...);
void pr_list_start(const char *name, const char *format, ...);
void pr_list_item(const char *format, ...);
void pr_list_end(void);
void pr_sep(void);
void pr_struct_err(const char *format, ...);

#define OFMT_PLAIN_TEXT                0
#ifdef WITH_JSON_C
#define OFMT_JSON                      1
#endif

struct ofmt
{
	void (*pr_comment)(const char *format, va_list args);
	void (*pr_info)(const char *format, va_list args);
	void (*pr_handle)(const struct dmi_header *h);
	void (*pr_handle_name)(const char *format, va_list args);
	void (*pr_attr)(const char *name, const char *format, va_list args);
	void (*pr_subattr)(const char *name, const char *format, va_list args);
	void (*pr_list_start)(const char *name, const char *format, va_list args);
	void (*pr_list_item)(const char *format, va_list args);
	void (*pr_list_end)(void);
	void (*pr_sep)(void);
	void (*pr_struct_err)(const char *format, va_list args);
};

extern struct ofmt *ofmt;

void set_output_format(int ofmt);
