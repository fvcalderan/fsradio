# fsradio
Control FS9/X Radio Stack with a simple Arduino system

## Article link
This repository contains the source code for
[this article](https://fvcalderan.github.io/myworks/articles/fsradio.pdf).

## Abstract
Flight simulation has been present in civil and military training for decades now. With them, pilots and student pilots can practice from simple tasks, such as taking off and landing the plane, to rare and complex situations, like very aggressive weather conditions and many types of failures. While dedicated flight-instruction institutions and airlines have the means to buy or build sophisticated hardware for flight simulation, hobbyists and aspiring pilots typically do not have the money to invest in such. Taking into account the demand for more pilots, it is essential to increase the accessibility of flight simulation for everyone, so that potential pilots can develop their skills and love for aviation. With this in mind, here it is proposed a very cost-effective solution to control, via dedicated hardware using Arduino, the radio stack, and autopilot system of most General Aviation aircraft in Microsoft Flight Simulator 2004 and X (with minor tweaks).

## Video demonstration
https://user-images.githubusercontent.com/24211835/211572993-662ddf4a-8d56-4d3d-a067-7c876ecff201.mp4

## Dependencies
Fsradio requires [FSUIPC](http://www.fsuipc.com/). Download and install the version for Microsoft Flight Simulator 2004 (FS9) or Microsoft Flight Simulator X (FSX), depending on which simulator you have installed. As of January 2023, the FS9 versio is no longer supported and can be downloaded and used free of charge, while the FSX version is still being sold.

## Device hardware
For details on how to build the actual device, please read [the article](https://fvcalderan.github.io/myworks/articles/fsradio.pdf). In summary, it consists of an Arduino Uno, some buttons and an LCD panel.

## How to install and run
- Compile and upload `fsradio.ino` to your Arduino Uno (or equivalent) board. If a different board is being used, the code may need to be changed.
- Place `fsradio.lua` inside the **Modules** folder of your Flight Simulator installation.
- Configure FSUIPC to start the LUA script (reading FSUIPC's manual is highly recommended).
- Start the LUA script through the configured method.

## FSX tweaks
Some few memory offsets and/or control numbers used are incompatible with FSX. This means that to make the device work properly on FSX, these must be changed. To find the correct indices for FSX, read FSUIPC's manual and make the appropriate changes.

## License
```
BSD 3-Clause License

Copyright (c) 2022, fvcalderan
All rights reserved.
```
