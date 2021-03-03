#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
/// split a string by line
/// @param dst the destination for splitee
/// @param input the input string
/// @param start_idx the starting index of the input
/// @returns the index of the first character of the next line,and -1 if it meets the end of the character
uint split_by_line(char* dst,const char* input,uint start_idx){
    // the sequence is a string which gets to the first character of the line
    const char* seq = input + start_idx;
    // if the input is neither 0 or EOF,then we go into the loop
    while(*seq != '\0' && *seq != '\n'){
        *dst++ = *seq++;
        start_idx++;
    }
    
    return *seq == '\0' ? -1 : start_idx;
}

int main(){
    char buffer[100] = {1};
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