#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<DOS.H>
#include<DIR.H>
#include<IO.H>
#include<ALLOC.H>
#include"IUtility.h"
#include"INode.h"
#include"IDirBase.h"

#define BUFFSIZE 256

IBool Icopy(IFileNode * inFile,IFileNode * outParent)
{
    FILE * fin,*fout;
    char * buff,inPath[50],outPath[50],temp[50],i[3],name[18],ext[5];
    int ret,j;
    IGetAbsolutePath(inFile,inPath);
    fin=fopen(inPath,"r");
    if(fin==NULL)
    {
#ifdef DB
        printf("failed to open %s\n",inFile->file.name);
#endif
        return 0;
    }

    IGetAbsolutePath(outParent,outPath);
    strcat(outPath,"\\");
    strcat(outPath,inFile->file.name);
    strcpy(name,inFile->file.name);
    if(!strcmp(inPath,outPath)) //原位置拷贝
    {
        strcpy(i,"_1");
        for(j=0;j<strlen(inFile->file.name);j++)
            if(inFile->file.name[j]=='.')
                break;
        strcpy(ext,inFile->file.name+j);
        IGetAbsolutePath(outParent,outPath);
        strcat(outPath,"\\");
        do
        {
            strcpy(name+j,"");
            strcat(name,i);
            strcat(name,ext);
            strcpy(temp,outPath);
            strcat(temp,name);
            i[1]++;
        }while(searchpath(temp));
        strcpy(outPath,temp);
    }

    fout=fopen(outPath,"w");
    if(fout==NULL)
    {
#ifdef DB
        printf("failed to open %s\n",inFile->file.name);
#endif
        return 0;
    }

    buff=malloc(BUFFSIZE);
    if(buff==NULL)
    {
#ifdef DB
        puts("not enough memory");
#endif
        return 0;
    }

    while(1)
    {
        ret=fread(buff,1,BUFFSIZE,fin);
        fwrite(buff,ret>BUFFSIZE?BUFFSIZE:ret,1,fout);
        if(feof(fin))
            break;
    }
    fclose(fin);
    fclose(fout);
    free(buff);

    IAddFileNode(outParent,name);  //添加新文件节点
    return 1;
}
IBool Irmf(IFileNode * fileNode)
{
    char tempStr[50];
    IGetAbsolutePath(fileNode,tempStr);
    remove(tempStr);    //删除文件
    return IDelFileNode(IFindParent(fileNode),fileNode->file.name);     //删除文件节点
}
void Imkdir(IFileNode * pathNode,char* folderName)    //创建文件夹，并更新节点
{
    char temp[80];
    
    IGetAbsolutePath(pathNode,temp);
    strcat(temp,"\\");
    strcat(temp,folderName);
    mkdir(temp);    //创建文件夹
    IAddFileNode(pathNode,folderName);
}
IBool Irmdir(IFileNode * node,int flag)  //删除所有空文件夹，并更新节点
{
    char temp[50];
    
    if(node->child) 
        return Irmdir(node->child,0);
    if(!flag)
    {
        if(node->next)
            return Irmdir(node->next,0);
    }
    IGetAbsolutePath(node,temp);
    IDelFileNode(IFindParent(node),node->file.name);
    return !rmdir(temp);
}
void ICopyAll(IFileNode * oldChildChild,IFileNode * newChild)   //复制链表
{
    char temp[50];

    if(IisFolder(oldChildChild))
    {
        if(!oldChildChild->child)
            IAddFilelist(oldChildChild);
        Imkdir(newChild,oldChildChild);
        if(oldChildChild->child)
            ICopyAll(oldChildChild->child,newChild->child);
    }
    else
    {
        Icopy(oldChildChild,newChild);
    }
    if(oldChildChild->next)
    {
        ICopyAll(oldChildChild->next,newChild);
    }
    IGetAbsolutePath(newChild,temp);
    IPeek(newChild,temp);
}
void Icpr(IFileNode * oldChild,IFileNode * newParent) //递归复制所有文件和文件夹
{
    char temp[50];
    IGetAbsolutePath(newParent,temp);
    strcat(temp,"\\");
    if(IisFolder(oldChild))
    {
        Imkdir(newParent,oldChild);
        IEntree(oldChild);
        strcat(temp,oldChild->file.name);
        ICopyAll(oldChild->child,IFindNodeByPath(temp,newParent));
    }
    else
        Icopy(oldChild,newParent);
}   
void IDelAll(IFileNode * oldChildChild) //删除链表
{
    IFileNode *tempNode=oldChildChild,*nextNode=oldChildChild->next;

    if(IisFolder(oldChildChild))
    {
        IEntree(oldChildChild);
        if(oldChildChild->child)
            IDelAll(oldChildChild->child);
    }
    else
    {
        Irmf(oldChildChild);
    }
    if(nextNode)
    {
        tempNode=nextNode;
        nextNode=nextNode->next;
        IDelAll(tempNode);
    }
}
void Irmr(IFileNode * oldChild)//递归删除所有文件和文件夹
{
    if(IisFolder(oldChild))
    {
        IEntree(oldChild);
        if(oldChild->child)
            IDelAll(oldChild->child);
    }
    else
        Irmf(oldChild);
    while(Irmdir(oldChild,1));
}
