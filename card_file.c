// file name : card_file.c
// ����Ϣ��¼

#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#include <string.h>
#include "card_file.h"
#include "model.h"
#include "tool.h"
#include "menu.h"
#include "global.h"

//����Ϣ
ipcardnode CardListHead = NULL;
int flag = 0;
int del;

ipcardnode* getCardListHead() {
	return &CardListHead;
}

card* lookrepet(char* aName, int* index) {										//���ұȶ�
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

void Cardlist() {                                      //��ʼ������
	ipcardnode phead = (ipcardnode)malloc(sizeof(cardnode));
	if (phead == NULL) {
		printf("�����ڴ�ʧ��");
		return;
	}
	phead->next = NULL;
	CardListHead = phead;
	return;
}

void releaseCardlist() {                                      //�ͷ�����
	ipcardnode temp = CardListHead;
	while (temp) {
		ipcardnode cur = temp->next;
		free(temp);
		temp = cur;
	}
	CardListHead = NULL;
}

int savecard(card pcard, char* ppath) {							//�洢 �¿���Ϣ���ı��ļ�
	FILE* fp = NULL;
	errno_t err = fopen_s(&fp, cardpath, "a");
	if (fp == NULL || err != 0) {
		printf("�ļ���ʧ�ܣ�");
		return FALSE;
	}
	char tmstart[20] = { 0 };
	Timeshow(pcard.tStart, tmstart);
	char tmlast[20] = { 0 };
	Timeshow(pcard.tLast, tmlast);
	char tmend[20] = { 0 };
	Timeshow(pcard.tEnd, tmend);
	//��ʽ������#����#״̬#����ʱ��#��ֹʱ��#�ۻ�����#���ʹ��ʱ��#ʹ�ô���#��ǰ���#ɾ����ʶ#
	fprintf(fp, "%s#%s#%d#%s#%s#%.1f#%s#%d#%.1f#%d#\n", pcard.aName, pcard.aPwd, pcard.nStatus, 
		tmstart, tmend, pcard.fTotalUse, tmlast, pcard.nUseCount, pcard.fBalance, pcard.nDel);
	fclose(fp);
	return TRUE;
}

//��ѯ��
void querycard() {
	char aName[19] = { 0 };
	printf("\n**************��ѯ��**************\n\n");
	int choice = 0;
	printf(" 1.��ȷ��ѯ \n 2.ģ����ѯ\n\n ��ѡ���ѯ���ͣ�");
	scanf_s("%d", &choice);
	switch (choice)
	{
	case 1:  queryInfo(); break;
	case 2:
		printf("\n==================================\n");
		printf("\n �������ѯ�Ŀ��� <����Ϊ1~18>��");							//���뿨��
		scanf_s("%s", aName, (unsigned int)sizeof(aName));
		ipcardnode query = search(aName);
		if (query != NULL) {
			ipcardnode temp = (ipcardnode)malloc(sizeof(cardnode));
			temp = query;
			printf("\n*****************************************��ѯ�Ŀ���Ϣ����*****************************************\n\n");
			printf("%-20s %-12s %-10s %-10s %-15s %-20s\n", "����", "״̬", "���", "�ۼ�����", "ʹ�ô���", "�ϴ�ʹ��ʱ��\n");
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
			printf("\nû�иÿ�����Ϣ��\n");
			printf("\n==================================\n");
		}
		break;
	default:
		printf("\n��������");
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
	printf("�������ѯ������<����Ϊ1~18>:");
	scanf_s("%s", str, 20);
	int flag1 = 0;
	int a = 0;
	card* pCard = lookrepet(str, &a);
	if (pCard == NULL) {													//չʾ��ѯ���  (��/��)
		printf("\nû�иÿ�����Ϣ��\n");
		printf("\n==================================\n");
	}
	else{
		flag1 = 1;
		printf("\n***************************************��ѯ�Ŀ���Ϣ����***************************************\n");
		printf("\n%-20s %-12s %-10s %-10s %-15s %-20s\n", "����", "״̬", "���", "�ۼ�����", "ʹ�ô���", "�ϴ�ʹ��ʱ��\n");
		char current[20] = { 0 };
		Timeshow(pCard->tLast, current);
		printf("%-20s  %-10d  %-10.1f  %-12.1f %-11d %-20s\n\n", pCard->aName, pCard->nDel, pCard->fBalance, pCard->fTotalUse, pCard->nUseCount, current);
		printf("================================================================================================\n\n");
	}
}

ipcardnode search(char* aName) { //Ѱ�ҿ���
	if (aName == NULL) {
		printf("��������Ŀ���Ϊ�գ�\n");
		return NULL;
	}
	ipcardnode temp = (ipcardnode)malloc(sizeof(cardnode));
	if (temp == NULL) {
		printf("tempΪ�գ�����ͷ������ʧ�ܣ�\n");
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
				printf("newcardΪ�գ��½�����ʧ�ܣ�\n");
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
		printf("����δ��ʼ����\n");
		return;
	}
	ipcardnode newcard = (ipcardnode)malloc(sizeof(cardnode));
	if (newcard == NULL) {
		printf("�ڴ����ʧ�ܣ�\n");
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


int getcardcount() {				//�ļ�����Ϣ����
	FILE* fp = NULL;
	int count = 0;
	char Buf[1024] = { 0 };
	errno_t err = fopen_s(&fp, cardpath, "r");
	if (fp == NULL) {
		printf("�ļ���ʧ�ܣ�");
		exit(1);
	}
	while (fgets(Buf, sizeof(Buf), fp) != NULL) {
		count++;
	}
	fclose(fp);
	return count;
}

int readcard() {				//��ȡ�ļ�����Ϣ
	FILE* fp = NULL;
	card card0 = { 0 };
	char Buf[1024] = { 0 };
	errno_t err = fopen_s(&fp, cardpath, "r");
	if (fp == NULL) {
		printf("�ļ���ʧ�ܣ�");
		exit(1);
	}
	releaseCardlist();
	Cardlist();
	del = 0;
	int count = getcardcount();
	if (count == 0) {
		printf("û�п���¼������������\n");
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

