// initial method:
//      inspired by the code listed in user/ls.c,I need to write the code that
//      access the filesystem recurslvely for the T_FILE,if its name equals to
//      the filename given in arguments,print it out ,if not, continue the loop.
//      for the T_DIR,if it is not empty,call the find function recurslvely.
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
char* get_filename(char* path) {
    static char buf[DIRSIZ + 1];
    char* p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), 0, DIRSIZ - strlen(p));
    return buf;
}
void find(char* path, char* filename) {
    //printf("%s %s",path,filename);
    char buffer[100], *tail;
    memset(buffer, 0, sizeof(buffer));
    int fd;
    struct stat file_status;
    struct dirent de;
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if (fstat(fd, &file_status) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        return;
    }
    switch (file_status.type) {
        case T_FILE:
            // if the filename equals to the filename given in arguments
            if (!strcmp(filename, get_filename(path))) {
                printf("%s\n", path);
                close(fd);
            }
            return;
        case T_DIR:
            // TODO: added the case if the directory's name equals to the
            // argument
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buffer)) {
                printf("find: path too long");
                close(fd);
                return;
            }
            strcpy(buffer, path);
            tail = buffer + strlen(path);
            *tail++ = '/';
            // read the file contained in the directory
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0)
                    continue;
                // the myth of how ls list all the file in this directory is
                // here:
                //  current_directory/file
                //  |                 |
                //  |                 |
                // buf                p
                // p receives the file name and sends it to p
                // fmtname reads the '/file' path (you can see the definition of
                // fmt above) and print it to the console
                if(!strcmp(de.name,".")  || !strcmp(de.name,".."))
                    continue;
                memmove(tail, de.name, DIRSIZ);
                tail[DIRSIZ] = 0;
                if (stat(buffer, &file_status) < 0) {
                    printf("ls: cannot stat %s\n", buffer);
                    continue;
                }
                // switch the file type ,if it is a dir , resursive.
                switch (file_status.type) {
                    case T_FILE:
                        
                        if (!strcmp(filename, get_filename(buffer))) {
                            printf("%s\n", buffer);
                        }
                        break;
                    case T_DIR:
                        find(buffer, filename);
                        break;
                }
            }
            close(fd);
    }
}
int main(int argc, char **argv){
    if(argc < 3){
        printf("USAGE:find <path> <filename>\n");
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}