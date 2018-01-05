#include <string.h>

char *reverse(char *s, size_t len)
{
    size_t i = 0;
	char c;
	do {
		c = s[i];
		s[i] = s[len - i];
		s[len - i] = c;
	} while (i++ < (len / 2));
    return s;
}