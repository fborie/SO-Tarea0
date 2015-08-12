#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define R_FLAG "-r"

bool isThereFileFromPath(const char *path){
    /*FILE *fp;
    fp = fopen(path,"r");   
    fclose(fp); */
    if(access(path,R_OK) != -1){ //syscall checking for read permission
        return true;
    }
    return false;
}

int main(int argc, char *argv[]){
    if(argc == 1){
        fprintf(stdout,"No arguments have been provided! Exiting ...\n");
        return false;
    }
    else{
        if(strcmp(argv[1],R_FLAG) == 0){
            if(argc > 2){
                int i;
                for(i = 2; i<argc;i++){
                    if(isThereFileFromPath(argv[i])){
                        fprintf(stdout,"FILE FOUND!");
                    }
                    else{
                        fprintf(stdout,"FILE NOT FOUND OR NO READ PERMS! Exiting ...");
                    }
                }
            }
            else{
                printf("No file paths provided! Exiting ...\n");
                return 0;
            }         
        }
        else{

        }
        
    }
    return 0;
}
