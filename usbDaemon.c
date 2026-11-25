#include <stdio.h>
#include <unistd.h>
#include <string.h>

//Library to alter file permissions
#include <sys/stat.h>


/* Create a file in the kernel that the module will read. This file returns (Y) if the key is in, (N) if the key is not. 
*/
#define KEY_STATE_FILE "/sys/module/usbkey/parameters/key_present"
#define CONFIG_FILE "filelist.conf"

int read_key_state (void){

    //First, read the file using fopen(xxx , "r").
    FILE *f = fopen(KEY_STATE_FILE, "r");

    //Return if there is no file for troubleshooting
    if (f== NULL){
        return -1;
    }

    //Read the first character of the file
    //Returns ASCII value of Y or N
    //Since fget returns an 
    int c = fgetc(f);
    
    fclose(f);

    return c == 'Y';

}


//UNLOCK FILE METHOD
void unlock_file(const char *file){
    //open and read the file in the CONFIG_FILE variable
    FILE *config = fopen(CONFIG_FILE, "r");
    
    //if can't open file print error
    if (config == NULL) {
        perror("FILE: Error opening file\n");
        return;
    }



    //create line variable (array of chars)
    char line [256];
    
    //walk through the file one line at a time
    while (fgets(line, sizeof(line), config) != NULL) {
        
        // clean line input
        if (strchr(line, '\n') == NULL && !feof(config)) {
            fprintf(stderr, "FILE: Path too long, skipping line\n");
            int ch;
            while ((ch = fgetc(config)) != '\n' && ch != EOF); // Flush rest of line
            continue;
        }

        // strip trailing newline ('\n') and return ('\r')
        line[strcspn(line, "\r\n")] = '\0';

        // skip empty lines or comment lines starting with '#'
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }
        
        

        //lock the file path at the current line
        if (chmod(line, 0775) == 0) {
            printf("FILE: file unlock success at %s\n", line);
        } else {
            printf("FILE: file unlock failed at %s\n", line);
        }

    }

    //close the file
    fclose(config);



}

//LOCK FILE METHOD
void lock_file(const char *file, mode_t mode){
    //open and read the file in the CONFIG_FILE variable
    FILE *config = fopen(CONFIG_FILE, "r");
    
    //if can't open file print error
    if (config == NULL) {
        perror("FILE: Error opening file\n");
        return;
    }



    //create line variable (array of chars)
    char line [256];
    
    //walk through the file one line at a time
    while (fgets(line, sizeof(line), config) != NULL) {
        
        // clean line input
        if (strchr(line, '\n') == NULL && !feof(config)) {
            fprintf(stderr, "FILE: Path too long, skipping line\n");
            int ch;
            while ((ch = fgetc(config)) != '\n' && ch != EOF); // Flush rest of line
            continue;
        }

        // strip trailing newline ('\n') and return ('\r')
        line[strcspn(line, "\r\n")] = '\0';

        // skip empty lines or comment lines starting with '#'
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }
        
        

        //lock the file path at the current line
        if (chmod(line, mode) == 0) {
            printf("FILE: file lock success at %s\n", line);
        } else {
            printf("FILE: file lock failed at %s\n", line);
        }

    }

    //close the file
    fclose(config);


}

int main (void){

    // TEMPORARY - lock when run - for testing
    lock_file(CONFIG_FILE, 0000);

    // create stat struct
    struct stat file_info;
    // load "TEST_FILE" file details into struct
    stat(CONFIG_FILE, &file_info);
    // set default_permission to current file permission
    //mode_t default_permission = file_info.st_mode;

    int last_state = -2;

    //set loop to constantly read the state of what USB is currently inserted

    //kill with ctrl+c
    while (1){

        int state =  read_key_state();
        //This will only run if the state changes within the file dumbass write it differently

        if(state != last_state){ 
            if (state == -1){
                printf("Daemon: kernel module is not loaded\n");
                
            }else if(state == 1){
                printf("Daemon: VALID KEY inserted\n");
                
            }else {
                printf("Daemon: KEY NOT INSERTED\n");
            }


            //Separated lock and unlock function from valid state
            if (state == 1){
                unlock_file(CONFIG_FILE);
                printf("Daemon: FILE UNLOCKED\n");
            }else{
                lock_file(CONFIG_FILE,0000);
                printf("Daemon: FILE LOCKED\n");
            }
            fflush(stdout);
            //I did

            //remember state for next loop iteration.
        last_state = state;
        }

        //wait another second to check again
        sleep(1);
    }


    return 0;

}

