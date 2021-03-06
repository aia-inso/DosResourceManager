/*
 **************************************************
 *   版本号：1.0
 *   作者：黄子昊
 *   生成日期：2020-9-4
 *   说明：与鼠标相关的部分函数
 **************************************************
 */

#ifndef __IMOUSE_H
#define __IMOUSE_H

//库文件
#include<STDLIB.H>
#include<GRAPHICS.H>
#include<DOS.H>
#include<CONIO.H>

//辅助模块
#include"IDefs.h"


//函数原型
void IMouseMath(int (*mouseDraw)[16]);
void IMouseOn(int x,int y,int (*mouseDraw)[16],int (*pixelSave)[16]);
void IMouseOff(int x,int y,int (*mouseDraw)[16],int (*pixelSave)[16]);
void IMouseSetLimit(int xMax,int yMax);
void IMouseGetXY(int*,int*);
int IMouseLeftRelease(void);
int ILeftPress(void);
int IRightPress(void);
int IMouseStatus(int*,int*,int (*mouseDraw)[16],int (*pixelSave)[16]);

#endif