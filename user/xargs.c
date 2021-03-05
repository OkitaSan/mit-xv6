#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"
/// split a string by line
/// @param dst the destination for splitee
/// @param input the input string
/// @param start_idx the starting index of the input
/// @returns the index of the first character of the next line,and -1 if it meets the end of the character
int split_by_line(char* dst,const char* input,uint start_idx){
    // the sequence is a string which gets to the first character of the line
    const char* seq = input + start_idx;
    uint len = 0;
    // if the input is neither 0 or EOF,then we go into the loop
    while(*seq != '\0' && *seq != '\n'){
        *dst++ = *seq++;
        start_idx++;
        len++;
    }
    if(len != 0 && *seq == '\n'){
        //dst[0] = '\n';
        start_idx++;
    }
    return *seq == '\0' ? -1 : start_idx;
}
/**
 * function xargs
 * @param programm command xargs nees to execute
 * @param input the argument comes along with the programm
 * 
 * 
 * 
 * 
 */
void xargs(char* programm,char* input){
    // the buffer stores the input from stdin
    // lines stands for the command got from each line

    char buffer[512],lines[20];
    // the index 
    uint idx = 0;
    read(0,buffer,200);
    //TODELETE
    //printf("%s\n",buffer);
    while((idx = split_by_line(lines,buffer,idx)) != -1){
        // printf("%s",buffer);
        // create chile process
        if(fork() == 0){
            //printf("%s %s",programm,input);
            char* arguments[MAXARG] = {programm,input,lines};
            // TODELETE
            // printf("index:%d,programm:%s,input:%s,lines:%s\n",idx,programm,input,lines); 
            exec(programm,arguments);
        }
        wait((int*)0);
        memset(lines,0,20);
    }
    return;
}
int main(int argc, char **argv){
    if(argc < 3){
        printf("Usage: xargs <command> <argument>");
        exit(0);
    }
    xargs(argv[1],argv[2]);
    exit(0);
}
int split_by_line_test(){
    char buffer[100];memset(buffer,0,100);
    char * test = "The\n quick\n brown\n fox\n jumps\n over\n the\n lazy dog\n";
    uint idx = 0;
    while ((idx = split_by_line(buffer,test,idx)) != -1)
    {
        printf("%d\n",idx);
        printf("%s",buffer);
        memset(buffer,0,100);
    }
    exit(0);
}