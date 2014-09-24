#include "commands.h"
#include "fileutils.h"
#include "stringutils.h"

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h> /* for struct dirnet */
#include <string.h>

#define A2DIR "A2dir"
#define FILELEN 256

/* Helper methods */
static char *fake_cwd;
static char *get_fake_cwd()
{
	if (fake_cwd == NULL) {
		fake_cwd = malloc(FILELEN * sizeof(char));
		fake_cwd[0] = '\0';
		strcpy(fake_cwd, "-");
	}
	return fake_cwd;
}
static void set_fake_cwd(char *wd)
{
	if (fake_cwd == NULL) {
		fake_cwd = malloc(FILELEN * sizeof(char));
		fake_cwd[0] = '\0';
	}
	strcpy(fake_cwd, wd);
}


char *get_real_root_dir() {
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));

	char *full_path;
	full_path = malloc(FILELEN * sizeof(char));
	full_path[0] = '\0';
	strcat(full_path, cwd);
	strcat(full_path, "/");
	strcat(full_path, A2DIR);

	return full_path;
}
int directory_exists(char *dir)
{
	int result = 0;
	struct dirent *next_file;

	DIR *real_dir;

	char *file_path;
	file_path = malloc(FILELEN * sizeof(char));

	real_dir = opendir(get_real_root_dir());

	while (next_file = readdir(real_dir)) {
		sprintf(file_path, "%s/%s", get_real_root_dir(),
				next_file->d_name);
		if (next_file->d_name[0] != '.')
			if (strncmp(dir, next_file->d_name, strlen(dir)) == 0 &&
					next_file->d_name[strlen(dir)] == '-')
				result = 1;
	}
	closedir(real_dir);
	return result;
}
/* Commands */

void pwd()
{
	printf("%s\n", get_fake_cwd());
}
void cd(char *args)
{
	//printf("cd called args = |%s|\n", args);
	if (args[0] == '\0') {
		set_fake_cwd("-");
	} else if (args[0] == '-') { /* Absolute */
		/* Need to check that directory actually exists first */
		if (directory_exists(args)) {
			set_fake_cwd(args);
		} else {
			printf("Directory does not exist\n");
		}
	} else { /* Relative */
		/* Convert to absolute and call recursively */
		char *abs_path = malloc(1024);
		strcpy(abs_path, get_fake_cwd());
		strcat(abs_path, args);
		//printf("abs_path = %s\n", abs_path);
		cd(abs_path);
	}
}
void ls() {
	char *cwd = get_fake_cwd();

	char *last_dir_printed = malloc(1024 * sizeof(char));
	last_dir_printed[0] = '\0';
	char *dir_to_print = malloc(1024 * sizeof(char));
	dir_to_print[0] = '\0';


	struct dir_info *inf;
	inf = create_dir_info(get_real_root_dir());
	char *filename = malloc(1024 * sizeof(char));
	while (filename = get_next_filename(inf)) {
		if (filename[0] != '.') {
			char *after_dir = strpbrk(&filename[strlen(cwd)], "-");
			if (after_dir != NULL) {
				strncpy(dir_to_print, &filename[strlen(cwd)], after_dir - &filename[strlen(cwd)]);
				if (!compare_strings(last_dir_printed, dir_to_print)) {
					printf("d: %s\n", dir_to_print);
					last_dir_printed[0] = '\0';
					strcpy(last_dir_printed, dir_to_print);
				}
			} else {
				printf("f: %s\n", &filename[strlen(cwd)]);
			}
		}
	}
	last_dir_printed[0] = '\0';
	
	//printf("Test new filenames function\n");
	char **filenames = get_all_filenames(get_real_root_dir());
	int i = 0;
	while (filenames[i]) {
		if (filenames[i][0] != '.') {
			char *after_dir = strpbrk(&filenames[i][strlen(cwd)], "-");
			if (after_dir != NULL) {
				strncpy(dir_to_print, &filenames[i][strlen(cwd)], after_dir - &filenames[i][strlen(cwd)]);
				//printf("dir_to_print = %s\n", dir_to_print);
				if (!compare_strings(last_dir_printed, dir_to_print)) {
					printf("d: %s\n", dir_to_print);
					last_dir_printed[0] = '\0';
					strcpy(last_dir_printed, dir_to_print);
				}
			} else {
				printf("f: %s\n", &filenames[i][strlen(cwd)]);
			}
		}


		i++;
	}

}
void rls()
{
	char *rls_cmd;
	rls_cmd = malloc(FILELEN * sizeof(char));
	rls_cmd[0] = '\0';
	strcpy(rls_cmd, "ls -l ");
	strcat(rls_cmd, get_real_root_dir());
	system(rls_cmd);
}
void tree()
{
}
void clear()
{
	struct dirent *next_file;

	DIR *real_dir;

	char *file_path;
	file_path = malloc(FILELEN * sizeof(char));

	real_dir = opendir(get_real_root_dir());

	while (next_file = readdir(real_dir)) {
		sprintf(file_path, "%s/%s", get_real_root_dir(),
				next_file->d_name);
		if (next_file->d_name[0] != '.')
			remove(file_path);
	}
	closedir(real_dir);
}
void create(char *filename)
{
	if (filename[0] == '-') { /* Absolute */
		char *full_name = malloc(256 * sizeof(char));
		full_name[0] = '\0';
		strcpy(full_name, get_real_root_dir());
		strcat(full_name, "/");
		strcat(full_name, filename);
		create_file(full_name);
	} else { /* Relative */
		char *full_name = malloc(256 * sizeof(char));
		full_name[0] = '\0';
		strcpy(full_name, get_real_root_dir());
		strcat(full_name, "/");
		strcat(full_name, get_fake_cwd());
		strcat(full_name, filename);
		create_file(full_name);
	}
}
void add()
{
}
void cat()
{
}
void delete()
{
}
void dd()
{
}
void quit()
{
	exit(0);
}
