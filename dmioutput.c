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

#define _GNU_SOURCE
#include <stdarg.h>
#include <stdio.h>
#include <json-c/json.h>
#include "dmioutput.h"

static int output_format = TEXT_FORMAT;

void pr_set_json_format(void)
{
	output_format = JSON_FORMAT;
}

void pr_printf(const char *format, ...)
{
	if (output_format == TEXT_FORMAT) {
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
	}
}

void pr_comment(const char *format, ...)
{
	va_list args;
	if (output_format == TEXT_FORMAT) {
		printf("# ");
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		printf("\n");
	}
}

void pr_info(const char *format, ...)
{
	va_list args;
	if (output_format == TEXT_FORMAT) {
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		printf("\n");
	}
}

json_object *pr_handle(const struct dmi_header *h)
{
	if (output_format == TEXT_FORMAT) {
		printf("Handle 0x%04X, DMI type %d, %d bytes\n",
			   h->handle, h->type, h->length);
	}
    if (output_format == JSON_FORMAT) {
        json_object *header = json_object_new_object();
        json_object_object_add(header, "handle", json_object_new_int(h->handle));
        json_object_object_add(header, "type", json_object_new_int(h->type));
        json_object_object_add(header, "length", json_object_new_int(h->length));
        return header;
    }
    return NULL;
}

void pr_handle_name(json_object *entry, const char *format, ...)
{
	va_list args;
	if (output_format == TEXT_FORMAT) {
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		printf("\n");
	}
    if (output_format == JSON_FORMAT && entry != NULL) {
		char *str;
		int ret;
		va_start(args, format);
		ret = vasprintf(&str, format, args);
		va_end(args);
		if (ret != -1) {
			json_object_object_add(entry, "name", json_object_new_string(str));
			free(str);
		}
    }
}

void pr_attr(json_object *entry, const char *name, const char *format, ...)
{
	va_list args;
	if (output_format == TEXT_FORMAT) {
		printf("\t%s: ", name);
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		printf("\n");
	}
	if (output_format == JSON_FORMAT && entry != NULL) {
		char *str;
		int ret;
		va_start(args, format);
		ret = vasprintf(&str, format, args);
		va_end(args);
		if (ret != -1) {
			json_object_object_add(entry, name, json_object_new_string(str));
			free(str);
		}
	}
}

void pr_subattr(json_object *entry, const char *name, const char *format, ...)
{
	va_list args;
	if (output_format == TEXT_FORMAT) {
		printf("\t\t%s: ", name);
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		printf("\n");
	}
	if (output_format == JSON_FORMAT && entry != NULL) {
		char *str;
		int ret;
		va_start(args, format);
		ret = vasprintf(&str, format, args);
		va_end(args);
		if (ret != -1) {
			json_object_object_add(entry, name, json_object_new_string(str));
			free(str);
		}
	}
}

void pr_list_start(const char *name, const char *format, ...)
{
	va_list args;
	if (output_format == TEXT_FORMAT) {
		printf("\t%s:", name);
		/* format is optional, skip value if not provided */
		if (format) {
			printf(" ");
			va_start(args, format);
			vprintf(format, args);
			va_end(args);
		}
		printf("\n");
	}
}

void pr_list_item(const char *format, ...)
{
	va_list args;
	if (output_format == TEXT_FORMAT) {
		printf("\t\t");
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		printf("\n");
	}
}

void pr_list_end(void)
{
	/* a no-op for text output */
}

void pr_sep(void)
{
	if (output_format == TEXT_FORMAT) {
		printf("\n");
	}
}

void pr_struct_err(const char *format, ...)
{
	va_list args;
	if (output_format == TEXT_FORMAT) {
		printf("\t");
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		printf("\n");
	}
}
