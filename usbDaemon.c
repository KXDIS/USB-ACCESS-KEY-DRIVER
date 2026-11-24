#include <stdio.h>
#include <unistd.h>

/* Create a file in the kernel that the module will read. This file returns (Y) if the key is in, (N) if the key is not. 
*/
#define KEY_STATE_FILE "/sys/module/usbkey/parameters/key_present"

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


int main (void){


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
            printf("Daemon: KEY NOT INSERTED inserted\n");
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

