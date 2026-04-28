/*********************************.FILE_HEADER.*******************************
<Copyright Notice>
.File : HI_Systick.c
.Summary : Hardware interface file for systick timer
.Note :
 
Author Date Description
------------------------------------------------------------------------------
<Author name> <DDMMMYYYY> <changes made>*/
/*Aldrin.Rebellow 24/04/2025 Initial Release*/
#ifndef GENERALAPI_H
#define GENERALAPI_H
long Map_LineraInterpolate_Positive(long x, long in_min, long in_max, long out_min, long out_max);
long Map_LineraInterpolate_Negative(long x, long in_min, long in_max, long out_min, long out_max);
#endif