// file name : card_file.c
// 卡信息记录

#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#include <string.h>
#include "card_file.h"
#include "model.h"
#include "tool.h"
#include "menu.h"
#include "global.h"

//卡信息
ipcardnode CardListHead = NULL;
int flag = 0;
int del;

ipcardnode* getCardListHead() {
	return &CardListHead;
}

card* lookrepet(char* aName, int* index) {										//查找比对
	if (aName == NULL || index == NULL) {
		return NULL;
	}
	ipcardnode temp = CardListHead;
	int nLine = 0;
	while (temp) {
		nLine++;
		if (temp != NULL ) {
			int num = strcmp(temp->cardo.aName, aName);
			if (!num ) {
				*index = nLine;
				card* pcardo = &temp->cardo;
				return pcardo;
			}
			temp = temp->next;
		}
	}
	return NULL;
}

void Cardlist() {                                      //初始化链表
	ipcardnode phead = (ipcardnode)malloc(sizeof(cardnode));
	if (phead == NULL) {
		printf("分配内存失败");
		return;
	}
	phead->next = NULL;
	CardListHead = phead;
	return;
}

void releaseCardlist() {                                      //释放链表
	ipcardnode temp = CardListHead;
	while (temp) {
		ipcardnode cur = temp->next;
		free(temp);
		temp = cur;
	}
	CardListHead = NULL;
}

int savecard(card pcard, char* ppath) {							//存储 新卡信息→文本文件
	FILE* fp = NULL;
	errno_t err = fopen_s(&fp, cardpath, "a");
	if (fp == NULL || err != 0) {
		printf("文件打开失败！");
		return FALSE;
	}
	char tmstart[20] = { 0 };
	Timeshow(pcard.tStart, tmstart);
	char tmlast[20] = { 0 };
	Timeshow(pcard.tLast, tmlast);
	char tmend[20] = { 0 };
	Timeshow(pcard.tEnd, tmend);
	//格式：卡号#密码#状态#开卡时间#截止时间#累积消费#最后使用时间#使用次数#当前余额#删除标识#
	fprintf(fp, "%s#%s#%d#%s#%s#%.1f#%s#%d#%.1f#%d#\n", pcard.aName, pcard.aPwd, pcard.nStatus, 
		tmstart, tmend, pcard.fTotalUse, tmlast, pcard.nUseCount, pcard.fBalance, pcard.nDel);
	fclose(fp);
	return TRUE;
}

//查询卡
void querycard() {
	char aName[19] = { 0 };
	printf("\n**************查询卡**************\n\n");
	int choice = 0;
	printf(" 1.精确查询 \n 2.模糊查询\n\n 请选择查询类型：");
	scanf_s("%d", &choice);
	switch (choice)
	{
	case 1:  queryInfo(); break;
	case 2:
		printf("\n==================================\n");
		printf("\n 请输入查询的卡号 <长度为1~18>：");							//输入卡号
		scanf_s("%s", aName, (unsigned int)sizeof(aName));
		ipcardnode query = search(aName);
		if (query != NULL) {
			ipcardnode temp = (ipcardnode)malloc(sizeof(cardnode));
			temp = query;
			printf("\n*****************************************查询的卡信息如下*****************************************\n\n");
			printf("%-20s %-12s %-10s %-10s %-15s %-20s\n", "卡号", "状态", "余额", "累计消费", "使用次数", "上次使用时间\n");
			while (temp) {
				char current[20] = { 0 };
				Timeshow(temp->cardo.tLast, current);
				printf("%-20s  %-10d  %-10.1f  %-12.1f %-11d %-20s\n\n", temp->cardo.aName, temp->cardo.nDel, temp->cardo.fBalance, temp->cardo.fTotalUse, temp->cardo.nUseCount, current);
				printf("================================================================================================\n\n");
				temp = temp->next;
			}
			ipcardnode p = query;
			while (query) {
				ipcardnode next = query->next;
				free(query);
				query = next;
			}
		}
		else {
			printf("\n没有该卡的信息！\n");
			printf("\n==================================\n");
		}
		break;
	default:
		printf("\n输入有误！");
		printf("\n==================================\n");
		break;
	}
	pause();
	return;
}
void queryInfo()
{
	char str[20] = { 0 };
	printf("\n==================================\n");
	printf("请输入查询卡卡号<长度为1~18>:");
	scanf_s("%s", str, 20);
	int flag1 = 0;
	int a = 0;
	card* pCard = lookrepet(str, &a);
	if (pCard == NULL) {													//展示查询结果  (无/有)
		printf("\n没有该卡的信息！\n");
		printf("\n==================================\n");
	}
	else{
		flag1 = 1;
		printf("\n***************************************查询的卡信息如下***************************************\n");
		printf("\n%-20s %-12s %-10s %-10s %-15s %-20s\n", "卡号", "状态", "余额", "累计消费", "使用次数", "上次使用时间\n");
		char current[20] = { 0 };
		Timeshow(pCard->tLast, current);
		printf("%-20s  %-10d  %-10.1f  %-12.1f %-11d %-20s\n\n", pCard->aName, pCard->nDel, pCard->fBalance, pCard->fTotalUse, pCard->nUseCount, current);
		printf("================================================================================================\n\n");
	}
}

ipcardnode search(char* aName) { //寻找卡号
	if (aName == NULL) {
		printf("错误：输入的卡号为空！\n");
		return NULL;
	}
	ipcardnode temp = (ipcardnode)malloc(sizeof(cardnode));
	if (temp == NULL) {
		printf("temp为空！虚拟头结点分配失败！\n");
		return NULL;
	}
	temp->next = NULL;
	ipcardnode result = temp;
	ipcardnode cur = CardListHead;
	cur = cur->next;
	while (cur) {
		if (cur->cardo.nDel == 1) {
			cur = cur->next;
			continue;
		}
		if (strstr(cur->cardo.aName, aName) != NULL) {
			ipcardnode newcard = (ipcardnode)malloc(sizeof(cardnode));
			if (newcard == NULL) {
				printf("newcard为空！新结点分配失败！\n");
				free(temp);
				return NULL;
			}
			newcard->cardo = cur->cardo;
			newcard->next = NULL;
			result->next = newcard;
			result = newcard;
		}
		cur = cur->next;
	}
	ipcardnode resultHead = temp->next;
	free(temp);
	return resultHead;
}

void addCardlist(card cardo) {
	if (CardListHead == NULL) {
		printf("链表未初始化！\n");
		return;
	}
	ipcardnode newcard = (ipcardnode)malloc(sizeof(cardnode));
	if (newcard == NULL) {
		printf("内存分配失败！\n");
		releaseCardlist();
		return;
	}
	newcard->next = NULL;
	newcard->cardo = cardo;
	ipcardnode tail = CardListHead;
	while (tail->next != NULL) {
		tail = tail->next;
	}
	tail->next = newcard;
}


int getcardcount() {				//文件卡信息计数
	FILE* fp = NULL;
	int count = 0;
	char Buf[1024] = { 0 };
	errno_t err = fopen_s(&fp, cardpath, "r");
	if (fp == NULL) {
		printf("文件打开失败！");
		exit(1);
	}
	while (fgets(Buf, sizeof(Buf), fp) != NULL) {
		count++;
	}
	fclose(fp);
	return count;
}

int readcard() {				//读取文件卡信息
	FILE* fp = NULL;
	card card0 = { 0 };
	char Buf[1024] = { 0 };
	errno_t err = fopen_s(&fp, cardpath, "r");
	if (fp == NULL) {
		printf("文件打开失败！");
		exit(1);
	}
	releaseCardlist();
	Cardlist();
	del = 0;
	int count = getcardcount();
	if (count == 0) {
		printf("没有卡记录，创建空链表\n");
		Cardlist();
		return TRUE;
	}
	for (int i = 0; i < count; i++) {
		fgets(Buf, sizeof(Buf), fp);
		char* temp = Buf;
		char tempS[32] = { 0 };
		char tempE[32] = { 0 };
		char tempL[32] = { 0 };
		char* token = strtok_s(temp, "#", &temp);
		if (token) strncpy_s(card0.aName, sizeof(card0.aName), token, _TRUNCATE);
		token = strtok_s(NULL, "#", &temp);
		if (token) strncpy_s(card0.aPwd, sizeof(card0.aPwd), token, _TRUNCATE);
		token = strtok_s(NULL, "#", &temp);
		card0.nStatus = token ? atoi(token) : 0;
		token = strtok_s(NULL, "#", &temp);
		card0.tStart = token ? cintime(token) : 0;
		token = strtok_s(NULL, "#", &temp);
		card0.tEnd = token ? cintime(token) : 0;
		token = strtok_s(NULL, "#", &temp);
		card0.fTotalUse = token ? atof(token) : 0.0;
		token = strtok_s(NULL, "#", &temp);
		card0.tLast = token ? cintime(token) : 0;
		token = strtok_s(NULL, "#", &temp);
		card0.nUseCount = token ? atoi(token) : 0;
		token = strtok_s(NULL, "#", &temp);
		card0.fBalance = token ? atof(token) : 0.0;
		token = strtok_s(NULL, "#", &temp);
		card0.nDel = token ? atoi(token) : 0;
		addCardlist(card0);
	}
	fclose(fp);
	return TRUE;
}

