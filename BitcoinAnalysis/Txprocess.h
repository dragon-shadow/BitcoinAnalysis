#pragma once
#include <Windows.h>
#include <mysql.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <ctime>
#include "fileprocess.h"
#include "set.h"
#include "mysql_api.h"

using namespace std;

//#define TXSTART 9445
//#define TXEND 12896

int main()
{
	MYSQL conn;
	start_mysql(conn);

	ofstream logfile;
	logfile.open("I:\\2.txprocess_logfile.txt", ios::app);//��־�ļ�
	time_t t = time(0);
	char time_c[64] = { 0 };
	strftime(time_c, sizeof(time_c), "%Y-%m-%d %X %A", localtime(&t));
	string OStime = time_c;
	logfile << "-----------------" << endl;
	logfile << OStime << endl;
	logfile << "-----------------" << endl;

	//stringstream ss; ��ͷ�ļ����԰���������Ϊȫ�ֱ���
	string txcounter;

	unit8 *curtx = NULL; // ���浱ǰ��������
	string curhash_s;

	unit8 *pretx = NULL; // ����input��ָ����һ������
	unit8  prehash[65] = { 0 }; // ÿ��input��ָ���prehash
	string prehash_s; // ����unit8  prehash[65]��ת����string��ʽ
	unit32 index = 0; // ÿ��input��������index

	unit32 length = 0;// ���׳���

	unit32 TXSTART; //9445 //���ļ������뽻����ʼ����
	unit32 TXEND; //12896
	fstream parameter;
	parameter.open("I:\\para\\2.tx_start_end.txt", ios::in);
	parameter >> TXSTART >> TXEND;
	parameter.close();

	unit32 i = TXSTART;
	logfile << "first transaction number: " << i << endl;
	for (i = TXSTART; i <= TXEND; i++)// 3��Ϊ����
	{
		if ((i == 142783) || (i == 142841))
		{
			continue;
		}
		if (i % 2048 == 0)
		{
			cout << i << endl;// �������������ʾ�������
		}

		ss.str("");
		ss.clear();
		txcounter.clear();
		ss << i;
		ss >> txcounter;
		// ȡ��txcounter��Ӧ���׵�hashֵ
		hash_mysql(txcounter, conn, curhash_s);

		curtx = readtx(curhash_s, length); // �����������׶������ļ�
		//cout<<"blocknumber: ";
		unit32 blocknumber = 0;
		memcpy(&blocknumber, (curtx + 0), 4);
		//cout << blocknumber << endl;

		unit32 incounter = 0; // ȡ��incounter
		memcpy(&incounter, (curtx + 80), 4);
		//cout << "in_counter: ";
		//cout << incounter << endl;

		unit8 *inputnew = new unit8[incounter * 48];
		unit32 inpos = 84; // input��ʼ���֣���ȡÿ��input
		for (unit32 i = 0; i < incounter; i++)
		{
			memset(prehash, 0, 65);
			memcpy(prehash, (curtx + inpos), 64); // ��ȡprehash
			memcpy(&index, (curtx + inpos + 64), 4); // ��ȡindex
			inpos = inpos + 68; // inposָ����һ��input

			//prehash_s.clear();
			prehash_s = (char*)prehash;
			//cout << "prehash: " << endl << prehash << endl;

			readoutput(blocknumber, prehash_s, index, (inputnew + i * 48));
		}

		unit32 lengthnew = length - incounter * 68 + incounter * 48;
		unit8 *txnew = new unit8[lengthnew];
		memcpy(txnew, curtx, 84);
		memcpy(txnew + 84, inputnew, incounter * 48);
		memcpy(txnew + 84 + incounter * 48, (curtx + 84 + (incounter * 68)), length - 84 - incounter * 68);
		// ��input���ִ����Ľ���д������ļ���ͬʱ���ý��׵��й���Ϣ����MySQL���ݿ�
		delete[] curtx;
		delete[] inputnew;

		string dataPath = "I:\\Block1\\" + curhash_s + ".txt";//ɾ���Ѵ�����ɵ��ļ�
		remove(dataPath.c_str());

		tx2diskmysql(curhash_s, txnew, lengthnew, conn);
		delete[] txnew;
	}
	mysql_close(&conn);
	logfile << "last transaction number: " << (i - 1) << endl;
	logfile.close();

	cout << "~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "txprocess ok :)" << endl << endl << endl;

	//getchar();
	return 0;
}
