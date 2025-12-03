// pk2cmd.cpp : Defines the entry point for the console application.
//
//                            Software License Agreement
//
// Copyright (c) 2005-2009, Microchip Technology Inc. All rights reserved.
//
// You may use, copy, modify and distribute the Software for use with Microchip
// products only.  If you distribute the Software or its derivatives, the
// Software must have this entire copyright and disclaimer notice prominently
// posted in a location where end users will see it (e.g., installation program,
// program headers, About Box, etc.).  To the maximum extent permitted by law,
// this Software is distributed "AS IS" and WITHOUT ANY WARRANTY INCLUDING BUT
// NOT LIMITED TO ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR
// PARTICULAR PURPOSE, or NON-INFRINGEMENT. IN NO EVENT WILL MICROCHIP OR ITS
// LICENSORS BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
// DAMAGES OF ANY KIND ARISING FROM OR RELATED TO THE USE, MODIFICATION OR
// DISTRIBUTION OF THIS SOFTWARE OR ITS DERIVATIVES.
//
//---------------------------------------------------------------------------
// version 1.00.00  -  16 October 2007 WEK
// Initial release 
//
// version 1.01.00  -  6 December 2007 WEK
// Added -T and -H options.
//  
// version 1.10.00  -  27 June 2008 WEK, JP, XC, SK
// Source changes for multi-OS support
//      Linux and MacOS platform support was made possible through the contributions 
//      of the following individuals:
//          Jeff Post
//          Xiaofan Chen
//          Shigenobu Kimura
//          Francis Perea
// Add -N option
// Add -S option
// Min FW version is 2.31.00
// Add LastVerifyLocation and VERIFY_MEM_SHORT so the verify on a -M program command of
//     program memory only verifies the portion of memory that was written.  This speeds
//     up programming.  (Excludes PIC18 J-series and PIC24FJ)
// Updated config handling for PIC18 J-Series and PIC24FJ on programming / erasing to
//     match PICkit 2 Programmer v2.5x and MPLAB.
// device file compatibility upped to 5, to use DF v1.51
//     Added support for MCP250xx, 11LCxxx, and PIC32 devices.
// Allows spacing between options and parameters, with the following exceptions:
//     -G option memory ranges must not include a space, i.e. x-y. 
//
// version 1.12.00  -  July 2008 WEK, JP, XC
// Changes to read UnitID via USB Serial Number String descriptor and no longer get FW version
//     when polling for multiple units.  This prevents HID transactions when polling from
//     messing up other software already communicating with a PICkit 2 unit.
// Bug fix - handles programming PIC32 with blank Boot Flash section without error or crash.
// Now executes commands not requiring PICkit 2 hardware (such as checksum on hex file) 
//     without requiring a Pk2 unit.
//
// version 1.20.00 - 7 January 2008 WEK
// Bug fix - deprecated RunScriptUploadNoLen2() as it was causing problems on MacOS X systems
//     due to the handling of the multiple blocking reads.
// Added PE support for PIC24 and dsPIC33 devices.  "-Q" option added to disable use of the PE.
// -B option added to specify the device file location.
// Added "fflush(stdout);" to most printf sections to assist with GUI integration.
// -i option now also shows device revison and part name.
// Bug Fix: fixed an issue with displaying 14 character UnitID strings, and limits Unit IDs
//     to 14 characters.
// -j option added: Provides a % completion on Writes, Reads, Verifies, and Blank Checks of
//     Program Memory (Flash) and Data EEPROM. -jn prints each % update on a newline for GUI
//     interfaces (requested by Alain Gibaud).
// -l (L) option added to allow ICSP clock rate to be specified.
// Bug Fix: LastVerifyLocation was not getting set in CPICkitFunctions::EepromWrite, which
//     caused serial EEPROM devices to not be verified.
// Bug Fix: ComputeChecksum change to correctly compute Baseline checksums when CP is enabled.
// Increased the size of the array "DevicePartParams PartsList[1024]" from 512 in DeviceFile.h
//     as the latest device file has exceeded 512 parts.
// -P modified to support autodetection.  auto detection cannot be used with -V or -X.
//     Added new return code AUTODETECT_FAILED, which means no known part was found.
//     -P with no -T or -R will now always turn VDD off, hold MCLR. (-p requires Pk2 operation)
// Updated PIC32 PE from version 0106 to 0109.  This fixes verify problems with some devices,
//     including PIC32MX320F parts.
// Broke up checkHelp into checkHelp1 and checkHelp2.  The latter has help commands that need
//     the device file, like -?v and -?p
// Added help command "-?P<string>" which allows listing of supported parts and listing of
//     supported parts beginning with the given string.  This uses the natural string sorting
//     algorithm developed by Martin Pool <mbp sourcefrog net>
//     See algorithm files strnatcmp.c/h
// Added -mv, -gv (undocumented)
// Added support for import/export of BIN files for serial EEPROMs.
// Fixed a bug that may have appended a space at the end of -gf file names on Linux.
//
// version 1.21.00 RC1 - 3.12.2010 MichaelS / Microchip
// Updated to work with version 2.6x device files
//
// version 1.21.00 - 25.2.2021 Miklos Marton
// Updated to work with both PICkit2 and PICkit3
// Updated Linux and Mac source files with device file v2.6x support (based on 1.21.00. RC1 WIN32 source)
//
// version 1.22.00 - 4.7.2021 JAKA
// Updated also Windows source files with PICkit3 support (based on work by Miklos Marton)
// Updated to work with new PIC16 and PIC18 families which use SPI-type programming (MSB1st in
// family name). Based on work by bequest333.
//
// version 1.22.01 - 8.7.2021 JAKA
// Updated also Mac OSX source files with PICkit3 support
// Display version on general help
//
// version 1.22.02 - 13.7.2021 JAKA
// Fixed devices with more than 32 UserID words
//
// version 1.22.03 - 16 August 2021 JAKA
// Bug Fix: Fix Q40,Q41,Q43 families. Configuration memory needs to be written or read byte at
//          a time on these devices, not word at a time. Added swap2Bytes() function to arrange
//          configuration bytes to correct order. Regocnized from 0x000c as 'IgnoreBytes'
// 
// version 1.22.04 - 31 August 2021 JAKA
// Bug Fix: Fix PIC18 Msb1 family devices with more than 64 kB of prog. flash
// Add support for devices with more than 9 config words (Q83 and Q84 families so far)
// Config masks and config blanks for the extra config words are set to 0xffff
// If even number of config bytes, like 35 in Q83 and Q84, the last one must be indicated
// by IgnoreAddress (0x00300023 in this case)
// 
// version 1.22.05 - 23 September 2021 JAKA
// Feature: Added Skip Blank Sections feature, active by default. Use -O to disable. Write and verify
//          skip program memory sections which are blank (e.g. 0x3fff or 0xffff). Many compilers
//          place some parts of code to very end of program memory, and default behavior was to
//          write and verify to last used memory address. Skipping the blank sections also from
//          the middle can greatly reduce programming time if program memory is not full.
//          The address set script of SPI-type PICs was updated in the device file to support
//          all 3 address bytes. Also, the address set function is changed, because with the new
//          script the bits must be reversed for SPI-type PICs. So the device file must be updated
//          together with the application to maintain compatibility! Device file needs to be 2.63.222
//          or later. Many other SPI-type PIC scripts on the 2.63.222 device file are also optimized
//          which has reduced write and read times substantially, 20...50%. In case of K40,42,83
//          EEPROM write the time has been reduced by 80%!
//
// version 1.22.06 - 6 December 2021 JAKA
// Bug Fix: Fix blank checking on devices which have Bandgap bits on configuration word
//
// version 1.23.00 - 15 June 2022 JAKA
// Feature: Compile option define to select between two types of address set script for MSB1st
//          families. (#define OLDSTYLE_MSB1ST_SETADDR at stdafx.h)
//          Note! Probably needs fixing with PIC18F either in software or a new script.
//          Current advise; do not use this define. It was intended just for performace test usage.
// Bug Fix: Disable resetting of PICkit3 if -T and/or -R options are used - otherwise those options
//          are useless.
// Feature: Support for PIC32MX1xx and 2xx. Based on work of Microchip forum users timijk, scasis
//          and TrevorW on thread https://www.microchip.com/forums/m764694.aspx
// Bug Fix: Increased the size of the array "DevicePartParams PartsList[1500]" from 1024 in DeviceFile.h
//          as the latest device file has exceeded 1024 parts.
//
// version 1.23.01 - 16 June 2022 JAKA
// Bug Fix: Disable resetting of PICkit3 also if -s or -s# options are used. With these options, the
//          resetting causes PICkit3 to go into some weird state and requires re-plugging before it
//          starts to work again.
//
// version 1.23.02 - 9 July 2022 JAKA
// Bug Fix: Disabled resetting of PICkit3 completely. On Linux, it was needed to solve PICkit3 not
//          found problem, when pk2cmd was run more than once. It seems that the stuck was caused by
//          first trying to set configuration 2 (which PICkit3 scripting firmware doesn't implement)
//          and then claiming the USB device. Disabling these two functions, the PICkit3 doesn't stuck
//          on Linux anymore. On Windows, the resetting wasn't needed at all, and it was actually
//          causing stuck problems (with -s option), rather than solving them. Code to enable resetting
//          is still there, set resetPK3OnExit to TRUE on cmd_app.cpp to re-enable it.
// Bug Fix: Specifying /x? was printing help for /y option as well. Fixed - thanks to Tony!
//
// version 1.23.03 - 21 July 2022 JAKA
// Bug Fix: Fixed blank section skipping on PIC24/dsPIC
// 
// version 1.23.04 - 15 November 2022 JAKA
// Bug Fix: Changed how different PIC32MX families are recognized. Now works properly with all 5xx
//          devices.
//
// version 1.23.05 - 21 January 2023 JAKA
// Bug Fix: Export of HEX file now discards the useless byte of partial last config word on Q71,Q83,Q84
//
// version 1.23.06 - 11 February 2023 JAKA
// Bug Fix: Increased number of USB devices to check from 24 to 64 (MemberIndex in usbhidioc.cpp)
//          Fixes PICkit detection on systems with many USB devices.
//
// version 1.23.07 - 15 April 2023 JAKA
// Bug Fix: Fixed FamilyIsEEPROM() function. Was causing at least .bin file support to not work, maybe
//          other problems as well.
// Bug Fix: Allocate more ProgramMemory if the default MAX_MEM is not enough. Required at least for
//          SPI FLASH devices bigger than 2 Mbit.
// Bug Fix: Use new function getDecValue instead of getValue for options -PF<id>, -H and -L.
//          Fixes auto-detecting within families with #ID higher than 9.
// Bug Fix: Importing / exporting .BIN files did not work if more than one priority1 args after the 
//          -F or -GF option.
// Bug Fix: ReadDeviceID() function was reading only 16-bit device ID, now 32-bit.
// Feature: Support of SPI FLASH chips. Use PROTOCOL_CFG = SPI_FLASH_BUS (first config mask)
//          With SPI FLASH chips, config read script is actually read STATUS reg. script
//          When doing chip erase (bulk erase), BUSY/WIP bit of STATUS register is polled to
//          determine when erase has finished. Optionally, typical and maximum bulk eraase times
//          can be specified as 5th and 6th config mask values, respectively. If not specified,
//          default values are used. 
//
// version 1.23.08 - 31 May 2023 JAKA
// Bug Fix: Always erase SPI FLASH device before programming.
//
// version 1.24.00 - 24 Jun 2023 JAKA
//          Release version with SPI FLASH device support  
//
// version 1.24.01 - 12 Jul 2023 JAKA
// Feature: Support 1 Mbit and 2 Mbit I2C EEPROMs which have BS bit(s) below CS bit(s) in control byte  
// Fearure: Allow I2C address selection for I2C EEPROMs
// Bug Fix: Fix EEPROM devices verify with blank skipping enabled
//
// version 1.24.02 - 10 Sep 2023 JAKA
// Feature: Support blank skipping with EEPROM writes. 
//          Devices must have ChipErase and ProgMemAddrSet scripts to support blank skipping for writing.
//          If part doesn't really use AddSet, like SPI devices which always write address for
//          reads and writes, set ProgMemAddrSet to 183 (empty script). For verify, only ProgMemAddrSet
//          is needed. (So for example, 24xx and 25xx devices must write all bytes but if blank skipping
//          enabled, verify skips blank sections)
// Bug Fix: Don't send address for verify of blank sections in EEPROM family devices.
// Feature: Reduce EEPROM read / verify times by sending the read address in same packet as run script
//          and data request. Reduce read times up to 15% (1 ms / 128 bytes).
// Feature: Check that busy bit is cleared after last write operation of SPI FLASH devices.
//          Uses config read script like in chip erase completion check. This change allows to use
//          write script which checks the busy bit before write, and ensures that busy has cleared
//          before verify.
//
// version 1.24.03 - 15 Sep 2023 JAKA
// Bug Fix: Don't use PE on PIC24FJ parts where it's not (yet) supported
//
// version 1.24.04 - 28 Nov 2023 JAKA
// Bug Fix: Bulk erase EEPROM if writing only EEPROM to chips which have EEPROM erase script
// 
// version 1.24.05 - 30 Nov 2023 JAKA
// Bug Fix: Update command like short help
// 
// version 1.25.00 - 27 Jan 2024 JAKA
// Bug Fix: Fix updating TBLPAG on PIC24/dsPIC devices, when TBLPAG border is not divisible by
//          words per write (at least PIC24EP / dsPIC33EP).
// Bug Fix: Fix verifying / blank checking user ID words if number of ID bytes smaller than device natural
//          word size.
// Bug Fix: Fix writing config registers when located in program memory and device has more config registers
//          than can be written on a single program memory write.
// Change:  Now setting config masks / blanks to 0xffff for parts which have >9 configs, during device file
//          read. Some extra code added earlier to handle such devices could be removed, but not done yet.
// Bug Fix: Fix writing parts which have configs in program memory and user IDs
// Feature: Improved handling of USB timeout
// Feature: Support for PKOB (PICkit 3 on board)
// Feature: More detailed information about state of PICkit (e.g. PICkit3/PKOB in bootloader or MPLAB mode)
// Feature: Show connected unit type when listing units with -S#
// Bug Fix: Fix compile error with Linux (was introduced in 1.24.00)
// Bug Fix: Fixed Mac OSX version to work with different types of PICkit (2,3 or PKOB) connected at same time
// 
// version 1.26.00 - 29 Feb 2024 JAKA
// Feature: Add support for PIC24 devices which have config words on last page, but not on the very last
//          program memory words (e.g. 24FJ GA70x, GL30x)
// 
// version 1.26.01 - 26 Jun 2024 JAKA
// Feature: Show hex file header comments on screen (but not comments at end of file)
// 
// version 1.26.02 - 18 Sep 2024 JAKA
// Bug Fix: Fix incorrectly written UserID on PIC32MX 1xx/2xx/5xx devices
// Feature: Support for Customer OTP memory found on some PIC24 / dsPIC devices
// Bug Fix: Use 3v3 instead of 3v0 for part detect as in GUI software
// 
// version 1.26.03 - 7 Nov 2024 JAKA
// Bug Fix: Increased partslist array to 2048 since devicefile is getting close to 1500 devices
// Feature: Show also device ID when listing parts with -?P
// 
// version 1.26.04 - 19 Nov 2024 JAKA
// Bug Fix: Support for PIC33CK devices
// 
// version 1.26.05 - 11 Jan 2025 JAKA
// Bug Fix: Fix progmem write on some PIC24 devices with -O option
// 
// version 1.26.06 - 4 May 2025 JAKA
// Bug Fix: Fix compile warnings on linux, thanks boborjan2!
//          Based on https://github.com/boborjan2/pk2cmd/tree/master/pk2cmd
// Change:  Use libusb-1.0 on linux instead of the old libusb-0.1, thanks boborjan2 (above url)
// Bug Fix: OS Firmware version response with -s# option when PK3 is not responding
// Feature: Support 'Immediate verify'. If writing only some areas, e.g. only program memory
//          and configuration with -MPC, and code protection bit is active, the verify will
//          fail for program memory because of code protect. By using new option immediate
//          verify with -M+PC, the programmed areas will be verified immediately after
//          programming, and verify succeeds. Thanks boborjan2 (again above url)
// Feature: With externally powered target, now possible to specify threshold for external
//          voltage. The programming will start only after the external voltage exceeds this
//          threshold. Thanks boborjan2 (again above url)
// Feature: Support EEPROMS/SPIFLASH 1v8 family
// Bug Fix: Vdd and Vpp readout with PK3 and PKOB was returning garbage, now fixed
// 
// version 1.27.00 - 21 Jun 2025 JAKA
// Feature: Support writing packed .hex files (omit blank data). Use -gu to export
//          unpacked .hex files as before
// Feature: Support PIC24/33 devices with auxiliary flash memory
//
// version 1.27.01 - 23 Jun 2025 tnurse18
// Bug Fix: Fix PICkit detection issues by correcting a logic error in usbhidioc.cpp
//
// version 1.27.02 - 9 Nov 2025 JAKA
// Bug Fix: Fix PIC18F WRTC bit workaround to trigger only on relevant families.
//          Thanks to NelsonBittencourt for reporting the problem
//
// version 1.27.03 - 3 Dec 2025 JAKA
// Bug Fix: Fix verify for parts which have configs in program memory and blank
//          values not correctly set in .hex



#include "stdafx.h"
#include "stdio.h"
#include "cmd_app.h"

Ccmd_app pk2app;

#ifdef WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char *argv[])
#endif
{
	pk2app.PK2_CMD_Entry(argc, argv);

	printf("\n");

	if (pk2app.ReturnCode == 0)
	{
		printf("Operation Succeeded\n");
	}

	pk2app.ResetAtExit();
	pk2app.PicFuncs.USBClose();

	//while(1);
	return pk2app.ReturnCode;
}


