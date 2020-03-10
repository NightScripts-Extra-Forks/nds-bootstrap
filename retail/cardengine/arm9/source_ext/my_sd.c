#include <nds/ipc.h>

#include "my_disc_io.h"
#include "my_sdmmc.h"

extern vu32* volatile sharedAddr;

/*-----------------------------------------------------------------
startUp
Initialize the interface, geting it into an idle, ready state
returns true if successful, otherwise returns false
-----------------------------------------------------------------*/
bool my_sdio_Startup(void) {
	#ifdef DEBUG
	nocashMessage("startup internal");
	#endif
	return true;
}

/*-----------------------------------------------------------------
isInserted
Is a card inserted?
return true if a card is inserted and usable
-----------------------------------------------------------------*/
bool my_sdio_IsInserted(void) {
	#ifdef DEBUG
	nocashMessage("isInserted internal");
	#endif
	return true;
}

/*-----------------------------------------------------------------
readSector
Read 1 512-byte sized sectors from the card into "buffer", 
starting at "sector". 
The buffer may be unaligned, and the driver must deal with this correctly.
return true if it was successful, false if it failed for any reason
-----------------------------------------------------------------*/
bool my_sdio_ReadSector(sec_t sector, void* buffer, u32 startOffset, u32 endOffset) {
	#ifdef DEBUG
	nocashMessage("readSector internal");
	#endif

	u32 commandRead = 0x53445231;

	sharedAddr[0] = sector;
	sharedAddr[1] = (vu32)buffer;
	sharedAddr[2] = startOffset;
	sharedAddr[3] = endOffset;
	sharedAddr[4] = commandRead;

    //IPC_SendSync(0x4);
	while (sharedAddr[4] == commandRead);
	return sharedAddr[4] == 0;
}

/*-----------------------------------------------------------------
readSectors
Read "numSectors" 512-byte sized sectors from the card into "buffer", 
starting at "sector". 
The buffer may be unaligned, and the driver must deal with this correctly.
return true if it was successful, false if it failed for any reason
-----------------------------------------------------------------*/
bool my_sdio_ReadSectors(sec_t sector, sec_t numSectors, void* buffer, int ndmaSlot) {
	#ifdef DEBUG
	nocashMessage("readSectors internal");
	#endif

	u32 commandRead = 0x53445244;

	sharedAddr[0] = sector;
	sharedAddr[1] = numSectors;
	sharedAddr[2] = (vu32)buffer;
	sharedAddr[3] = ndmaSlot;
	sharedAddr[4] = commandRead;

    //IPC_SendSync(0x4);
	while (sharedAddr[4] == commandRead);
	return sharedAddr[4] == 0;
}

/*-----------------------------------------------------------------
readSectors
Read "numSectors" 512-byte sized sectors from the card into "buffer", 
starting at "sector". 
The buffer may be unaligned, and the driver must deal with this correctly.
return true if it was successful, false if it failed for any reason
-----------------------------------------------------------------*/
int my_sdio_ReadSectors_nonblocking(sec_t sector, sec_t numSectors, void* buffer, int ndmaSlot) {
	#ifdef DEBUG
	nocashMessage("my_sdio_ReadSectors_nonblocking");
	#endif
	return false;
}

bool  my_sdio_check_command(int cmd, int ndmaSlot) {
	#ifdef DEBUG
	nocashMessage("my_sdio_check_command");
	#endif
	return false;
}

/*-----------------------------------------------------------------
writeSectors
Write "numSectors" 512-byte sized sectors from "buffer" to the card, 
starting at "sector".
The buffer may be unaligned, and the driver must deal with this correctly.
return true if it was successful, false if it failed for any reason
-----------------------------------------------------------------*/
bool my_sdio_WriteSectors(sec_t sector, sec_t numSectors, const void* buffer, int ndmaSlot) {
	#ifdef DEBUG
	nocashMessage("writeSectors internal");
	#endif
	return false;
}


/*-----------------------------------------------------------------
clearStatus
Reset the card, clearing any status errors
return true if the card is idle and ready
-----------------------------------------------------------------*/
bool my_sdio_ClearStatus(void) {
	#ifdef DEBUG
	nocashMessage("clearStatus internal");
	#endif
	return true;
}

/*-----------------------------------------------------------------
shutdown
shutdown the card, performing any needed cleanup operations
Don't expect this function to be called before power off, 
it is merely for disabling the card.
return true if the card is no longer active
-----------------------------------------------------------------*/
bool my_sdio_Shutdown(void) {
	#ifdef DEBUG	
	nocashMessage("shutdown internal");
	#endif	
	return true;
}

const NEW_DISC_INTERFACE __myio_dsisd = {
	DEVICE_TYPE_DSI_SD,
	FEATURE_MEDIUM_CANREAD,
	(FN_MEDIUM_STARTUP)&my_sdio_Startup,
	(FN_MEDIUM_ISINSERTED)&my_sdio_IsInserted,
    (FN_MEDIUM_READSECTOR)&my_sdio_ReadSector,
	(FN_MEDIUM_NEW_READSECTORS)&my_sdio_ReadSectors,
    (FN_MEDIUM_READSECTORS_NONBLOCKING)&my_sdio_ReadSectors_nonblocking,
    (FN_MEDIUM_CHECK_COMMAND)&my_sdio_check_command,
	(FN_MEDIUM_NEW_WRITESECTORS)&my_sdio_WriteSectors,
	(FN_MEDIUM_CLEARSTATUS)&my_sdio_ClearStatus,
	(FN_MEDIUM_SHUTDOWN)&my_sdio_Shutdown
};
