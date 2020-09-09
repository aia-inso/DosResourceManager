/*
    版本号：1.0
    作者：黄子昊
    生成日期：2020-9-8
    说明：视图函数等
*/


#include<GRAPHICS.H>
#include<CONIO.H>
#include<STDIO.H>
#include<STRING.H>
#include<STDLIB.H>
#include"IEvent.h"
#include"IUtility.h"
#include"IDiry.h"
#include"IHanZi.h"
#include"IDraw.h"
#include"IIcon.h"
#include"IActive.h"
#include"IMenu.h"
#include"ISound.h"
#include"IView.h"

/*
    函数功能：画界面背景
    输入参数：无
    输出参数：无
    返回值：无
*/
void IPlainView(FILE* fpHZ)
{
    if(fpHZ==NULL)
    {
        setcolor(0);
        outtextxy(100,100,"fpHZ is NULL in IPlainView");
    }
    setlinestyle(SOLID_LINE,0,NORM_WIDTH);
    settextstyle(1,0,2);

    setfillstyle(SOLID_FILL,255);
    bar(0,0,1024,768);
    setfillstyle(SOLID_FILL,84);
    bar(0,0,1024,22);
    setcolor(0);
    Iouttextxy(10,5,"仿Windows资源管理器",fpHZ);

    setcolor(7);
    line(0,44,1024,44);
    line(0,45,1024,45);
    IPointerDown(1000,32);
    rectangle(152,51,824,78);
    line(803,52,803,76);
    rectangle(832,51,1017,78);
    line(851,51,851,78);
    line(0,84,1024,84);
    line(0,85,1024,85);
    IPointerRight(172,62);
    IRefresh(814,64);
    IMagnifier(841,61);
    IExit(1004,8);

    setcolor(7);
    line(240,88,240,742);
    line(0,744,1024,744);
}

/*
    函数功能：左侧窗口
    输入参数：root——文件根节点, curNode——当前目录二级指针, nodeX——辅助文件节点, top——事件栈, (beginX,beginY)——开始画的位置, page——页码, flag——标志位
    输出参数：无
    返回值：纵坐标的偏移量
*/
int IView0(IFileNode* root,IFileNodePointer ** curNode,IFileNodePointer* nodeX,IEventStackNode* top,int beginX,int beginY,char* page,char flag)
{
    int increaceY=0,temp,n;
    char thisPage=1;
    char tempStr[3];
    IEvent tempEvent;

    if(!root) return 0;         

    if(beginY<110+600*(*page-1)||beginY>110+600*(*page))
        thisPage=0;

    if(thisPage&&(*curNode)->child==root)
    {
        setfillstyle(SOLID_FILL,139);
        bar(0,beginY-(*page-1)*600,238,beginY-(*page-1)*600+22);
    }

    if(IisFolder(root))
    {
        if(!(root->flags&8))         //空文件夹
        {
            if(thisPage)
            {
                Ifolder(beginX+11,beginY-(*page-1)*600+4);
                setcolor(0);
                outtextxy(beginX+25+10,beginY-(*page-1)*600+7,root->file.name);
                ISetEvent(&tempEvent,0,beginY-(*page-1)*600,238,beginY-(*page-1)*600+22,8,IAfterEntree,(IFileNode*)curNode,(IFileNode*)nodeX,6);
                IEventStackPush(top,tempEvent);
                ISetEvent(&tempEvent,0,beginY-(*page-1)*600,238,beginY-(*page-1)*600+22,8,IEntreeActive,root,(IFileNode*)curNode,-1);
                IEventStackPush(top,tempEvent);
            }
            increaceY+=24;
        }
        else if(root->file.type[0]=='0')   //未打开
        {
            if(thisPage)
            {
                IPointerRight(beginX+1,beginY-(*page-1)*600+8);
                if(root->file.type[1]=='d'||root->file.type[1]=='\\')
                    Idisk(beginX+11,beginY-(*page-1)*600+4);
                else
                    Ifolder(beginX+11,beginY-(*page-1)*600+4);
                setcolor(0);
                outtextxy(beginX+25+10,beginY-(*page-1)*600+7,root->file.name);
                ISetEvent(&tempEvent,beginX,beginY-(*page-1)*600+6,beginX+16,beginY-(*page-1)*600+14,2,IAfterEntree,(IFileNode*)curNode,(IFileNode*)nodeX,6);
                IEventStackPush(top,tempEvent);
                ISetEvent(&tempEvent,beginX,beginY-(*page-1)*600+6,beginX+16,beginY-(*page-1)*600+14,2,IEntreeActive,root,(IFileNode*)curNode,-1);
                IEventStackPush(top,tempEvent);
                ISetEvent(&tempEvent,0,beginY-(*page-1)*600,238,beginY-(*page-1)*600+22,8,IAfterEntree,(IFileNode*)curNode,(IFileNode*)nodeX,6);
                IEventStackPush(top,tempEvent);
                ISetEvent(&tempEvent,0,beginY-(*page-1)*600,238,beginY-(*page-1)*600+22,8,IEntreeActive,root,(IFileNode*)curNode,-1);
                IEventStackPush(top,tempEvent);
            }
            increaceY+=24;
        }
        else        //打开的文件夹
        {
            if(thisPage)
            {
                IPointerDown(beginX,beginY+9);
                if(root->file.type[1]=='d'||root->file.type[1]=='\\')
                    Idisk(beginX+11,beginY-(*page-1)*600+4);
                else
                    Ifolder(beginX+11,beginY-(*page-1)*600+4);
                setcolor(0);
                outtextxy(beginX+25+10,beginY-(*page-1)*600+7,root->file.name);
                ISetEvent(&tempEvent,beginX,beginY-(*page-1)*600+6,beginX+16,beginY-(*page-1)*600+14,2,IDetreeActive,root,(IFileNode*)curNode,6);
                IEventStackPush(top,tempEvent);
                ISetEvent(&tempEvent,beginX,beginY-(*page-1)*600+6,beginX+16,beginY-(*page-1)*600+14,2,ISetXNull,root,(IFileNode*)nodeX,-1);
                IEventStackPush(top,tempEvent);
                ISetEvent(&tempEvent,0,beginY-(*page-1)*600,238,beginY-(*page-1)*600+22,8,IDetreeActive,root,(IFileNode*)curNode,6);
                IEventStackPush(top,tempEvent);
                ISetEvent(&tempEvent,0,beginY-(*page-1)*600,238,beginY-(*page-1)*600+22,8,ISetXNull,root,(IFileNode*)nodeX,-1);
                IEventStackPush(top,tempEvent);
            }
            increaceY+=24;
            if(root->child)
            {
                increaceY+=IView0(root->child,curNode,nodeX,top,beginX+8,beginY+increaceY,page,0);
            }
        }
    }
    if(root->next)
    {
        increaceY+=IView0(root->next,curNode,nodeX,top,beginX,beginY+increaceY,page,0);
    }

    if(flag)
    {
        if(*page>1)
        {
            ISetEvent(&tempEvent,150,720,168,738,2,ILastPage,(IFileNode*)page,NULL,2);
            IEventStackPush(top,tempEvent);
            setcolor(0);
        }
        else
            setcolor(LIGHTGRAY);
        IGoLeft(150,720);
        if(*page<=((increaceY-24)/600))
        {
            ISetEvent(&tempEvent,210,720,228,738,2,INextPage,(IFileNode*)page,NULL,2);
            IEventStackPush(top,tempEvent);
            setcolor(0);
        }
        else
            setcolor(LIGHTGRAY);
        IGoRight(210,720);
        sprintf(tempStr,"%d",*page);
        setcolor(0);
        outtextxy(182,722,tempStr);
    }
    //只画一次左右箭头
    return increaceY;
}

/*
    函数功能：右侧窗口
    输入参数：curNode——当前目录二级指针, nodeX——辅助节点指针，用于保存被复制/剪切的节点,top——事件栈, (beginX,beginY)——开始画的位置, page——页码, flag——标志位
    输出参数：无
    返回值：纵坐标的偏移量
*/
int IView1(IFileNodePointer ** curNode,IFileNodePointer* nodeX,IEventStackNode* top,char* page,char* menuFlag,FILE* fpHZ)
{

    int i,y,numOfItem=0,numOfSelected=0;
    IFileNode* tempNode;
    IEvent tempEvent;
    char temp[50];

    settextstyle(0,0,0);
    if(fpHZ==NULL)
    {
        setcolor(0);
        outtextxy(100,100,"fpHZ is NULL in IView1");
    }
    setcolor(0);

    tempNode=(*curNode)->child;
    IGetAbsolutePath(tempNode,temp);
    outtextxy(192,61,temp);
    //路径栏的显示

    ISetEvent(&tempEvent,803,51,824,78,2,INOP,NULL,NULL,6);
    IEventStackPush(top,tempEvent);
    //刷新功能

    if(IFindParent((*curNode)->child))
    {
        setcolor(0);
        ISetEvent(&tempEvent,108,57,120,76,2,IAfterEntree,(IFileNode*)curNode,(IFileNode*)nodeX,6);
        IEventStackPush(top,tempEvent);
        ISetEvent(&tempEvent,108,57,120,76,2,IEntreeActive,IFindParent((*curNode)->child),(IFileNode*)curNode,-1);
        IEventStackPush(top,tempEvent);
    }
    else
        setcolor(LIGHTGRAY);
    IGoUp(108,57);
    //返回上一级功能

    if((*curNode)->pre&&(*curNode)->pre->child)
    {
        setcolor(0);
        ISetEvent(&tempEvent,25,59,40,76,2,IGoLeftActive,(IFileNode*)curNode,NULL,6);
        IEventStackPush(top,tempEvent);
    }
    else
        setcolor(LIGHTGRAY);
    IGoLeft(25,60);
    //返回上一目录功能

    if((*curNode)->next&&(*curNode)->next->child)
    {
        setcolor(0);
        ISetEvent(&tempEvent,62,59,92,76,2,IGoRightActive,(IFileNode*)curNode,NULL,6);
        IEventStackPush(top,tempEvent);
    }
    else
        setcolor(LIGHTGRAY);
    IGoRight(62,60);
    //返回下一目录功能
        
    setcolor(0);
    setfillstyle(SOLID_FILL,255);
    bar(853,52,1016,77);
    if(!tempNode->child)
    {
        Iouttextxy(853,61,"无内容",fpHZ);
        if(!strcmp(tempNode->file.type,"0ds"))
            Iouttextxy(560,240,"仅管理员可见",fpHZ);
        else
            Iouttextxy(530,240,"未打开文件夹或未检索到内容",fpHZ);
        return 0;
    }
    else if(tempNode->file.type[1]=='\\')
        Iouttextxy(853,61,"根目录无法搜索文件",fpHZ);
    else
    {
        Iouttextxy(853,61,"在本文件夹中搜索",fpHZ);
        ISetEvent(&tempEvent,853,52,1016,77,2,ISearchActive,(IFileNode*)(*curNode),NULL,8);
        IEventStackPush(top,tempEvent);
    }
    //搜索栏显示
    
    setcolor(50);
    IPutsHZ16(254,94,"文件名",fpHZ);
    IPutsHZ16(430,94,"修改日期",fpHZ);
    IPutsHZ16(686,94,"类型",fpHZ);
    IPutsHZ16(830,94,"大小",fpHZ);
    setcolor(247);
    line(424,88,424,112);
    line(680,88,680,112);
    line(824,88,824,112);
    line(936,88,936,112);
    setcolor(0);
    //文件详细信息的表头

    tempNode=(*curNode)->child->child;
    while(tempNode)
    {
        numOfItem++;
        if(tempNode->flags&2)
            numOfSelected++;
        tempNode=tempNode->next;
    }
    //统计总文件数和被选中的文件数

    IView1PageControl(curNode,page,numOfItem);
    if(*page==(numOfItem-1)/30+1)
        (*menuFlag)|=8;
    else
        (*menuFlag)&=7;
    //页码控制

    tempNode=(*curNode)->child->child;
    for(i=0;i<(*page-1)*(30);i++)
        tempNode=tempNode->next;
    //跳转至当前页

    y=116;
    while(tempNode)     //对每一个当前页码的文件
    {
        IView1DrawIcon(tempNode,y);
        //根据文件类型，画图标

        setcolor(0);
        Iouttextxy(256+20,y+6,tempNode->file.name,fpHZ);
        sprintf(temp,"%d/%d/%d %02d:%02d",tempNode->file.date/512+1980,(tempNode->file.date%512)/32,tempNode->file.date%32,tempNode->file.time/2048,(tempNode->file.time%2048)/32);
        outtextxy(432,y+6,temp);
        if(!IisFolder(tempNode))
        {
            settextjustify(2,2);
            sprintf(temp,"%d KB",tempNode->file.size);
            outtextxy(928,y+6,temp);
            settextjustify(0,2);
        }
        //文件详细信息

        if((*menuFlag)&4)
            ISetEvent(&tempEvent,248,y,936,y+19,2,ICtrlSelect,tempNode,NULL,4);
        else
            ISetEvent(&tempEvent,248,y,936,y+19,2,ISelect,tempNode,NULL,4);
        IEventStackPush(top,tempEvent);
        //根据是否Ctrl,设置选择类型

        if(IisFolder(tempNode))
        {
            ISetEvent(&tempEvent,248,y,936,y+19,8,IAfterEntree,(IFileNode*)curNode,(IFileNode*)nodeX,6);
            IEventStackPush(top,tempEvent);
            ISetEvent(&tempEvent,248,y,936,y+19,8,IEntreeActive,tempNode,(IFileNode*)curNode,-1);
            IEventStackPush(top,tempEvent);
            if(tempNode->file.type[1]=='d')
                Iouttextxy(688,y+6+3,"磁盘",fpHZ);
            else
                Iouttextxy(688,y+6+3,"文件夹",fpHZ);
        }
        else
        {
            outtextxy(688,y+6,tempNode->file.type);
            if(!strcmp(tempNode->file.type,"EXE")||!strcmp(tempNode->file.type,"BAT"))
            {
                ISetEvent(&tempEvent,248,y,936,y+19,8,IexeActive,tempNode,NULL,6);
                IEventStackPush(top,tempEvent);
            }
            if(!strcmp(tempNode->file.type,"TXT")||!strcmp(tempNode->file.type,"C")||!strcmp(tempNode->file.type,"H")||!strcmp(tempNode->file.type,"CPP")||!strcmp(tempNode->file.type,"ASM"))
            {
                ISetEvent(&tempEvent,248,y,936,y+19,8,ItxtActive,tempNode,NULL,6);
                IEventStackPush(top,tempEvent);
            }
        }
        //文件打开方式

        if(y>682) break;
        y+=20;
        tempNode=tempNode->next;
    }

    sprintf(temp,"%d个项目",numOfItem);
    Iouttextxy(16,752+3,temp,fpHZ);
    if(numOfSelected)
    {
        sprintf(temp,"选中%d个项目",numOfSelected);
        Iouttextxy(160,752+3,temp,fpHZ);
    }
    if((*menuFlag)&4)
    {
        setcolor(144);
        outtextxy(320,752,"CTRL");
    }
    //状态栏
    
    if((*menuFlag)&2)
    {
        (*menuFlag)&=13;
        setcolor(84);
        rectangle(412,334,662,434);
        setfillstyle(SOLID_FILL,84);
        bar(412,334,662,356);
        setfillstyle(SOLID_FILL,255);
        bar(413,357,661,433);
        rectangle(591,403,641,423);
        rectangle(521,403,571,423);

        setcolor(0);
        sprintf(temp,"%s%d%s","确定要删除选中的",numOfSelected,"个项目吗？");
        Iouttextxy(433,377,temp,fpHZ);
        IPutsHZ16(530,406,"确定",fpHZ);
        IPutsHZ16(600,406,"取消",fpHZ);
        IWarningBeep();

        ISetEvent(&tempEvent,0,0,1024,768,2,INOP,NULL,NULL,0);
        IEventStackPush(top,tempEvent);
        ISetEvent(&tempEvent,521,403,571,423,2,ISetDelete,(IFileNode*)(*curNode),(IFileNode*)nodeX,6);
        IEventStackPush(top,tempEvent);
        ISetEvent(&tempEvent,591,403,641,423,2,INOP,NULL,NULL,4);
        IEventStackPush(top,tempEvent);
    }
    //删除确认

    return numOfSelected;
}

/*
    函数功能：根据文件类型，画图标
    输入参数：tempNode——文件节点, y——纵向位置
    输出参数：无
    返回值：无
*/
void IView1DrawIcon(IFileNode* tempNode,int y)
{
    if(tempNode->flags&2)
    {
        setfillstyle(SOLID_FILL,139);
        bar(248,y,936,y+19);
    }
    if(!strcmp(tempNode->file.type,"TXT")||!strcmp(tempNode->file.type,"txt"))
        Itxt(256,y+2);
    else if(!strcmp(tempNode->file.type,"DOC")||!strcmp(tempNode->file.type,"doc"))
        Idoc(256,y+2);
    else if(!strcmp(tempNode->file.type,"C")||!strcmp(tempNode->file.type,"c")||!strcmp(tempNode->file.type,"CPP")||!strcmp(tempNode->file.type,"cpp"))
        Ic(256,y+2);
    else if(!strcmp(tempNode->file.type,"H")||!strcmp(tempNode->file.type,"h")||!strcmp(tempNode->file.type,"HPP")||!strcmp(tempNode->file.type,"hpp"))
        Ih(256,y+2);
    else if(!strcmp(tempNode->file.type,"obj")||!strcmp(tempNode->file.type,"OBJ"))
        Iobj(256,y+2);
    else if(!strcmp(tempNode->file.type,"exe")||!strcmp(tempNode->file.type,"EXE"))
        Iexe(256,y+2);
    else if(!strcmp(tempNode->file.type,"jpg")||!strcmp(tempNode->file.type,"JPG")||!strcmp(tempNode->file.type,"bmp")||!strcmp(tempNode->file.type,"BMP"))
        Ipic(256,y+2);
    else if(IisFolder(tempNode))
    {
        if(tempNode->file.type[1]=='d')
            Idisk(256,y+2);
        else
            Ifolder(256,y+2);
    }
    else
        Imystery(256,y+2);
}

/*
    函数功能：根据文件类型，画图标
    输入参数：curNode——当前目录二级指针, page——页码, numOfItem——项目总数
    输出参数：无
    返回值：无
*/
void IView1PageControl(IFileNodePointer** curNode,char *page,int numOfItem)
{
    static IFileNode* lastCurNode;
    char temp[3];
    
    if(lastCurNode!=(*curNode)->child)
        *page=1;
    else if((numOfItem-1)/30+1<*page)
        (*page)--;
    else if(!(*page))
        *page=1;
    lastCurNode=(*curNode)->child;
    //页码控制

    if(*page<(numOfItem-1)/30+1)
        setcolor(0);
    else
        setcolor(LIGHTGRAY);
    IGoRight(992,720);
    if(*page>1)
        setcolor(0);
    else
        setcolor(LIGHTGRAY);
    IGoLeft(928,720);
    sprintf(temp,"%d",*page);
    setcolor(0);
    outtextxy(962,722,temp);
}

/*
    函数功能：画查找后的显示界面
    输入参数：page——页数, fpHZ——汉字库
    输出参数：无
    返回值：无
*/
void IView2(char* page,FILE* fpHZ)
{

    FILE* searched=fopen("C:\\DOSRES\\ETC\\SEARCH.TXT","r");
    char tempStr[60];
    char name[15];
    int y=120,i,n,j=0,numOfsearched=0;


    setcolor(50);
    IPutsHZ16(250,94,"匹配文件名",fpHZ);
    IPutsHZ16(500,94,"绝对路径",fpHZ);


    while(fgets(tempStr,60,searched))
    {
        if(++numOfsearched>=120)
        {
            Iouttextxy(500,753,"只显示前120个匹配项",fpHZ);
            break;
        }
        //搜索到多于120个匹配项
    }
    if((numOfsearched-1)/30+1<*page)
        (*page)--;
    else if(!(*page))
        *page=1;
    //页码控制
    
    rewind(searched);
    while(fgets(tempStr,60,searched)&&((++j)<=120))
    {
        if((j-1)/30+1<*page)
            continue;
        else if((j-1)/30+1>*page)
            continue;
        
        n=strlen(tempStr);
        if(tempStr[n-1]=='\n')
        {
            tempStr[n-1]='\0';
            n-=1;
        }
        IGetNameByPath(tempStr,name);

        setcolor(0);
        Iouttextxy(250,y,name,fpHZ);
        Iouttextxy(500,y,tempStr,fpHZ);

        y+=20;
    }
    if(!j)
        IPutsHZ16(530,240,"未检测到匹配项",fpHZ);
    //未搜索到文件
    else
    {
        if(*page>1)
            setcolor(0);
        else
            setcolor(LIGHTGRAY);
        IGoLeft(928,720);
        if(*page<(j-2)/30+1)
            setcolor(0);
        else
            setcolor(LIGHTGRAY);
        IGoRight(992,720);
        sprintf(tempStr,"%d",*page);
        setcolor(0);
        outtextxy(962,722,tempStr);
    }
    //搜索到文件

    fclose(searched);
}
