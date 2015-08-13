#include <iconv.h>

int code_convert(wchar_t *from_wchar_tset, wchar_t *to_wchar_tset, wchar_t *inbuf, int inlen, wchar_t *outbuf, int outlen);

int u2g(wchar_t *inbuf, int inlen, wchar_t *outbuf, int outlen);

int g2u(wchar_t *inbuf, size_t inlen, wchar_t *outbuf, size_t outlen);