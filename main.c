#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#define R_FLAG "-r"

bool is_there_file_from_path(const char *path){
    if(access(path,R_OK) != -1){ //syscall checking for read permission
        return true;
    }
    return false;
}

int compare_func_straight(const void * line1, const void * line2){
    const char * a = *(const char **)line1;
    const char * b = *(const char **)line2;
    return strcmp(a,b);
}

int compare_func_reverse(const void *line1, const void * line2){
    const char * a = *(const char **)line1;
    const char * b = *(const char **)line2;
    int resp = strcmp(a,b);
    if(resp < 0 )
        return 1;
    else if(resp > 0)
        return -1;
    else{
        return 0;
    }
}

int count_reverse_flags(int argc,char * argv[]){ //return the number of r flags
    int r_count =0;
    int i; 
    for(i=1;i<argc;i++){
        if(strcmp(argv[i],R_FLAG) == 0){
            r_count++;
        }
    }
    return r_count;    
}

char ** extract_paths_from_argv(int argc, int r_count, char* argv[]){
    char **paths = malloc(sizeof(char**)*(argc-r_count-1));
    size_t num_paths = 0;
    int i;
    for(i=1;i<argc;i++){
        if(strcmp(argv[i],R_FLAG) != 0){
            paths[num_paths] = argv[i];
            num_paths++;
        }
    }
    return paths;    
}


int main(int argc, char *argv[]){
    if(argc == 1){
        fprintf(stderr,"No arguments have been provided! Exiting ...\n");
        return 1;
    }
    else{
        int r_flag_count = count_reverse_flags(argc,argv);
        if(r_flag_count == 0){ // without reverse flag
            if(argc > 1){//if there are some arguments
                size_t max_lines = 20;
                char ** lines = malloc(max_lines*sizeof(char*)); 
                size_t lines_counter = 0;
                int i;
                for(i = 1; i<argc;i++){
                    if(is_there_file_from_path(argv[i])){
                        
                        FILE * file;
                        file = fopen(argv[i],"r");
                        if(file == NULL){
                            fprintf(stderr,"COULDN'T OPEN %s FILE! Exiting ...\n",argv[i]);
                            return 0;
                        }
                         
                        size_t current_max = 70;
                        size_t old_max;
                        char * buffer = malloc(sizeof(char)*current_max); //buffer to store thee current line of the reading file
                        
                        while(fgets(buffer,current_max,file) != NULL && feof(file) == 0 ){
                            char * tmp = malloc(sizeof(char)*current_max); //temp pointer for manteining the address of buffer
                            strcpy(tmp,buffer);
                            lines[lines_counter] = tmp;
                            lines_counter++;
                            if(buffer[current_max-2] != 0 && buffer[current_max-2] != '\n'){ //current_max-2 ya que si leo current_max-1 me va a retornar siempre el caracter nulo '\0',
                                                                                            //dado que fgets lee n-1 chars del stream y ademas escribe el nul char al final
                                old_max = current_max;
                                current_max *= 2;
                                char * tmp_buffer = realloc(buffer,sizeof(char)*current_max);
                                if(tmp_buffer == NULL){
                                    fprintf(stderr,"ERROR REALLOC BUFFER! Exiting ...");
                                    return 1;
                                }
                                buffer = tmp_buffer;
                                lines_counter--; //go back one line
                                fseek(file,-old_max+1,SEEK_CUR); // plus 1 for not get out of the bounds of the line
                            }
                            if(lines_counter == (max_lines-1)){ //in case of fullfil the array of pointers
                                max_lines *= 2;
                                char ** tmp_lines = realloc(lines,max_lines*sizeof(char*));
                                if(tmp_lines == NULL){
                                    fprintf(stderr,"ERROR REALLOC BUFFER! Exiting ...");
                                    return 1;
                                }
                                lines = tmp_lines;
                            }
                        }
                        fclose(file);
                    }
                    else{
                        fprintf(stderr,"%s NOT FOUND OR NO READ PERMS! Exiting ...\n",argv[i]);
                        return 1;
                    }
                }
                qsort(lines,lines_counter,sizeof(char*),compare_func_straight);
                for(i=0;i<lines_counter;i++){
                    printf("%s",lines[i]);
                    free(lines[i]);
                }
                free(lines);
            }
            else{ 
                fprintf(stderr,"NO PATHS PROVIDED! Exiting ...\n");
                return 1;
            }
        }         
        
        else{ //with reverse flag
            char ** paths = extract_paths_from_argv(argc, r_flag_count,argv);
            size_t max_lines = 20;
            char ** lines = malloc(max_lines*sizeof(char*)); 
            size_t lines_counter = 0;
            const size_t num_files = argc-r_flag_count - 1;
            int i;
            for(i = 0; i<num_files;i++){
                if(is_there_file_from_path(paths[i])){
                    FILE * file;
                    file = fopen(paths[i],"r");
                    if(file == NULL){
                        fprintf(stderr,"COULDN'T OPEN %s FILE! Exiting ...\n",paths[i]);
                        return 0;
                    }
                         
                     size_t current_max = 70;
                     size_t old_max;
                     char * buffer = malloc(sizeof(char)*current_max); //buffer to store thee current line of the reading file
                        
                     while(fgets(buffer,current_max,file) != NULL && feof(file) == 0 ){
                        char * tmp = malloc(sizeof(char)*current_max); //temp pointer for manteining the address of buffer
                        strcpy(tmp,buffer);
                        lines[lines_counter] = tmp;
                        lines_counter++;
                        if(buffer[current_max-2] != 0 && buffer[current_max-2] != '\n'){ //current_max-2 ya que si leo current_max-1 me va a retornar siempre el caracter nulo '\0',
                                                                                            //dado que fgets lee n-1 chars del stream y ademas escribe el nul char al final
                            old_max = current_max;
                            current_max *= 2;
                            buffer = realloc(buffer,sizeof(char)*current_max);
                            if(buffer == NULL){
                                fprintf(stderr,"ERROR REALLOC BUFFER! Exiting ...");
                                return 1;
                            }
                            lines_counter--; //go back one line
                            fseek(file,-old_max+1,SEEK_CUR); // plus 1 for not get out of the bounds of the line
                        }
                        if(lines_counter == (max_lines-1)){ //in case of fullfil the array of pointers
                            max_lines *= 2;
                            char ** tmp_lines = realloc(lines,max_lines*sizeof(char*));
                            if(tmp_lines == NULL){
                                fprintf(stderr,"ERROR REALLOC BUFFER! Exiting ...");
                                return 1;
                            }
                            lines = tmp_lines;
                        }
                    }
                    fclose(file);
                }
                else{
                    fprintf(stderr,"%s NOT FOUND OR NO READ PERMS! Exiting ...\n",paths[i]);
                    return 1;
                }
            }
            for(i=0;i<num_files;i++){
                free(paths[i]);
            }
            free(paths);

            qsort(lines,lines_counter,sizeof(char*),compare_func_reverse);
            for(i=0;i<lines_counter;i++){
                printf("%s",lines[i]);
                free(lines[i]);
            }
            free(lines);
        }
    return 0;
    }
}
