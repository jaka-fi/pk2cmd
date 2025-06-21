pk2cmd for PICkit2 and PICkit3 programmers
==========================================
Notice
------
THE CODE IN THIS REPOSITORY IS NOT FREE SOFTWARE; it is distributed under a highly restrictive, non-free license. Do not copy, branch, or fork this repository unless you have agreed to this license.

__To download this software for Windows, Linux or Mac, see 'Releases' on right edge of this github page.__  &rarr;

This repository contains pk2cmd tool, originally developed by Microchip. It has been improved in many ways, and besides PICkit2, now supports PICkit3 and PKOB programmers too. Also support for hundreds of devices has been added. For GUI software (Windows only), see my another repository [PICkitminus](https://github.com/jaka-fi/PICkitminus).

Features
--------
- Supports 1588 devices, see [here](https://github.com/jaka-fi/PICkitminus/blob/master/PICkitminus_supported_devices.txt) for list
- Supports PICkit2 and PICkit3 programmers, including clones and derivatives like PKOB, PICkit3.5 or [PK2M](http://kair.us/projects/pk2m_programmer/index.html)
- Improved auto detection of parts
- Optimized programming scripts for MSB1st families to reduce write and verify times
- Improved blank section skipping for write and verify, to further reduce programming times
- [Improved operation](https://forum.microchip.com/s/topic/a5C3l000000MdWiEAK/t381995) with PICkit3 clones
- [SPI FLASH device support](http://kair.us/projects/pickitminus/program_spi_flash_devices_with_pickit2_and_pickit3.html)
- Command line software works on Windows XP, 7, 10, 11, Linux and MacOS
- Retains all the good features from original Microchip pk2cmd

About the device file
-----------------
The device file, PK2DeviceFile.dat contains information of different chips and all the scripts needed to program them. This file was maintained by Microchip until 2012 when the last PICkit3 software was released. The device file 1.62.15 supported 639 devices.

After the official support ended, many people started to modify the device file with the [editor by dougy83](https://sites.google.com/site/pk2devicefileeditor/). The result was many different versions of the device file which supported slightly different devices. Also many incorrectly created/copied devices creeped into the device files that time. I also shared a fork on my web site, to which I had added or corrected some devices which I used in my projects. The last version was 1.63.149 from 13.2.2017, supporting 743 devices.

When Anobium started the PICkitplus project, he made a huge effort to gather all the different device files floating around, and merged those into one file with all devices created so far. He also added many new devices, fixed problems with existing devices, and most importantly, added PIC16 and PIC18 MSB1st families, partly based on work of bequest333. The last openly published PICkitplus device file 2.63.218.15 at [Anobium's Github repo](https://github.com/Anobium/PICKitPlus/releases) from December 2020 has 969 devices. I used this as starting point for PICkitminus.

If you want to support Anobium's efforts and contribution to the device file, consider buying the [PICkitPlus software](https://www.pickitplus.co.uk/).

Notes on Linux
--------------
Originally, pk2cmd on Linux used libusb-0.1.12. Beginning from version 1.26.06, the pk2cmd is changed to used libusb-1.0. This should hopefully make compiling easier on modern distributions. The pk2cmd also works a little quicker with libusb-1.0. There are Linux prebuilt binaries in AppImage format, which should run on many distributions.

If you run pk2cmd on Linux and get a message 'PICkit 2/3/PKOB not found', quite probable reason is that normal user doesn't have proper rights to the USB device. A simple solution is to run pk2cmd as root, but this is a bit ugly. On systems with udev, you can use [this udev rules file](https://github.com/jaka-fi/pk2cmd/blob/master/60-pickit.rules) which gives appropriate rights for PICkit2 and PICkit3. Just copy this file to /etc/udev/rules.d/ and restart udev (or restart PC). You will also need to re-plug the PICkit.

Final note for Linux; the pk2cmd currently doesn't support updating PICkit3 firmware. If your PICkit3 doesn't have scripting firmware installed, you must use the GUI software on Windows PC to update it.

Notes on PICkit3 and PKOB
-------------------------
The PICkit3 and PKOB (PICKit On Board) require a special 'scripting' firmware, just like the Microchip original PICkit3 standalone GUI software. When you start the GUI software, it guides you to update the firmware if needed. The pk2cmd doesn't yet support firmware updates for PICkit3 or PKOB.

When you want to use MPLAB, MPLAB-X or MPLAB-X IPE again, you must revert the PICkit3 to bootloader. To do this, select 'Revert to MPLAB mode' from Tools menu. Then start MPLAB(-X), and it will update the correct firmware for MPLAB usage. If you don't do this, you will get all kind of errors when trying to use your PICkit3 or PKOB with MPLAB.

PKOB operation has been tested with the following development boards:

- Curiosity PIC32MX470 Development Board (DM320103)
- PIC24F Curiosity Development Board (DM240004)
- Microstick II SK (DM330013-2)
- Explorer 16/32 Development Board (DM240001-2)

Please Note that all new devboards have PKOB4 or some other solution, those are not supported. Also many older boards have been updated to new revision. For example Curiosity HPC board (DM164136) originally had PKOB (based on PICkit3), but revision 2 has PKOB4 (based on PICkit4). The easiest way is to look at the microcontroller type on the devboard. If it is PIC24FJ256GB106, it is very likely PKOB, and probably will work.

Downloads
---------
To download this software, see 'Releases' on right edge of this github page.  &rarr;

Thanks
------
I haven't developed this software all by myself. The biggest part has of course been Microchip's original work, and all the contributions they had received from PICkit2 users. In addition to that, I have used work from other people. My thanks go to all contributions listed below:

- bequest333 [for initially adding support for MSB 1st chips](https://www.eevblog.com/forum/microcontrollers/pic16f18857-programming-with-pickit2/)
- Anobium from [PICkitPlus](https://www.pickitplus.co.uk/) team for [providing updated device file until 2020](https://github.com/Anobium/PICKitPlus/releases)
- dougy83 for creating [device file editor](https://sites.google.com/site/pk2devicefileeditor/)
- Miklós Márton for [adding PICkit3 support to pk2cmd](https://github.com/martonmiklos/pk2cmd)
- timijk, scasis and TrevorW for [adding support for all PIC32MX](https://forum.microchip.com/s/topic/a5C3l000000MOXFEA4/t324373)
- Adem Gdk for adding some SPI FLASH devices and testing SPI FLASH support
- Jaren Sanson for [tool which adds some PIC24 devices](https://jared.geek.nz/2013/08/pickit2-revisited/)
- boborjan2 for [libusb-1.0 support on linux and other improvements](https://github.com/boborjan2/pk2cmd)
- All people who have sent me bug reports
