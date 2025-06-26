//file name : fee_count.c
// 费用管理

#include "global.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "card_file.h"
#include "bill_service.h"
#include "card_service.h"
#include "fee_count.h"
#include "model.h"
#include "menu.h"
#include "querycount.h"
int mflag = 0;

//充值
void recharge() {
	card* pCard = NULL;
	double money = 0;
	char aNum[20] = { 0 };
	char aCode[10] = { 0 };
	printf("**************充值**************\n");
	printf("请输入卡号<长度为1~18>：");
	scanf_s("%s", aNum, 20);
	int virtual = 0;
	card* position = lookrepet(aNum, &virtual);
	if (position == NULL) {
		printf("\n该卡号不存在！\n");
		printf("\n==================================\n");
		pause();
		return;
	}
	printf("请输入密码<长度为1~8>:");
	scanf_s("%s", aCode, 10);
	pCard = domoney_recharge(aNum, aCode, &money);
	if (pCard != NULL)
	{
		printf("\n*****************充值信息如下*****************\n\n");
		printf("%-20s %-12s %-10s\n", "卡号","充值金额","余额");
		printf("%-20s %-9.2f %9.2f\n", pCard->aName, money, pCard->fBalance);
		printf("\n*****************充值成功！*****************\n");
	}
	else {
		printf("\n==================================\n");
	}
	pause();
}

card* domoney_recharge(char* aName, char* aPwd, double* money) {
	int index = 0;
	if (readcard() == FALSE)
	{
		return NULL;
	}
	ipcardnode pCur = *getCardListHead();
	if (pCur != NULL)
	{
		pCur = pCur->next;
		while (pCur != NULL)
		{
			if (strcmp(pCur->cardo.aName, aName) == 0)
			{
				if (strcmp(pCur->cardo.aPwd, aPwd) != 0)
				{
					printf("\n输入的密码错误！\n");
					return NULL;
				}
				else
				{
					if (pCur->cardo.nStatus != 0 && pCur->cardo.nStatus != 1)
					{
						switch (pCur->cardo.nStatus) {
						case 2:printf("\n该卡已注销！\n"); return NULL; break;
						case 3:printf("\n该卡已失效！仅可以进行退费！\n"); return NULL; break;
						default:printf("\n充值系统出错\n"); break;
						}
					}
					else {
						printf("请输入充值金额:");
						if (scanf_s("%lf", money) != 1 || *money < 0) {
							printf("\n金额格式不正确！\n");
							return NULL;
						}
						pCur->cardo.fBalance += *money;
						pCur->cardo.tLast = time(NULL);
						// 更新费用记录并文件
						refee fee = { 0 };
						strcpy_s(fee.aName, sizeof(fee.aName), pCur->cardo.aName);
						fee.money = *money;
						fee.feetime = pCur->cardo.tLast;
						fee.nDel = 0;
						fee.operation = 0; // 0表示充值
						if (FALSE != updatecard(&pCur->cardo, index))
						{
							return &pCur->cardo;
						}
						else {
							printf("\n更新费用失败！\n");
							return NULL;
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


card* domoney_refund(char* aName, char* aPwd, double* money) {
	int index = 0;
	if (readcard() == FALSE)
	{
		return NULL;
	}
	ipcardnode pCur = *getCardListHead();
	if (pCur != NULL)
	{
		pCur = pCur->next;
		while (pCur != NULL)
		{
			if (strcmp(pCur->cardo.aName, aName) == 0)
			{
				if (strcmp(pCur->cardo.aPwd, aPwd) != 0)
				{
					printf("\n输入的密码错误！\n");
					return NULL;
				}
				else
				{
					if (pCur->cardo.nStatus == 2 ) {
						printf("\n该卡已注销！\n");
						return NULL;
					}
					if (pCur->cardo.nStatus == 3) {
						pCur->cardo.nStatus = 2;
						printf("\n该卡成功退费！已自动注销！\n");
					}
					printf("请输入退费金额:");
					if (scanf_s("%lf", money) != 1 || *money <= 0) {
						printf("\n金额格式不正确！\n");
						return NULL;
					}
					if (pCur->cardo.fBalance < *money) {
						printf("\n余额不足，无法退费！\n");
						return NULL;
					}
					pCur->cardo.fBalance -= *money;
					pCur->cardo.tLast = time(NULL);
					// 更新费用记录
					refee fee = { 0 };
					strcpy_s(fee.aName, sizeof(fee.aName), pCur->cardo.aName);
					fee.money = *money;
					fee.feetime = pCur->cardo.tLast;
					fee.nDel = 0;
					fee.operation = 1; // 1表示退费
					if (FALSE != updatecard(&pCur->cardo, index))
					{
						return &pCur->cardo;
					}
					else {
						printf("\n更新费用失败！\n");
						return NULL;
					}
				}
			}
			pCur = pCur->next;
			index++;
		}
	}
	return NULL;
}

//退费
void refund() {
	printf("\n**************退费**************\n");
	card* pCard = NULL;
	double money = 0;
	char aNum[20] = { 0 };
	char aCode[10] = { 0 };
	printf("请输入卡号<长度为1~18>：");
	scanf_s("%s", aNum, 20);
	int virtual = 0;
	card* position = lookrepet(aNum, &virtual);
	if (position == NULL) {
		printf("\n该卡号不存在！\n");
		printf("\n==================================\n");
		pause();
		return;
	}
	printf("请输入密码<长度为1~8>:");
	scanf_s("%s", aCode, 10);
	pCard = domoney_refund(aNum, aCode, &money);
	if (pCard != NULL)
	{
		printf("\n**************退费信息如下**************\n\n");
		printf("%-20s %-12s %-10s\n", "卡号", "退费金额", "余额");
		printf("%-20s %-9.2f %9.2f\n", pCard->aName, money, pCard->fBalance);
		printf("\n****************退费成功！***************\n");
	}
	else {
		printf("\n==================退费失败！==================\n");
	}
	pause();
}

double fee_count(time_t logtime) {
	double minutes = logtime;
	int time = (int)ceil( minutes / UNIT);
	double fee = FEE * time ;
	return fee;
}
