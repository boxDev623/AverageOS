#include "string.h"
#include "ctype.h"
#include "float.h"
#include "limits.h"
#include "errno.h"

double strtod(const char *str, char **endptr) {
	double number;
	int exponent;
	int negative;
	char *p = (char *) str;
	double p10;
	int n;
	int num_digits;
	int num_decimals;

	while (isspace(*p)) p++;

	negative = 0;
	switch (*p) {
		case '-': negative = 1;
		case '+': p++;
	}

	number = 0.;
	exponent = 0;
	num_digits = 0;
	num_decimals = 0;

	// Process string of digits
	while (isdigit(*p)) {
		number = number * 10. + (*p - '0');
		p++;
		num_digits++;
	}

	// Process decimal part
	if (*p == '.') {
		p++;

		while (isdigit(*p)) {
		number = number * 10. + (*p - '0');
		p++;
		num_digits++;
		num_decimals++;
		}

		exponent -= num_decimals;
	}

	if (num_digits == 0) {
		errno = ERANGE;
		return 0.0;
	}

	// Correct for sign
	if (negative) number = -number;

	// Process an exponent string
	if (*p == 'e' || *p == 'E') {
		// Handle optional sign
		negative = 0;
		switch (*++p) {
		case '-': negative = 1;   // Fall through to increment pos
		case '+': p++;
		}

		// Process string of digits
		n = 0;
		while (isdigit(*p)) {
		n = n * 10 + (*p - '0');
		p++;
		}

		if (negative) {
		exponent -= n;
		} else {
		exponent += n;
		}
	}

	if (exponent < DBL_MIN_EXP  || exponent > DBL_MAX_EXP) {
		errno = ERANGE;
		return UINT_MAX;
	}

	p10 = 10.;
	n = exponent;
	if (n < 0) n = -n;
	while (n) {
		if (n & 1) {
		if (exponent < 0) {
			number /= p10;
		} else {
			number *= p10;
		}
		}
		n >>= 1;
		p10 *= p10;
	}

	if (number == UINT_MAX) errno = ERANGE;
	if (endptr) *endptr = p;

	return number;
}