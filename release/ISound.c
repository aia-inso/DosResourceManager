/*
    �汾�ţ�1.0
    ���ߣ������
    �������ڣ�2020-9-8
    ˵������Ҫ����ϵͳ��ʾ������
*/


#include<DOS.H>
#include"ISound.h"


/*
    �������ܣ�����ϵͳ��ʾ��
    �����������
    �����������
    ����ֵ����
*/
void IWarningBeep()
{
    sound(700);
    delay(200);
    sound(300);
    delay(200);
    nosound();
}