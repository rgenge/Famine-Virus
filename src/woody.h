#ifndef WOODY_H
#define WOODY_H

#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <elf.h>
#include <dirent.h>
#include <string.h>

#define SIZE  18056
#define BUFFER_SIZE 4096
#define signature "fafafa"
#define __debug false
#define MAX_FILES 10
#define MAX_FILENAME 100
#ifndef FUN
#define FUN 1
#endif
#ifndef COLORMODE
#define COLORMODE 1
#endif

typedef struct elf_type
{
	char *data; // raw bin content
	uint32_t data_size;
	unsigned char bit_class; // 32 || 64 bit
	uint32_t phnum;			 // variable loc
	uint32_t shnum;			 // variable loc
	uint32_t shstrndx;		 // variable loc
	union
	{
		Elf32_Ehdr *_32;
		Elf64_Ehdr *_64;
	} ehdr;
	union
	{
		Elf32_Phdr *_32;
		Elf64_Phdr *_64;
	} phdr;
	union
	{
		Elf32_Shdr *_32;
		Elf64_Shdr *_64;
	} shdr;
} elf_t;

typedef struct injection
{
	char *data;
	char *bin;
	uint32_t data_size;
	uint32_t bin_size;
} injector;

ssize_t ft_sendfile(int out_fd, int in_fd, off_t *offset, size_t count) ;
off_t get_filesize(int fd);
int read_original(char *filename, elf_t **);
void *ft_memcpy(void *dst, const void *src, size_t n);
int free_all();
void read_original_elf(char *filename);
void M(size_t offset, size_t c);

#define bool char
#define false 0
#define true !false
#define ___die(_bool, _msg) \
	if (_bool)              \
	die(_msg)
#define ___die2(_bool, _msg) \
	if (_bool)               \
	return die2(_msg)

void die(char *message);
int die2(char *message);
#define _E32 elf->ehdr._32
#define _P32 elf->phdr._32
#define _S32 elf->shdr._32
#define _E64 elf->ehdr._64
#define _P64 elf->phdr._64
#define _S64 elf->shdr._64

#endif
