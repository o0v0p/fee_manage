//file name : bill_service.c
// 计费管理
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "card_file.h"
#include "card_service.h"
#include "bill_service.h"
#include "bill_file.h"
#include "fee_count.h"
#include "tool.h"
#include "model.h"
#include "menu.h"

//消费信息
ipbillnode BillListHead = NULL;
int aflag = 0;
int adel;


ipbillnode* getBillListHead() {
	return &BillListHead;
}
//上机
void logOn() {										//上机验证
	char aName[19] = { 0 };
	char aPwd[9] = { 0 };
	int virtual = 0;
	printf("\n**************上机**************\n");
	printf("请输入卡号 <长度为1~18>：");
	scanf_s("%s", aName, (unsigned int)sizeof(aName));
	card* position = lookrepet(aName, &virtual);
	if (position == NULL) {
		printf("\n该卡号不存在！\n");
		printf("\n==================================\n");
		pause();
		return;
	}
	printf("请输入密码 <长度为1~9>：");
	scanf_s("%s", aPwd, (unsigned int)sizeof(aPwd));
	card* pCard = doLogonInfo(aName, aPwd);
	if (pCard != NULL)
	{
		printf("上机成功！\n");
		char tmlogon[20] = { 0 };
		Timeshow(pCard->tLast, tmlogon);
		printf("\n******************************上机信息如下******************************\n\n");
		printf(" %-19s %-19s %-19s\n", "卡号", "余额", "上机时间");
		printf(" %-20s %-15.0f %-20s\n", pCard->aName,pCard->fBalance, tmlogon);
		printf("\n===============================上机成功！===============================\n\n");
	}
	else {
		printf("\n=================上机失败！=================\n");
	}
	pause();
}
card* doLogon(char* aName, char* aCode)
{
	int nIndex = 0;
	if (readcard() == FALSE)
	{
		return NULL;
	}
	ipcardnode cardList = *getCardListHead();
	ipcardnode pCur = cardList;
	if (cardList != NULL)
	{
		pCur = pCur->next;
		while (pCur != NULL)
		{
			if (strcmp(pCur->cardo.aName, aName) == 0)
			{
				if (pCur->cardo.nStatus == 2)
				{
					pCur = pCur->next;
					nIndex++;
					continue;
				}
					if (strcmp(pCur->cardo.aPwd, aCode) != 0)
					{
						printf("\n密码错误！\n");
						return NULL;
					}
						
					if (pCur->cardo.fBalance <= 0)
					{
						printf("\n余额不足请先充值！\n");
						return NULL;
					}
					if (pCur->cardo.nStatus != 0)
					{
						switch (pCur->cardo.nStatus) {
						case 1:printf("\n该卡正在上机！\n"); return NULL; break;
						case 2:printf("\n该卡已注销！不可操作！\n"); return NULL; break;
						case 3:printf("\n该卡已失效！仅可以进行退费！\n"); return NULL; break;
						default:printf("\n退费系统出错\n"); break;
						}
					}
					//更新
					pCur->cardo.nStatus = 1;
					pCur->cardo.nUseCount++;
					pCur->cardo.tLast = time(NULL);
					if (FALSE != updatecard(&pCur->cardo,nIndex))
					{
						return &pCur->cardo;
					}
			}
			pCur = pCur->next;
			nIndex++;
		}
	}
	return NULL;
}

card* doLogonInfo(char* aName, char* aCode)
{
	double money = 0;
	card* pCard = NULL;
	pCard = doLogon(aName, aCode);
	if (pCard == NULL)
	{
		return NULL;
	}
	Billing bill;
	strcpy_s(bill.aName, sizeof(bill.aName),pCard->aName);
	bill.tStart = time(NULL);
	bill.fAmount = 0;
	bill.tEnd = 0;
	bill.nStatus = 0;
	bill.fAmount = money;
	bill.nDel = 0;
	if (TRUE == savebilling(&bill))
	{
		return pCard;
	}
	else {
		return NULL;
	}
}


int savebilling(Billing* bill) {										//保存计费信息
	FILE* fp = NULL;
	if (!aflag) {
		fopen_s(&fp, billpath, "wb+");
		if (fp == NULL) {
			printf("文件打开失败！");
			return FALSE;
		}
	}
	else {
		fopen_s(&fp, billpath, "ab+");
		if (fp == NULL) {
			printf("文件打开失败！");
			return FALSE;
		}
	}
	fwrite(bill, sizeof(Billing), 1, fp);
	aflag++;
	fclose(fp);
	return TRUE;
}

//下机
void logOut() {	
	card* pCard = NULL;
	char aName[19] = { 0 };
	char aPwd[9] = { 0 };
	double fAmount = 0;
	//获取卡信息
	if (!readcard()) {
		printf("读取卡信息失败，无法进行下机操作！\n");
		pause();
		return;
	}
	printf("\n**************下机**************\n");
	printf("请输入卡号 <长度为1~18>：");
	scanf_s("%s", aName, (unsigned int)sizeof(aName));
	int virtual = 0;
	card* position = lookrepet(aName, &virtual);
	if (position == NULL) {
		printf("\n该卡号不存在！\n");
		printf("\n==================================\n");
		pause();
		return;
	}
	printf("请输入密码 <长度为1~9>：");
	scanf_s("%s", aPwd, (unsigned int)sizeof(aPwd));
	logoutinfo* info = doSettleInfo(aName, aPwd, &fAmount);
	if (info == NULL) {
		pause();
		return;
	}
	else{
		//展示
		char tmlogon[20] = { 0 };
		Timeshow(info->tlogon, tmlogon);
		char tmlogout[20] = { 0 };
		Timeshow(info->tlogout, tmlogout);
		printf("\n******************************下机信息如下******************************\n\n");
		printf(" %-19s %-8s %-11s %-19s %-19s\n", "卡号", "消费", "余额", "上机时间", "下机时间");
		printf(" %-20s %-8.2f %-8.2f %-18s %-17s\n", info->aName, info->fUse,info->fBalance, tmlogon,tmlogout);
		printf("\n******************************下机成功！******************************\n\n");
		pause();
		return;
	}
}
logoutinfo* doSettleInfo(char* aNum, char* aCode, double* fee)
{
	int Indexbill = 0;
	int Indexcard = 0;
	Billing* pBill = checkBilling(aNum, &Indexbill, fee);
	card* pCard = doSettle(aNum, aCode, &Indexcard, fee);
	if (pBill != NULL && pCard != NULL)
	{
		logoutinfo* info = (logoutinfo*)malloc(sizeof(logoutinfo));
		if (info == NULL) {
			printf("内存分配失败，无法进行下机操作！\n");
			return NULL;
		}
		strcpy(info->aName, pCard->aName);
		info->fBalance = pCard->fBalance;
		info->fUse = *fee;
		info->tlogon = pBill->tStart;
		info->tlogout = pBill->tEnd;
		//保存文件
		if ((TRUE == updatebill(pBill, Indexbill)) && (TRUE == updatecard(pCard, Indexcard)))
		{
			readcard();
			getBilling();
			return info;
		}
		else {
			printf("更新账单或卡信息失败！\n");
			free(info);
			return NULL;
		}
	}
	else {
		printf("\n=================下机失败！=================\n");
		return NULL;
	}
}

card* doSettle(char* aNum, char* aCode, int* Indexc,double* fee)
{
	if (readcard() == FALSE)
	{
		return NULL;
	}
	ipcardnode pCur = *getCardListHead();
	if (pCur != NULL)
	{
		while (pCur != NULL)
		{
			if (strcmp(pCur->cardo.aName, aNum) == 0)
			{
				if (pCur->cardo.nStatus != 2)
				{
					//卡信息
					if (strcmp(pCur->cardo.aPwd, aCode) != 0)
					{
						printf("\n密码错误！\n");
						return NULL;
					}
					if (pCur->cardo.nStatus != 1)
					{
						switch (pCur->cardo.nStatus) {
						case 0:printf("\n该卡未上机！\n"); return NULL; break;
						case 2:printf("\n该卡已注销！不可重复操作！\n"); return NULL; break;
						case 3:printf("\n该卡已失效！仅可以进行退费！\n"); return NULL; break;
						default:printf("\n退费系统出错\n"); break;
						}
					}
					if (pCur->cardo.fBalance < *fee) {
						printf("\n下机失败！余额不足请先充值！\n");
						return NULL;
					}
					//卡信息
					pCur->cardo.nStatus = 0;
					timetostr(pCur->cardo.tLast, pCur->cardo.everytime3[pCur->cardo.nUseCount - 1]);
					Timeshow(pCur->cardo.tLast, pCur->cardo.everytime1[pCur->cardo.nUseCount - 1]);
					pCur->cardo.tLast = time(NULL);
					timetostr(pCur->cardo.tLast, pCur->cardo.everytime4[pCur->cardo.nUseCount - 1]);
					Timeshow(pCur->cardo.tLast, pCur->cardo.everytime2[pCur->cardo.nUseCount - 1]);
					pCur->cardo.fBalance -=  * fee;
					pCur->cardo.fTotalUse += *fee;
					pCur->cardo.everymoney[pCur->cardo.nUseCount - 1] += *fee;
					return &pCur->cardo;
				}
			}
			pCur = pCur->next;
			(*Indexc)++;
		}
	}
	return NULL;
}

void Billlist() {													//初始化链表
	ipbillnode phead = NULL;
	phead = (ipbillnode)malloc(sizeof(billnode));
	if (phead == NULL) {
		printf("\n分配内存失败\n");
		return;
	}
	else {
		phead->next = NULL;
		BillListHead = phead;
		return;
	}
}

void releaseBill()
{
	ipbillnode pCur = NULL;
	ipbillnode pNext = NULL;
	if (BillListHead != NULL)
	{
		pCur = BillListHead;
		pNext = pCur->next;
		while (pCur != NULL)
		{
			free(pCur);
			pCur = NULL;
			if (pNext != NULL)
			{
				pCur = pNext;
				pNext = pCur->next;
			}
		}
	}
}

ipbillnode querybill(char* aName, int* index) {										//查找比对
	if (BillListHead == NULL || BillListHead->next == NULL) {
		printf("\n错误：BillListHead 为空或没有数据，链表未初始化！\n");
		return NULL;
	}
	ipbillnode temp = BillListHead->next;  // 跳过虚拟头结点
	readBilling(&temp->bill);
	int nLine = 0;
	while (temp) {
		if (strcmp(temp->bill.aName, aName) == 0 && temp->bill.nStatus == 0) {
			*index = nLine;
			return temp;
		}
		temp = temp->next;
		nLine++;
	}
	return NULL;
}

//更新文件
int updatefile(card* tocard) {
	FILE* fp = NULL;
	FILE* tempFp = NULL;
	char Buf[1024] = { 0 };
	char tmp[1024] = { 0 };
	char tmstart[20], tmlast[20], tmend[20];

	Timeshow(tocard->tStart, tmstart);
	Timeshow(tocard->tLast, tmlast);
	Timeshow(tocard->tEnd, tmend);

	fopen_s(&fp, cardpath, "r");
	if (fp == NULL) {
		printf("\n文件打开失败！cardpath: %s\n", cardpath);
		return FALSE;
	}

	char tempFile[] = "card_temp.txt";
	fopen_s(&tempFp, tempFile, "w");
	if (tempFp == NULL) {
		printf("\n临时文件打开失败！\n");
		fclose(fp);
		return FALSE;
	}

	int found = 0;
	while (fgets(Buf, sizeof(Buf), fp)) {
		char bufCopy[1024];
		strcpy_s(bufCopy, sizeof(bufCopy), Buf);
		char* digit = strtok(bufCopy, "#");

		if (digit && strcmp(digit, tocard->aName) == 0) {
			snprintf(tmp, sizeof(tmp), "%s#%s#%d#%s#%s#%.1f#%s#%d#%.1f#%d#\n",
				tocard->aName, tocard->aPwd, tocard->nStatus,
				tmstart, tmend, tocard->fTotalUse, tmlast,
				tocard->nUseCount, tocard->fBalance, tocard->nDel);
			fputs(tmp, tempFp);
			found = 1;
		}
		else {
			fputs(Buf, tempFp);
		}
	}

	fclose(fp);
	fclose(tempFp);

	if (found) {
		if (remove(cardpath) != 0) {
			printf("\n旧文件删除失败！\n");
			return FALSE;
		}
		if (rename(tempFile, cardpath) != 0) {
			printf("\n重命名临时文件失败！\n");
			return FALSE;
		}
		return TRUE;
	}
	else {
		printf("\n未找到指定卡号记录！\n");
		remove(tempFile);
		return FALSE;
	}
}


int getBilling()
{
	int billingCnt = 0;
	int a = getbillcount();
	if (a == 0){
		return FALSE;
	}
	int i = 0;
	Billing* pBilling = NULL;
	pBilling = (Billing*)malloc(sizeof(Billing) * a);
	if (pBilling == NULL)
	{
		printf("\n堆区内存已经用完！\n");
		exit(1);
	}
	if (readBilling(pBilling) == FALSE)
	{
		printf("\n文件读取失败！");
		return FALSE;
	}
	ipbillnode node = NULL;
	ipbillnode pCur = NULL;
	if (Billlist != NULL)
	{
		releaseBill();
	}
	Billlist();
	node = *getBillListHead();
	for (i = 0; i < a; i++)
	{
		if ((pCur = (ipbillnode)malloc(sizeof(billnode))) == NULL)
		{
			free(pBilling);
			pBilling = NULL;
			return FALSE;
		}
		memset(pCur, 0, sizeof(billnode));
		pCur->bill = pBilling[i];
		pCur->next = NULL;
		while (node->next != NULL)
		{
			node = node->next;
		}
		node->next = pCur;
	}
	free(pBilling);
	pBilling = NULL;
	return TRUE;
}