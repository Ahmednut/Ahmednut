#ifndef __CCE_FMCVITA__
#define __CCE_FMCVITA__
#include "cce.h"
#include "bas_error.h"


Result FMCEEprom_eeprom_read_recv(connection_state * state);


Result FMCEEprom_eeprom_write_recv(connection_state * state);


Result FMCEEprom_eeprom_get_idstring_recv(connection_state * state);


Result FMCEEprom_eeprom_get_manuf_recv(connection_state * state);


Result FMCEEprom_eeprom_get_board_state_recv(connection_state * state);


Result FMCEEprom_eeprom_get_board_type_recv(connection_state * state);


Result FMCEEprom_eeprom_get_core_recv(connection_state * state);


Result FMCEEprom_eeprom_get_carriervadj_recv(connection_state * state);


Result FMCEEprom_eeprom_get_partnum_recv(connection_state * state);


Result FMCEEprom_eeprom_get_serialnum_recv(connection_state * state);


Result FMCEEprom_eeprom_get_hardrev_recv(connection_state * state);


Result FMCEEprom_eeprom_get_firmrev_recv(connection_state * state);


Result FMCEEprom_eeprom_get_modvadj_recv(connection_state * state);


Result FMCEEprom_eeprom_get_corename_recv(connection_state * state);


Result FMCEEprom_eeprom_wipe_recv(connection_state * state);

/** @name FMCVITA Error codes
 * 
 *  List all runtime related error codes of a FMCVITA module.
 */
enum FMCVITA_error {
    FMCVITA_OK                     = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_FMCVITA, 0), ///< No error
    FMCVITA_ERR_INVALID_EEPROMNUM  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCVITA, 1),   ///< Too many eeprom available or invalid 2402 eeprom types
    FMCVITA_ERR_EEPROM_NOTBLANK    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCVITA, 2),   ///< Eeeprom is not blanked and cannot be overrided
    FMCVITA_ERR_INVALID_EEPROMSIZE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCVITA, 3),   ///< Eeeprom size specified in buffer is invalid
    FMCVITA_ERR_INVALID_NOEEPROM   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCVITA, 4),   ///< No eeprom detected
    FMCVITA_ERR_INVALID_PASSWORD   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCVITA, 5)    ///< Invalid password for a protected feature
};


#endif


