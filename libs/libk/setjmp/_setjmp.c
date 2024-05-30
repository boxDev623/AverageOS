#include "setjmp.h"

int _setjmp(jmp_buf buffer)
{
	return __builtin_setjmp(buffer);
}