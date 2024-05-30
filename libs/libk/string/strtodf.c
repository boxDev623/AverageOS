#include "string.h"

float strtof(const char *str, char **endptr) {
	return (float) strtod(str, endptr);
}