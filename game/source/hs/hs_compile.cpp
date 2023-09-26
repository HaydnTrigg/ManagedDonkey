#include "hs/hs_compile.hpp"

#include "hs/hs.hpp"

#include <ctype.h>
#include <stdlib.h>

bool hs_type_primitive_parser_bool(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char const* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_boolean);
	ASSERT(expression->constant_type == expression->type);

	bool& value = *reinterpret_cast<bool*>(expression->data);

	if (csstrcmp(source_offset, "true") == 0 || csstrcmp(source_offset, "on") == 0 || csstrcmp(source_offset, "1") == 0)
	{
		value = true;
		return true;
	}
	if (csstrcmp(source_offset, "false") == 0 || csstrcmp(source_offset, "off") == 0 || csstrcmp(source_offset, "0") == 0)
	{
		value = false;
		return true;
	}

	hs_compile_globals.error_message = "i expected \"true\" or \"false\".";
	hs_compile_globals.error_offset = expression->source_offset;

	value = false;
	return false;
}

bool hs_type_primitive_parser_real(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char const* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_real);
	ASSERT(expression->constant_type == expression->type);

	real& value = *reinterpret_cast<real*>(expression->data);

	if (*source_offset == '-')
		++source_offset;

	bool result = true;
	bool error_occurred = false;
	while (*source_offset)
	{
		if (!isdigit(*source_offset))
		{
			if (error_occurred || *source_offset != '.')
			{
				hs_compile_globals.error_message = "this is not a valid real number.";
				hs_compile_globals.error_offset = expression->source_offset;

				result = false;
				break;
			}
			error_occurred = true;
		}
		source_offset++;
	}

	value = static_cast<real>(atof(&hs_compile_globals.compiled_source[expression->source_offset]));
	return result;
}

bool hs_type_primitive_parser_integer(long expression_index)
{
	hs_syntax_node* expression = hs_syntax_get(expression_index);
	char const* source_offset = &hs_compile_globals.compiled_source[expression->source_offset];

	ASSERT(expression->type == _hs_type_short_integer || expression->type == _hs_type_long_integer);
	ASSERT(expression->constant_type == expression->type);

	short& short_value = *reinterpret_cast<short*>(expression->data);
	long& long_value = *reinterpret_cast<long*>(expression->data);

	if (*source_offset == '-')
		++source_offset;

	bool result = true;
	bool error_occurred = false;
	while (*source_offset)
	{
		if (!isdigit(*source_offset))
		{
			if (error_occurred || *source_offset != '.')
			{
				hs_compile_globals.error_message = "this is not a valid integer.";
				hs_compile_globals.error_offset = expression->source_offset;

				result = false;
				break;
			}
			error_occurred = true;
		}
		source_offset++;
	}

	long source_value = atoi(&hs_compile_globals.compiled_source[expression->source_offset]);
	if (result && !(expression->type == _hs_type_short_integer || IN_RANGE_INCLUSIVE(source_value, -32767, 32768)))
	{
		hs_compile_globals.error_message = "shorts must be in the range [-32767, 32768].";
		hs_compile_globals.error_offset = expression->source_offset;

		result = false;
	}

	short_value = static_cast<short>(source_value);
	if (expression->type == _hs_type_long_integer)
		long_value = static_cast<long>(source_value);

	return result;
}

hs_type_primitive_parser_t hs_type_primitive_parsers[k_hs_type_count]
{
	// non-types
	nullptr,
	nullptr,
	nullptr,

	// void
	nullptr,

	hs_type_primitive_parser_bool,
	hs_type_primitive_parser_real,
	hs_type_primitive_parser_integer,
	hs_type_primitive_parser_integer,
};

