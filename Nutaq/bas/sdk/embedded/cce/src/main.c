#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <math.h>
#include <setjmp.h>

#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>

#include <assert.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#include "cce.h"
#include "utils.h"
#include "bas_error.h"

#include "eapi_serialize.h"
#include "cce_io.h"
#include "bas_debug.h"
#include "cce_mainloop.h"
#include "common_utils.h"

#include "carrier_service.h"
#include "carrier_service_def.h"
#include "carrier_fmc_service.h"
#include "carrier_fmc_service_def.h"

#include "FMCEEprom_def.h"
#include "FMCEEprom_eeprom_id.h"

#include "cce_radio640.h"
#include "Radio640.h"
#include "Radio640_defines.h"


#ifdef PERSEUS
#include "recplay.h"
#include "rtdex_access.h"

#include "sysmon.h"

#include "timestamp.h"

#include "cce_ppssync.h"
#include "ppssync.h"
#include "ppssync_types.h"
#include "ppssync_defines.h"

#include "bitstream.h"

#include "perseus601x.h"
#include "perseus611x.h"

#include "cce_adc5000.h"
#include "adc5000.h"
#include "adc5000_defines.h"

#include "cce_adac250.h"
#include "adac250_user.h"

#include "cce_radio420.h"
#include "fmc_radio.h"

#include "MI125_mi125.h"

#include "cce_mi250.h"
#include "mi250_defines.h"
#include "mi250_types.h"
#include "mi250_user.h"

#include "cce_lvdsxinxout.h"
#include "fmclvds_pcf8574.h"
#include "fmclvds.h"
#include "fmclvds_pcf8574_defines.h"

#include "cce_lvds.h"

#include "aurora.h"

#include "Mo1000.h"

#ifdef FMCOMMS3_IN_USE
#include "cce_fmcomms3.h"
#include "fmcomms3.h"
#include "ad9361_api.h"
#endif //FMCOMMS3_IN_USE
#endif //PERSEUS

#ifdef ZC706 
#include "zc706.h"
#endif //ZC706

//cce version
#define _STRINGIZE(x)  #x
#define STRINGIZE(x)   _STRINGIZE(x)
#define CCE_MAJOR 3
#define CCE_MINOR 5
#define CCE_REVISION 2
#ifdef DEBUG
#define CCE_VERSION (STRINGIZE(CCE_MAJOR) "." STRINGIZE(CCE_MINOR) "." STRINGIZE(CCE_REVISION) " - " __DATE__ " " __TIME__)
#else
#define CCE_VERSION (STRINGIZE(CCE_MAJOR) "." STRINGIZE(CCE_MINOR) "." STRINGIZE(CCE_REVISION) " ")
#endif
const char cce_version[]=CCE_VERSION;

int dev_mem;
char dev_mem_device[]="/dev/mem";

char cce_mac[20];
unsigned char temp_mac[6];
volatile unsigned * carrier_space;
volatile unsigned * register_space;
volatile unsigned * fmc_space;
volatile unsigned * fmc_i2c_space;
volatile unsigned * dpram_space;
volatile unsigned * ddr3_i2c_space;
uint32_t register_begin,    register_size,  register_end;
uint32_t fmc_begin,         fmc_size,       fmc_end;
uint32_t fmc_i2c_begin,     fmc_i2c_size,   fmc_i2c_end;
uint32_t dpram_begin,       dpram_size,     dpram_end;
uint32_t ddr3_i2c_begin,    ddr3_i2c_size,  ddr3_i2c_end;

#define MAXBOARD_FOR_SINGLE (CARRIER_FMC_MAXCONNECTOR * 1)
#define MAXBOARD_FOR_STACK  (CARRIER_FMC_MAXCONNECTOR * CARRIER_FMC_MAXBOARD)

#ifdef PERSEUS
//PPS-SYNC
PPSSync_t * PPSSync[MAXBOARD_FOR_SINGLE];
void * PPSSyncBoardHandle[MAXBOARD_FOR_SINGLE];
PPSSync_pfDriveDac_t PPSSync_pfDriveDac[MAXBOARD_FOR_SINGLE];
int ppssync_selected_inst = 0;


//ADAC250
ADAC250Board_t * adac250[MAXBOARD_FOR_SINGLE];
int adac250_selected_inst = 0;

//ADC5000
adc5000 adc5000board[MAXBOARD_FOR_SINGLE];
int adc5000_selected_inst = 0;

//MI250
MI250Board_t * mi250_fmcboard[MAXBOARD_FOR_SINGLE];
int mi250_selected_inst = 0;

//FMCLVDS
unsigned selected_fmclvds=0;
fmclvds fmclvds_handle[MAXBOARD_FOR_STACK];
FMCLVDS_pcf8574_core fmclvds_com_handle[MAXBOARD_FOR_STACK];

//LVDS
lvds lvds_handle[LVDS_GROUP_MAX];

//FMCRADIO
unsigned selected_fmc_radio = 0;
fmc_radio fmc_radio_handle[MAXBOARD_FOR_STACK];

// Perseus I2C bus release control register
#define PERSEUS_I2C_RELEASE_REG  ((MI125_i2c_release *)(TARGET_MMC_CTRL))

MI125_mi125 mi125_board[MAXBOARD_FOR_STACK];
MI125_mi125_IoDelayRef Mi125IoDelayRef = MI125_REF200MHZ;    // FPGA reference clock used (default for perseus)

// MO1000 core addresses
// Perseus I2C bus release control register
#define MO1000_PERSEUS_I2C_RELEASE_REG  ((DevCom_stI2cRelease *)(TARGET_MMC_CTRL))
#define MO1000_BOTTOM_ADDRESS (fmc_begin)
#define MO1000_TOP_ADDRESS    (fmc_begin + fmc_board_size)
// MO1000
Mo1000_stMo1000 Mo1000Board[MAXBOARD_FOR_STACK];
Mo1000_eIoDelayRef Mo1000IoDelayRef = eMo1000Ref200Mhz;

#ifdef FMCOMMS3_IN_USE
// FMCOMMS3
Fmcomms3_Handle_t ahFmcomms3[MAXBOARD_FOR_SINGLE] = {NULL};
#endif //FMCOMMS3_IN_USE

// Aurora
aurora_core aurora[8];

Sysmon_handle_t hSysmon = NULL;

//TIMESTAMP
Timestamp_Handle_t hTimestamp;
#endif //PERSEUS
struct ifreq ifr;
// Radio640
Radio640_hRadio640 ahRadio640[MAXBOARD_FOR_STACK] = {NULL};

// Carrier definitions
CARRIER_detected CarrierDetected;
FMCEEprom_module_types aMi125ModList[2][2] = {{FMCEEPROM_EEPROM_MOD_MI125E,FMCEEPROM_EEPROM_MOD_MI125WE},
                                              {FMCEEPROM_EEPROM_MOD_MI125,FMCEEPROM_EEPROM_MOD_MI125W}};
FMCEEprom_module_types aRadioModList[2][3]  = {{FMCEEPROM_EEPROM_MOD_FMCRADIO,FMCEEPROM_EEPROM_MOD_FMCRADIOE,FMCEEPROM_EEPROM_MOD_FMCRADIOE1V8},
                                               {FMCEEPROM_EEPROM_MOD_FMCRADIO,FMCEEPROM_EEPROM_MOD_FMCRADIOS,FMCEEPROM_EEPROM_MOD_FMCRADIOS1V8}};
char acHardRevision[MAXBOARD_FOR_STACK][100];
int CarrierFoundAll;

// Calibration for modules
FMCEEPROM_HPC_DATA_Specs CalibDelay[MAXBOARD_FOR_STACK];


int iFpgaModuleCnt = 0;
// -----------------------------------------------

Carrier_handle_t hCarrier = NULL;
int bInvalidCarrier = 0;

/*--------------------------------------------------------
--       Private Functions Definitions                  --
--------------------------------------------------------*/

Result _MapMemorySpace();

/*--------------------------------------------------------
--           Public Functions                           --
--------------------------------------------------------*/

int main (int argc, char * argv[])
{
    Result res;
    unsigned int u32PlatformID;

    struct  sockaddr_in sad;
    struct  sockaddr_in cad;
    socklen_t sock_size;
    int i;
    int socks = socket (AF_INET, SOCK_STREAM, 0);

    CARRIER_printout printout = CARRIER_PRINTOUT_SUMMARY;

    unsigned int uLastBoard, uLastConnector;
    int iCarrierBoard, iCarrierConnector;
    FMCEEprom_module_types CarrierModList;

    unsigned int uiModuleBaseAddr;
#ifdef PERSEUS
    unsigned int uiModuleMaxNum;
#endif //PERSEUS
    printf("Central Communication Engine - %s\n", cce_version);
    debug("Build %d - debug\n",0);
    printf("Nutaq\n\n");
    
    /*
     *  Open file descriptor to memory
     */
    dev_mem = open(dev_mem_device, O_RDWR);
    if(dev_mem<0)
    {
        fprintf(stderr, "couldn't open %s\n", dev_mem_device);
        return -1;
    }

    /*
     *  User space
     */
    carrier_space = mmap((void*)CARRIER_REG_BASE_ADDR, CARRIER_REG_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, dev_mem, CARRIER_REG_BASE_ADDR);
    if( carrier_space == MAP_FAILED )
    {
        char error[512];
        fprintf(stderr, "mmap() failed\n");
        perror(error);
        printf("%s\n", error);
        return -1;
    }

    /*
     * Open the carrier object based on the platform register ID
     * All carrier platform register ID must be located at the same address
     */
    debug("u32PlatformID\n",0);
    u32PlatformID = *((volatile unsigned int *)CARRIER_REG_BASE_ADDR) >> 16;
    debug("u32PlatformID Done\n",0);

    switch(u32PlatformID)
    {
#ifdef PERSEUS      
        case PERSEUS601X_CORE_ID:
            debug("Perseus601X_Open\n",0);
            res = Perseus601X_Open(&hCarrier, usleep);
            debug("Perseus601X_Open Done\n",0);
            break;
        case PERSEUS611X_CORE_ID:
            debug("Perseus611X_Open\n",0);
            res = Perseus611X_Open(&hCarrier, usleep);
            debug("Perseus611X_Open Done\n",0);
            break;
#endif
#ifdef ZC706       
        case ZC706_CORE_ID:
            debug("ZC706_Open\n",0);
            res = ZC706_Open(&hCarrier, usleep);
            debug("ZC706_Open Done\n",0);
            break;     
#endif         
        default:
            printf("\nInvalid carrier board detected!\n");
            printf("Update the FPGA bitstream with a design that has a valid carrier board instantiated.\n\n");
            bInvalidCarrier = 1;
#ifdef PERSEUS              
            // Open for Perseus601X to allow the user to update its bitstream
            res = Perseus601X_Open(&hCarrier, usleep);
            debug("Perseus601X_Open Done\n",0);
#else
            res = CARRIER_NOT_OPEN;
            goto error_end;
#endif            
            break;
    }
    if(FAILURE(res)) goto error_end;
  

    /*
     * Unmap carrier memory space.
     * This needs to be done since the carrier space is contained
     * in the register space that will be mapped.
     */
    munmap((void *)carrier_space, CARRIER_REG_SIZE);

    /*
     * Map memory space based on the carrier
     */
    debug("_MapMemorySpace\n",0);
    res = _MapMemorySpace();
    if(FAILURE(res)) goto error_end;
    debug("_MapMemorySpace Done\n",0);
    
    /*
     * TCP connection
     */
    memset((char *)&sad, 0, sizeof(sad));
    sad.sin_family = AF_INET;
    sad.sin_addr.s_addr = INADDR_ANY;
    sad.sin_port = htons(CCEIPPORT);

    if (bind(socks, (struct sockaddr *)&sad, sizeof(sad)) < 0)
    {
        fprintf(stderr,"Bind failed\n");
        exit(-1);
    }
    setsockopt(socks, SOL_SOCKET, SO_KEEPALIVE, &i, sizeof(i));
    //setsockopt(socks, SOL_SOCKET, SO_LINGER, &i, sizeof(i));
    signal(SIGPIPE, SIG_IGN);//dont close cce on tcp socket reset
    if (listen(socks, 10) < 0)
    {
        fprintf(stderr, "Listen failed\n");
        exit(-2);
    }

    memset (&ifr,0,sizeof(ifr));

    /* name of the card */
    /***** How are NIC devices named in SFU? *****/
    strcpy(ifr.ifr_name,"eth0");

    ioctl(socks,SIOCGIFHWADDR,&ifr);
    memcpy(temp_mac,ifr.ifr_ifru.ifru_hwaddr.sa_data,6);
    sprintf(cce_mac,"%02x:%02x:%02x:%02x:%02x:%02x",temp_mac[0],temp_mac[1],temp_mac[2],temp_mac[3],temp_mac[4],temp_mac[5]);
    printf("MAC Address is %s\r\n",cce_mac);


    /*
     * Display bitstream information located inside the flash memory
     */
#ifdef PERSEUS
    printf("\nBitstream configurations in flash memory\n");
    fpgaflash_print_info();
#endif
    
    if(bInvalidCarrier == 1) goto skip_detection;
    
    /*
     * Scan all available FMC modules
     */
	res = Carrier_FmcDiscover(hCarrier, printout);
	if(FAILURE(res))
	{
		fprintf(stderr, "Carrier_FmcDiscover FAILED: 0x%08X\n",res);
	}

    /*
     * Open all FMC module instance possible
     */
    res = Carrier_GetFmcConnectorNumMax(hCarrier, &uLastConnector);
	if(FAILURE(res))
	{
		printf("Carrier_GetFmcConnectorNumMax FAILED: 0x%08X\n",res);
		return (res);
	}

	res = Carrier_GetFmcBoardNumMax(hCarrier, &uLastBoard);
	if(FAILURE(res))
	{
		printf("Carrier_GetFmcBoardNumMax FAILED: 0x%08X\n",res);
		return (res);
	}

	printf("\r\nFPGA module enumeration\r\n");

    for (iCarrierConnector = Carrier_eFmcConnector0; iCarrierConnector < uLastConnector; iCarrierConnector++)
    {
        // For each board instance that we may have in a slot
        for (iCarrierBoard = Carrier_eFmcBoard0, CarrierFoundAll = 0; iCarrierBoard < uLastBoard && CarrierFoundAll == 0; iCarrierBoard++)
        {
            unsigned int inst = iCarrierConnector * uLastBoard + iCarrierBoard;

            // Fetch core base address
            res = Carrier_GetModuleBaseAddr(hCarrier, Carrier_eModuleFMC, inst, &uiModuleBaseAddr);
            if(FAILURE(res)) goto error_end;

            // By default did not find anything in that board position
            CarrierDetected = CARRIER_NOT_DETECTED;

            
            // For each Radio640 module id type that could fit at a board position in the slot
            for (CarrierModList = (FMCEEPROM_EEPROM_MOD_RADIO640E + iCarrierBoard); CarrierModList <= FMCEEPROM_EEPROM_MOD_RADIO640 && CarrierDetected != CARRIER_EXACT_DETECTED; CarrierModList++)
            {
                // Check for this module type presence
                res = Carrier_FmcValidatePresence(hCarrier, iCarrierConnector, iCarrierBoard, &CarrierModList, sizeof(CarrierModList)/sizeof(FMCEEprom_module_types), &CarrierDetected);
                if(FAILURE(res)) fprintf(stderr, "CARRIER_validate_presence() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);

                // Did we exactly found this module type (for module operating in mode FMCEEPROM_EEPROM_OPER_SAFE)
                if (CarrierDetected == CARRIER_EXACT_DETECTED)
                {
                    // Get the board hardware revision
                    res = Carrier_FmcGetInfo(hCarrier, iCarrierConnector, iCarrierBoard,
                        CARRIER_INFO_HARDREV,
                        acHardRevision[inst],
                        sizeof(acHardRevision[inst]));
                    if(FAILURE(res)) fprintf(stderr, "CARRIER_get_info() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);

                    // Get the board lane calib info
                    // Not needed when using the Radio640
                    // res = Carrier_GetSignalsInfo(hCarrier, iCarrierConnector, iCarrierBoard, &CalibDelay[inst]);
                    // if(FAILURE(res)) fprintf(stderr, "CARRIER_get_signals_info() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);

                    // Open the Radio640 module
                    debug("Radio640_Open() connector: %d, board: %d, type: %d\n",iCarrierConnector+1, iCarrierBoard+1, CarrierModList - FMCEEPROM_EEPROM_MOD_RADIO640E);
                    res = Radio640_Open(&ahRadio640[inst],
                        uiModuleBaseAddr,
                        hCarrier);
                    if(FAILURE(res)) fprintf(stderr, "Radio640_Open() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);
                    debug("Radio640_Open() instance addr: 0x%x\n",(unsigned)(&ahRadio640[inst]));

                    // If a top module type found in the board position, nothing else in the slot
                    if (CarrierModList == FMCEEPROM_EEPROM_MOD_RADIO640)
                        CarrierFoundAll = 1;

                    printf("    -Radio640 connector %d, board %d. Version is %d.%d\r\n",
                        iCarrierConnector+1, iCarrierBoard+1,
                        ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF00) >> 8,
                        ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF));
                    iFpgaModuleCnt++;
#ifdef PERSEUS                   
                    // Set PPSSync parameters
                    if (iCarrierBoard == Carrier_eFmcBoard0)
                    {
                        PPSSyncBoardHandle[iCarrierConnector] = ahRadio640[inst];
                        PPSSync_pfDriveDac[iCarrierConnector] = (PPSSync_pfDriveDac_t)Radio640_TuneOsc;
                    }
#endif                    
                }
            }
            if (CarrierDetected == CARRIER_EXACT_DETECTED) continue;
#ifdef PERSEUS
            // For each mo1000 module id type that could fit at a board position in the slot
            for (CarrierModList = (FMCEEPROM_EEPROM_MOD_MO1000E + iCarrierBoard); CarrierModList <= FMCEEPROM_EEPROM_MOD_MO1000 && CarrierDetected != CARRIER_EXACT_DETECTED; CarrierModList++)
            {
                // Check for this module type presence
                res = Carrier_FmcValidatePresence(hCarrier, iCarrierConnector, iCarrierBoard, &CarrierModList, sizeof(CarrierModList)/sizeof(FMCEEprom_module_types), &CarrierDetected);
                if(FAILURE(res)) fprintf(stderr, "CARRIER_validate_presence() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);

                // Did we exactly found this module type (for module operating in mode FMCEEPROM_EEPROM_OPER_SAFE)
                if (CarrierDetected == CARRIER_EXACT_DETECTED)
                {
                    // Get the board hardware revision
                    res = Carrier_FmcGetInfo(hCarrier, iCarrierConnector, iCarrierBoard,
                        CARRIER_INFO_HARDREV,
                        acHardRevision[inst],
                        sizeof(acHardRevision[inst]));
                    if(FAILURE(res)) fprintf(stderr, "CARRIER_get_info() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);

                    // Get the board lane calib info
                    res = Carrier_GetSignalsInfo(hCarrier, iCarrierConnector, iCarrierBoard, &CalibDelay[inst]);
                    if(FAILURE(res)) fprintf(stderr, "CARRIER_get_signals_info() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);

                    // Open the mo1000 module
                    debug("Mo1000_Open() connector: %d, board: %d, type: %d\n",iCarrierConnector+1, iCarrierBoard+1, CarrierModList - FMCEEPROM_EEPROM_MOD_MO1000E);
                    res = Mo1000_Open(&Mo1000Board[inst],
                                           (Mo1000_stRegisterMap *)uiModuleBaseAddr,
                                           hCarrier,
                                           acHardRevision[inst],
                                           Mo1000IoDelayRef,
                                           &Mo1000Board[iCarrierConnector * uLastBoard].ucDataRefIoDelay,
                                           &Mo1000Board[iCarrierConnector * uLastBoard].ucDciIoDelay,
                                           &Mo1000Board[iCarrierConnector * uLastBoard].ucDciInverted,
                                           &CalibDelay[inst].delta[0],
                                           &Mo1000Board[iCarrierConnector * uLastBoard].usRefDelay,
                                           &Mo1000Board[iCarrierConnector * uLastBoard].usDciDelay);
                    if(FAILURE(res)) fprintf(stderr, "Mo1000_Open() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);
                    debug("Mo1000_Open() instance addr: 0x%x\n",(unsigned)(&Mo1000Board[inst]));

                    // If a top module type found in the board position, nothing else in the slot
                    if (CarrierModList == FMCEEPROM_EEPROM_MOD_MO1000)
                        CarrierFoundAll = 1;

                    printf("    -MO1000 connector %d, board %d. Version is %d.%d\r\n",
                        iCarrierConnector+1, iCarrierBoard+1,
                        ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF00) >> 8,
                        ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF));
                    iFpgaModuleCnt++;
                }
            }
            if (CarrierDetected == CARRIER_EXACT_DETECTED) continue;

            // For each mi125 module id type that could fit at a board position in the slot
            for (CarrierModList = (FMCEEPROM_EEPROM_MOD_MI125E + iCarrierBoard); CarrierModList <= FMCEEPROM_EEPROM_MOD_MI125 && CarrierDetected != CARRIER_EXACT_DETECTED; CarrierModList++)
            {                
                // Check for this module type presence
                res = Carrier_FmcValidatePresence(hCarrier, iCarrierConnector, iCarrierBoard,
                    aMi125ModList[CarrierModList-FMCEEPROM_EEPROM_MOD_MI125E],
                    sizeof(aMi125ModList[CarrierModList-FMCEEPROM_EEPROM_MOD_MI125E])/sizeof(FMCEEprom_module_types),
                    &CarrierDetected);
                if(FAILURE(res)) fprintf(stderr, "CARRIER_validate_presence() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);

                // Did we exactly found this module type (for module operating in mode FMCEEPROM_EEPROM_OPER_SAFE)
                if (CarrierDetected == CARRIER_EXACT_DETECTED)
                {
                    unsigned int uRefBoardInst = inst;
                    unsigned int uRefBoardFound = 0;
                    unsigned int uClockMaster = 0;
                    
                    // Get the board lane calib info
                    res = Carrier_GetSignalsInfo(hCarrier, iCarrierConnector, iCarrierBoard, &CalibDelay[inst]);
                    if(FAILURE(res)) fprintf(stderr, "CARRIER_get_signals_info() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);

                    // Open the mi125 module
                    debug("MI125_mi125_open() connector: %d, board: %d, type: %d\n",iCarrierConnector+1, iCarrierBoard+1, CarrierModList - FMCEEPROM_EEPROM_MOD_MI125E);
                    res = MI125_mi125_open(
                        &mi125_board[inst],
                        (MI125_mi125_Register_Map *)uiModuleBaseAddr,
                        hCarrier);
                    if(FAILURE(res)) fprintf(stderr, "MI125_mi125_open() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);
                    debug("MI125_mi125_open() instance addr: 0x%x\n",(unsigned)(&mi125_board[inst]));

                    while(uRefBoardFound == 0)
                    {
                        // Find which board is clock master
                        res = MI125_mi125_get_clkmaster(&mi125_board[uRefBoardInst], &uClockMaster);
                        if(FAILURE(res)) 
                        {
                            // Not a mi125 board.
                            uClockMaster = 0;
                        }
                        debug("Trying to detect mi125 clock master at instance #%d\n",uRefBoardInst);
                        
                        // If clock master is current board, we found it at uRefBoardInst
                        if(uClockMaster) 
                        {
                            debug("    Clock master successfully found at instance #%d!\n",uRefBoardInst);
                            uRefBoardFound = 1; 
                            break;
                        }
                        
                        // If we reach the last instance and does not find the clock master,
                        // use the current instance
                        if(uRefBoardInst == 0) 
                        {
                            debug("    Clock master failed to be found!\n",0);
                            uRefBoardInst = inst; 
                            uRefBoardFound = 1; 
                            break;
                        }
                        
                        /*
                         * Examples: 
                         *  "Top 1 Slave" -> "Bottom 1 Master"
                         *  "Top 2 Slave" -> "Bottom 2 Master"
                         *  "Top 2 Slave" -> "Bottom 2 Slave" -> "Bottom 1 Master"
                         */
                        
                        // If the board is not at the bottom position, try the bottom position
                        if(uRefBoardInst % uLastBoard) 
                        {
                            uRefBoardInst -= (uRefBoardInst % uLastBoard);
                        }
                        // If the board is at the bottom position, try the previous board at the bottom position
                        else 
                        {
                            uRefBoardInst -= uLastBoard;
                        }
                    }
                    
                    // Proceed lane delays calibration
                    res = MI125_mi125_set_datapaths_calibration(
                        &mi125_board[inst],
                        Mi125IoDelayRef,
                        &CalibDelay[inst].delta[0],
                        &mi125_board[uRefBoardInst].ucDataRefIoDelay,
                        &mi125_board[uRefBoardInst].usRefDelay,
                        &mi125_board[uRefBoardInst].usDcoDelay,
            		    &mi125_board[uRefBoardInst].ucBitslipIndex);
                    if(FAILURE(res)) fprintf(stderr, "MI125_mi125_set_datapaths_calibration() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);

                    // If a top module type found in the board position, nothing else in the slot
                    if (CarrierModList == FMCEEPROM_EEPROM_MOD_MI125)
                        CarrierFoundAll = 1;

                    printf("    -MI125 connector %d, board %d. Version is %d.%d\r\n",
                        iCarrierConnector+1, iCarrierBoard+1,
                        ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF00) >> 8,
                        ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF));
                    iFpgaModuleCnt++;
                }
            }
            if (CarrierDetected == CARRIER_EXACT_DETECTED) continue;

            // For each fmclvds module id type that could fit at a board position in the slot
            for (CarrierModList = (FMCEEPROM_EEPROM_MOD_LVDSE + iCarrierBoard); CarrierModList <= FMCEEPROM_EEPROM_MOD_LVDS && CarrierDetected != CARRIER_EXACT_DETECTED; CarrierModList++)
            {
                // Check for this module type presence
                res = Carrier_FmcValidatePresence(hCarrier, iCarrierConnector, iCarrierBoard, &CarrierModList, sizeof(CarrierModList)/sizeof(FMCEEprom_module_types), &CarrierDetected);
                if(FAILURE(res)) fprintf(stderr, "CARRIER_validate_presence() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);

                // Did we exactly found this module type (for module operating in mode FMCEEPROM_EEPROM_OPER_SAFE)
                if (CarrierDetected == CARRIER_EXACT_DETECTED)
                {
                    debug("fmclvds_open() connector: %d, board: %d, type: %d\n",iCarrierConnector+1, iCarrierBoard+1, CarrierModList - FMCEEPROM_EEPROM_MOD_LVDSE);
                    res= fmclvds_open(
                        &fmclvds_handle[inst],
                        (fmclvds_core *)uiModuleBaseAddr,
                        inst,
                        hCarrier);
                    if(FAILURE(res)) fprintf(stderr, "FMCLVDS_Open() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);
                    debug("fmclvds_open() instance addr: %u\n",(unsigned)(&fmclvds_handle[inst]));

                    res= FMCLVDS_pcf8574_open(
                        &(fmclvds_com_handle[inst]),
                        &(fmclvds_handle[inst].com));
                    if(FAILURE(res)) fprintf(stderr, "FMCLVDS_pcf8574_open() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);
                    debug("FMCLVDS_pcf8574_open() instance addr: 0x%x\n",(unsigned)(&(fmclvds_com_handle[inst])));

                    // If a top module type found in the board position, nothing else in the slot
                    if (CarrierModList == FMCEEPROM_EEPROM_MOD_LVDS)
                        CarrierFoundAll = 1;

                    printf("    -FMC LVDS connector %d, board %d. Version is %d.%d\r\n",
                        iCarrierConnector+1, iCarrierBoard+1,
                        ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF00) >> 8,
                        ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF));
                    iFpgaModuleCnt++;

                    continue;
                }
            }
            if (CarrierDetected == CARRIER_EXACT_DETECTED) continue;

            // For each fmcradio module id type that could fit at a board position in the slot
            for (CarrierModList = (FMCEEPROM_EEPROM_MOD_FMCRADIOE + iCarrierBoard); CarrierModList <= FMCEEPROM_EEPROM_MOD_FMCRADIOS && CarrierDetected == CARRIER_NOT_DETECTED; CarrierModList++)
            {
                // Check for this module type presence
                res = Carrier_FmcValidatePresence(hCarrier, iCarrierConnector, iCarrierBoard,
                    aRadioModList[CarrierModList-FMCEEPROM_EEPROM_MOD_FMCRADIOE],
                    sizeof(aRadioModList[CarrierModList-FMCEEPROM_EEPROM_MOD_FMCRADIOE])/sizeof(FMCEEprom_module_types),
                    &CarrierDetected);
                if(FAILURE(res)) fprintf(stderr, "CARRIER_validate_presence() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);

                // Did we found this module type
                if (CarrierDetected != CARRIER_NOT_DETECTED)
                {
                    //FMC Radio
                    debug("fmc_radio_sdr_open() connector: %d, board: %d, type: %d\n",iCarrierConnector+1, iCarrierBoard+1, CarrierModList - FMCEEPROM_EEPROM_MOD_FMCRADIOE);
                    res = fmc_radio_sdr_open(&fmc_radio_handle[inst], (fmc_radio_core *)uiModuleBaseAddr, FMCRADIOREVSDR_C, usleep);
                    if(FAILURE(res)) fprintf(stderr, "fmc_radio_sdr_open() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);
                    debug("fmc_radio_sdr_open() instance addr: 0x%x\n",(unsigned)(&fmc_radio_handle[inst]));

                    printf("    -Radio420 connector %d, board %d. Version is %d.%d\r\n",
                        iCarrierConnector+1, iCarrierBoard+1,
                        ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF00) >> 8,
                        ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF));
                    iFpgaModuleCnt++;
#ifdef PERSEUS                    
                    // Set PPSSync parameters
                    if (iCarrierBoard == Carrier_eFmcBoard0)
                    {
                        PPSSyncBoardHandle[iCarrierConnector] = &(fmc_radio_handle[inst]);
                        PPSSync_pfDriveDac[iCarrierConnector] = (PPSSync_pfDriveDac_t)fmc_radio_tune_osc;
                    }
#endif 
                    continue;
                }
            }
            if (CarrierDetected != CARRIER_NOT_DETECTED) continue;

            // For other single slot modules that could fit at bottom position only
            if (iCarrierBoard != Carrier_eFmcBoard0) continue;

            /*
             * ADAC250
             */
            CarrierModList = FMCEEPROM_EEPROM_MOD_ADAC250;

            // Check for this module type presence
            res = Carrier_FmcValidatePresence(hCarrier, iCarrierConnector, iCarrierBoard,
                &CarrierModList,
                sizeof(CarrierModList)/sizeof(FMCEEprom_module_types),
                &CarrierDetected);
            if(FAILURE(res)) fprintf(stderr, "CARRIER_validate_presence() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);

            // Did we found this module type
            if (CarrierDetected != CARRIER_NOT_DETECTED)
            {
                debug("ADAC250_Open() connector: %d, board: %d\n",iCarrierConnector+1, iCarrierBoard+1);
                res = ADAC250_Open(
                    uiModuleBaseAddr,
                    usleep,
                    &adac250[iCarrierConnector]);
                if(FAILURE(res)) fprintf(stderr, "ADAC250_Open() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);
                else
                {
                    // No more board to find in this slot
                    CarrierFoundAll = 1;
                }
                debug("ADAC250_Open() instance addr: 0x%x\n",(unsigned)(&adac250[iCarrierConnector]));

                printf("    -ADAC250 connector %d. Version is %d.%d\r\n",
                    iCarrierConnector+1,
                    ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF00) >> 8,
                    ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF));
				iFpgaModuleCnt++;
                
                // Set PPSSync parameters
                if (iCarrierBoard == Carrier_eFmcBoard0)
                {
                    PPSSyncBoardHandle[iCarrierConnector] = adac250[iCarrierConnector];
                    PPSSync_pfDriveDac[iCarrierConnector] = (PPSSync_pfDriveDac_t)ADAC250_PLL_setRefClkTuning;
                }

                continue;
            }

            /*
             * ADC5000
             */
            CarrierModList = FMCEEPROM_EEPROM_MOD_ADC5000;

            // Check for this module type presence
            res = Carrier_FmcValidatePresence(hCarrier, iCarrierConnector, iCarrierBoard,
                &CarrierModList,
                sizeof(CarrierModList)/sizeof(FMCEEprom_module_types),
                &CarrierDetected);
            if(FAILURE(res)) fprintf(stderr, "CARRIER_validate_presence() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);

            // Did we found this module type
            if (CarrierDetected != CARRIER_NOT_DETECTED)
            {
                debug("adc5000_open() connector: %d, board: %d\n",iCarrierConnector+1, iCarrierBoard+1);
                res = adc5000_open(
                    &adc5000board[iCarrierConnector],
                    (adc5000_core *)uiModuleBaseAddr,
                    hCarrier);
                if(FAILURE(res)) fprintf(stderr, "adc5000_open() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);
                else
                {
                    debug("adc5000board: %p\n", adc5000board[iCarrierConnector]);
                    // No more board to find in this slot
                    CarrierFoundAll = 1;
                }
                debug("adc5000_open() instance addr: 0x%x\n",(unsigned)(&adc5000board[iCarrierConnector]));

                printf("    -ADC5000 connector %d. Version is %d.%d\r\n",
                    iCarrierConnector+1,
                    ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF00) >> 8,
                    ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF));
				iFpgaModuleCnt++;

                continue;
            }

            /*
             * MI250
             */
            CarrierModList = FMCEEPROM_EEPROM_MOD_MI250;

            // Check for this module type presence
            res = Carrier_FmcValidatePresence(hCarrier, iCarrierConnector, iCarrierBoard,
                &CarrierModList,
                sizeof(CarrierModList)/sizeof(FMCEEprom_module_types),
                &CarrierDetected);
            if(FAILURE(res)) fprintf(stderr, "CARRIER_validate_presence() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);

            // Did we found this module type
            if (CarrierDetected != CARRIER_NOT_DETECTED)
            {
                // Open the MI250 board object
                debug("MI250_Open() connector: %d, board: %d\n",iCarrierConnector+1, iCarrierBoard+1);
                res= MI250_Open(uiModuleBaseAddr, usleep, &mi250_fmcboard[iCarrierConnector]);
                if(FAILURE(res)) fprintf(stderr, "MI250_Open() error connector: %d, board: %d, code: 0x%08X\n",iCarrierConnector+1, iCarrierBoard+1, res);
                else
                {
                    // No more board to find in this slot
                    CarrierFoundAll = 1;
                }
                debug("MI250_Open() instance addr: 0x%x\n",(unsigned)(&mi250_fmcboard[iCarrierConnector]));

                printf("    -MI250 connector %d. Version is %d.%d\r\n",
                    iCarrierConnector+1,
                    ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF00) >> 8,
                    ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF));
				iFpgaModuleCnt++;
                
                // Get the board lane calib info if it is available.
                // This is only used during a clock slave module data lane calibration.
                res = Carrier_GetSignalsInfo(hCarrier, iCarrierConnector, iCarrierBoard, &CalibDelay[inst]);
                if(SUCCESS(res))
                {
                    MI250_SetLaneDelay(mi250_fmcboard[iCarrierConnector], &CalibDelay[inst]);
                }
                
                // The clock slave module will use the calibration result from the clock master module
                // The clock master module is always the one at the "FMC Connector 0" when one MI250
                // board is the clock slave of the other MI250 board.
                MI250_SetMasterCalibResult(mi250_fmcboard[iCarrierConnector], mi250_fmcboard[0]);

                continue;
            }
            
#ifdef FMCOMMS3_IN_USE
            // Try opening FMCOMMS3
            {
                res = Fmcomms3_open(&ahFmcomms3[iCarrierConnector],    
                    uiModuleBaseAddr, uiModuleBaseAddr + 0x8000, usleep);
                if(SUCCESS(res))
                {
                    CarrierFoundAll = 1;
                    
                    debug("Fmcomms3_open() instance addr: 0x%x\n",(unsigned)(&ahFmcomms3[iCarrierConnector]));

                    printf("    -FMCOMMS3 connector %d. Version is %d.%d\r\n",
                        iCarrierConnector+1,
                        ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF00) >> 8,
                        ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF));
                    iFpgaModuleCnt++;
                    
                    continue;
                }
            }
#endif //FMCOMMS3_IN_USE
#endif //PERSEUS
            // No more board to find in this slot
            CarrierFoundAll = 1;
            debug("No valid fmc module with expected fpga core found in connector: %d, board: %d\n\n",iCarrierConnector+1, iCarrierBoard+1);
        }
    }
#ifdef PERSEUS
    /*
     * LVDS
     */
    debug("LVDS Open\n",0);

    // Fetch module max size
    res = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModuleLVDS, &uiModuleMaxNum);
    if(FAILURE(res)) {uiModuleMaxNum = 0;}
    debug("LVDS num : %d\n",uiModuleMaxNum);

    for(i=0; i<uiModuleMaxNum; i++)
    {
        // Fetch core base address
        debug("LVDS get addr\n",0);
        res = Carrier_GetModuleBaseAddr(hCarrier, Carrier_eModuleLVDS, i, &uiModuleBaseAddr);
        if(FAILURE(res)) goto error_end;
        debug("LVDS addr : 0x%x\n",uiModuleBaseAddr);

        if (sigsetjmp(sj_env, 1)) {
            printf("No LVDS core detected...\n");
        } else {
            res = lvds_open(
                &lvds_handle[i], 
                (lvds_core *)uiModuleBaseAddr,
                usleep);
            debug("lvds_open() instance 0 addr: 0x%x\n",(unsigned)(&lvds_handle[i]));

            if(FAILURE(res)) debug("LVDS Core %d is not present\n",i);

            if(SUCCESS(lvds_presence(&lvds_handle[i])))
            {
                printf("    -LVDS %d core. Version is %d.%d\r\n",
                    i,
                    (lvds_handle[i].core->coreversid.coreversion >> 8) & 0xFF,
                    lvds_handle[i].core->coreversid.coreversion & 0xFF);
                iFpgaModuleCnt++;
            }
        }
    }
    debug("LVDS section done\n",0);

    /*
     * PPS Sync
     */
    // Fetch module max size
    debug("PPS Sync get num\n",0);
    res = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModulePpsSync, &uiModuleMaxNum);
    if(FAILURE(res)) {uiModuleMaxNum = 0;}
    debug("PPS Sync num : %d\n",uiModuleMaxNum);

    for(i=0; i<uiModuleMaxNum; i++)
    {
        // Fetch core base address
        res = Carrier_GetModuleBaseAddr(hCarrier, Carrier_eModulePpsSync, i, &uiModuleBaseAddr);
        if(FAILURE(res)) goto error_end;
        debug("PPS Sync addr : 0x%x\n",uiModuleBaseAddr);

        if (sigsetjmp(sj_env, 1)) {
            printf("No PPS sync core detected...\n");
        } else {
            res = PPSSync_Open(&PPSSync[i], uiModuleBaseAddr,
                PPSSync_pfDriveDac[i], PPSSyncBoardHandle[i]);
            debug("PPSSync_Open() instance addr: %u\n",(unsigned)(&PPSSync[i]));
            if(FAILURE(res))
            {
                printf("[ERR ] Failed to open PPSSYNC object\n");
            }
            if((*(volatile unsigned int*)(uiModuleBaseAddr)) >> 16 == PERSEUS_PPS_SYNC_COREID)
            {
                printf("    -PPSSYNC %d core. Version is %d.%d\r\n",
                    i,
                    ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF00) >> 8,
                    (*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF);
                iFpgaModuleCnt++;
                
            }
        }
    }
  
    /*
     * Aurora
     */
    // Fetch module max size
    res = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModuleAurora, &uiModuleMaxNum);
    if(FAILURE(res)) {uiModuleMaxNum = 0;}
    debug("Aurora num : %d\n",uiModuleMaxNum);

    for(i=0; i<uiModuleMaxNum; i++)
    {
        unsigned int version;

        debug("Get Aurora addr\n",0);
        res = Carrier_GetModuleBaseAddr(hCarrier, Carrier_eModuleAurora, i, &uiModuleBaseAddr);
        if(FAILURE(res)) goto error_end;
        debug("Aurora addr : 0x%x\n",uiModuleBaseAddr);

        if (sigsetjmp(sj_env, 1)) {
            printf("No Aurora core detected...\n");
        } else {
            res = Aurora_Open(&aurora[i], uiModuleBaseAddr, usleep);
            debug("aurora_open() instance %d addr: 0x%x\n", i, (unsigned)(&(aurora[i])));
    
            res = Aurora_GetVersion(&aurora[i], &version);
            if(SUCCESS(res))
            {
                printf("    -Aurora %d core. Version is %d.%d\r\n",
                    i,
                    (version >> 8) & 0xFF,
                    version & 0xFF);
                iFpgaModuleCnt++;
            }
        }
    }

    /*
     * RTDEx Ethernet
     */
    debug("Get RTDEx Eth addr\n",0);
    res = Carrier_GetModuleBaseAddr(hCarrier, Carrier_eModuleRTDExGigE, 0, &uiModuleBaseAddr);
    if(!FAILURE(res))
    {
        debug("RTDEx Eth addr : 0x%x\n",uiModuleBaseAddr);
        if((*(volatile unsigned int*)(uiModuleBaseAddr)) >> 16 == RTDEX_COREID)
        {
              debug("Print RTDEx Eth version\n",0);
              
          printf("    -Ethernet RTDEx core. Version is %d.%d\r\n",
                  ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF00) >> 8,
                  ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF));
          iFpgaModuleCnt++;
        }
    }

    /*
     * Record and playback
     */
    debug("Get RecPlay addr\n",0);
    res = Carrier_GetModuleBaseAddr(hCarrier, Carrier_eModuleRecPlay, 0, &uiModuleBaseAddr);
    if(!FAILURE(res))
    {
        debug("RecPlay addr : 0x%x\n",uiModuleBaseAddr);

        if((*(volatile unsigned int*)(uiModuleBaseAddr)) >> 16 == RECPLAY_ACCESS_COREID)
        {
              unsigned int uiDdr3I2cBaseAddr;
          int size;
          printf("    -Record/Playback core. Version is %d.%d\r\n",
                  ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF00) >> 8,
                  ((*(volatile unsigned int*)(uiModuleBaseAddr)) & 0xFF));
          iFpgaModuleCnt++;

              debug("Get DDR3 I2C addr\n",0);
              res = Carrier_GetModuleBaseAddr(hCarrier, Carrier_eModuleDdr3I2C, 0, &uiDdr3I2cBaseAddr);
              if(SUCCESS(res))
              {
                  debug("DDR3 I2C addr : 0x%x\n",uiDdr3I2cBaseAddr);
                  
                  size = memory_verify_size(uiDdr3I2cBaseAddr);
                  if(size == 17)
                  {
                      *(volatile unsigned int*)(uiModuleBaseAddr + RECPLAY_ACCESS_DDR3SIZE_INDEX) = 1;
                      printf("    -DDR3 Size is 1 GByte\r\n");
                  }
                  else if(size == 33)
                  {
                      *(volatile unsigned int*)(uiModuleBaseAddr + RECPLAY_ACCESS_DDR3SIZE_INDEX) = 4;
                      printf("    -DDR3 Size is 4 GBytes\r\n");
                  }
                  else
                  {
                      *(volatile unsigned int*)(uiModuleBaseAddr + RECPLAY_ACCESS_DDR3SIZE_INDEX) = 0;
                      printf("    -DDR3 Size is unverified\r\n");
                  }

      #ifdef DISPLAY_DDR3_SODIMM_SPD
                  printf("\n");
                  printf("DDR3 SODIMM SPD\n");
                  for(i=0;i<32;i++)
                  {
                      printf("%d : 0x%X \n",i,read_sodimm_spd_register(uiDdr3I2cBaseAddr,i));
                  }
                  printf("\n");
      #endif /*DISPLAY_DDR3_SODIMM_SPD*/
              }

        }
    }

    /*
     * Timestamp
     */
    res = Carrier_GetModuleBaseAddr(hCarrier, Carrier_eModuleTimestamp, 0, &uiModuleBaseAddr);
    if(!FAILURE(res))
    {

        res = Timestamp_Open(&hTimestamp, uiModuleBaseAddr, usleep);
        if(FAILURE(res))
            printf("\n\r Failed to open Timestamp handle\r\n");
        else
            debug("Timestamp_Open() instance addr: %u\n",(unsigned)(&hTimestamp));

        if(SUCCESS(Timestamp_Presence(hTimestamp)))
        {
            unsigned int uiVersion;

            if(SUCCESS(Timestamp_GetVersion(hTimestamp, &uiVersion)))
            {
                printf("    -Timestamp core. Version is %d.%d\r\n",
                    (uiVersion >> 8) & 0xFF,
                    uiVersion & 0xFF);
                iFpgaModuleCnt++;
            }
        }
    }

    /*
     * System Monitor
     */
    res = Carrier_GetModuleBaseAddr(hCarrier, Carrier_eModuleSysMon, 0, &uiModuleBaseAddr);
    if(!FAILURE(res))
    {
        res = Sysmon_Open(&hSysmon, uiModuleBaseAddr);
        if(FAILURE(res))
            printf("\n\r Failed to open System Monitor handle\r\n");
        else
            debug("Sysmon_Open() instance addr: %u\n",(unsigned)(&hSysmon));
    }

#endif //PERSEUS
    if(iFpgaModuleCnt == 0)
	{
		printf("    -No module detected\r\n");
	}
    
    
skip_detection:

    while(1)
    {
        connection_state * tcp_con;
        pthread_t thread_id;

        tcp_con = calloc(1, sizeof(connection_state));
        sock_size = sizeof(cad);
		debug("Waiting connection\n", 0);
        if ( (tcp_con->stream = accept(socks, (struct sockaddr *)&cad, &sock_size)) < 0)
        {
            fprintf(stderr, "Accept failed\n");
            exit(-1);
        }
		debug("Connected\n", 0);
        //spawn thread here, if we need to communicate to multiple client at once
        i = pthread_create(&thread_id, NULL, (void *) &cce_mainloop, tcp_con);
		pthread_detach(thread_id);
        debug("pthread_create: %i\n", i);
    }
#ifdef PERSEUS 
    Sysmon_Close(hSysmon);
    Timestamp_Close(hTimestamp);
#endif //PERSEUS
    //fmc_radio_sdr_close(&fmc_radio);
    //ADAC250_Close(adac250);
    //munmap(register_space);
    //munmap(fmc_space);
    return OK;


error_end :
    fprintf(stderr, "BAS Error : 0x%x\n", res);
    return res;
}


/*--------------------------------------------------------
--       Private Functions Declaration                  --
--------------------------------------------------------*/

Result _MapMemorySpace()
{
    Result res;

    /*
     *  Populate global variables
     */
    res = Carrier_GetMemSpaceBaseAddr(hCarrier, Carrier_eMemSpaceUser, &register_begin);
    if(FAILURE(res)) return res;
    res = Carrier_GetMemSpaceSize(hCarrier, Carrier_eMemSpaceUser, &register_size);
    if(FAILURE(res)) return res;
    register_end = register_begin + register_size - 1;
    debug("register space: 0x%x - 0x%x\n", register_begin, register_end);

    res = Carrier_GetMemSpaceBaseAddr(hCarrier, Carrier_eMemSpaceFMC, &fmc_begin);
    if(FAILURE(res)) return res;
    res = Carrier_GetMemSpaceSize(hCarrier, Carrier_eMemSpaceFMC, &fmc_size);
    if(FAILURE(res)) return res;
    fmc_end = fmc_begin + fmc_size - 1;
    debug("fmc space: 0x%x - 0x%x\n", fmc_begin, fmc_end);

    res = Carrier_GetModuleBaseAddr(hCarrier, Carrier_eModuleI2C, 0, &fmc_i2c_begin);
    if(FAILURE(res)) return res;
    res = Carrier_GetModuleSize(hCarrier, Carrier_eModuleI2C, &fmc_i2c_size);
    if(FAILURE(res)) return res;
    fmc_i2c_end = fmc_i2c_begin + fmc_i2c_size - 1;
    debug("fmc i2c space: 0x%x - 0x%x\n", fmc_i2c_begin, fmc_i2c_end);
    
    res = Carrier_GetModuleBaseAddr(hCarrier, Carrier_eModuleDPRAM, 0, &dpram_begin);
    if(FAILURE(res))
    {
        dpram_begin = 0;
        dpram_end = 0;
        dpram_size = 0;
        debug("No DPRAM module detected\n");
    }
    else
    {
        res = Carrier_GetModuleSize(hCarrier, Carrier_eModuleDPRAM, &dpram_size);
        if(FAILURE(res)) return res;
        dpram_end = dpram_begin + dpram_size - 1;
        debug("DPRAM space: 0x%x - 0x%x\n", dpram_begin, dpram_end);
    }
    
    res = Carrier_GetModuleBaseAddr(hCarrier, Carrier_eModuleDdr3I2C, 0, &ddr3_i2c_begin);
    if(FAILURE(res)) 
    {
        ddr3_i2c_begin = 0;
        ddr3_i2c_end = 0;
        ddr3_i2c_size = 0;
        debug("No DDR3 I2C module detected\n");
    }
    else
    {
        res = Carrier_GetModuleSize(hCarrier, Carrier_eModuleDdr3I2C, &ddr3_i2c_size);
        if(FAILURE(res)) return res;
        ddr3_i2c_end = ddr3_i2c_begin + ddr3_i2c_size - 1;
        debug("ddr3 i2c space: 0x%x - 0x%x\n", ddr3_i2c_begin, ddr3_i2c_end);
    }

    /*
     *  User space
     */
    register_space = mmap((void*)register_begin, register_size, PROT_READ | PROT_WRITE, MAP_SHARED, dev_mem, register_begin);
    if( register_space == MAP_FAILED )
    {
        char error[512];
        fprintf(stderr, "mmap() failed\n");
        perror(error);
        printf("%s\n", error);
        return -1;
    }
    debug("register_space: %p - %p\n", register_space, register_end);

    /*
     *  FMC module space
     */
    fmc_space = mmap((void*)fmc_begin, fmc_size, PROT_READ | PROT_WRITE, MAP_SHARED, dev_mem, fmc_begin);
    if( fmc_space == MAP_FAILED )
    {
        char error[512];
        fprintf(stderr, "mmap() failed\n");
        perror(error);
        printf("%s\n", error);
        return -1;
    }
    debug("fmc_space: %p - %p\n", fmc_space, fmc_end);

    /*
     *  I2C module space
     */
    fmc_i2c_space = mmap((void*)fmc_i2c_begin, fmc_i2c_size, PROT_READ | PROT_WRITE, MAP_SHARED, dev_mem, fmc_i2c_begin);
    if( fmc_i2c_space == MAP_FAILED )
    {
        char error[512];
        fprintf(stderr, "mmap() failed\n");
        perror(error);
        printf("%s\n", error);
        return -1;
    }
    debug("fmc_i2c_space: %p - %p\n", fmc_i2c_space, fmc_i2c_end);

    /*
     *  DDR3 space
     */
    if(dpram_begin != 0)
    {
        dpram_space = mmap((void*)dpram_begin, dpram_size, PROT_READ | PROT_WRITE, MAP_SHARED, dev_mem, dpram_begin);
        if( dpram_space == MAP_FAILED )
        {
            char error[512];
            fprintf(stderr, "mmap() failed\n");
            perror(error);
            printf("%s\n", error);
            return -1;
        }
        debug("dpram_space: %p - %p\n", dpram_space, dpram_end);
    }

    /*
     *  DDR3 I2C sodimm space
     */
    if(ddr3_i2c_begin != 0)
    {
        ddr3_i2c_space = mmap((void*)ddr3_i2c_begin, ddr3_i2c_size, PROT_READ | PROT_WRITE, MAP_SHARED, dev_mem, ddr3_i2c_begin);
        if( ddr3_i2c_space == MAP_FAILED )
        {
            char error[512];
            fprintf(stderr, "mmap() failed\n");
            perror(error);
            printf("%s\n", error);
            return -1;
        }
        debug("ddr3_i2c_space: %p - %p\n", ddr3_i2c_space, ddr3_i2c_end);
    }

    return OK;
}
