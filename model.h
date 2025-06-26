#pragma once// �ļ�����model.h
// ���ݣ�������������ݽṹ����

#include <stdio.h>
#include <time.h>

//����Ϣ
typedef struct Card {
	char aName[19];		//���ţ�����Ϊ1~18���ַ�
	char aPwd[9];		//���룬����Ϊ1~8���ַ�
	int nUseCount;		//ʹ�ô���
	int nStatus;		//��״̬  (0-δ�ϻ���1-�����ϻ���2-��ע����3-ʧЧ)
	int nDel; 			//ɾ����־  (0-δɾ����1-��ɾ��)
	time_t tStart;		//����ʱ��,��ʼĬ��Ϊ��ӿ���ʱ��
	time_t tEnd;		//��ֹʱ��,��ʼĬ��Ϊ��ӿ���ʱ��
	time_t tLast;		//���ʹ��ʱ��,��ʼĬ��Ϊ��ӿ���ʱ��
	double fTotalUse;	//�ۼ�����
	double fBalance;		//���
	double everymoney[100];  // ÿ���ϻ�����
	char everytime1[100][20];  //��¼�ϻ�ʱ��
	char everytime2[100][20];   //��¼�»�ʱ��
	char everytime3[100][20];  //��¼�ϻ�ʱ��(��������)
	char everytime4[100][20];   //��¼�»�ʱ��(��������)
}card;

//�Ʒ���Ϣ
typedef struct Fee {
	char aName[19];		//���ţ�����Ϊ1~18���ַ�
	double fAmount;		//����
	time_t tStart;		//�ϻ�ʱ��
	time_t tEnd;		//�»�ʱ��
	int nStatus;		//��״̬  (0-δ���㣻1-�ѽ���)
	int nDel; 			//ɾ����־  (0-δɾ����1-��ɾ��)
}Billing;

//�Ʒ�����
typedef struct Billnode {
	Billing bill;
	struct Billnode* next;
}billnode, * ipbillnode;

//�ϻ���Ϣ
typedef struct logonInfo {
	char aName[19];		//�ϻ�����
	double fBalance;	//�ϻ�ʱ�����
	time_t tlogon;		//�ϻ�ʱ��
}logoninfo;

//�»���Ϣ
typedef struct logoutInfo {
	char aName[19];			//����
	double fUse;	        //����
	double fBalance;	    //���
	time_t tlogon;		//�ϻ�ʱ��
	time_t tlogout;		//�»�ʱ��
}logoutinfo;

//�Ʒѱ�׼��Ϣ
typedef struct Standard {
	double charge;		//ÿ���Ʒѵ�Ԫ���շ�
	int Starttime;		//��ʼʱ��
	int Endtime;		//����ʱ��
	int ratetype;		//�շ����(0-��ͨ��1-��ҹ��2-����)
	int unit;			//��С�Ʒѵ�Ԫ(����)
	int del; 			//ɾ����־  (0-δɾ����1-��ɾ��)
}standard;

//��ֵ�˷���Ϣ
typedef struct Refee {
	char aName[19];		//���ţ�����Ϊ1~18���ַ�
	double money;		//����
	time_t feetime;		//����ʱ��
	int operation;		//�������  (0-��ֵ��1-�˷�)
	int nDel; 			//ɾ����־  (0-δɾ����1-��ɾ��)
}refee;

//����Ա��Ϣ
typedef struct Manager {
	char name[19];		//�û���������Ϊ1~18���ַ�
	char Pwd[9];		//���룬����Ϊ1~8���ַ�
	int privilge;		//Ȩ�޵ȼ�
	int nDel; 			//ɾ����־  (0-δɾ����1-��ɾ��)
}manager;

//��Ƭ����
typedef struct Cardnode {
	card cardo;
	struct Cardnode* next;
}cardnode, * ipcardnode;