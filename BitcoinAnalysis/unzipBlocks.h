#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <string>
#include "BlockProcess.h"
#include "BlockFileName.h"
#include "TransactionProcess.h"
#include "Statistical.h"
#include "timerange.h"

using namespace std;

//#define BLKFILE 0
//#define BLKPOS 2171908

//#define STOPYEAR 2009
//#define STOPMON 4
//#define STOPDAY 30

//#define BLOCKNUM 9355 //���ļ���¼��+1
//#define TXCOUNTER 9445 //���ļ���¼��+1

int unzipBlocks(MYSQL &conn, leveldb::DB* &tx_db)
{
	//maskfile.open("I:\\maskfile.txt", ios::app);//��־�ļ�

	ofstream logfile; //������־�ļ�

	logfile.open("H:\\1.unzipBlock_logfile.txt", ios::app);//��־�ļ�
	//��¼ϵͳ����ʱ�䣬д����־�ļ�
	time_t t = time(0);
	char time_c[64] = { 0 };
	strftime(time_c, sizeof(time_c), "%Y-%m-%d %X %A", localtime(&t));
	string OStime = time_c;
	logfile << "-----------------" << endl;
	logfile << OStime << endl;
	logfile << "-----------------" << endl;

	//MYSQL conn;
	//leveldb::DB* tx_db;
	//start_database(conn, tx_db);//ͬʱ����MySQL���ݿ��LevelDB���ݿ�

	unit32 testflag = 0;

	//����ǰ���޸ĵĲ���
	unit32 BLKFILE;// 0 //���ļ��� '0'
	unit32 BLKPOS; //2171908 //���ļ���ʼ��ȡλ�� '0'

	int BLOCKNUM;// 9355 //���ļ���¼��+1 //��ʼ���п�� '-1'
	int TXCOUNTER;// 9445 //���ļ���¼��+1 // ��ʼ���н��׺� '-1'

	unit32 STOPYEAR;// 2009
	unit32 STOPMON;// 4
	unit32 STOPDAY;// 30
	//int para = 19;
	//logfile << "-----------------" << endl;
	//logfile << "para: " << para << endl;
	//logfile << "-----------------" << endl;
	//set_parameter(para);

	ifstream parameter;
	parameter.open("H:\\para\\1.unzip_0.txt", ios::in);
	parameter >> BLKFILE >> BLKPOS >> BLOCKNUM >> TXCOUNTER >> entitycnt;
	parameter.close();

	parameter.open("H:\\para\\1.unzip_1.txt", ios::in);
	parameter >> STOPYEAR >> STOPMON >> STOPDAY; //���ļ�������Ҫ���е���ֹ��ʱ��
	parameter.close();

	//ÿ������ǰ��Ҫ�޸�
	int BlockNum = BLOCKNUM + 1; // ���
	int txcounter = TXCOUNTER + 1; // ���׼���������¼�ܹ��ǵڼ�������
	logfile << "First Block Number: " << BlockNum << endl;
	logfile << "First Transaction Number: " << txcounter << endl;

	struct tm stoptm; //��ֹ��ʱ��
	stoptm.tm_year = STOPYEAR - 1900;
	stoptm.tm_mon = STOPMON - 1;
	stoptm.tm_mday = STOPDAY;
	stoptm.tm_hour = 23;
	stoptm.tm_min = 59;
	stoptm.tm_sec = 59;
	stoptm.tm_isdst = 0;
	time_t stoptm_t;
	stoptm_t = mktime(&stoptm);

	ifstream BlockFile; //�����block�ļ�
	int blkfile_num = 0;//��¼�����ڼ����ļ�
	int blockPos = 0;//��¼�ļ�ָ��
	for (blkfile_num = BLKFILE; blkfile_num < 218; blkfile_num++) // �������п��ļ�
	{
		string blkname_s = blkname[blkfile_num];
		cout << blkname_s; // �����ǰ���ļ�������ʾ������������

		BlockFile.open(blkname[blkfile_num], ios::in | ios::binary);

		if (BlockFile.fail())
		{
			cout << "  open error " << endl;
			logfile << blkname_s;
			logfile << "  open error " << endl;
		}
		if (!BlockFile.fail())
		{
			cout << "  open ok " << endl;
			logfile << blkname_s;
			logfile << "  open ok " << endl;
		}

		blockPos = 0; // ��ǰ���ļ�blk*****.dat�ļ�ָ��λ��
		if (blkfile_num == BLKFILE) // ���¿�ʼ�Ŀ���Ҫ�����Ѿ���ȡ�Ĳ���
		{
			blockPos = BLKPOS;
			BlockFile.seekg(blockPos, ios::beg);
		}

		while (BlockFile.peek() != EOF)
		{
			char blockStart[88] = { 0 };//����ײ�����88bytes
			BlockFile.read(blockStart, 88);
			Block curBlock; //��ŵ�ǰblock��Ϣ
			memcpy(&curBlock.Head, blockStart, 88);
			//���㵱ǰblock��hashֵ��������magic number
			twiceSHA256(blockStart + 8, 80, curBlock.curhash);
			curBlock.blocknumber = BlockNum;
			BlockNum++;

			//logfile << "Block #: " << curBlock.blocknumber << "(";
			//cout << "Block Size: " << curBlock.Head.Blocksize << endl;
			//cout << "Block ver: " << curBlock.Head.Version << endl;
			//cout << "Prev hash: ";  printHash(curBlock.Head.preBlockHash);
			//cout << "curr hash: ";  printHash(curBlock.curhash);
			//cout << "Merk Root: ";  printHash(curBlock.Head.merkRoot);
			//char time_p[21];
			//getTimeString(time_p, curBlock.Head.timeStamp);
			//cout << "Timestamp: "<<time_p<<endl;
			//cout << "Block bits: " << curBlock.Head.bits << endl;
			//cout << "Block nonce: " << curBlock.Head.nonce << endl;

			//�ж�block����ʱ�䣬������ֹʱ�����˳�
			time_t blktm(curBlock.Head.timeStamp);
			double difftm = difftime(blktm, stoptm_t);
			if (difftm > 0)
			{
				testflag = 1;
				logfile << "File Pointer: " << blockPos << endl;
				break;
			}

			//����ռ��Ž���
			char* transSec = new char[curBlock.Head.Blocksize - 80];
			BlockFile.seekg(blockPos + 88, ios::beg);
			BlockFile.read(transSec, (curBlock.Head.Blocksize - 80));

			int transPos = 0; // ��ǰ�����ļ�ָ��λ��

			curBlock.trans_count = readVarLen(transSec, transPos);
			cout << "Block #: " << curBlock.blocknumber << "(" << curBlock.trans_count << ")" << endl;
			//logfile << curBlock.trans_count << ")" << endl;
			unit16 txNum = 0;
			for (unit32 k = 0; k < curBlock.trans_count; k++)
			{
				Transaction tx; // ��ŵ�ǰtransaction��Ϣ
				//cout << k << endl;
				tx.blocknumber = curBlock.blocknumber; // ��¼�������ڿ��
				tx.txnumber = txNum; // �ǿ��еĵڼ�������
				tx.txcounter = txcounter; // �ܹ��ڼ�������
				tx.txtime = curBlock.Head.timeStamp; // ��¼���׷���ʱ��
				txNum++;
				txcounter++;

				if (txcounter % 1000 == 0)
				{
					cout << txcounter << endl;// �������������ʾ�������
				}

				unit32 tx_start = transPos; // ������ʼλ��

				tx.ver = readU32(transSec, transPos); // �汾��

				tx.in_count = readVarLen(transSec, transPos); // input������
				for (unit32 i = 0; i < tx.in_count; i++)
				{
					Txin txin;
					readHash(txin.hash, transSec, transPos);
					txin.index = readU32(transSec, transPos);
					txin.script_length = readVarLen(transSec, transPos);
					txin.script = readBufferAdvance(transSec, transPos, txin.script_length);
					txin.seq = readU32(transSec, transPos);
					tx.ins.push_back(txin);
				}

				tx.out_count = readVarLen(transSec, transPos);
				unit64 sumoutvalue = 0;
				for (unit32 i = 0; i < tx.out_count; i++)
				{
					Txout txout;
					txout.val = readU64(transSec, transPos);
					// ���㵱ǰBLOCK output valueֵ�ĺ�
					if (tx.txnumber == 0)
					{
						sumoutvalue = sumoutvalue + txout.val;
					}
					txout.script_length = readVarLen(transSec, transPos);
					txout.script = readBufferAdvance(transSec, transPos, txout.script_length);
					//��Scriptת��Ϊʵ�ʵ�20bytes Address
					//unit8 Address[20] = { 0 };
					if (!outAddress(txout.script, txout.script_length, txout.address))
					{
						logfile << "!!! outAddress error !!!" << endl;
						logfile << tx.blocknumber << "(" << tx.txnumber << ")" << endl;
					}
					tx.outs.push_back(txout);
				}
				tx.lock_time = readU32(transSec, transPos);

				unit32 tx_end = transPos;

				tx.length = tx_end - tx_start; // �����ļ�����
				twiceSHA256(transSec + tx_start, tx.length, tx.curhash);

				//curBlock.trans.push_back(tx);
				// ���㵱ǰ��Ľ���
				if (tx.txnumber == 0)
				{
					if (curBlock.blocknumber<210000)
					{
						curBlock.blockReward = sumoutvalue - (unit64)50 * ONE_BTC;
					}
					else
					{
						curBlock.blockReward = sumoutvalue - (unit64)25 * ONE_BTC;
					}
				}

				//tx2diskmysql(tx, conn);
				if ((tx.txcounter != 142783) && (tx.txcounter != 142841))
				{
					TransactionProcess(tx, conn, tx_db);
				}


				//cout << mask++ << endl;

				// ���ָ�� input �� output �е� script ����
				for (unit32 i = 0; i < tx.in_count; i++)
				{
					delete[] tx.ins[i].script;
				}

				for (unit32 i = 0; i < tx.out_count; i++)
				{
					delete[] tx.outs[i].script;
				}
			}
			//cout << endl << "****************************************" << endl;

			delete[] transSec;

			//cout << mask++ << endl;

			BlockProcess(curBlock, conn);
			//Initial 8 blocks + size
			blockPos = blockPos + curBlock.Head.Blocksize + 8;
			BlockFile.seekg(blockPos, ios::beg); // �����Զ�ȡ��Block

			// ������ֹ����
			//if (blockNum > 180000) // Only for test
			//{
			//testflag = 1;
			//break; 
			//}	
			//if (txcounter > 2527) // Only for test
			//{
			//	testflag = 1;
			//	break; 
			//}
		}
		BlockFile.close();

		// ������ֹ����
		if (testflag)
			break;
	}
	//cout << mask++ << endl;

	update_entity_mysql(entitychange_v, conn);//����entity

	//cout << mask++ << endl;

	//�˴�Ӧ�����ͳ�ƺ���

	//delete tx_db;

	//statistical(conn);

	//mysql_close(&conn);

	// ����־��д�� �����Ŀ����ͽ�����
	logfile << "File contains " << (BlockNum - 1) << " blocks." << endl;
	logfile << "File contains " << txcounter << " transactions." << endl;
	logfile << "Last Block Number: " << (BlockNum - 2) << endl;
	logfile << "Last Transaction Number: " << (txcounter - 1) << endl;
	logfile.close();

	ofstream trans_self;
	trans_self.open("H:\\para\\1.unzip_0.txt", ios::out | ios::trunc);//���ݸ�����Ĳ���
	trans_self << blkfile_num << endl;//��ǰ���ļ���
	trans_self << blockPos << endl;//��ǰ���ļ�ָ��
	trans_self << (BlockNum - 2) << endl; //��������һ����ţ���ʼֵ-1��
	trans_self << (txcounter - 1) << endl; //��������һ�����׺ţ���ʼֵ-1��
	trans_self << entitycnt << endl;
	trans_self.close();

	cout << "File contains " << (BlockNum - 1) << " blocks." << endl;
	cout << "File contains " << txcounter << " transactions." << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "all process OK :) !" << endl << endl << endl;

	//getchar();
	return true;
}
