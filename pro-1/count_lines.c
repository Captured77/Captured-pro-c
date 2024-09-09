#include <stdio.h>
#include <dirent.h>
#define MAX 256
#include <sys/stat.h>

#include <unistd.h>

#include <string.h>

long total;

void findAllDirs(const char *path);
int isCode(const char *filename);

void findAllDirs(const char *path) {
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    int countLines(const char *filename);  // Function prototype added

    if ((dp = opendir(path)) == NULL) {
        fprintf(stderr, "The path %s is wrong!\n", path);
        return;
    }

    chdir(path);

    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name, &statbuf);

        if(!strcmp(".", entry->d_name) || !strcmp("..", entry->d_name)) {
            continue;
        }

        if(S_ISDIR(statbuf.st_mode)) {
            findAllDirs(entry->d_name);
        } else {
            if (isCode(entry->d_name)) {
                total += countLines(entry->d_name);
            }
        }
    }

    chdir("..");
    closedir(dp);
}

int isCode(const char *filename) {
    int length;
    length = strlen(filename);

    if(!strcmp(filename + (length - 2), ".c")) {
        return 1;
    } else {
        return 0;
    }
}

int countLines(const char *filename) {
    FILE *fp;
    int count = 0;
    int temp;

    if((fp = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "The file %s is wrong!\n", filename);
        return 0;
    }

    while((temp = fgetc(fp)) != EOF ) {
        if(temp == '\n') {
            count++;
        }
    }
    fclose(fp);
    return count;
}

int main() {
    char path[MAX] = ".";

    printf("计算中...\n");
    findAllDirs(path);

    printf("共有 %ld 行代码\n", total);

    // printf("Hello World:\n");

    return 0;
}
