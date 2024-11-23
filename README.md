# Kernel-Anticheat - Check your detection vectors
https://www.unknowncheats.me/forum/anti-cheat-bypass/569165-kernel-anticheat-check-detection-vectors.html

## Project Overview
This project is a kernel driver made for Windows that will scan your pc for cheat behaviour

## Features
- **Drivers**: Checks for invalid/unsigned drivers.
- **Handles**: Checks for usermode physical memory handles.
- **Hypervisor**: Simple hypervisor detection using vm_read.
- **Memory**: Checks bigpool for some traces.
- **Serial**: Check your own serials.
- **Traces**: Will search for Kdmapper traces.
- **NMI**: Scans cores for suspicious activity by firing NMI.
- **Threads**: Checks systemthreads for invalid ones.


## How to Run
1. Build the project:
```
Using Visual Studio 2022 or higher
```
2. Enable Loading of Test Signed Drivers
```
Bcdedit.exe -set TESTSIGNING ON
```
3. Restart local machine
```
shutdown /r
```
4. Setup the driver
```
sc create anticheat_drv type=kernel binpath={PATH_TO_BIN}
```
4. Load the driver
```
sc start anticheat_drv
```

## License
This project is licensed under the MIT License. See the LICENSE file for details.
