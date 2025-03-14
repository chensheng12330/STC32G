/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCAI.com ---------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- BBS: www.STCAIMCU.com  -----------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
/*---------------------------------------------------------------------*/

#include "STC32G_PWM.h"

//========================================================================
// 函数: PWMA_ISR_Handler
// 描述: PWMA中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2023-04-15
//========================================================================
void PWMA_ISR_Handler (void) interrupt PWMA_VECTOR
{
	// TODO: 在此处添加用户代码
    if (PWMA_SR1 & 0x01)    //UIFA 更新中断
    {
        PWMA_SR1 &= ~0x01;

    }
    if (PWMA_SR1 & 0x02)    //CC1IF 捕获/比较中断
    {
        PWMA_SR1 &= ~0x02;

    }
    if (PWMA_SR1 & 0x04)    //CC2IF 捕获/比较中断
    {
        PWMA_SR1 &= ~0x04;

    }
    if (PWMA_SR1 & 0x08)    //CC3IF 捕获/比较中断
    {
        PWMA_SR1 &= ~0x08;

    }
    if (PWMA_SR1 & 0x10)    //CC4IF 捕获/比较中断
    {
        PWMA_SR1 &= ~0x10;

    }
    if (PWMA_SR1 & 0x20)    //COMIFA 中断
    {
        PWMA_SR1 &= ~0x20;

    }
    if (PWMA_SR1 & 0x40)    //TIFA 触发器中断
    {
        PWMA_SR1 &= ~0x40;

    }
    if (PWMA_SR1 & 0x80)    //BIFA 刹车中断
    {
        PWMA_SR1 &= ~0x80;

    }
}

//========================================================================
// 函数: PWMB_ISR_Handler
// 描述: PWMB中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2023-04-15
//========================================================================
void PWMB_ISR_Handler (void) interrupt PWMB_VECTOR
{
	// TODO: 在此处添加用户代码
    if (PWMB_SR1 & 0x01)    //UIFB 更新中断
    {
        PWMB_SR1 &= ~0x01;

    }
    if (PWMB_SR1 & 0x02)    //CC5IF 捕获/比较中断
    {
        PWMB_SR1 &= ~0x02;

    }
    if (PWMB_SR1 & 0x04)    //CC6IF 捕获/比较中断
    {
        PWMB_SR1 &= ~0x04;

    }
    if (PWMB_SR1 & 0x08)    //CC7IF 捕获/比较中断
    {
        PWMB_SR1 &= ~0x08;

    }
    if (PWMB_SR1 & 0x10)    //CC8IF 捕获/比较中断
    {
        PWMB_SR1 &= ~0x10;

    }
    if (PWMB_SR1 & 0x20)    //COMIFB 中断
    {
        PWMB_SR1 &= ~0x20;

    }
    if (PWMB_SR1 & 0x40)    //TIFB 触发器中断
    {
        PWMB_SR1 &= ~0x40;

    }
    if (PWMB_SR1 & 0x80)    //BIFB 刹车中断
    {
        PWMB_SR1 &= ~0x80;

    }
}


