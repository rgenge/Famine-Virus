#include "woody.h"

extern elf_t			*elf;
extern injector		*inj;
extern bool				elf_alloc;
extern bool				elf_data_alloc;
extern bool				inj_alloc;
extern bool				inj_data_alloc;

off_t	get_filesize(int fd)
{
	off_t result = lseek(fd, 0, SEEK_END);
	if (result == -1 || lseek(fd, 0, SEEK_SET) == -1) {
		return -1;
	}
	return result;
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char				*d;
	const char	*s;
	if ((!dst) && (!src))
		return (NULL);
	d = dst;
	s = src;
	while (n--)
		*d++ = *s++;
	return (dst);
}

