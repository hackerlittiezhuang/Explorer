/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by Hu wenjie(CN)<1@GhostBirdOS.org>,Linus Torvalds
 *Explorer C standard string
 *ExplorerOS/Kernel/lib/string.h
 *version:Alpha
 *7/20/2014
 */

#include <lib/string.h>
#include <stddef.h>

/**
 *strncmp - Compare two length-limited strings
 *@cs: One string
 *@ct: Another string
 *@count: The maximum number of bytes to compare
 */
int strncmp(const char *cs, const char *ct, size_t count)
{
	unsigned char c1, c2;

	while (count) {
		c1 = *cs++;
		c2 = *ct++;
		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
		count--;
	}
	return 0;
}

/**
 *strcpy - Copy a %NUL terminated string
 *@dest: Where to copy the string to
 *@src: Where to copy the string from
 */
char *strcpy(char *dest, const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		/*nothing */;
	return tmp;
}

/**
 *strncpy - Copy a length-limited, %NUL-terminated string
 *@dest: Where to copy the string to
 *@src: Where to copy the string from
 *@count: The maximum number of bytes to copy
 *
 *The result is not %NUL-terminated if the source exceeds
 *@count bytes.
 *
 *In the case where the length of @src is less than  that  of
 *count, the remainder of @dest will be padded with %NUL.
 *
 */
char *strncpy(char *dest, const char *src, size_t count)
{
	char *tmp = dest;

	while (count--) {
		if ((*tmp = *src) != 0)
			src++;
		tmp++;
	}
	return dest;
}

/**
 *strlcpy - Copy a %NUL terminated string into a sized buffer
 *@dest: Where to copy the string to
 *@src: Where to copy the string from
 *@size: size of destination buffer
 *
 *Compatible with *BSD: the result is always a valid
 *NUL-terminated string that fits in the buffer (unless,
 *of course, the buffer size is zero). It does not pad
 *out the result like strncpy() does.
 */
size_t strlcpy(char *dest, const char *src, size_t size)
{
	size_t ret = strlen(src);

	if (size) {
		size_t len = (ret >= size) ? size - 1 : ret;
		memcpy(dest, src, len);
		dest[len] = '\0';
	}
	return ret;
}

/**
 *strlen - Find the length of a string
 *@s: The string to be sized
 */
size_t strlen(const char *s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}

/**
 * strspn - Calculate the length of the initial substring of @s which only contain letters in @accept
 * @s: The string to be searched
 * @accept: The string to search for
 */
size_t strspn(const char *s, const char *accept)
{
	const char *p;
	const char *a;
	size_t count = 0;

	for (p = s; *p != '\0'; ++p) {
		for (a = accept; *a != '\0'; ++a) {
			if (*p == *a)
				break;
		}
		if (*a == '\0')
			return count;
		++count;
	}
	return count;
}

/**
 * memset - Fill a region of memory with the given value
 * @s: Pointer to the start of the area.
 * @c: The byte to fill the area with
 * @count: The size of the area.
 *
 * Do not use memset() to access IO space, use memset_io() instead.
 */
void *memset(void *s, int c, size_t count)
{
	char *xs = s;

	while (count--)
		*xs++ = c;
	return s;
}

/**
 * memcpy - Copy one area of memory to another
 * @dest: Where to copy to
 * @src: Where to copy from
 * @count: The size of the area.
 *
 * You should not use this function to access IO space, use memcpy_toio()
 * or memcpy_fromio() instead.
 */
void *memcpy(void *dest, const void *src, size_t count)
{
	char *tmp = dest;  
	const char *s = src;  

    while (count--)  
		*tmp++ = *s++;  
	return dest;  
}

/**
 * memmove - Copy one area of memory to another
 * @dest: Where to copy to
 * @src: Where to copy from
 * @count: The size of the area.
 *
 * Unlike memcpy(), memmove() copes with overlapping areas.
 */
void *memmove(void *dest, const void *src, size_t count)
{
	char *tmp;
	const char *s;

	if (dest <= src) {
		tmp = dest;
		s = src;
		while (count--)
			*tmp++ = *s++;
	} else {
		tmp = dest;
		tmp += count;
		s = src;
		s += count;
		while (count--)
			*--tmp = *--s;
	}
	return dest;
}
