
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/usb.h>
#include <linux/notifier.h>

//macro for the USB device's vendor and product IDs
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hendrick");
MODULE_DESCRIPTION("A simple USB key kernel module");

//set variables to the vendor and product IDs of the USB device

static int vid;
static int pid;
static char *serial = "";
//unix permission 0444 that allows for read-only permission
//Makes 
module_param(vid,int,0444); 
module_param(pid, int,0444);
module_param(serial,charp,0444);

//conditional if the USB is inserted. This will be default set to false.
static bool key_present = false;


//udev refers to all of the info of the USB device
static bool isCorrectKey(struct usb_device *udev){
    

//le16 us a kernel level function that converts 16 bit unsigned little endian to 
//host processor byte order
  return le16_to_cpu(udev->descriptor.idVendor) == vid &&
           le16_to_cpu(udev->descriptor.idProduct) == pid &&
           udev->serial != NULL &&
           //strcmp() String comparison method that returns 0,1, -1

           strcmp(udev->serial,serial)==0;
}

//Detects when USB is inserted or removed
//If the inserted USB matches the credentials of the USB Access key ->key_present = true;
//Else, upon usb_event deteching removal, => key_present = false;


//action 
static int usb_event(struct notifier_block *nb, unsigned long action, void *data){

    struct usb_device *udev = data;

      if (action == USB_DEVICE_ADD){

        if (isCorrectKey(udev)){
          key_present = true;
          pr_info("usbkey: CORRECT KEY INSERTED\n");
          }else{
            pr_info("usbkey: WRONG USB, USB IGNORED");
          } 

      }else if (action == USB_DEVICE_REMOVE){
        if (isCorrectKey(udev)){
          key_present = false;
          pr_info("usbkey: USB REMOVED"); 
        }
      }
  return NOTIFY_OK;
}

static struct notifier_block usb_watcher ={
  .notifier_call = usb_event,
};


/*Runs on startup
  This method checks whether or not the USB device is already plugged in on startup. 
*/
static int check_already_plugged_in(struct usb_device *udev, void *data){
  if (isCorrectKey(udev)){
    key_present = true;
    pr_info("usbkey: USB KEY ALREADY PLUGGED IN");
    }

  return 0;
}

/*Runs on startup
  errors if vid, pid and serial are not initialized
*/
static int __init usbkey_start(void){

  if (vid == 0 || pid == 0 || serial[0] == '\0'){
    pr_err("usbkey : YOU MUST GIVE VID, PID AND SERIAL\n");
    return -EINVAL;
  }

    usb_register_notify(&usb_watcher);

    usb_for_each_dev(NULL,check_already_plugged_in);

    pr_info("usbkey: STARTED: WAITING FOR USB KEY INSERTION\n");

    return 0;
}

static void __exit usbkey_stop(void){

  usb_unregister_notify(&usb_watcher);

  pr_info("usbkey: STOPPED\n");
}

module_init(usbkey_start);

module_exit(usbkey_stop);


