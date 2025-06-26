//file name : bill_service.c
// �Ʒѹ���
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

//������Ϣ
ipbillnode BillListHead = NULL;
int aflag = 0;
int adel;


ipbillnode* getBillListHead() {
	return &BillListHead;
}
//�ϻ�
void logOn() {										//�ϻ���֤
	char aName[19] = { 0 };
	char aPwd[9] = { 0 };
	int virtual = 0;
	printf("\n**************�ϻ�**************\n");
	printf("�����뿨�� <����Ϊ1~18>��");
	scanf_s("%s", aName, (unsigned int)sizeof(aName));
	card* position = lookrepet(aName, &virtual);
	if (position == NULL) {
		printf("\n�ÿ��Ų����ڣ�\n");
		printf("\n==================================\n");
		pause();
		return;
	}
	printf("���������� <����Ϊ1~9>��");
	scanf_s("%s", aPwd, (unsigned int)sizeof(aPwd));
	card* pCard = doLogonInfo(aName, aPwd);
	if (pCard != NULL)
	{
		printf("�ϻ��ɹ���\n");
		char tmlogon[20] = { 0 };
		Timeshow(pCard->tLast, tmlogon);
		printf("\n******************************�ϻ���Ϣ����******************************\n\n");
		printf(" %-19s %-19s %-19s\n", "����", "���", "�ϻ�ʱ��");
		printf(" %-20s %-15.0f %-20s\n", pCard->aName,pCard->fBalance, tmlogon);
		printf("\n===============================�ϻ��ɹ���===============================\n\n");
	}
	else {
		printf("\n=================�ϻ�ʧ�ܣ�=================\n");
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
						printf("\n�������\n");
						return NULL;
					}
						
					if (pCur->cardo.fBalance <= 0)
					{
						printf("\n�������ȳ�ֵ��\n");
						return NULL;
					}
					if (pCur->cardo.nStatus != 0)
					{
						switch (pCur->cardo.nStatus) {
						case 1:printf("\n�ÿ������ϻ���\n"); return NULL; break;
						case 2:printf("\n�ÿ���ע�������ɲ�����\n"); return NULL; break;
						case 3:printf("\n�ÿ���ʧЧ�������Խ����˷ѣ�\n"); return NULL; break;
						default:printf("\n�˷�ϵͳ����\n"); break;
						}
					}
					//����
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


int savebilling(Billing* bill) {										//����Ʒ���Ϣ
	FILE* fp = NULL;
	if (!aflag) {
		fopen_s(&fp, billpath, "wb+");
		if (fp == NULL) {
			printf("�ļ���ʧ�ܣ�");
			return FALSE;
		}
	}
	else {
		fopen_s(&fp, billpath, "ab+");
		if (fp == NULL) {
			printf("�ļ���ʧ�ܣ�");
			return FALSE;
		}
	}
	fwrite(bill, sizeof(Billing), 1, fp);
	aflag++;
	fclose(fp);
	return TRUE;
}

//�»�
void logOut() {	
	card* pCard = NULL;
	char aName[19] = { 0 };
	char aPwd[9] = { 0 };
	double fAmount = 0;
	//��ȡ����Ϣ
	if (!readcard()) {
		printf("��ȡ����Ϣʧ�ܣ��޷������»�������\n");
		pause();
		return;
	}
	printf("\n**************�»�**************\n");
	printf("�����뿨�� <����Ϊ1~18>��");
	scanf_s("%s", aName, (unsigned int)sizeof(aName));
	int virtual = 0;
	card* position = lookrepet(aName, &virtual);
	if (position == NULL) {
		printf("\n�ÿ��Ų����ڣ�\n");
		printf("\n==================================\n");
		pause();
		return;
	}
	printf("���������� <����Ϊ1~9>��");
	scanf_s("%s", aPwd, (unsigned int)sizeof(aPwd));
	logoutinfo* info = doSettleInfo(aName, aPwd, &fAmount);
	if (info == NULL) {
		pause();
		return;
	}
	else{
		//չʾ
		char tmlogon[20] = { 0 };
		Timeshow(info->tlogon, tmlogon);
		char tmlogout[20] = { 0 };
		Timeshow(info->tlogout, tmlogout);
		printf("\n******************************�»���Ϣ����******************************\n\n");
		printf(" %-19s %-8s %-11s %-19s %-19s\n", "����", "����", "���", "�ϻ�ʱ��", "�»�ʱ��");
		printf(" %-20s %-8.2f %-8.2f %-18s %-17s\n", info->aName, info->fUse,info->fBalance, tmlogon,tmlogout);
		printf("\n******************************�»��ɹ���******************************\n\n");
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
			printf("�ڴ����ʧ�ܣ��޷������»�������\n");
			return NULL;
		}
		strcpy(info->aName, pCard->aName);
		info->fBalance = pCard->fBalance;
		info->fUse = *fee;
		info->tlogon = pBill->tStart;
		info->tlogout = pBill->tEnd;
		//�����ļ�
		if ((TRUE == updatebill(pBill, Indexbill)) && (TRUE == updatecard(pCard, Indexcard)))
		{
			readcard();
			getBilling();
			return info;
		}
		else {
			printf("�����˵�����Ϣʧ�ܣ�\n");
			free(info);
			return NULL;
		}
	}
	else {
		printf("\n=================�»�ʧ�ܣ�=================\n");
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
					//����Ϣ
					if (strcmp(pCur->cardo.aPwd, aCode) != 0)
					{
						printf("\n�������\n");
						return NULL;
					}
					if (pCur->cardo.nStatus != 1)
					{
						switch (pCur->cardo.nStatus) {
						case 0:printf("\n�ÿ�δ�ϻ���\n"); return NULL; break;
						case 2:printf("\n�ÿ���ע���������ظ�������\n"); return NULL; break;
						case 3:printf("\n�ÿ���ʧЧ�������Խ����˷ѣ�\n"); return NULL; break;
						default:printf("\n�˷�ϵͳ����\n"); break;
						}
					}
					if (pCur->cardo.fBalance < *fee) {
						printf("\n�»�ʧ�ܣ��������ȳ�ֵ��\n");
						return NULL;
					}
					//����Ϣ
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

void Billlist() {													//��ʼ������
	ipbillnode phead = NULL;
	phead = (ipbillnode)malloc(sizeof(billnode));
	if (phead == NULL) {
		printf("\n�����ڴ�ʧ��\n");
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

ipbillnode querybill(char* aName, int* index) {										//���ұȶ�
	if (BillListHead == NULL || BillListHead->next == NULL) {
		printf("\n����BillListHead Ϊ�ջ�û�����ݣ�����δ��ʼ����\n");
		return NULL;
	}
	ipbillnode temp = BillListHead->next;  // ��������ͷ���
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

//�����ļ�
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
		printf("\n�ļ���ʧ�ܣ�cardpath: %s\n", cardpath);
		return FALSE;
	}

	char tempFile[] = "card_temp.txt";
	fopen_s(&tempFp, tempFile, "w");
	if (tempFp == NULL) {
		printf("\n��ʱ�ļ���ʧ�ܣ�\n");
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
			printf("\n���ļ�ɾ��ʧ�ܣ�\n");
			return FALSE;
		}
		if (rename(tempFile, cardpath) != 0) {
			printf("\n��������ʱ�ļ�ʧ�ܣ�\n");
			return FALSE;
		}
		return TRUE;
	}
	else {
		printf("\nδ�ҵ�ָ�����ż�¼��\n");
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
		printf("\n�����ڴ��Ѿ����꣡\n");
		exit(1);
	}
	if (readBilling(pBilling) == FALSE)
	{
		printf("\n�ļ���ȡʧ�ܣ�");
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