#include <stdint.h>
#include <string.h>

static const char conv[] = "0123456789abcdefghijklmnopqrstuvwxzy";
int u32tostr(char *str, size_t len, uint32_t value, int base)
{
	uint32_t i = 0;
	char s[32 + 1] = "";
	if (base < 2 || base > 36)
		return -1;
	do {
		s[i++] = conv[value % base];
	} while ((value /= base));
	if (i > len)
		return -1;
	reverse(s, i - 1);
	s[i] = '\0';
	memmove(str, s, i);
	return 0;
}

int s32tostr(char *str, size_t len, int32_t value, unsigned base)
{
	int32_t neg = value;
	uint32_t i = 0, x = value;
	char s[32 + 2] = "";
	if (base < 2 || base > 36)
		return -1;
	if (x > INT32_MAX)
		x = -x;
	do {
		s[i++] = conv[x % base];
	} while ((x /= base) > 0);
	if (neg < 0)
		s[i++] = '-';
	if (i > len)
		return -1;
	reverse(s, i - 1);
	s[i] = '\0';
	memmove(str, s, i);
	return 0;
}