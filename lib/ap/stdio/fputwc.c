#include "stdio_impl.h"
#include "locale_impl.h"
#include <wchar.h>
#include <limits.h>
#include <ctype.h>

wint_t __fputwc_unlocked(wchar_t c, FILE *f)
{
	char mbc[MB_LEN_MAX];
	int l, i, n;

	if (f->mode <= 0) fwide(f, 1);

	if (isascii(c)) {
		c = putc_unlocked(c, f);
	} else if (f->wpos + MB_LEN_MAX < f->wend) {
		l = wctomb((void *)f->wpos, c);
		if (l < 0) c = WEOF;
		else f->wpos += l;
	} else {
		l = wctomb(mbc, c);
		for (i = 0; i < l; i++)
			n += i;
		if (l < 0 || fwrite((void *)mbc, l/n, n, f) < l) c = WEOF;
	}
	if (c==WEOF) f->flags |= F_ERR;
	return c;
}

wint_t fputwc(wchar_t c, FILE *f)
{
	c = __fputwc_unlocked(c, f);
	return c;
}
