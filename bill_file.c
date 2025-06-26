//file name : bill_file.c
// 计费记录
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "fee_count.h"
#include "bill_file.h"
#include "bill_service.h"
#include "card_file.h"
#include "model.h"
#include "tool.h"
#include "global.h"
#include "menu.h"
int pdel;

int getbillcount() {				//计费文件计数
	FILE* fp = NULL;
	int nIndex = 0; 
	Billing* pBilling = NULL;
	if ((fp = fopen(billpath, "rb")) == NULL)
	{
		if ((fp = fopen(billpath, "wb+")) == NULL)
		{
			printf("文件打开失败!\n");
			return FALSE;
		}
	}
	pBilling = (Billing*)malloc(sizeof(Billing));
	while (!feof(fp))
	{
		if (pBilling != 0) {
			if (0 != fread(pBilling, sizeof(Billing), 1, fp))
			{
				nIndex++;
			}
		}
	}
	fclose(fp);
	return nIndex;
}

int readBilling(Billing* pBilling)
{
	FILE* fp = NULL;
	Billing* plBiling = NULL;
	int i = 0;
	if ((fp = fopen(billpath, "rb")) == NULL)
	{
		printf("文件打开失败!\n");
		return FALSE;
	}
	if ((plBiling = (Billing*)malloc(sizeof(Billing))) == NULL)
	{
		return FALSE;
	}
	while (fread(plBiling, sizeof(Billing), 1, fp) == 1)
	{
		pBilling[i++] = *plBiling;
	}
	free(plBiling);
	fclose(fp);
	return TRUE;
}
int updatebill(Billing* pBilling, int nIndex)
{
	FILE* fp = NULL;
	Billing* plBilling = NULL;
	int nLine = 0;
	long lPosition = 0;
	if ((fopen_s(&fp,billpath, "rb+"))!= 0)
	{
		return FALSE;
	}
	if ((plBilling = (Billing*)malloc(sizeof(Billing))) == NULL)
	{
		return FALSE;
	}
	while ((!feof(fp)) && (nLine < nIndex))
	{
		if (0 != fread(plBilling, sizeof(Billing), 1, fp))
		{
			lPosition = ftell(fp);
			nLine++;
		}
	}
	fseek(fp, lPosition, 0);
	fwrite(pBilling, sizeof(Billing), 1, fp);
	fclose(fp);
	return TRUE;
}

Billing* checkBilling(char* aNum, int* pIndex,double* fee)
{
	if (getBilling() == FALSE)
	{
		return NULL;
	}
	ipbillnode pCur = NULL;
	Billing* target = NULL;
	ipbillnode BillingList = *getBillListHead();
	if (BillingList != NULL)
	{
		pCur = BillingList->next;
		while (pCur != NULL)
		{
			if ((strcmp(pCur->bill.aName, aNum) == 0) && (pCur->bill.nStatus == 0)&&(pCur->bill.tEnd == 0))
			{
				//计费信息
				strcpy(pCur->bill.aName, aNum);
				pCur->bill.nDel = 0;
				pCur->bill.nStatus = 1;
				pCur->bill.tEnd = time(NULL);
				time_t logtime = pCur->bill.tEnd - pCur->bill.tStart;
				if (pCur->bill.tEnd < pCur->bill.tStart) {
					printf("计费时间异常，终止操作！\n");
					return NULL;
				}
				*fee = fee_count(logtime);
				pCur->bill.fAmount = *fee;
				target = &pCur->bill;
			}
			pCur = pCur->next;
			(*pIndex)++;
		}
		return target;
	}
	return NULL;
}