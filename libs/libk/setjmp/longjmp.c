#include "setjmp.h"

void longjmp(jmp_buf buffer, int value)
{
	__builtin_longjmp(buffer, 1);
}