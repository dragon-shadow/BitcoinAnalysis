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

//#define BLOCKNUM 9355 //“文件记录”+1
//#define TXCOUNTER 9445 //“文件记录”+1

int unzipBlocks(MYSQL &conn, leveldb::DB* &tx_db)
{
	//maskfile.open("I:\\maskfile.txt", ios::app);//日志文件

	ofstream logfile; //定义日志文件

	logfile.open("H:\\1.unzipBlock_logfile.txt", ios::app);//日志文件
	//记录系统运行时间，写入日志文件
	time_t t = time(0);
	char time_c[64] = { 0 };
	strftime(time_c, sizeof(time_c), "%Y-%m-%d %X %A", localtime(&t));
	string OStime = time_c;
	logfile << "-----------------" << endl;
	logfile << OStime << endl;
	logfile << "-----------------" << endl;

	//MYSQL conn;
	//leveldb::DB* tx_db;
	//start_database(conn, tx_db);//同时连接MySQL数据库和LevelDB数据库

	unit32 testflag = 0;

	//运行前需修改的参数
	unit32 BLKFILE;// 0 //块文件名 '0'
	unit32 BLKPOS; //2171908 //块文件开始读取位置 '0'

	int BLOCKNUM;// 9355 //“文件记录”+1 //开始运行块号 '-1'
	int TXCOUNTER;// 9445 //“文件记录”+1 // 开始运行交易号 '-1'

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
	parameter >> STOPYEAR >> STOPMON >> STOPDAY; //在文件中设置要运行的终止的时间
	parameter.close();

	//每次运行前需要修改
	int BlockNum = BLOCKNUM + 1; // 块号
	int txcounter = TXCOUNTER + 1; // 交易计数器，记录总共是第几个交易
	logfile << "First Block Number: " << BlockNum << endl;
	logfile << "First Transaction Number: " << txcounter << endl;

	struct tm stoptm; //终止块时间
	stoptm.tm_year = STOPYEAR - 1900;
	stoptm.tm_mon = STOPMON - 1;
	stoptm.tm_mday = STOPDAY;
	stoptm.tm_hour = 23;
	stoptm.tm_min = 59;
	stoptm.tm_sec = 59;
	stoptm.tm_isdst = 0;
	time_t stoptm_t;
	stoptm_t = mktime(&stoptm);

	ifstream BlockFile; //定义打开block文件
	int blkfile_num = 0;//记录处理到第几个文件
	int blockPos = 0;//记录文件指针
	for (blkfile_num = BLKFILE; blkfile_num < 218; blkfile_num++) // 遍历所有块文件
	{
		string blkname_s = blkname[blkfile_num];
		cout << blkname_s; // 输出当前那文件名，表示程序正在运行

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

		blockPos = 0; // 当前块文件blk*****.dat文件指针位置
		if (blkfile_num == BLKFILE) // 重新开始的块需要跳过已经读取的部分
		{
			blockPos = BLKPOS;
			BlockFile.seekg(blockPos, ios::beg);
		}

		while (BlockFile.peek() != EOF)
		{
			char blockStart[88] = { 0 };//存放首部，共88bytes
			BlockFile.read(blockStart, 88);
			Block curBlock; //存放当前block信息
			memcpy(&curBlock.Head, blockStart, 88);
			//计算当前block的hash值，不包含magic number
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

			//判断block生成时间，大于终止时间则退出
			time_t blktm(curBlock.Head.timeStamp);
			double difftm = difftime(blktm, stoptm_t);
			if (difftm > 0)
			{
				testflag = 1;
				logfile << "File Pointer: " << blockPos << endl;
				break;
			}

			//申请空间存放交易
			char* transSec = new char[curBlock.Head.Blocksize - 80];
			BlockFile.seekg(blockPos + 88, ios::beg);
			BlockFile.read(transSec, (curBlock.Head.Blocksize - 80));

			int transPos = 0; // 当前交易文件指针位置

			curBlock.trans_count = readVarLen(transSec, transPos);
			cout << "Block #: " << curBlock.blocknumber << "(" << curBlock.trans_count << ")" << endl;
			//logfile << curBlock.trans_count << ")" << endl;
			unit16 txNum = 0;
			for (unit32 k = 0; k < curBlock.trans_count; k++)
			{
				Transaction tx; // 存放当前transaction信息
				//cout << k << endl;
				tx.blocknumber = curBlock.blocknumber; // 记录交易所在块号
				tx.txnumber = txNum; // 是块中的第几个交易
				tx.txcounter = txcounter; // 总共第几个交易
				tx.txtime = curBlock.Head.timeStamp; // 记录交易发生时间
				txNum++;
				txcounter++;

				if (txcounter % 1000 == 0)
				{
					cout << txcounter << endl;// 输出计数器，表示运行情况
				}

				unit32 tx_start = transPos; // 交易起始位置

				tx.ver = readU32(transSec, transPos); // 版本号

				tx.in_count = readVarLen(transSec, transPos); // input计数器
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
					// 计算当前BLOCK output value值的和
					if (tx.txnumber == 0)
					{
						sumoutvalue = sumoutvalue + txout.val;
					}
					txout.script_length = readVarLen(transSec, transPos);
					txout.script = readBufferAdvance(transSec, transPos, txout.script_length);
					//将Script转化为实际的20bytes Address
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

				tx.length = tx_end - tx_start; // 交易文件长度
				twiceSHA256(transSec + tx_start, tx.length, tx.curhash);

				//curBlock.trans.push_back(tx);
				// 计算当前块的奖励
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

				// 清空指向 input 和 output 中的 script 部分
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
			BlockFile.seekg(blockPos, ios::beg); // 跳过以读取的Block

			// 调试终止控制
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

		// 调试终止控制
		if (testflag)
			break;
	}
	//cout << mask++ << endl;

	update_entity_mysql(entitychange_v, conn);//更新entity

	//cout << mask++ << endl;

	//此处应该添加统计函数

	//delete tx_db;

	//statistical(conn);

	//mysql_close(&conn);

	// 向日志中写入 包含的块数和交易数
	logfile << "File contains " << (BlockNum - 1) << " blocks." << endl;
	logfile << "File contains " << txcounter << " transactions." << endl;
	logfile << "Last Block Number: " << (BlockNum - 2) << endl;
	logfile << "Last Transaction Number: " << (txcounter - 1) << endl;
	logfile.close();

	ofstream trans_self;
	trans_self.open("H:\\para\\1.unzip_0.txt", ios::out | ios::trunc);//传递给自身的参数
	trans_self << blkfile_num << endl;//当前块文件号
	trans_self << blockPos << endl;//当前块文件指针
	trans_self << (BlockNum - 2) << endl; //处理的最后一个块号，初始值-1。
	trans_self << (txcounter - 1) << endl; //处理的最后一个交易号，初始值-1。
	trans_self << entitycnt << endl;
	trans_self.close();

	cout << "File contains " << (BlockNum - 1) << " blocks." << endl;
	cout << "File contains " << txcounter << " transactions." << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "all process OK :) !" << endl << endl << endl;

	//getchar();
	return true;
}
