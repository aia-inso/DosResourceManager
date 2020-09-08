/*
    版本号：1.0
    作者：黄子昊
    生成日期：2020-9-8
    说明：主要定义系统提示音函数
*/


#include<DOS.H>
#include"ISound.h"


/*
    函数功能：发出系统提示音
    输入参数：无
    输出参数：无
    返回值：无
*/
void IWarningBeep()
{
    sound(700);
    delay(200);
    sound(300);
    delay(200);
    nosound();
}