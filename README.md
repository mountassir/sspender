# What is sspender?
A couple of years ago, I buit a NAS / media / backup server to play with, which I didn't want to be ON all the time as it didn't do anything that required a 24/7 up time.

Simply suspending the server at pre-defined times was not an option for me, simply because my usage of the server varied and I had other machines depending on it.

To be more specific, I wanted the server to suspend, but only if:
- Certain machines were not online, because they needed the server to be ON.
- Certain drives are idle, otherwise scheduled backups and tasks would have been interrupted.
- CPU is idle otherwise scheduled tasks would have been interrupted.

I also wanted:
- To be able to define what usage is considered "idle" for both CPU and disks.
- The server to resume from suspend at certain times during the day, because the server is required to be ON at those times.
- Certain drives to spin down if they are idle.
- Run some tasks when the server recovers from suspend (not yet implemented).
- Monitor network interfaces usage (not yet implemented).

I searched around for a solution but could not find a program that fulfilled all of my requirements, so I decided to write my own. 

I have been using sspender for a while now, deployed on few machines I got and it has served me very well. I am sharing this and the source code in case some else has similar requirements and needs.


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

### pm-utils
pm-utils is required to suspend the machine, only required if you choose pm-utils instead of rtcwake in sspender.cfg.
``` bash
$ apt install pm-utils
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
