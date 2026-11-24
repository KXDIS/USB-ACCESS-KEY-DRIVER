# CUSTOM USB ACCESS KEY DEVICE DRIVER

## How to run

### 1. Build the module and daemon
```
make
make daemon
```

### 2. Prepare the terminal
```
sudo rmmod usbkey
sudo dmesg -C
sudo -v
```
`rmmod` unloads any old copy (an error saying it's not loaded is fine). `dmesg -C` clears old log messages, and `sudo -v` enters your password ahead of time so the background command below doesn't get stuck asking for it.

### 3. Start the live output
```
sudo dmesg -w | grep --line-buffered usbkey &
./usbkeyd &
```

### 4. Load the module with your key's values
```
sudo insmod usbkey.ko vid=0x13fe pid=0x6300 serial="07192164148CE881"
```
Now insert and remove the USB key. The kernel messages and the daemon's output appear in the terminal as it happens.

### 5. Stop
```
sudo rmmod usbkey
kill %1 %2
```
