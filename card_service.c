// file name : card_service.c
// 卡管理
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "bill_service.h"
#include "card_service.h"
#include "card_file.h"
#include "model.h"
#include "tool.h"
#include "global.h"
#include "menu.h"

int deletecard() {  
   printf("\n**************注销卡**************\n");  
   char aName[19] = { 0 };  
   char aPwd[9] = { 0 };  
   int virtual = 0;  
   card* pCard = NULL;
   printf("请输入卡号 <长度为1~18>：");  
   scanf_s("%s", aName, (unsigned int)sizeof(aName));  
   card* position = lookrepet(aName, &virtual);  
   if (position == NULL) {  
       printf("\n卡号不存在！\n");  
       printf("\n==================================\n");  
       pause();  
       return FALSE;  
   } 
   else if (position->nDel == 1) {
	   printf("\n该卡已注销！勿重复操作！\n"); 
	   pause();
	   return FALSE;
   }
   else {
	   printf("请输入密码 <长度为1~9>：");
	   scanf_s("%s", aPwd, (unsigned int)sizeof(aPwd));
	   int check = strcmp(position->aPwd, aPwd);
	   if (position != NULL && !check) {
		   printf("\n**************注销卡信息如下**************\n\n");
		   printf("%-20s %-12s %-10s\n", "卡号", "消费金额", "余额");
		   printf("%-20s %-9.2f %9.2f\n", position->aName, position->fTotalUse, position->fBalance);
		   printf("\n------------------------------------------\n");
		   printf("\n是否确定注销该卡（1---是 2---否）：");
		   int choice = 0;
		   scanf_s("%d", &choice);
		   switch (choice) {
		   case 1:
			   pCard = doCancel(aName, aPwd);
			   if (pCard != NULL) {
				   printf("\n**************注销成功！**************\n");
				   printf("卡号：%s\n", pCard->aName);
				   printf("\n退款金额：%.1f\n", pCard->fBalance);
				   printf("\n==================================\n");
			   }
			   break;
		   case 2:
			   printf("\n================已撤回注销！================\n");
			   break;
		   default:
			   printf("\n================输入错误！================\n");
		   }
	   }
	   else {
		   if (check)
			   printf("\n密码错误！\n");
		   printf("\n================注销失败！================\n");
	   }
	   pause();
	   return TRUE;
   }
}

card* doCancel(const char* name, const char* code)
{
	int index = 0;
	if (FALSE == readcard())
	{
		return NULL;
	}
	ipcardnode pCur = *getCardListHead();
	while (pCur != NULL)
	{
		pCur = pCur->next;
		while (pCur != NULL)
		{
			if (strcmp(pCur->cardo.aName, name) == 0)
			{
				if (strcmp(pCur->cardo.aPwd, code) != 0)
				{
					printf("\n密码错误！\n");
					return NULL;
				}
				else
				{
					if (pCur->cardo.nStatus !=0 )
					{
						switch (pCur->cardo.nStatus) {
							case 1:printf("\n该卡正在上机！\n"); return NULL; break;
							case 2:printf("\n该卡已注销！勿重复操作！\n"); return NULL; break;
							case 3:printf("\n该卡已失效！仅可以进行退费！\n"); return NULL; break;
							default:printf("\n退费系统出错\n"); break;
						}
					}
					else
					{
						pCur->cardo.nStatus = 2;
						pCur->cardo.nDel = 1;
						pCur->cardo.tLast = time(NULL);
						pCur->cardo.tEnd = pCur->cardo.tLast;
						char tmlast[20] = { 0 };
						if (FALSE != updatecard(&pCur->cardo, index))
						{
							return &pCur->cardo;
						}
					}
				}
			}
			pCur = pCur->next;
			index++;
		}
	}
	return NULL;
}

int updatecard(card* pCard, int nIndex) {
    FILE* fp = NULL;
    FILE* tempFp = NULL;
    char Buf[1024] = { 0 };
    int found = 0;
    fopen_s(&fp, cardpath, "r");
    if (fp == NULL) {
        printf("\n文件打开失败！\n");
        return FALSE;
    }
     fopen_s(&tempFp,"card.txt", "w");
    if (tempFp == NULL) {
        printf("\n临时文件打开失败！\n");
        fclose(fp);
        return FALSE;
    }
    while (fgets(Buf, sizeof(Buf), fp)) {
		char lineCopy[1024];
		strcpy_s(lineCopy, sizeof(lineCopy), Buf);
		char* token = strtok(lineCopy, "#");
		if (token != NULL && strcmp(token, pCard->aName) == 0) {
			found = 1;
			char tmstart[20], tmlast[20], tmend[20];
			Timeshow(pCard->tStart, tmstart);
			Timeshow(pCard->tLast, tmlast);
			Timeshow(pCard->tEnd, tmend);
			char newLine[1024];
			snprintf(newLine, sizeof(newLine),
				"%s#%s#%d#%s#%s#%.1f#%s#%d#%.1f#%d#\n",
				pCard->aName, pCard->aPwd, pCard->nStatus,
				tmstart, tmend, pCard->fTotalUse, tmlast,
				pCard->nUseCount, pCard->fBalance, pCard->nDel);
			fputs(newLine, tempFp);
		}
		else {
			fputs(Buf, tempFp);
		}
    }
    fclose(fp);
    fclose(tempFp);
    if (found) {
        remove(cardpath);
        rename("card.txt", cardpath);
		readcard();
        return TRUE;
    }
    else {
        printf("\n未找到指定卡号记录！\n");
        remove("card.txt");
        return FALSE;
    }
}

void updateExpiredCards() {
	ipcardnode head = *getCardListHead();
	if (!head) return;
	int index = 0;
	time_t now = time(NULL);
	ipcardnode pCur = head->next;
	while (pCur) {
		if (pCur->cardo.nStatus != 2 && pCur->cardo.tEnd < now) {
			pCur->cardo.nStatus = 3;  // 设为失效
			updatecard(&pCur->cardo,&index); // 更新到文件
		}
		pCur = pCur->next;
	}
}
