# What is sspender?
sspender allows you to suspend your server when it is not being used, it also allows you to spin down any disks that are not active.

It supports the following conditions before suspending the server:
- User defined usage of the CPU.
- User defined usage of some, or all disks (useful to avoid interrupting scheduled backups and other tasks).
- Whether another server is online (useful if another machine requires your server to stay ON).

I also allows you to:
- Decide how the server should be suspended, standby (ACPI S1), to memory (ACPI S3) or to disk (ACPI S4).
- Define what usage is considered "idle" for both CPU and disks.
- Define a time when the server should weake up after it has been suspended (useful if you require your server to be ON at ceartain times).
- Define what drives you want to spin down if they are idle.
- Run some tasks when the server recovers from suspend (not yet implemented).
- Monitor network interfaces usage (not yet implemented).


# Running environment: 
This has been developed and used under Ubuntu, I have not tested it in any other systems.


# Dependencies:
### libconfig++-dev
libconfig is required during the build of sspender, as well as during execution in order to decode sspender.cfg.
``` bash
$ apt install libconfig++-dev
```

### rtcwake
rtcwake is required to suspend the machine, this is the default way.
``` bash
$ apt install rtcwake
```

### hdparm
hdparm is required to spin down the drives, only required if you have spindown option enabled in sspender.cfg.
``` bash
$ apt install hdparm
```

# Building from Source:
### Configuring with CMake
Compile and install sspender in /usr/local/bin
``` bash
$ cd sspender
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
```

# Usage:
Download the executable from the latest release or compile your own from the source code in the folder "src", update the config file sspender.cfg and run the executable.

As sspender relies on hdparm and rtcwake to spin down the drives and suspend the machine, sspender needs to be executed as root. You can run it as a normal user and it will still monitor all the devices, but it won't be able to spin down the drives or suspend the machine.

``` bash
$ sudo sspender sspender.cfg
```

To direct logs to /var/log/sspender.log
``` bash
$ sudo sspender sspender.cfg > /var/log/sspender.log 2>&1 &
```

# Limitations:
- As explained above, this has only been used under Ubuntu so far, it might fail to work as expected under different environments, if you find any issues with your set-up please open an issue and I will have a look. 
- I had some issues with pm-utils failing to suspend certain machines before and I have since switched to using rtcwake. The code to use pm-utils is still there and I plan on adding an option in sspender.cfg so that users can use both, in case one fails on their machines.
