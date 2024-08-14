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

int elf_init(char *vict, elf_t **elf_ptr)
{
	if(!read_original(vict, &(*elf_ptr)))
		return 0;
	(*elf_ptr)->bit_class =
		(*elf_ptr)->data[EI_CLASS] == ELFCLASS64   ? 64
		: (*elf_ptr)->data[EI_CLASS] == ELFCLASS32 ? 32
												   : 0;
	___die2((*elf_ptr)->bit_class == 32, "Not accepting 32bit files");
	if ((*elf_ptr)->bit_class == 64)
		return 1;
	return 0;
}

void get_files_list(char *directory_path, char *file_array[MAX_FILES], int *files_count)
{
	struct dirent *entry;
	DIR *dir = opendir(directory_path);
	___die(dir == NULL, "Could not open directory");
	char full_path[100];

	while ((entry = readdir(dir)) != NULL && (*files_count) < MAX_FILES)
	{
		if (entry->d_type == DT_REG)
		{
			file_array[(*files_count)] = malloc(MAX_FILENAME);
			if (file_array[(*files_count)] != NULL)
			{
				strcpy(full_path, directory_path);
				strcat(full_path, "/");
				strcat(full_path, entry->d_name);
				ft_memcpy(file_array[(*files_count)], full_path, MAX_FILENAME);
				file_array[(*files_count)][MAX_FILENAME - 1] = '\0';
				(*files_count)++;
			}
		}
	}
	closedir(dir);
}

void inject(const char *woody)
{
	printf("%s", woody);
}

int main(void)
{
	char *file_array[MAX_FILES];
	char *directory_path = "./tmp";
	int files_count = 0;
	get_files_list(directory_path, file_array, &files_count);
	for (int i = 0; i < files_count; i++)
	{
		printf("%s \n", file_array[i]);
		if(elf_init(file_array[i], &elf) == 1)
			printf("infect\n\n");
		else
			printf("not infect\n\n");
		free(file_array[i]);
	}
	//	inject("woody");
	return free_all();
}
