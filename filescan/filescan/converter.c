#include "converter.h"
#include <Windows.h>

int code_convert(wchar_t *from_wchar_tset, wchar_t *to_wchar_tset, wchar_t *inbuf, size_t inlen, wchar_t *outbuf, size_t outlen)
{
	iconv_t cd;
	int rc;
	wchar_t **pin = &inbuf;
	wchar_t **pout = &outbuf;

	cd = iconv_open(to_wchar_tset, from_wchar_tset);
	if (cd == 0) return -1;
	memset(outbuf, 0, outlen);
	if (iconv(cd, pin, &inlen, pout, &outlen) == -1) return -1;
	iconv_close(cd);
	return 0;
}

int g2u(wchar_t *inbuf, size_t inlen, wchar_t *outbuf, size_t outlen)
{
	return code_convert("gb2312", "utf-8", inbuf, inlen, outbuf, outlen);
}