//#include "lib/libcsv.c"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include "lib/csv.c"
#include "lib/hashmap.c"

void load_table(char* table_name, hashmap* dbs);
void load_db(char* db_name, hashmap* dbs);
void console_loop();
int iterate_folder(char*, void (*f)(char*, hashmap*), hashmap*);

int main (void) {
	//load all databases into memory
	hashmap *dbs = mk_hmap(str_hash_fn, str_eq_fn, str_del_fn);
	iterate_folder("databases/", load_db, dbs);
	printf("Test:%s\n", (char*)hmap_get(dbs, "test"));
	printf("Array:\n", hmap_get(hmap_get(dbs, "db1"), "test"));
	//execute the console thing, where user can enter commands (while command != exit listen for another command)
	console_loop();	
	free_hmap(dbs);
	printf("Bye !\n");
	return 1;
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
			//TODO: parse the command line and do the sorting
			//TODO: print the result
			printf("	Column1		Column2		Column3\n");
			printf("0	Row1		Row2		Row3\n");
		}else{
			loop_exit = 1;
		}
	}
}

void load_table(char* table_name, hashmap *dbs){
	//TODO: load csv file into memory
	FILE *file = fopen ( table_name, "r" );
	//get db name
	char *db_name = strtok(table_name, "/");
	db_name = strtok(NULL, "/");
	//get table name
	char *short_table_name = strtok(NULL, "/");
	printf("        -%s\n", short_table_name);
	hmap_add(hmap_get(dbs, db_name), short_table_name, mk_hmap(str_hash_fn, str_eq_fn, str_del_fn));
	hmap_add(hmap_get(dbs, db_name), "test", "toto");
	//printf("%s\n", hmap_get(hmap_get(dbs, db_name), "test"));
	if ( file != NULL )
	{
		char line [512]; /* or other suitable maximum line size */
		FIELDS* columns;
		int firstLine = 1;
		//printf("Adding fields map in %s map in %s map in dbs\n", short_table_name, db_name);
		hmap_add(hmap_get(hmap_get(dbs, db_name), short_table_name), "fields", mk_hmap(str_hash_fn, str_eq_fn, str_del_fn));
		int index = 0;
		while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
		{
			if(!firstLine){
				FIELDS* pfields = CsvToFields(line);
				hmap_add(hmap_get(hmap_get(hmap_get(dbs, db_name), short_table_name), "fields"), index, pfields);
				index++;
			}else{
				columns = CsvToFields(line);
				hmap_add(hmap_get(hmap_get(dbs, db_name), short_table_name), "columns", columns);
				firstLine = 0;
			}
		}
		hmap_add(hmap_get(hmap_get(dbs, db_name), short_table_name), "fieldcount", index);
		//printf("Fields count:%i\n", hmap_get(hmap_get(hmap_get(dbs, db_name), short_table_name), "fieldcount"));
		fclose ( file );
	}
	else
	{
		perror ( table_name ); /* why didn't the file open? */
	}
}

void load_db(char* db_name, hashmap* dbs){
	printf("Loading %s\n", db_name);
	strcat(db_name, "/");
	char splited[20];
	strcpy(splited, db_name);
	strtok(splited, "/");
	//printf("DB_TOK: %s\n", strtok(NULL, "/"));
	hmap_add(dbs, strtok(NULL, "/"), mk_hmap(str_hash_fn, str_eq_fn, str_del_fn));	
	iterate_folder(db_name, load_table, dbs);
}

int iterate_folder(char* folder_name, void (*f)(char*, hashmap*), hashmap* dbs){
	DIR* dirp = opendir(folder_name);
	int errno;
	struct dirent* dp;
	int db_count = 0;

	while (dirp) {
		errno = 0;
		if ((dp = readdir(dirp)) != NULL && (strcmp(dp->d_name, "..") != 0) && (strcmp(dp->d_name, ".svn") != 0) && (strcmp(dp->d_name, ".") != 0)) {
			db_count++;
			char path[50] = "";
			strcpy(path, folder_name);
			strcat(path, dp->d_name);
			(*f)(path, dbs);
		} else {
			if(dp == NULL){
				closedir(dirp);
				return -1;
			}
		}
	}
	printf("\n");
	return db_count;
}
