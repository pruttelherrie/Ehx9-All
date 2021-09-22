/*
__ __| |           |  /_) |     ___|             |           |
  |   __ \   _ \  ' /  | |  / |      _ \ __ \   |      _` | __ \   __|
  |   | | |  __/  . \  |   <  |   |  __/ |   |  |     (   | |   |\__ \
 _|  _| |_|\___| _|\_\_|_|\_\\____|\___|_|  _| _____|\__,_|_.__/ ____/
  -----------------------------------------------------------------------------
  USBMIDIKLIK 4X4 - USB Midi advanced firmware for STM32F1 platform.
  Copyright (C) 2019 by The KikGen labs.
  LICENCE CREATIVE COMMONS - Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)

  This file is part of the USBMIDIKLIK-4x4 distribution
  https://github.com/TheKikGen/USBMidiKliK4x4
  Copyright (c) 2019 TheKikGen Labs team.
  -----------------------------------------------------------------------------
  Disclaimer.

  This work is licensed under the Creative Commons Attribution-NonCommercial 4.0 International License.
  To view a copy of this license, visit http://creativecommons.org/licenses/by-nc/4.0/
  or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.

  NON COMMERCIAL - PERSONAL USE ONLY : You may not use the material for pure
  commercial closed code solution without the licensor permission.

  You are free to copy and redistribute the material in any medium or format,
  adapt, transform, and build upon the material.

  You must give appropriate credit, a link to the github site
  https://github.com/TheKikGen/USBMidiKliK4x4 , provide a link to the license,
  and indicate if changes were made. You may do so in any reasonable manner,
  but not in any way that suggests the licensor endorses you or your use.

  You may not apply legal terms or technological measures that legally restrict
  others from doing anything the license permits.

  You do not have to comply with the license for elements of the material
  in the public domain or where your use is permitted by an applicable exception
  or limitation.

  No warranties are given. The license may not give you all of the permissions
  necessary for your intended use.  This program is distributed in the hope that
  it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*/
///////////////////////////////////////////////////////////////////////////////
//  EEPROM UTILITIES
//-----------------------------------------------------------------------------
// Due to the unusual make process of Arduino platform, this file is directly
// included in the main ino one.
///////////////////////////////////////////////////////////////////////////////
//#pragma once

///////////////////////////////////////////////////////////////////////////////
//  FUNCTIONS PROTOTYPES
///////////////////////////////////////////////////////////////////////////////
void EEPROM_ParamsInit(bool factorySettings=false);
void EEPROM_Put(uint8_t* ,uint16_t );
void EEPROM_Get(uint8_t* ,uint16_t );
void EEPROM_ParamsLoad();
void EEPROM_ParamsSave();


///////////////////////////////////////////////////////////////////////////////
// Intialize parameters stores in EEPROM
//----------------------------------------------------------------------------
// Retrieve global parameters from EEPROM, or Initalize it
// If factorySetting is true, all settings will be forced to factory default
//////////////////////////////////////////////////////////////////////////////
void EEPROM_ParamsInit(bool factorySettings)
{

  // Read the EEPROM parameters structure
  EEPROM_ParamsLoad();

  // If the signature is not found, of not the same version of parameters structure,
  // or new version, or new size then initialize (factory settings)

  // New fimware  uploaded
  if ( memcmp(EEPROM_Params.TimestampedVersion,TimestampedVersion,sizeof(EEPROM_Params.TimestampedVersion)) ) {
      factorySettings = true;
  }

  // Force factory setting
  if (  factorySettings ) {
  Serial.println("forcing factory settings.");
    memset( &EEPROM_Params,0,sizeof(EEPROM_Params_t) );
    memcpy( EEPROM_Params.signature,EE_SIGNATURE,sizeof(EEPROM_Params.signature) );

    EEPROM_Params.majorVersion = VERSION_MAJOR;
    EEPROM_Params.minorVersion = VERSION_MINOR;

    EEPROM_Params.prmVersion = EE_PRMVER;
    EEPROM_Params.size = sizeof(EEPROM_Params_t);

    memcpy( EEPROM_Params.TimestampedVersion,TimestampedVersion,sizeof(EEPROM_Params.TimestampedVersion) );

    EEPROM_Params.debugMode = false;

    //EEPROM_Params.program[2]=6;

     
    //Write the whole param struct
    EEPROM_ParamsSave();

  } else {    
    Serial.println("settings found and read.");
    Serial.print("value: ");
    
    Serial.println(EEPROM_Params.userprograms[2].program);
    
  }
}

///////////////////////////////////////////////////////////////////////////////
// Optimized put byte buffer to EEPROM
//////////////////////////////////////////////////////////////////////////////
void EEPROM_Put(uint8_t* bloc,uint16_t sz)
{
  uint16_t addressWrite = 0x10;
  uint8_t * pp = bloc;
  //uint16_t value = 0;

  // Write 2 uint8_t in an uint16t
  for (uint16_t i=1; i<=sz; i++) {
//      if ( i % 2 ) {
 //       value = *pp;
//        value = value << 8;
//      } else {
//        value |= *pp;
//        EEPROM.write(addressWrite++, value);
        EEPROM.write(addressWrite++, *pp);
//      }
      pp++;
  }
  // Proceed the last write eventually
//  if (sz % 2 ) EEPROM.write(addressWrite, value);
}

///////////////////////////////////////////////////////////////////////////////
// Optimized get byte buffer to EEPROM
//////////////////////////////////////////////////////////////////////////////
void EEPROM_Get(uint8_t* bloc,uint16_t sz)
{
  uint16_t addressRead = 0x10;
  uint8_t * pp = bloc;
  //uint8_t value = 0;

  // Read 2 uint8_t in an uint16t
  for (uint16_t i=1; i<=sz; i++) {
      //if ( i % 2 ) {
      *pp = EEPROM.read(addressRead++);
      //  value=EEPROM.read(addressRead++);
      //  *pp = value >> 8;
      //} else {
      //  *pp = value & 0x00FF;
      //}
      pp++;
  }
}

///////////////////////////////////////////////////////////////////////////////
// HIGH LEVEL LOAD PARAMETERS FROM EEPROM
//----------------------------------------------------------------------------
// High level abstraction parameters read function
//////////////////////////////////////////////////////////////////////////////
void EEPROM_ParamsLoad()
{

  EEPROM_Get((uint8_t *)&EEPROM_Params,sizeof(EEPROM_Params_t));

  // copy to RAM version
  //memcpy(userList,EEPROM_Params.userprograms,sizeof(EEPROM_Params.userprograms) );

}

///////////////////////////////////////////////////////////////////////////////
// HIGH LEVEL SAVE PARAMETERS TO EEPROM
//----------------------------------------------------------------------------
// High level abstraction parameters read function
//////////////////////////////////////////////////////////////////////////////
void EEPROM_ParamsSave()
{

  // copy RAM version to params struct
  //memcpy( EEPROM_Params.userprograms,userList,sizeof(EEPROM_Params.userprograms) );
  EEPROM_Put((uint8_t *)&EEPROM_Params,sizeof(EEPROM_Params_t)) ;
  Serial.println("stored all programs in EEPROM.");
}
