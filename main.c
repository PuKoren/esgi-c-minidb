#include "lib/libcsv.c"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

void load_table(char* table_name);
void load_db(char* db_name);
int load_db_list();
void console_loop();

int main (void) {
	//load all databases into memory
	iterate_folder("databases/", load_db);
	//load_db_list();

	//execute the console thing, where user can enter commands (while command != exit listen for another command)
	console_loop();

	printf("Bye !\n");
}

void console_loop(){
	char user_input[255] = "";
	int loop_exit = 0;
	while(!loop_exit){
		printf(">");
		//get command line
		fgets(user_input, sizeof user_input, stdin);
		char *p = strchr(user_input, '\n');
		//remove \n
		if(p){
			*p = 0;
		}
		if(strcmp(user_input, "quit") != 0 && strcmp(user_input, "exit") != 0){
			//parse the command line and do the sorting
			//print the result
			printf("	Column1		Column2		Column3\n");
			printf("0	Row1		Row2		Row3\n");
		}else{
			loop_exit = 1;
		}
	}
}

void load_table(char* table_name){
	printf("	-%s\n", table_name);
}

void load_db(char* db_name){
	printf("Loading %s...\n", db_name);
	char path[50] = "databases/";
	strcat(path, db_name);
	iterate_folder(path, load_table);
}

int iterate_folder(char* folder_name, void (*f(char*))){
	DIR* dirp = opendir(folder_name);
	int errno;
	struct dirent* dp;
	int db_count = 0;

	while (dirp) {
		errno = 0;
		if ((dp = readdir(dirp)) != NULL && (strcmp(dp->d_name, ".svn") != 0) && (strcmp(dp->d_name, ".") != 0)) {
			db_count++;
			char path[50] = "";
			strcat(path, dp->d_name);
			(*f)(dp->d_name);
		} else {
			if (errno == 0) {
				closedir(dirp);
				return 0;
			}
			closedir(dirp);
			return -1;
		}
	}
	printf("\n");
	return db_count;
}

int load_db_list(){
	DIR* dirp = opendir("databases/");
	int errno;
	struct dirent* dp;
	int db_count = 0;

	while (dirp) {
		errno = 0;
		if ((dp = readdir(dirp)) != NULL && (strcmp(dp->d_name, ".svn") != 0) && (strcmp(dp->d_name, ".") != 0)) {
			db_count++;
			load_db(dp->d_name);
		} else {
			if (errno == 0) {
				closedir(dirp);
				return 0;
			}
			closedir(dirp);
			return -1;
		}
	}
	printf("\n");
	return db_count;
}
