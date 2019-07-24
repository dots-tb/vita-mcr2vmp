# vita-mcr2vmp
by @dots_tb - signs psx mcr files for use with the vita's psp emulator

With help from the CBPS (https://discord.gg/2nDCbxJ) , especially:

 @AnalogMan151
 
 @teakhanirons
 
## Usage:

Drag and drop a PSX MCR save file onto the exe. It will generate a VMP that you may use with your Vita's PSP emulator.

Or use CMD:

	./vita-mcr2vmp mem.mcr

## Further Instructions:

### I wanna edit my PSX Save Data from the vita!

	1. Grab your save data from here: ux0:/pspemu/PSP/SAVEDATA/<titleid>/SCEVMC<0|1>.VMP
	2. Open your save data on the computer with a hexeditor and delete the first 0x80, this will convert it to an MCR.
	3. Save it as <name>.MCR.
	4. You may now use it with your save editor or emulators(I think) of choice.
	5. Follow the USAGE section to convert it back to a VMP. Make sure you place it back to the same location with the same file name.
	6. ENJOY!
