savedcmd_usbkey.mod := printf '%s\n'   usbkey.o | awk '!x[$$0]++ { print("./"$$0) }' > usbkey.mod
