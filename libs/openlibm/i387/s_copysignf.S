/*
 * Written by J.T. Conklin <jtc@netbsd.org>.
 * Public domain.
 */

#include <i387/bsd_asm.h>

//__FBSDID("$FreeBSD: src/lib/msun/i387/s_copysignf.S,v 1.3 2011/01/07 16:13:12 kib Exp $");
/* RCSID("$NetBSD: s_copysignf.S,v 1.3 1995/05/08 23:53:25 jtc Exp $") */

ENTRY(copysignf)
	movl	8(%esp),%edx
	andl	$0x80000000,%edx
	movl	4(%esp),%eax
	andl	$0x7fffffff,%eax
	orl	%edx,%eax
	movl	%eax,4(%esp)
	flds	4(%esp)
	ret
END(copysignf)

	
/* Enable stack protection */
#if defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
