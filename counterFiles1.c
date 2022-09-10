#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>

void* countFiles(void* argc)
{
    DIR* d = (DIR*)argc;
    struct dirent* dir;
    int* counter = calloc(2,sizeof(int));
    while((dir=readdir(d))!=NULL){
        if(dir->d_type == DT_DIR && (strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0))    
        {
             counter[0]++;   
        }
        else if(dir->d_type == DT_REG)
        {
             counter[1]++;   
        }    
    }
    rewinddir(d);
    return counter;
}

void* printPaths(void* argc)
{
    DIR* d = (DIR*)argc;
    struct dirent* dir;
    while((dir=readdir(d))!=NULL){
        if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0)
        {
            if(dir->d_type == DT_DIR)    
            {
                printf("Directory: "); 
            }
            else if(dir->d_type == DT_REG)
            {
                printf("File: ");
            } 
            printf("%s\n",dir->d_name);   
        }
    }
}

int main(int argc, char** argv)
{
    if(argc < 2){
        fprintf(stderr,"Too few arguments\n");
        exit(EXIT_FAILURE);
    }
    
    int* files_directories = malloc(2*sizeof(int));
    pthread_t th[2];
    DIR* d = opendir(argv[1]);

    if (pthread_create(&th[0], NULL, &countFiles, d) != 0) {
        fprintf(stderr,"Cannot create thread\n");
        exit(EXIT_FAILURE);
    }
    
    if (pthread_create(&th[1], NULL, &printPaths, d) != 0) {
        fprintf(stderr,"Cannot create thread\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(th[0], (void**)&files_directories) != 0) {
        fprintf(stderr,"Cannot create thread\n");
        exit(EXIT_FAILURE);
    }
    
    if (pthread_join(th[1], NULL) != 0) {
        fprintf(stderr,"Cannot create thread\n");
        exit(EXIT_FAILURE);
    }

    printf("\n\nNumber of directories in %s directory: %d\n",argv[1],files_directories[0]);
    printf("Number of files in %s directory: %d\n",argv[1],files_directories[1]);
    
    return 0;
}
