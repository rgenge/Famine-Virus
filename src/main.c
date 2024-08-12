#include "woody.h"
#include "buzz_buzzard.h"

extern unsigned char src_buzz_buzzard_bin[];
extern unsigned int src_buzz_buzzard_bin_len;

elf_t *elf;
injector *inj;
bool elf_alloc = false;
bool elf_data_alloc = false;
bool inj_alloc = false;
bool inj_data_alloc = false;

void elf_init(char *vict, elf_t **elf_ptr)
{
	read_original(vict, &(*elf_ptr));
	(*elf_ptr)->bit_class =
		(*elf_ptr)->data[EI_CLASS] == ELFCLASS64   ? 64
		: (*elf_ptr)->data[EI_CLASS] == ELFCLASS32 ? 32
												   : 0;
	___die((*elf_ptr)->bit_class == 32, "Not accepting 32bit files");
	if ((*elf_ptr)->bit_class == 64)
	{
		_E64 = (Elf64_Ehdr *)(*elf_ptr)->data;
		_P64 = (Elf64_Phdr *)((*elf_ptr)->data + _E64->e_phoff);
		_S64 = (Elf64_Shdr *)((*elf_ptr)->data + _E64->e_shoff);
		(*elf_ptr)->phnum = _E64->e_phnum == PN_XNUM ? _S64->sh_info : _E64->e_phnum;
		(*elf_ptr)->shnum = _E64->e_shnum == 0 ? _S64[0].sh_size : _E64->e_shnum;
		(*elf_ptr)->shstrndx = _E64->e_shstrndx == SHN_XINDEX ? _S64[0].sh_link : _E64->e_shstrndx;
	}
}

void get_files_list(const char *directory_path)
{
	struct dirent *entry;
	DIR *dir = opendir(directory_path);
	___die2(dir == NULL,"Could not open directory");
	char *file_array[MAX_FILES];
	int fileCount = 0;

	while ((entry = readdir(dir)) != NULL && fileCount < MAX_FILES)
	{
		if (entry->d_type == DT_REG)
		{
			file_array[fileCount] = malloc(MAX_FILENAME);
			if (file_array[fileCount] != NULL)
			{
				ft_memcpy(file_array[fileCount], entry->d_name, MAX_FILENAME);
				file_array[fileCount][MAX_FILENAME - 1] = '\0';
				fileCount++;
			}
		}
	}
	closedir(dir);
	// Print the list of files
	printf("Files in directory %s:\n", directory_path);
	for (int i = 0; i < fileCount; i++)
	{
		printf("%s\n", file_array[i]);
		free(file_array[i]);
	}
}

void inject(const char *woody)
{
	printf("%s", woody);
}

int main(int argc, char **argv)
{
	___die(argc != 2, "Usage: `woody_woodpacker binary_file`");
	const char *directory_path = "./tmp";
	get_files_list(directory_path);
	elf_init(argv[1], &elf);
	//	inject("woody");
	return free_all();
}
