#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "headers/reuse_func.h"
void checkIfReciveIsSame(char *sender_file_path ,char *reicver_file_path){
    FILE *f_1 = fopen(sender_file_path , "r");
    FILE *f_2 = fopen(reicver_file_path , "r");
    if(f_1 == NULL ){
        printf("send file not found\n");
        return;
    }
    if(f_2 == NULL){
        printf("reciver file not found\n");
        return ;
    }
    char data_1[1024];
    char data_2[1024];
    int line_1 = 1;
    int line_2 = 1;
    long file_size_1 = getFile_size(f_1);
    long file_size_2  = getFile_size(f_2);
    assert(file_size_1 == file_size_2);
    printf("test-case 1 (same file size ): pass\n");

    while(line_1>0 && line_2>0){
        line_1  = fread(data_1 ,sizeof(char) , sizeof(data_1) , f_1 );
        line_2 = fread(data_2 , sizeof(char) , sizeof(data_2) , f_2);

        assert(strcmp(data_1 , data_2) );
        assert(line_1 == line_2 );
    }
    printf("test-case 2 (content in file is same ): pass\n");
}

int main(){
    char send_file[1024];
    char recived_file[1024];
    printf("---> Send file path : ");
    fgets(send_file ,1024 ,stdin );
    printf("-->reciver file paht : ");
    fgets(recived_file ,1024 ,stdin );
    send_file[strlen(send_file)-1]= '\0';
    recived_file[strlen(recived_file)-1] = '\0';
    printf("%ldd %ld\n" , strlen(send_file) , strlen(recived_file));
    checkIfReciveIsSame(send_file ,recived_file);
}