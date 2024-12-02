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
#ifdef WITH_JSON_C
#include <json-c/json.h>
#include <string.h>
#include <ctype.h>
#endif
#include "dmioutput.h"

static void pr_init_ptext(void)
{
	/* a no-op for text output */
}

static void pr_finish_ptext(void)
{
	/* a no-op for text output */
}

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
	.pr_init        = pr_init_ptext,
	.pr_finish      = pr_finish_ptext,
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

#ifdef WITH_JSON_C

json_dmi_output json_dmi_out = {
	.root = NULL,
	.array = NULL,
	.item = NULL,
	.values = NULL,
};

static void pr_init_json(void)
{
	json_dmi_out.root = json_object_new_object();
	json_dmi_out.array = json_object_new_array();
}

static void pr_finish_json(void)
{
	/* Add array of decoded entries */
	if (json_dmi_out.root && json_dmi_out.array)
	{
		json_object_object_add(json_dmi_out.root, "dmi_data", json_dmi_out.array);
		/* Print json object to stdout */
		printf("%s\n", json_object_to_json_string_ext(json_dmi_out.root, JSON_C_TO_STRING_PRETTY));
		/* Free root JSON object */
		json_object_put(json_dmi_out.root);
	}
}

static void pr_comment_json(const char *format, va_list args)
{
	(void)format;
	(void)args;
}

static void pr_info_json(const char *format, va_list args)
{
	(void)format;
	(void)args;
}

static void pr_handle_json(const struct dmi_header *h)
{
	int ret;
	char handle_str[7];

	json_dmi_out.item = json_object_new_object();

	sprintf(handle_str, "0x%04X", h->handle);
	ret = json_object_object_add(
		json_dmi_out.item,
		"handle",
		json_object_new_string(handle_str)
		);
	if (ret < 0)
		fprintf(stderr, "Unable to add JSON object: '%d' with key: 'handle'\n", h->handle);

	ret = json_object_object_add(json_dmi_out.item, "type", json_object_new_int(h->type));
	if (ret < 0)
		fprintf(stderr, "Unable to add JSON object: '%d' with key: 'type'\n", h->type);

	ret = json_object_object_add(json_dmi_out.item, "length", json_object_new_int(h->length));
	if (ret < 0)
		fprintf(stderr, "Unable to add JSON object: '%d' with key: 'length'\n", h->length);

	if (h->type == 126)
	{
		json_object_object_add(json_dmi_out.item, "active", json_object_new_boolean(0));
	}
	else
	{
		json_object_object_add(json_dmi_out.item, "active", json_object_new_boolean(1));
	}

	json_dmi_out.values = json_object_new_object();
	ret = json_object_object_add(json_dmi_out.item, "values", json_dmi_out.values);
	if (ret < 0)
		fprintf(stderr, "Unable to add JSON object with key: 'values'\n");
}

static void pr_handle_name_json(const char *format, va_list args)
{
	char *str = NULL;
	int ret;
	if (json_dmi_out.item)
	{
		ret = vasprintf(&str, format, args);
		if (ret != -1)
		{
			ret = json_object_object_add(json_dmi_out.item, "description", json_object_new_string(str));
			if (ret < 0)
			{
				fprintf(stderr, "Unable to add JSON object: '%s' with key: 'description'\n", str);
			}
			free(str);
		}
	}
	else
	{
		ret = vasprintf(&str, format, args);
		if (ret != -1)
		{
			fprintf(stderr, "Unable to add JSON object: '%s' with key: 'description' (no SMBIOS struct)\n", str);
			free(str);
		}
	}
}

static void attr_json(const char *name, const char *format, va_list args) {
	char *str = NULL;
	int ret;
	ret = vasprintf(&str, format, args);
	if (ret != -1)
	{
		/* Convert name to lowercase and replace " " with "_" */
		char *key = strdup(name);
		if (key != NULL)
		{
			int i;
			for (i = 0; key[i]; i++)
			{
				if (key[i] == ' ')
				{
					key[i] = '_';
				}
				else
				{
					key[i] = tolower(key[i]);
				}
			}
			ret = json_object_object_add(json_dmi_out.values, key, json_object_new_string(str));
			if (ret < 0)
				fprintf(stderr, "Unable to add JSON object: '%s' with key: '%s'\n", str, key);
			free(key);
		}
		else
		{
			fprintf(stderr, "Unable to create JSON key: '%s'\n", name);
		}
		free(str);
	} else {
		fprintf(stderr, "Unable to create JSON key from name: '%s' and format: '%s'\n", name, format);
	}
}

static void pr_attr_json(const char *name, const char *format, va_list args)
{
	attr_json(name, format, args);
}

static void pr_subattr_json(const char *name, const char *format, va_list args)
{
	attr_json(name, format, args);
}

static void pr_list_start_json(const char *name, const char *format, va_list args)
{
	(void)name;
	(void)format;
	(void)args;
}

static void pr_list_item_json(const char *format, va_list args)
{
	(void)format;
	(void)args;
}

static void pr_list_end_json(void)
{
}

static void pr_sep_json(void)
{
	if (json_dmi_out.item)
	{
		json_object_array_add(json_dmi_out.array, json_dmi_out.item);
		json_dmi_out.item = NULL;
	}
}

static void pr_struct_err_json(const char *format, va_list args)
{
	(void)format;
	(void)args;
}

static struct ofmt ofmt_json =
{
	.pr_init        = pr_init_json,
	.pr_finish      = pr_finish_json,
	.pr_comment     = pr_comment_json,
	.pr_info        = pr_info_json,
	.pr_handle      = pr_handle_json,
	.pr_handle_name = pr_handle_name_json,
	.pr_attr        = pr_attr_json,
	.pr_subattr     = pr_subattr_json,
	.pr_list_start  = pr_list_start_json,
	.pr_list_item   = pr_list_item_json,
	.pr_list_end    = pr_list_end_json,
	.pr_sep         = pr_sep_json,
	.pr_struct_err  = pr_struct_err_json,
};
#endif

struct ofmt *ofmt = NULL;

void pr_init(void)
{
	ofmt->pr_init();
}

void pr_finish(void)
{
	if (ofmt != NULL)
		ofmt->pr_finish();
}

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
#ifdef WITH_JSON_C
		case OFMT_JSON:
			ofmt = &ofmt_json;
			break;
#endif
		default:
			ofmt = &ofmt_plain_text;
		break;
	}
}
