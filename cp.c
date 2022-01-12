#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    char buffer[1024];
    int fich[2];
    ssize_t count;
    struct stat statfich1;
    struct stat statfich2;


    if (argc < 3){
    printf("Usage: %s <Source file path> <Destination file path>\n", argv[0]);
    exit(1);
    }
    
    fich[0] = open(argv[1], O_RDONLY);
    if (fich[0] == -1){
	perror(argv[1]);
	exit(1);}

    fstat(fich[0], &statfich1);    


    fich[1] = open(argv[2], O_WRONLY | O_CREAT | S_IRUSR | S_IWUSR, statfich1.st_mode);
    if (fich[1] == -1){
    perror(argv[2]);
    exit(1);}


    while ((count = read(fich[0], buffer, sizeof(buffer))) != 0){
        write(fich[1], buffer, count);
    }


    return 0;
}