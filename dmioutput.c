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

#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "dmioutput.h"

bool use_json = false;

bool begin_of_list = false;
bool begin_of_dict = false;

void pr_set_json_format(void)
{
	use_json = true;
}

/* Replace all occurance of " with \" from strings, because
 * " has special meaning (it starts and terminate string) */
static char *fix_json_string(char *str)
{
	/* TODO: try to find all occurance of " and replace it
	   with \" */
	for(size_t idx=0; idx<strlen(str); ++idx)
	{
		if (str[idx] == '"')
		{
			str[idx] = '\'';
		}
	}
	return str;
}

void pr_comment(const char *format, ...)
{
	va_list args;

	if (use_json == true)
		return;

	printf("# ");
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	printf("\n");
}

void pr_info(const char *format, ...)
{
	va_list args;

	if (use_json == true)
		return;

	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	printf("\n");
}

void pr_handle(const struct dmi_header *h)
{
	if (use_json == true)
		return;

	printf("Handle 0x%04X, DMI type %d, %d bytes\n",
	       h->handle, h->type, h->length);
}

void pr_handle_name(const char *format, ...)
{
	va_list args;

	if (use_json == true)
	{
		begin_of_dict = true;
		printf("\"");
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		printf("\": {");
	} else {
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		printf("\n");
	}
}

void pr_attr(const char *name, const char *format, ...)
{
	va_list args;

	if (use_json == true)
	{
		if (begin_of_dict == true) {
			printf("\"%s\": \"", name);
			begin_of_dict = false;
		}
		else
			printf(",\"%s\": \"", name);
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		printf("\"");
	}
	else
	{
		printf("\t%s: ", name);
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		printf("\n");
	}
}

void pr_subattr(const char *name, const char *format, ...)
{
	va_list args;

	if (use_json == true)
	{
		printf("\"%s\": ", name);
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		printf("\",");
	}
	else
	{
		printf("\t\t%s: ", name);
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		printf("\n");
	}
}

void pr_list_start(const char *name, const char *format, ...)
{
	va_list args;

	begin_of_list = true;

	if (use_json == true)
	{
		if (begin_of_dict == true) {
			printf("\"%s:", name);
			begin_of_dict = false;
		}
		else
			printf(", \"%s:", name);
		/* format is optional, skip value if not provided */
		if (format)
		{
			printf(" ");
			va_start(args, format);
			vprintf(format, args);
			va_end(args);
		}
		printf("\": [");
	}
	else
	{
		printf("\t%s:", name);
		/* format is optional, skip value if not provided */
		if (format)
		{
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

	if (use_json == true)
	{
		char str[2048];
		if (begin_of_list == true) {
			printf("\"");
			begin_of_list = false;
		}
		else
			printf(",\"");
		va_start(args, format);
		vsnprintf(str, 2048, format, args);
		va_end(args);
		fix_json_string(str);
		printf("%s\"", str);
	}
	else
	{
		printf("\t\t");
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		printf("\n");
	}
}

void pr_list_end(void)
{
	if (use_json == true)
	{
		printf("]");
	}
	/* a no-op for text output */
}

void pr_sep(bool end_of_table)
{
	if (use_json == true) {
		if (end_of_table == false)
			printf("},");
		else
			printf("}");
	} else {
		printf("\n");
	}
}

void pr_struct_err(const char *format, ...)
{
	va_list args;

	if (use_json == true)
		return;

	printf("\t");
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	printf("\n");
}
