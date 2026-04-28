/*********************************.FILE_HEADER.*******************************
<Copyright Notice>
.File : HI_Systick.c
.Summary : Hardware interface file for systick timer
.Note :
 
Author Date Description
------------------------------------------------------------------------------
<Author name> <DDMMMYYYY> <changes made>*/
/*Aldrin.Rebellow 24/04/2024 Initial Release*/
#include "main.h"
#include "GeneralApi.h"

/******************************.FUNCTION_HEADER.******************************
.Purpose : 
.Returns :
.Note :
******************************************************************************/
long Map_LineraInterpolate_Positive(long x, long in_min, long in_max, long out_min, long out_max)
{
    // perform the linear interpolation
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : 
.Returns :
.Note :
******************************************************************************/
long Map_LineraInterpolate_Negative(long x, long in_min, long in_max, long out_min, long out_max)
{
    // perform the linear interpolation
    return (in_max - x) * (out_max - out_min) / (in_max - in_min) + out_min;
}
