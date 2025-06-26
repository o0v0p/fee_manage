//file name : fee_count.c
// ���ù���

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

//��ֵ
void recharge() {
	card* pCard = NULL;
	double money = 0;
	char aNum[20] = { 0 };
	char aCode[10] = { 0 };
	printf("**************��ֵ**************\n");
	printf("�����뿨��<����Ϊ1~18>��");
	scanf_s("%s", aNum, 20);
	int virtual = 0;
	card* position = lookrepet(aNum, &virtual);
	if (position == NULL) {
		printf("\n�ÿ��Ų����ڣ�\n");
		printf("\n==================================\n");
		pause();
		return;
	}
	printf("����������<����Ϊ1~8>:");
	scanf_s("%s", aCode, 10);
	pCard = domoney_recharge(aNum, aCode, &money);
	if (pCard != NULL)
	{
		printf("\n*****************��ֵ��Ϣ����*****************\n\n");
		printf("%-20s %-12s %-10s\n", "����","��ֵ���","���");
		printf("%-20s %-9.2f %9.2f\n", pCard->aName, money, pCard->fBalance);
		printf("\n*****************��ֵ�ɹ���*****************\n");
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
					printf("\n������������\n");
					return NULL;
				}
				else
				{
					if (pCur->cardo.nStatus != 0 && pCur->cardo.nStatus != 1)
					{
						switch (pCur->cardo.nStatus) {
						case 2:printf("\n�ÿ���ע����\n"); return NULL; break;
						case 3:printf("\n�ÿ���ʧЧ�������Խ����˷ѣ�\n"); return NULL; break;
						default:printf("\n��ֵϵͳ����\n"); break;
						}
					}
					else {
						printf("�������ֵ���:");
						if (scanf_s("%lf", money) != 1 || *money < 0) {
							printf("\n����ʽ����ȷ��\n");
							return NULL;
						}
						pCur->cardo.fBalance += *money;
						pCur->cardo.tLast = time(NULL);
						// ���·��ü�¼���ļ�
						refee fee = { 0 };
						strcpy_s(fee.aName, sizeof(fee.aName), pCur->cardo.aName);
						fee.money = *money;
						fee.feetime = pCur->cardo.tLast;
						fee.nDel = 0;
						fee.operation = 0; // 0��ʾ��ֵ
						if (FALSE != updatecard(&pCur->cardo, index))
						{
							return &pCur->cardo;
						}
						else {
							printf("\n���·���ʧ�ܣ�\n");
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
					printf("\n������������\n");
					return NULL;
				}
				else
				{
					if (pCur->cardo.nStatus == 2 ) {
						printf("\n�ÿ���ע����\n");
						return NULL;
					}
					if (pCur->cardo.nStatus == 3) {
						pCur->cardo.nStatus = 2;
						printf("\n�ÿ��ɹ��˷ѣ����Զ�ע����\n");
					}
					printf("�������˷ѽ��:");
					if (scanf_s("%lf", money) != 1 || *money <= 0) {
						printf("\n����ʽ����ȷ��\n");
						return NULL;
					}
					if (pCur->cardo.fBalance < *money) {
						printf("\n���㣬�޷��˷ѣ�\n");
						return NULL;
					}
					pCur->cardo.fBalance -= *money;
					pCur->cardo.tLast = time(NULL);
					// ���·��ü�¼
					refee fee = { 0 };
					strcpy_s(fee.aName, sizeof(fee.aName), pCur->cardo.aName);
					fee.money = *money;
					fee.feetime = pCur->cardo.tLast;
					fee.nDel = 0;
					fee.operation = 1; // 1��ʾ�˷�
					if (FALSE != updatecard(&pCur->cardo, index))
					{
						return &pCur->cardo;
					}
					else {
						printf("\n���·���ʧ�ܣ�\n");
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

//�˷�
void refund() {
	printf("\n**************�˷�**************\n");
	card* pCard = NULL;
	double money = 0;
	char aNum[20] = { 0 };
	char aCode[10] = { 0 };
	printf("�����뿨��<����Ϊ1~18>��");
	scanf_s("%s", aNum, 20);
	int virtual = 0;
	card* position = lookrepet(aNum, &virtual);
	if (position == NULL) {
		printf("\n�ÿ��Ų����ڣ�\n");
		printf("\n==================================\n");
		pause();
		return;
	}
	printf("����������<����Ϊ1~8>:");
	scanf_s("%s", aCode, 10);
	pCard = domoney_refund(aNum, aCode, &money);
	if (pCard != NULL)
	{
		printf("\n**************�˷���Ϣ����**************\n\n");
		printf("%-20s %-12s %-10s\n", "����", "�˷ѽ��", "���");
		printf("%-20s %-9.2f %9.2f\n", pCard->aName, money, pCard->fBalance);
		printf("\n****************�˷ѳɹ���***************\n");
	}
	else {
		printf("\n==================�˷�ʧ�ܣ�==================\n");
	}
	pause();
}

double fee_count(time_t logtime) {
	double minutes = logtime;
	int time = (int)ceil( minutes / UNIT);
	double fee = FEE * time ;
	return fee;
}
