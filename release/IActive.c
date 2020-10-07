/*
 **************************************************
 *   版本号：1.0
 *   作者：郭一菲
 *   生成日期：2020-9-4
 *   说明：Active类函数处于中间层，用于将后端（如Diry等）函数的接口统一化，供前端调用栈使用
 **************************************************
 */

#include"IActive.h"

/*
    函数功能：在激活IEntree函数后，回收垃圾内存
    输入参数：X——辅助文件节点
    输出参数：cur——当前节点
    返回值：无
*/
void IAfterEntree(IFileNode* cur,IFileNode* X)
{
#ifdef LM
    IFileNodePointer ** curNode=(IFileNodePointer **)cur;
    IFileNodePointer * nodeX=(IFileNodePointer *)X;
    IFileNodePointer * tempNode=(*curNode)->pre,*lastNode=tempNode->pre;
    char path1[150],path2[150],path[150];

    IGetAbsolutePath((*curNode)->child,path1);
    if(nodeX&&nodeX->child)
        IGetAbsolutePath(nodeX->child,path2);
    else
        strcpy(path2,"");

    while(tempNode)
    {
        lastNode=tempNode->pre;
        IGetAbsolutePath(tempNode->child,path);
        if(strcmp(path,"DOS")&&strcmp(path,path1)&&strcmp(path,path2)&&!IisChild(path1,path)&&!IisChild(path2,path))
        {
            IDetree(tempNode->child);
            if(tempNode->pre)
            {
                tempNode=tempNode->pre;
                tempNode->next=tempNode->next->next;
                free(tempNode->next->pre);
                tempNode->next->pre=tempNode;
            }
            else
            {
                tempNode->next->pre=NULL;
                free(tempNode);
            }
        }
        tempNode=lastNode;
    }
#endif
}

/*
    函数功能：激活IEntree函数，更改当前节点
    输入参数：node——需要IEntree的节点
    输出参数：cur——当前节点
    返回值：无
*/
void IEntreeActive(IFileNode* node,IFileNode* cur)
{
    IFileNodePointer ** curNode=(IFileNodePointer **)cur;
	IFileNodePointer * newCurNode=(IFileNodePointer *)malloc(sizeof(IFileNodePointer));     //新节点
    IFileNodePointer * tempNode=NULL,*nextNode=NULL;

    tempNode=(*curNode)->next;
    while(tempNode)
    {
        nextNode=tempNode->next;
        tempNode->pre->next=NULL;
        free(tempNode);
        tempNode=nextNode;
    }
    //假如curNode链表有后一项，递归删除后面的所有节点

    newCurNode->child=node;
    newCurNode->next=NULL;
    newCurNode->wait=10;
    newCurNode->pre=*curNode;
    (*curNode)->next=newCurNode;
    *curNode=newCurNode;
    //新节点初始化

    IEntree(node,node->flags&16);
    //激活IEntree函数

    tempNode=(*curNode)->pre;
    while(tempNode)
    {
        tempNode->wait--;
        if(tempNode->wait<0)
        {
            if(tempNode->pre)
            {
                if(tempNode->next)
                {
                    tempNode=tempNode->pre;
                    tempNode->next=tempNode->next->next;
                    free(tempNode->next->pre);
                    tempNode->next->pre=tempNode;
                }
                else
                {
                    tempNode=tempNode->pre;
                    free(tempNode->next);
                    tempNode->next=NULL;
                }
            }
            else
            {
                tempNode->next->pre=NULL;
                free(tempNode);
                break;
            }
        }
        else
            tempNode=tempNode->pre;
    }
    //删除curNode链表中的超时节点，以节约内存
}

/*
    函数功能：根据情况将nodeX置为NULL
    输入参数：node——需要IDetree的节点
    输出参数：X——辅助文件节点
    返回值：无
*/
void ISetXNull(IFileNode* node,IFileNode* X)
{
    IFileNodePointer * nodeX=(IFileNodePointer *)X;
    char path1[150],path2[150];

    if(node->file.type[1]=='\\')
        return;

    IGetAbsolutePath(node,path1);
    if(nodeX&&nodeX->child)
        IGetAbsolutePath(nodeX->child,path2);   
    if(!strcmp(path1,path2)||IisChild(path2,path1))
        nodeX->child=NULL;
}

/*
    函数功能：激活IDetree函数，更改当前节点
    输入参数：node——需要IDetree的节点
    输出参数：cur——当前节点
    返回值：无
*/
void IDetreeActive(IFileNode* node,IFileNode* cur)
{
    IFileNodePointer ** curNode=(IFileNodePointer **)cur;
    IFileNodePointer * tempNode,*nextNode,*lastNode;
    char path1[150],path2[150];

    if(node->file.type[1]=='\\') return;

    tempNode=(*curNode)->next;
    while(tempNode)
    {
        nextNode=tempNode->next;
        tempNode->pre->next=NULL;
        free(tempNode);
        tempNode=nextNode;
    }
    //假如curNode链表有后一项，递归删除后面的所有节点

    tempNode=(*curNode);
    IGetAbsolutePath(node,path2);
    while(tempNode)
    {
        lastNode=tempNode->pre;
        IGetAbsolutePath(tempNode->child,path1);
        if(IisChild(path1,path2))
        {
            if(tempNode->pre)
            {
                if(tempNode->next)
                {
                    tempNode=tempNode->pre;
                    tempNode->next=tempNode->next->next;
                    free(tempNode->next->pre);
                    tempNode->next->pre=tempNode;
                }
                else
                {
                    tempNode=tempNode->pre;
                    free(tempNode->next);
                    tempNode->next=NULL;
                    (*curNode)=tempNode;
                }
            }
            else
            {
                tempNode->next->pre=NULL;
                free(tempNode);
                break;
            }
        }
        tempNode=lastNode;
    }
    //如果curNode链表中有node节点，将该节点从链表中除去

    if(node->file.type[1]!='\\')
        (*curNode)->child=IFindParent(node);
    //更改curNode节点

    IDetree(node);
    //激活IDetree函数

    node->file.type[0]='0';
    //关闭文件夹
}

/*
    函数功能：在View1中（单一地）选中文件节点，显示高亮
    输入参数：node——被选中的节点, null——用于占位
    输出参数：无
    返回值：无
*/
void ISelect(IFileNode* node,IFileNode* null)
{
    IFileNode* tempNode=IFindParent(node)->child;

    while(tempNode)
    {
        tempNode->flags&=29;
        tempNode=tempNode->next;
    }
    //如果当前路径下已经有被选中的节点，将该节点置为未被选中

    node->flags|=2;
    //选中该节点
}

/*
    函数功能：在View1中（复合地）选中文件节点，显示高亮
    输入参数：node——被选中的节点, null——用于占位
    输出参数：无
    返回值：无
*/
void ICtrlSelect(IFileNode* node,IFileNode* null)
{
    if(!(node->flags&2))
        node->flags|=2;
    else
        node->flags&=5;
    //如果该节点已被选中，则解选中；否则选中该节点
}

/*
    函数功能：空操作，可用于占位/截断/刷新等
    输入参数：null1——用于占位, null2——用于占位
    输出参数：无
    返回值：无
*/
void INOP(IFileNode* null1,IFileNode* null2){}

/*
    函数功能：返回上一目录
    输入参数：null——用于占位
    输出参数：cur——当前节点
    返回值：无
*/
void IGoLeftActive(IFileNode* cur,IFileNode* null)
{
    IFileNodePointer ** curNode=(IFileNodePointer **)cur;

    *curNode=(*curNode)->pre;
    //返回上一目录
}

/*
    函数功能：返回下一目录
    输入参数：null——用于占位
    输出参数：cur——当前节点
    返回值：无
*/
void IGoRightActive(IFileNode* cur,IFileNode* null)
{
    IFileNodePointer ** curNode=(IFileNodePointer **)cur;

    *curNode=(*curNode)->next;
    //返回下一目录
}

/*
    函数功能：激活查找函数
    输入参数：cur——当前节点, null——用于占位
    输出参数：无
    返回值：无
*/
void ISearchActive(IFileNode* cur,IFileNode* null)
{
    IFileNodePointer * curNode=(IFileNodePointer *)cur;
    FILE* fp=fopen("C:\\DOSRES\\ETC\\SEARCH.TXT","w+"); //覆盖的方式打开用于记录的文件
    char temp[20],path[150];     //辅助字符串

    IGetAbsolutePath(curNode->child,path);  //获得需要查找的路径
    strcpy(temp,"\0");
    IGetString(851+DF,51+DF,166,temp,1);  
    //得到查找的pattern

    setfillstyle(SOLID_FILL,255);
    bar(900+DF,745+DF,1020+DF,765+DF);
    setcolor(144);
    outtextxy(900+DF,753+DF,"Searching...");

    if(temp[0])
        ISearch(path,temp,fp);
    //激活查找函数
    fclose(fp);

    setfillstyle(SOLID_FILL,255);
    bar(900+DF,745+DF,1020+DF,765+DF);
}

/*
    函数功能：打开exe类文件
    输入参数：exe——可执行的文件节点, null——用于占位
    输出参数：无
    返回值：无
*/
void IexeActive(IFileNode* exe,IFileNode* null)
{
    char temp[180];

    IGetAbsolutePath(exe,temp);

    setfillstyle(SOLID_FILL,0);
    bar(0,0,1280,1024);
    strcat(temp,">>C:\\DOSRES\\ETC\\log.txt");
    system(temp);
    //系统调用，借用编辑器打开文本文件
    delay(2000);
    IQuit();
}

/*
    函数功能：打开txt类文件
    输入参数：txt——可编辑的文件节点, null——用于占位
    输出参数：无
    返回值：无
*/
void ItxtActive(IFileNode* txt,IFileNode* null)
{
    char temp[150];

    strcpy(temp,"C:\\BORLANDC\\BIN\\BC.EXE ");
    IGetAbsolutePath(txt,temp+23);
    
    strcat(temp,">>C:\\DOSRES\\ETC\\log.txt");
    system(temp);
    //系统调用，借用BC编辑器打开文本文件
    IQuit();
}

/*
    函数功能：View0上一页
    输入参数：pag——View0的页码, null——用于占位
    输出参数：无
    返回值：无
*/
void ILastPage(IFileNode *pag,IFileNode* null)
{
    char* page=(char*)pag;

    *page-=1;
    //翻到上一页
}

/*
    函数功能：View0下一页
    输入参数：pag——View0的页码, null——用于占位
    输出参数：无
    返回值：无
*/
void INextPage(IFileNode *pag,IFileNode* null)
{
    char* page=(char*)pag;

    *page+=1;
    //翻到下一页
}

/*
    函数功能：设置View0为详细信息
    输入参数：flag——View0的控制位, null——用于占位
    输出参数：无
    返回值：无
*/
void ISetView10(IFileNode * flag,IFileNode* null)
{
    char *menuFlag=(char*)flag;

    (*menuFlag)&=63;
}

/*
    函数功能：设置View0为大图标
    输入参数：flag——View0的控制位, null——用于占位
    输出参数：无
    返回值：无
*/
void ISetView11(IFileNode * flag,IFileNode* null)
{
    char *menuFlag=(char*)flag;

    (*menuFlag)|=64;
}