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

ssize_t ft_sendfile(int out_fd, int in_fd, off_t *offset, size_t count) {
    char buffer[BUFFER_SIZE];
    size_t bytes_left = count;
    ssize_t bytes_read, bytes_written, total_bytes_sent = 0;
    off_t current_offset;
    // If offset is NULL, we start from the current position
    if (offset == NULL) {
        current_offset = lseek(in_fd, 0, SEEK_CUR);
        if (current_offset == -1) {
            return -1;
        }
    } else {
        current_offset = *offset;
    }
    while (bytes_left > 0) {
        // Set the file offset for the input file
        if (lseek(in_fd, current_offset, SEEK_SET) == -1) {
            return -1;
        }
        // Read from the input file
        bytes_read = read(in_fd, buffer, (bytes_left < BUFFER_SIZE) ? bytes_left :BUFFER_SIZE);
        if (bytes_read == -1) {
            return -1;
        }
        if (bytes_read == 0) break;  // End of file
        // Write to the output file
        char *buf_ptr = buffer;
        size_t bytes_to_write = bytes_read;
        while (bytes_to_write > 0) {
            bytes_written = write(out_fd, buf_ptr, bytes_to_write);
            if (bytes_written == -1) {
                return -1;
            }
            buf_ptr += bytes_written;
            bytes_to_write -= bytes_written;
        }
        bytes_left -= bytes_read;
        current_offset += bytes_read;
        total_bytes_sent += bytes_read;
    }
    // Update offset if it's not NULL
    if (offset != NULL) {
        *offset = current_offset;
    }
    return total_bytes_sent;
}
