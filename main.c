//file name : main.c
// �Ʒѹ���ϵͳ������

#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "model.h"
#include "global.h"
#include "bill_service.h"
#include "bill_file.h"
#include "card_file.h"
#include "card_service.h"
#include "fee_count.h"
#include "querycount.h"

void implement(int n);

int main() {                                    //���������
    int choice = -1;
    char ch;
    Cardlist();
    Billlist();
    printf("\n��ӭ�����Ʒѹ���ϵͳ!\n");
    do {
        choice = -1;
        outmenu();                                //����˵�
        printf("��ѡ��˵����ţ�0-8��: ");
        int check = scanf_s("%d", &choice);
        while ((ch = getchar()) != '\n' && ch != EOF);
        if (0 <= choice && choice <= 8 && check)
            implement(choice);                        //���������� ִ�� ��Ӧ����
        else {
            printf("==================================\n");
            printf("\n����������������룡\n");
        }
    } while (choice != 0);                        //ѭ���˵�
    return 0;
}

void implement(int n) {
    switch (n) {
    case 1: addcard(); break;                //��ӿ�
    case 2: querycard(); break;                //��ѯ��
    case 3: logOn(); break;                    //�ϻ�
    case 4: logOut(); break;                //�»�
    case 5: recharge(); break;                //��ֵ
    case 6: refund(); break;                //�˷�
    case 7: deletecard(); break;            //ע����
    case 8: query_count(); break;            //��ѯͳ��
    case 0: 
        printf("=================================\n");
        printf("\n��лʹ�ã����򼴽��˳���\n");
        exit(0);
        break;                    //�˳�ϵͳ
    default: {
        printf("\n��Ч�Ĳ˵�ѡ�������ѡ��\n");
        break;
    }
    }
}

