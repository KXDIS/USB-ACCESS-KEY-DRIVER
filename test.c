#include <stdio.h>
#include <sys/stat.h>

#define TEST_FILE "/home/kali/Documents/Locked_File"

int main(void){

    //initialize struct to hold info
    struct stat file_info;
    
    //set TEST_FILE to
    stat(TEST_FILE, &file_info);
        
    mode_t default_permission = file_info.st_mode;

    chmod(TEST_FILE, 0775);
    stat(TEST_FILE, &file_info);

    printf("FP unlocked: %o", file_info.st_mode);

    
    chmod(TEST_FILE, default_permission);
    stat(TEST_FILE, &file_info);
    printf("FP default: %o", file_info.st_mode);

    return 0;

}