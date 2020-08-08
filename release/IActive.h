#ifndef __IACTIVE_H
#define __IACTIVE_H

#include"IType.h"

void IEntreeActive(IFileNode* node,IFileNode* cur);
void IDetreeActive(IFileNode* node,IFileNode* cur);
void ISelect(IFileNode* node,IFileNode* null);
void ICtrlSelect(IFileNode* node,IFileNode* null);
void INOP(IFileNode* null1,IFileNode* null2);
void IGoLeftActive(IFileNode* cur,IFileNode* null);
void IGoRightActive(IFileNode* cur,IFileNode* null);
char* IGetString(int x,int y,int length,char* string,int flag);
void IexeActive(IFileNode* exe,IFileNode* null);
void ItxtActive(IFileNode* txt,IFileNode* null);
void ISortMenuActive(IFileNode* flag,IFileNode* null);
void ISortActive(IFileNode* parent,IFileNode* pfun);
void ISearchActive(IFileNode* cur,IFileNode* null);
void IGetName(IFileNode* nam,IFileNode* null);
void IGetPassword(IFileNode* pass,IFileNode* null);

#endif