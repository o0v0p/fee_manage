//file name : querycrd.c
// ��ѯͳ��
#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include "menu.h"
#include "card_file.h"
#include "card_service.h"
#include "querycount.h"
#include "global.h"
#include <string.h>

void query_count() {
    printf("\n**************��ѯͳ��**************\n");
    printf("\n1.  ��״̬��ѯ\n2.  Ӫҵ��ͳ��\n");
    printf("\n====================================\n");
    printf("\n��������ѡ���ѯ��Ŀ��");
    int choice;
    scanf_s("%d", &choice);
    switch (choice) {
    case 1: checkcardStd(); break;
    case 2: totalMoney(); break;
    default: printf("\n��������\n"); 
        printf("\n====================================\n"); 
        break;
    }
    pause();
}

void checkcardStd()
{
    if (readcard() == FALSE)
        return;
    int num1 = 0, num2 = 0, num3 = 0;
    ipcardnode pCur = *getCardListHead();
    if (pCur != NULL)
    {
        pCur = pCur->next;
        while (pCur != NULL)
        {
            if (pCur->cardo.nStatus == 0 || pCur->cardo.nStatus == 1)
                num2++;
            if (pCur->cardo.nStatus == 2 || pCur->cardo.nStatus == 3)
                num3++;
            pCur = pCur->next;
            num1++;
        }
    }
    printf("\n*********************��״̬��ѯ��Ϣ��Ϣ����*********************\n");
    printf("\n��ֹĿǰ��\n����%d�ſ����ע�ᣡ\n��%d�ſ���Ȼ��ʹ����!\n��%d�ſ��Ѿ�����ע��!\n", num1, num2, num3);
    printf("\n================================================================\n");
}


void totalMoney() {
    double totalUsed = 0.0, totalRecharge = 0.0;
    ipcardnode pCur = NULL;
    ipcardnode head = *getCardListHead();
    if (head != NULL) {
        pCur = head->next;
        while (pCur != NULL) {
            totalUsed += pCur->cardo.fTotalUse;
            totalRecharge += pCur->cardo.fTotalUse + pCur->cardo.fBalance;
            pCur = pCur->next;
        }
        printf("\n*********************Ӫҵ��ͳ����Ϣ��Ϣ����*********************\n");
        printf("\nӪҵ����: \n�����û���ֵ%.2lfԪ!\nʹ�ö�ȹ�%.2lfԪ\n", totalRecharge, totalUsed);
        printf("\n================================================================\n");
    }
}