//file name : menu.c
// �˵�

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "model.h"
#include "global.h"
#include "bill_file.h"
#include "bill_service.h"
#include "card_file.h"
#include "card_service.h"
#include "fee_count.h"
#include "tool.h"
#include "menu.h"
#include "querycount.h"

int cardcount = 0;
void outmenu() {										//����˵�
	printf("\n**************�˵�**************\n\n");
	printf("1. ��ӿ�\n");
	printf("2. ��ѯ��\n");
	printf("3. �ϻ�\n");
	printf("4. �»�\n");
	printf("5. ��ֵ\n");
	printf("6. �˷�\n");
	printf("7. ע����\n");
	printf("8. ��ѯͳ��\n");
	printf("0. �˳�ϵͳ\n\n");
	updateExpiredCards();
}

void addcard()
{
	card card0 = { 0 };
	int namesize = 0, pwdsize = 0;
	printf("\n**************��ӿ�**************\n\n");
	printf("�����뿨�� <����Ϊ1~18>��");							//���뿨��
	scanf_s("%s", card0.aName, (unsigned int)sizeof(card0.aName));
	namesize = getsize(card0.aName);
	if (namesize > 18 || namesize < 1) {
		printf("\n���ų��Ȳ�����Ҫ��\n");
		printf("\n==================================\n");
		pause();
		return;
	}
	if (cardcount != 0) {
		int virtual = 0;
		card* position = lookrepet(card0.aName, &virtual);								//���Ų����ظ�
		if (position != NULL) {
			printf("\n�ÿ����Ѵ��ڣ�\n");
			printf("\n==================================\n");
			pause();
			return;
		}
	}
	printf("���������� <����Ϊ1~8>��");								//��������
	scanf_s("%s", card0.aPwd, (unsigned int)sizeof(card0.aPwd));
	pwdsize = getsize(card0.aPwd);
	if (pwdsize > 8 || pwdsize < 1) {
		printf("\n���볤�Ȳ�����Ҫ��\n");
		printf("\n==================================\n");
		pause();
		return;
	}
	printf("�����뿪����� <RMB>��");								//������
	if (scanf_s("%lf", &card0.fBalance) == 0) {
		printf("\n����ʽ����ȷ��\n");
		printf("\n==================================\n");
		pause();
		return;
	}
	// ��ʼ��ʱ���ֶ�
	card0.fTotalUse = 0;							//��ʼ��
	card0.nDel = 0;
	card0.nStatus = 0;
	card0.nUseCount = 0;
	card0.tStart = card0.tLast = card0.tEnd = time(NULL);
	struct tm Tm = { 0 };
	localtime_s(&Tm, &card0.tStart);
	Tm.tm_year += 1;
	card0.tEnd = mktime(&Tm);
	// ��ʼ��ʱ������
	for (int i = 0; i < 100; i++) {
		memset(card0.everytime1[i], 0, 20);
		memset(card0.everytime2[i], 0, 20);
		memset(card0.everytime3[i], 0, 20);
		memset(card0.everytime4[i], 0, 20);
		card0.everymoney[i] = 0;
	}
	if (cardcount == 0) {										//¼�ļ�������
		FILE* fp;
		errno_t err = fopen_s(&fp, cardpath, "w+");
		if (err != 0 || fp == NULL)
			printf("�ļ���ʧ�ܣ�");
		if (fp != 0)
			fclose(fp);
		else {
			printf("�ļ�¼�����");
			exit(1);
		}
	}
	cardcount++;
	if (savecard(card0, cardpath))
	{
		//չʾ
		printf("\n*********************��ӳɹ����¿���Ϣ����*********************\n\n");
		printf("%-21s %-14s %-11s %-10s\n", "����", "����", "״̬", "�������");
		printf("%-20s  %-15s  %-10d  %-10.1f \n\n", card0.aName, card0.aPwd, card0.nDel, card0.fBalance);
		printf("================================================================\n");
		readcard();
		pause();
		return;
	}
	else
	{
		printf("���ʧ��!\n");
	}
}

int getsize(const char* input) {							//�ж����볤��
	int size = 0;
	size = (int)strlen(input);
	return size;
}

void pause() {									//��Enter������
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF);
	printf("��Enter������...");
	while ((ch = getchar()) != '\n' && ch != EOF);
}
