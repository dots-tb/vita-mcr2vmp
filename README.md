# vita-mcr2vmp
by [@dots_tb](https://github.com/dots-tb) - signs PSOne MCR files  to create VMP files for use with Sony Vita/PSP

With help from the CBPS (https://discord.gg/2nDCbxJ) , especially:
 [@AnalogMan151](https://github.com/AnalogMan151)
 [@teakhanirons](https://github.com/teakhanirons)
 
## Usage:

Drag and drop a PSOne MCR save file onto the program. It will generate a VMP that you may use with your Vita/PSP.
You may also drag and drop a VMP file and extract the contained MCR file for editing or sharing. 

Or use CMD:
	./vita-mcr2vmp <memorycard.mcr|SCEVMC*.VMP>

## Further Instructions:

### I wanna edit my PSX Save Data from the vita!

	1. Grab your save data from here: ux0:/pspemu/PSP/SAVEDATA/<titleid>/SCEVMC<0|1>.VMP
	2. Open your save data on the computer with a hexeditor and delete the first 0x80, this will convert it to an MCR.
	3. Save it as <name>.MCR.
	4. You may now use it with your save editor or emulators(I think) of choice.
	5. Follow the USAGE section to convert it back to a VMP. Make sure you place it back to the same location with the same file name.
	6. ENJOY!