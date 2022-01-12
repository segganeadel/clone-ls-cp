#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>

void affiche(char* fich);

int main(int argc, char *argv[]) {
    struct stat StatusBuffer;
    struct dirent *entry;
    DIR* dir;
    extern int optind, opterr, optopt;
    char c;
    int folder=0;
    int file=0;


    while ((c = getopt (argc, argv, "rf")) != -1){
        switch (c)
        {
        case 'r':
            folder = 1;
            break;
        case 'f':
            file = 1;
            break;
        default:
            printf("Usage: %s [Options] Path\n",argv[0]);
            exit(1);
        break;
        }
    }



    switch (optind) {
        case 1:
            if (argc==2){
                if ((dir = opendir(argv[1])) == NULL){
                    perror(argv[1]);
                    exit(1);
                }
            }
            else dir = opendir(".");
            //block
            while ((entry = readdir(dir)) != NULL){
                affiche(entry->d_name);
            }   
        break;
        case 2:
            if (argc==3){
                if ((dir = opendir(argv[optind])) == NULL){
                    perror(argv[1]);
                    exit(1);
                }
            }
            else dir = opendir(".");
            if (file){
                //block
                while ((entry = readdir(dir)) != NULL){
                    if (DTTOIF(entry->d_type)==S_IFREG) affiche(entry->d_name);
                }  
            }else if (folder) {
                //blcok
                while ((entry = readdir(dir)) != NULL){
                    if (DTTOIF(entry->d_type)==S_IFDIR) affiche(entry->d_name);
                } 
            }           
        break;
        default:
            printf("Usage: %s [Options] Path\n",argv[0]);
            exit(1);
    
        break;
    }



	
    return EXIT_SUCCESS;
}

void affiche(char* fich){
    struct stat StatusBuffer;
    if (stat(fich, &StatusBuffer) == -1) {
        perror(fich);
        exit(1);
    }
    
   switch (StatusBuffer.st_mode & S_IFMT) {
    case S_IFREG:  printf("-");     break;
    case S_IFDIR:  printf("d");     break;
    case S_IFLNK:  printf("l");     break;
    case S_IFBLK:  printf("b");     break;
    case S_IFCHR:  printf("c");     break;
    case S_IFSOCK: printf("s");     break;
    case S_IFIFO:  printf("p");     break;
    default:  break;
    }

    printf ("%c", StatusBuffer.st_mode & S_IRUSR ? 'r' : '-');
    printf ("%c", StatusBuffer.st_mode & S_IWUSR ? 'w' : '-');
    printf ("%c", StatusBuffer.st_mode & S_IXUSR ? 'x' : '-');

    printf ("%c", StatusBuffer.st_mode & S_IRGRP ? 'r' : '-');
    printf ("%c", StatusBuffer.st_mode & S_IWGRP ? 'w' : '-');
    printf ("%c", StatusBuffer.st_mode & S_IXGRP ? 'x' : '-');

    printf ("%c", StatusBuffer.st_mode & S_IROTH ? 'r' : '-');
    printf ("%c", StatusBuffer.st_mode & S_IWOTH ? 'w' : '-');
    printf ("%c", StatusBuffer.st_mode & S_IXOTH ? 'x' : '-');

    printf(" %5ld", (long) StatusBuffer.st_nlink);

    printf(" %s",getpwuid(StatusBuffer.st_uid)->pw_name);

    printf(" %s",getpwuid(StatusBuffer.st_gid)->pw_name);

    printf(" %5ld",StatusBuffer.st_size);

    char temps[20];
    strftime(temps, sizeof(temps),"%b %e %R", localtime( &StatusBuffer.st_mtime ));
    printf(" %s",temps);

    printf(" %s\n",fich);
}