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
 * @file    ConfigFile.c 
 * @brief   Function definitions for a .ini parser to get all necessary parameters for
 *          a specific application
 * 
 * Copyright (C) 2015, Nutaq, Canada. All rights reserved.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <errno.h>
#include "ConfigFile.h"

static CONFIGFILE_SECTION SplitSection(const char line[], char ** Section, uint32_t * u32NbSections);

/****************************************************************************************************************************
*										     GetConfigParameterValue Definition
****************************************************************************************************************************/
Result ConfigFile_GetParamValue(const char * Filename, const char * Section, const char * Parameter, const char * ValueType, void * pValue, uint32_t ValueMaxSize)
{
	/*** Variables Declaration ***/
	FILE * pFile;
	int res = 0;
	char line[255];
	CONFIGFILE_SECTION eSection = CONFIGFILE_SECTION_NOT_VALID;
	/*** File open and validation ***/
	if(strstr(Filename,".ini") == NULL)
	{
		return CONFIGFILEBADFILEEXTENSION;
	}

	pFile = fopen( (const char *)Filename, "r");
	if (pFile == NULL)
	{		
		//printf("Something went wrong with fopen()! %s\n", strerror(errno));
		return CONFIGFILEFILENOTFOUND;
	}

	/*** Line by line loop until EOF***/
	while ( fgets(line, sizeof(line), pFile) )
	{
		/*** Bypass if line starts with a comment symbol ***/
		if (line[0] == '#' || line[0] == ';')
		{
			continue;
		}

		/*** Section == NULL ***/
		if(Section == NULL)
		{
			//if find '[' at index 0 without having found the parameter => error
			if(line[0] == '[')
			{				
				break;
			}

			res = GetParameterValueInLine(line, Parameter, ValueType, pValue, ValueMaxSize);
			if(FAILURE(res)) return res;
			if (res == 1)
			{				
				break;
			}
		}

		/*** Section != NULL ***/
		else
		{
			//find parameter if in good section
			if(eSection == CONFIGFILE_SECTION_VALID)
			{
				res = GetParameterValueInLine(line, Parameter, ValueType, pValue, ValueMaxSize);
				if(FAILURE(res)) return res;
				if (res == 1)
				{					
					break;
				}	
			}

			//find '[' for a New Section
			if(line[0] == '[')
			{
				eSection = ValidateSection(line, Section);				
			}			
		}
	}

	fclose(pFile);

	if (FAILURE(res)) 
	{
		return res;
	}
	else if (res == CONFIGFILE_PARAM_NOT_FOUND) //parameter not found
	{		
		return CONFIGFILEPARAMNOTFOUND;
	}
	else //parameter found
	{	
		return CONFIGFILEOK;
	}
}

/****************************************************************************************************************************
*								ConfigFile_GetSectionNameFromParameter Definition
****************************************************************************************************************************/
Result ConfigFile_GetSectionNamesFromParameter(const char * Filename, const char * Parameter, char ** Section, uint32_t * u32NbSections)
{
	 /*** Variables Declaration ***/
	FILE * pFile;
	char line[255];
	char lower_line[255];
	char lower_Parameter[255];
	char * param;
	char currentSection[255] = {"NULL\n"};

    *u32NbSections = 0;

	/*** File open and validation ***/
	if(strstr(Filename,".ini") == NULL)
	{
		return CONFIGFILEBADFILEEXTENSION;
	}

	pFile = fopen( (const char *)Filename, "r");
	if (pFile == NULL)
	{		
		return CONFIGFILEFILENOTFOUND;
	}

	/*** Lower Parameter case ***/
	sprintf(lower_Parameter,"%s",Parameter);
	string_lower(lower_Parameter); //Lower case

	/*** Line by line loop until EOF***/
	while ( fgets(line, sizeof(line), pFile) )
	{
		/*** Bypass if line starts with a comment symbol ***/
		if (line[0] == '#' || line[0] == ';')
		{
			continue;
		}

		/*** Lower line case ***/
		sprintf(lower_line,"%s",line);
		string_lower(lower_line); //Lower case

		/*** find '[' for a New Section ***/
		if(lower_line[0] == '[')
		{
			sprintf(currentSection,"%s",line);	
		}

		/*** find Parameter substring in line ***/
		param = strstr((char *)lower_line, lower_Parameter);
		if(param != NULL)
		{
            uint32_t CurrentSectionNb = 0;

            SplitSection(currentSection, &Section[*u32NbSections], &CurrentSectionNb);
			(*u32NbSections)+=CurrentSectionNb;
		}
	}

	return OK;
}

/****************************************************************************************************************************
*										     Static Function Definitions
****************************************************************************************************************************/

/*
 * Returns CONFIGFILE_PARAM_NOT_FOUND if the parameter was not found
 * Returns CONFIGFILE_PARAM_FOUND if the parameter is found, void * pValue is set to value
 */
static Result GetParameterValueInLine(const char line[], const char * Parameter, const char * ValueType, void * pValue, uint32_t ValueMaxSize)
{
	char * param;
	char tmp_line[255];
	char lower_Parameter[255];
	int iNbCopied;
	

	sprintf(tmp_line,"%s",line);	
	string_lower(tmp_line); //Lower case

	sprintf(lower_Parameter,"%s",Parameter);
	string_lower(lower_Parameter); //Lower case

	//find Parameter substring in line
	param = strstr((char *)tmp_line, lower_Parameter);
	if(param != NULL)
	{
		// Check if the Parameter is at index 0
		if(param - tmp_line == 0) 
		{
			//Check if after parameter, it has '='
			if(tmp_line[strlen(Parameter)] == '=')
			{
				if(!strcmp(ValueType, "%s"))
				{					
					if( strlen(&line[strlen(Parameter) + 1]) > ValueMaxSize)
					{
                        printf("%d\n", ValueMaxSize);
						return CONFIGFILEMAXSIZE;
					}
				}

				iNbCopied = sscanf(&line[strlen(Parameter) + 1], ValueType, pValue);

				//Validate if copy was really done
				if ( iNbCopied != 1)
				{
					return CONFIGFILEVALUETYPE;
				}				
				return CONFIGFILE_PARAM_FOUND;
			}
		}				
	}
	return CONFIGFILE_PARAM_NOT_FOUND;
}

/*
 * Returns CONFIGFILE_SECTION_NOT_VALID if the section is not valid
 * Returns CONFIGFILE_SECTION_VALID if the section is valid
 */
static CONFIGFILE_SECTION ValidateSection(const char line[], const char * Section)
{
	char * sect;
	char tmp_line[255];
	char lower_Section[255];

	sprintf(tmp_line,"%s",line);
	string_lower(tmp_line); //Lower case

	sprintf(lower_Section,"%s",Section);
	string_lower(lower_Section); //Lower case

	//Check if Section is in this line
	sect = strstr((char *)tmp_line, lower_Section);
	if(sect != NULL)
	{
		//find if index before Section name in file, it has ',' or '['
		if(tmp_line[sect - tmp_line - 1] == '[' || tmp_line[sect-tmp_line-1] == ',')
		{
			//find if index after Section name in file, it has ',', or ']'
			if(line[sect - tmp_line + strlen(Section)] == ']' || line[sect - tmp_line + strlen(Section)] == ',')
			{							
				return CONFIGFILE_SECTION_VALID;
			}
		}
	}
	return CONFIGFILE_SECTION_NOT_VALID;
}

/*
 * Returns CONFIGFILE_SECTION_NOT_VALID if the section is not valid
 */
static CONFIGFILE_SECTION SplitSection(const char line[], char ** Section, uint32_t * u32NbSections)
{
	char tmp_line[255];
    char * token1;
    const char delim[2] = ",";

	sprintf(tmp_line,"%s",line);

	remove_character_from_line(tmp_line,'[');
	remove_character_from_line(tmp_line,']');
    remove_character_from_line(tmp_line,'\n');	

    *u32NbSections = 0;

    token1 = strtok(tmp_line, delim);
    while (token1)
    {
        strcpy(Section[*u32NbSections],token1);
        (*u32NbSections)++;

        token1 = strtok(0, delim);
    }

    if(*u32NbSections)
	    return CONFIGFILE_SECTION_VALID;
    else
	    return CONFIGFILE_SECTION_NOT_VALID;
}

/*
 * Convert string to lower case
 */
static void string_lower(char string_to_lower[])
{
	int i;

	for(i = 0; string_to_lower[i]; i++)
	{
		string_to_lower[i] = tolower(string_to_lower[i]);
	}
}

/*
 * Remove specific character from string
 */
static void remove_character_from_line(char * line, const char character_to_remove)
{
	int i = 0, c = 0;
	for(; i < strlen(line); i++)
	{
		if (line[i] != character_to_remove)
		{
			line[c] = line[i];
			c++;
		}
	}
	line[c] = '\0';
}