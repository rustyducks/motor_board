#!/bin/bash
dfu-util -d 0483:df11 -c 1 -i 0 -a 0 -s 0x08000000:leave -D build/ch.bin
