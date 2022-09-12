/****************************************************************************  
 *
 *    ****                              *                                    
 *   ******                            ***                                   
 *   *******                           ****                                  
 *   ********    ****  ****     **** *********    ******* ****    ***********
 *   *********   ****  ****     **** *********  **************  *************
 *   **** *****  ****  ****     ****   ****    *****    ****** *****     ****
 *   ****  ***** ****  ****     ****   ****   *****      ****  ****      ****
 *  ****    *********  ****     ****   ****   ****       ****  ****      ****
 *  ****     ********  ****    *****  ****    *****     *****  ****      ****
 *  ****      ******   ***** ******   *****    ****** *******  ****** *******
 *  ****        ****   ************    ******   *************   *************
 *  ****         ***     ****  ****     ****      *****  ****     *****  ****
 *                                                                       ****
 *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       ****
 *                                                                        ***
 *
 ************************************************************************//**
 *
 * @file       mi250_monitoring.c 
 * @brief      MI250 configuration functions API for the monitoring
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup MI250
 *
 * This file is part of Nutaq’s BAS Software Suite.
 *
 * You may at your option receive a license to Nutaq’s BAS Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq’s BAS Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq’s BAS Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq’s BAS Software Suite under the GPL requires that your work based on
 * Nutaq’s BAS Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq’s BAS Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq’s BAS Software Suite.
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/
#include "mi250.h"
#include "mi250_user.h"
#include "mi250_ADT7411_drv.h"
#include <string.h> //memset
#include <stdio.h>

// Defines
#define SCALE_FACTOR_3_3V_3_8V  2
#define SCALE_FACTOR_VCP        3
#define SCALE_FACTOR_12V        7

#define MAX_MEAS_VDD            7
#define TEMPR_CONV_DIVIDER      4

// Function prototypes
char convertToTwoComplement(char number);
Result readAINMeas(unsigned i2cBaseAddr, unsigned char i2cDeviceAddr, 
                   float refVoltage, MONITOR_MEAS_ID measId, 
                   float * measValue, int * binMeasValue);
Result readVddMeas(unsigned i2cBaseAddr, unsigned char i2cDeviceAddr, 
                   float * measValue, int * binMeasValue);
Result readTemperatureMeas(unsigned i2cBaseAddr, unsigned char i2cDeviceAddr, 
                           MONITOR_MEAS_ID measId, 
                           float * measValue, int * binMeasValue);

char convertToTwoComplement(char number)
{
    char convResult;
    
    // Positive number stay the same.
    convResult = number;

    // Negative number is converted this way. 
    // Take the positive number, invert all bits, add one to the result or
    // take the number as is, substract 256.
    if((number & 0x80) == 1) {
        convResult = number - 256;
    }
    
    return(convResult);
}

Result readAINMeas(unsigned i2cBaseAddr, unsigned char i2cDeviceAddr, 
                   float refVoltage, MONITOR_MEAS_ID measId, 
                   float * measValue, int * binMeasValue)
{
    int result;
    float oneLSBSize;
    float convertedValue;
    int AINValue; 
    ADT7411_Register_Map_t reg;
    unsigned char regValueLsb;
    unsigned char regValueMsb;
    unsigned short regAddr;
    int scaleFactor;     

    memset(&reg, 0, sizeof(reg));
    scaleFactor = 1;

    // Get the measurement value.
    // Read lsb value.
    switch(measId) {
        case MON_AIN3: {
            regAddr = ADT7411_REGISTER_0A;
            result = ADT7411_readSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_04, &reg.Register_04.Byte);
            if(result == -1) {
                return(result);
            }
            regValueLsb = reg.Register_04.Bit.lsbAIN3;
            break;
        } 
        case MON_AIN4: {
            regAddr = ADT7411_REGISTER_0B;
            result = ADT7411_readSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_04, &reg.Register_04.Byte);
            if(result == -1) {
                return(result);
            }
            regValueLsb = reg.Register_04.Bit.lsbAIN4;
            scaleFactor = SCALE_FACTOR_3_3V_3_8V;
            break;
        } 
        case MON_AIN5: {
            regAddr = ADT7411_REGISTER_0C;
            result = ADT7411_readSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_05, &reg.Register_05.Byte);
            if(result == -1) {
                return(result);
            }
            regValueLsb = reg.Register_05.Bit.lsbAIN5;
            scaleFactor = SCALE_FACTOR_3_3V_3_8V;
            break;
        } 
        case MON_AIN6: {
            regAddr = ADT7411_REGISTER_0D;
            result = ADT7411_readSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_05, &reg.Register_05.Byte);
            if(result == -1) {
                return(result);
            }
            regValueLsb = reg.Register_05.Bit.lsbAIN6;
            scaleFactor = SCALE_FACTOR_3_3V_3_8V;
            break;
        } 
        case MON_AIN7: {
            regAddr = ADT7411_REGISTER_0E;
            result = ADT7411_readSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_05, &reg.Register_05.Byte);
            if(result == -1) {
                return(result);
            }
            regValueLsb = reg.Register_05.Bit.lsbAIN7;
            scaleFactor = SCALE_FACTOR_VCP;
            if(i2cDeviceAddr == MI250_MON_DEVICE2_ADDRESS) {
                scaleFactor = SCALE_FACTOR_12V;
            }
            break;
        } 
        case MON_AIN8:  {
            regAddr = ADT7411_REGISTER_0F;
            result = ADT7411_readSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_05, &reg.Register_05.Byte);
            if(result == -1) {
                return(result);
            }
            regValueLsb = reg.Register_05.Bit.lsbAIN8;
            scaleFactor = SCALE_FACTOR_3_3V_3_8V;
            break;
        }
    }

    // Read Msb register.
    result = ADT7411_readSingleReg(i2cBaseAddr, i2cDeviceAddr, regAddr, &regValueMsb);
    if(result == -1) {
        return(result);
    }

    // Rebuild the binary value (10 bits).
    AINValue = 0;
    AINValue = (regValueMsb << 2) | regValueLsb;
    *binMeasValue = AINValue;
    
    // Convert read value to voltage.
    oneLSBSize = refVoltage / (float)1024;
    convertedValue = ((float)AINValue * oneLSBSize) * (float)scaleFactor;
    *measValue = convertedValue;
    
    return(0);
}

Result readVddMeas(unsigned i2cBaseAddr, unsigned char i2cDeviceAddr, 
                   float * measValue, int * binMeasValue)
{
    int result;
    float oneLSBSize;
    float convertedValue;
    float refVoltage;
    int vddValue; 
    ADT7411_Register_Map_t reg;
    unsigned char regValueLsb;
    unsigned char regValueMsb;
    float scaleFactor;     

    memset(&reg, 0, sizeof(reg));

    // Get the measurement value.
    // Read lsb value.
    result = ADT7411_readSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_03, &reg.Register_03.Byte);
    if(result == -1) {
        return(result);
    }
    regValueLsb = reg.Register_03.Bit.lsbVdd;

    // Read Msb register.
    result = ADT7411_readSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_06, &regValueMsb);
    if(result == -1) {
        return(result);
    }

    // Rebuild the binary value (10 bits).
    vddValue = 0;
    vddValue = (regValueMsb << 2) | regValueLsb;
    *binMeasValue = vddValue;
    
    // Convert read value to voltage.
    refVoltage = (float)2.25;
    oneLSBSize = refVoltage / (float)1024;
    //!!!!CL To add ??? Not sure... scaleFactor2 = SCALE_FACTOR_3_3V_3_8V;
    scaleFactor = (float)MAX_MEAS_VDD / refVoltage;
    convertedValue = ((float)vddValue * oneLSBSize) * (float)scaleFactor;
    *measValue = convertedValue;
    
    return(0);
}


Result readTemperatureMeas(unsigned i2cBaseAddr, unsigned char i2cDeviceAddr, 
                           MONITOR_MEAS_ID measId, 
                           float * measValue, int * binMeasValue)
{
    int result;
    float oneLSBSize;
    float convertedValue;
    int temprValue; 
    ADT7411_Register_Map_t reg;
    unsigned char regValueLsb;
    unsigned char regValueMsb;
    unsigned short regAddr;

    memset(&reg, 0, sizeof(reg));

    // Get the measurement value.
    // Read lsb value.
    switch(measId) {
        case MON_INTERNAL_TEMPERATURE: {
            regAddr = ADT7411_REGISTER_07;
            result = ADT7411_readSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_03, &reg.Register_03.Byte);
            if(result == -1) {
                return(result);
            }
            regValueLsb = reg.Register_03.Bit.lsbIntTempr;
            break;
        } 
        case MON_EXTERNAL_TEMPERATURE: {
            regAddr = ADT7411_REGISTER_08;
            result = ADT7411_readSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_04, &reg.Register_04.Byte);
            if(result == -1) {
                return(result);
            }
            regValueLsb = reg.Register_04.Bit.lsbExtTempr;
            break;
        }
    }

    // Read Msb register.
    result = ADT7411_readSingleReg(i2cBaseAddr, i2cDeviceAddr, regAddr, &regValueMsb);
    if(result == -1) {
        return(result);
    }

    // Rebuild the binary value (10 bits).
    temprValue = 0;
    temprValue = (regValueMsb << 2) | regValueLsb;
    *binMeasValue = temprValue;
    
    // Convert read value to voltage.
    if((temprValue & 0x00000200) == 0) {
        convertedValue = (float)temprValue / (float)TEMPR_CONV_DIVIDER;
    }
    else {
        // Remove the sign.
        temprValue = temprValue & 0x000001FF;
        convertedValue = (float)(temprValue - 512) / (float)TEMPR_CONV_DIVIDER;
    }

    *measValue = convertedValue;
    
    return(0);
}

Result MI250_MON_setMonitoringOnOff(unsigned i2cBaseAddr, unsigned char i2cDeviceAddr, unsigned char monitorEnable)
{
    int result;
    ADT7411_Register_Map_t reg;
    memset(&reg, 0, sizeof(reg));

    result = ADT7411_readSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_18, &reg.Register_18.Byte);
    if(result == -1) {
        return(result);
    }
    reg.Register_18.Bit.startMonitoring = monitorEnable;
    result = ADT7411_writeSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_18, &reg.Register_18.Byte);

    return(result);
}

Result MI250_MON_setConfiguration(unsigned i2cBaseAddr, unsigned char i2cDeviceAddr, 
                                   unsigned char interruptEnable, unsigned char interruptPolarity, 
                                   unsigned char SMBusTimeoutEnable, unsigned char averageEnable, 
                                   unsigned char conversionCycleType, unsigned char singleChnl, 
                                   unsigned char ADCRef, unsigned char ADCSpeed)
{
    int result;
    ADT7411_Register_Map_t reg;
    memset(&reg, 0, sizeof(reg));

    result = ADT7411_readSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_18, &reg.Register_18.Byte);
    if(result == -1) {
        return(result);
    }
    reg.Register_18.Bit.interruptPolarity = interruptPolarity;
    reg.Register_18.Bit.enableInterrupt = (~interruptEnable) & 0x01;
    reg.Register_18.Bit.selectAIN1andAIN2OrExtTempr = ADT7411_EXT_TEMPR_SELECT;
    result = ADT7411_writeSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_18, &reg.Register_18.Byte);
    if(result == -1) {
        return(result);
    }

    result = ADT7411_readSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_19, &reg.Register_19.Byte);
    if(result == -1) {
        return(result);
    }
    reg.Register_19.Bit.enableSMBusTimeout = SMBusTimeoutEnable;
    reg.Register_19.Bit.enableAveraging = (~averageEnable) & 0x01;
    reg.Register_19.Bit.selectRoudRobSingleChnl = conversionCycleType;
    reg.Register_19.Bit.selectSingleChannel = singleChnl;
    result = ADT7411_writeSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_19, &reg.Register_19.Byte);
    if(result == -1) {
        return(result);
    }

    result = ADT7411_readSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_1A, &reg.Register_1A.Byte);
    if(result == -1) {
        return(result);
    }
    reg.Register_1A.Bit.selectADCRef = ADCRef;
    reg.Register_1A.Bit.selectADCSpeed = ADCSpeed;
    result = ADT7411_writeSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_1A, &reg.Register_1A.Byte);
    
    return(result);
}

Result MI250_MON_init(unsigned i2cBaseAddr, unsigned char i2cDeviceAddr)
{
    int result;
    int tempr;
    ADT7411_Register_Map_t reg;
    memset(&reg, 0, sizeof(reg));

    // Default setting: register 18 Control configuration 1
    // Interrupt polarity is active low.
    // Interrupt output is enabled.
    result = ADT7411_readSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_18, &reg.Register_18.Byte);
    if(result == -1) {
        return(result);
    }
    // Set Pin7 and Pin8 to external temperature.
    reg.Register_18.Bit.selectAIN1andAIN2OrExtTempr = ADT7411_EXT_TEMPR_SELECT;
    reg.Register_18.Bit.reserved2 = 1;
    reg.Register_18.Bit.enableInterrupt = 1;
    result = ADT7411_writeSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_18, &reg.Register_18.Byte);
    if(result == -1) {
        return(result);
    }

    // Default setting: register 19 Control configuration 2
    // SMBus timeout is disabled.
    // Averaging is enabled.
    // Conversion cycle type is round robin.
    // Single channel selection is Vdd.

    // Default setting: register 1A Control configuration 3
    // ADC reference is internal Vref.
    // ADC speed is normal 1.4KHz.

    // Disable interrupt AIN2.
    reg.Register_1D.Byte = 0x20;
    result = ADT7411_writeSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_1D, &reg.Register_1D.Byte);
    if(result == -1) {
        return(result);
    }

    // Set internal temperature High limit to 85.
    reg.Register_25.Byte = 85;
    result = ADT7411_writeSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_25, &reg.Register_25.Byte);
    if(result == -1) {
        return(result);
    }

    // Set internal temperature Low limit to -40.
    reg.Register_26.Byte = -40;
    result = ADT7411_writeSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_26, &reg.Register_26.Byte);
    if(result == -1) {
        return(result);
    }

    // Set external temperature High limit to 85.
    reg.Register_27.Byte = 85;
    result = ADT7411_writeSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_27, &reg.Register_27.Byte);
    if(result == -1) {
        return(result);
    }

    // Set external temperature Low limit to -40.
    reg.Register_28.Byte = -40;
    result = ADT7411_writeSingleReg(i2cBaseAddr, i2cDeviceAddr, ADT7411_REGISTER_28, &reg.Register_28.Byte);
    if(result == -1) {
        return(result);
    }
    
    // Start monitoring.
    result = MI250_MON_setMonitoringOnOff(i2cBaseAddr, i2cDeviceAddr, ADT7411_START_MONITORING);
                                           
    if( result == -1 )
      return(result);
    else
      return(0);
}

Result MI250_MON_readMeasurement(unsigned i2cBaseAddr, unsigned char i2cDeviceAddr, 
                                  float refVoltage, MONITOR_MEAS_ID measId, 
                                  float * measValue, int * binMeasValue) 
{
    int result;
    
    switch(measId) {
        case MON_INTERNAL_TEMPERATURE: 
        case MON_EXTERNAL_TEMPERATURE: {
            result = readTemperatureMeas(i2cBaseAddr, i2cDeviceAddr, measId, measValue, binMeasValue);
            break;
        }
        
        case MON_VDD: {
            result = readVddMeas(i2cBaseAddr, i2cDeviceAddr, measValue, binMeasValue);
            break;
        }
        
        case MON_AIN3: 
        case MON_AIN4: 
        case MON_AIN5: 
        case MON_AIN6: 
        case MON_AIN7: 
        case MON_AIN8: {
            result = readAINMeas(i2cBaseAddr, i2cDeviceAddr, refVoltage, measId, measValue, binMeasValue);
            break;
        }
        
        default: {
            result = -1;
            break;
        }
    }
    
    return(result);
}

Result MI250_MON_readSingleReg(unsigned i2cBaseAddr, unsigned char i2cDeviceAddr, unsigned short addr, unsigned char * data)
{
    return(ADT7411_readSingleReg(i2cBaseAddr, i2cDeviceAddr, addr, data));
}

Result MI250_MON_writeSingleReg(unsigned i2cBaseAddr, unsigned char i2cDeviceAddr, unsigned short addr, unsigned char * data)
{
    return(ADT7411_writeSingleReg(i2cBaseAddr, i2cDeviceAddr, addr, data));
}












