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
#include "dmioutput.h"

static void pr_comment_ptext(const char *format, va_list args)
{
	printf("# ");
	vprintf(format, args);
	printf("\n");
}

static void pr_info_ptext(const char *format, va_list args)
{
	vprintf(format, args);
	printf("\n");
}

static void pr_handle_ptext(const struct dmi_header *h)
{
	printf("Handle 0x%04X, DMI type %d, %d bytes\n",
		h->handle, h->type, h->length);
}

static void pr_handle_name_ptext(const char *format, va_list args)
{
	vprintf(format, args);
	printf("\n");
}

static void pr_attr_ptext(const char *name, const char *format, va_list args)
{
	printf("\t%s: ", name);
	vprintf(format, args);
	printf("\n");
}

static void pr_subattr_ptext(const char *name, const char *format, va_list args)
{
	printf("\t\t%s: ", name);
	vprintf(format, args);
	printf("\n");
}

static void pr_list_start_ptext(const char *name, const char *format, va_list args)
{
	printf("\t%s:", name);
	/* format is optional, skip value if not provided */
	if (format)
	{
		printf(" ");
		vprintf(format, args);
	}
	printf("\n");
}

static void pr_list_item_ptext(const char *format, va_list args)
{
	printf("\t\t");
	vprintf(format, args);
	printf("\n");
}

static void pr_list_end_ptext(void)
{
	/* a no-op for text output */
}

static void pr_sep_ptext(void)
{
	printf("\n");
}

static void pr_struct_err_ptext(const char *format, va_list args)
{
	printf("\t");
	vprintf(format, args);
	printf("\n");
}

static struct ofmt ofmt_plain_text =
{
	.pr_comment     = pr_comment_ptext,
	.pr_info        = pr_info_ptext,
	.pr_handle      = pr_handle_ptext,
	.pr_handle_name = pr_handle_name_ptext,
	.pr_attr        = pr_attr_ptext,
	.pr_subattr     = pr_subattr_ptext,
	.pr_list_start  = pr_list_start_ptext,
	.pr_list_item   = pr_list_item_ptext,
	.pr_list_end    = pr_list_end_ptext,
	.pr_sep         = pr_sep_ptext,
	.pr_struct_err  = pr_struct_err_ptext,
};

struct ofmt *ofmt;

void pr_comment(const char *format, ...)
{
	va_list args;

	va_start(args, format);
	ofmt->pr_comment(format, args);
	va_end(args);
}

void pr_info(const char *format, ...)
{
	va_list args;

	va_start(args, format);
	ofmt->pr_info(format, args);
	va_end(args);
}

void pr_handle(const struct dmi_header *h)
{
	ofmt->pr_handle(h);
}

void pr_handle_name(const char *format, ...)
{
	va_list args;

	va_start(args, format);
	ofmt->pr_handle_name(format, args);
	va_end(args);
}

void pr_attr(const char *name, const char *format, ...)
{
	va_list args;

	va_start(args, format);
	ofmt->pr_attr(name, format, args);
	va_end(args);
}

void pr_subattr(const char *name, const char *format, ...)
{
	va_list args;

	va_start(args, format);
	ofmt->pr_subattr(name, format, args);
	va_end(args);
}

void pr_list_start(const char *name, const char *format, ...)
{
	va_list args;

	va_start(args, format);
	ofmt->pr_list_start(name, format, args);
	va_end(args);
}

void pr_list_item(const char *format, ...)
{
	va_list args;

	va_start(args, format);
	ofmt->pr_list_item(format, args);
	va_end(args);
}

void pr_list_end(void)
{
	/* a no-op for text output */
	ofmt->pr_list_end();
}

void pr_sep(void)
{
	ofmt->pr_sep();
}

void pr_struct_err(const char *format, ...)
{
	va_list args;

	va_start(args, format);
	ofmt->pr_struct_err(format, args);
	va_end(args);
}

void set_output_format(int ofmt_nr)
{
	switch (ofmt_nr)
	{
		default:
			ofmt = &ofmt_plain_text;
		break;
	}
}
