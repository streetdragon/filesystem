/* 
 * Name: Kurt McAlpine
 * UPI: kmca733
 * ID: 2004750
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fileutils.h"

#define A2DIR "A2dir"

#define FILELEN 256

#define PWD "pwd"
#define CD "cd"
#define LS "ls"
#define RLS "rls"
#define TREE "tree"
#define CLEAR "clear"
#define CREATE "create"
#define ADD "add"
#define CAT "cat"
#define DELETE "delete"
#define DD "dd"
#define QUIT "quit"

static char *fake_cwd;

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

static char *get_fake_cwd()
{
	if (fake_cwd == NULL) {
		fake_cwd = malloc(FILELEN * sizeof(char));
		fake_cwd[0] = '\0';
		strcpy(fake_cwd, "-");
	}
	return fake_cwd;
}

void execute_cmd(char *cmd) {
	if (cmd != NULL && strlen(cmd) > 0) {
		cmd[strlen(cmd) - 1] = '\0';
	} else {
		return;
	}
	if (strncmp(CREATE, cmd, strlen(CREATE)) == 0
			&& cmd[strlen(CREATE)] == ' ') {
		/* Remove the command from the front of the string */
		char *filename = cmd + strlen(CREATE) + 1;
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
}

int main(void)
{
	/* Initially check if the "A2dir" directory exists and if it doesn't
	 * create it */
	char str[BUFSIZ];
	if (isatty(fileno(stdin))) {
		printf("\nffs> ");
		while (fgets(str, BUFSIZ, stdin) != NULL) {
			//printf("You entered: %s", str);
			execute_cmd(str);
			printf("ffs> ");
		}
	} else {
		printf("\n");
		while (fgets(str, BUFSIZ, stdin) != NULL) {
			printf("ffs> %s", str);
			execute_cmd(str);
		}
	}
	return 0;
}
