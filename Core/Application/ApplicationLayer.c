/*********************************.FILE_HEADER.*******************************
<Copyright Notice>
.File : Application.c
.Summary : Application layer main header file
.Note :

Author Date Description
------------------------------------------------------------------------------
<Author name> <DDMMMYYYY> <changes made>*/
/*Aldrin.Rebellow 29/03/2026 Initial Release*/
#include "main.h"
#include "Drv_ET6226.h"
#include "ApplicationLayer.h"

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void ApplicationLayer_Init(void)
{
	ET6226M_Init();
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void ApplicationLayer_Exe(void)
{
	ET6226M_RefreshDisplay();
}
