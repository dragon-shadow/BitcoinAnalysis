#pragma once
#include "TransactionProcess.h"

using namespace std;

string blk_insert;

int BlockProcess(Block &blk, MYSQL &conn)
{
	//string filename;
	//ss.str("");
	//ss.clear();
	//ss << blk.blocknumber;
	//ss >> filename;
	//string dataPath("H:\\Blockinfo\\");
	//dataPath.append(filename);
	//dataPath.append(".txt");
	//string dataPath = "I:\\Blockinfo\\" + filename + ".txt";
	//const char *p = dataPath.data();
	//ofstream outfile(dataPath.c_str(), ios::out | ios::binary);
	//if (!outfile)
	//{
	//	cerr << "open error!" << endl;
	//	abort();//退出程序
	//}
	// 计算二进制文件的长度，不包含length本身
	/* 文件结构
	-----------
	binlength	4 （计算长度不包含binlength）
	-----------
	blocknumber	4
	Blocksize	4
	trans_count	4
	Timestamp	4
	bits		4
	reward		8
	curhash		32
	-----------
	*/
	//unit32 binlength = 4 + 4 + 4 + 4 + 4 + 8 + 32;
	//outfile.write((char*)&binlength, 4); // 二进制文件长度
	//outfile.write((char*)&blk.blocknumber, 4); // 所属的块号
	//outfile.write((char*)&blk.Head.Blocksize, 4); // 交易发生时间
	//outfile.write((char*)&blk.trans_count, 4); // 全部的第几个交易
	//outfile.write((char*)&blk.Head.timeStamp, 4); // 原始交易文件长度
	//outfile.write((char*)&blk.Head.bits, 4); // 原始交易文件长度
	//outfile.write((char*)&blk.blockReward, 8); // 原始交易文件长度
	//outfile.write((char*)blk.curhash, 32); // 原始交易文件长度
	//outfile.close();

	// 整理blcok信息，存入MySQL
	string blknum_s;
	ss.str("");
	ss.clear();
	ss << blk.blocknumber;
	ss >> blknum_s;

	// 当前block的hash值，首先将hash转换成字符，用来初始化string，作为文件名
	unit8 hashwrite1[32] = { 0 }; // 反转HASH值，符合主流规范
	for (int k = 31, j = 0; k >= 0; --k, ++j)
	{
		hashwrite1[j] = blk.curhash[k];
	}
	unit8 hashwrite2[65] = { 0 }; // 将16进制hash值
	unit8 *hashwrite2ptr = hashwrite2;
	for (int n = 0; n < 32; n++)
	{
		sprintf_s((char*)hashwrite2ptr, 3, "%02x", hashwrite1[n]); // 用法不明
		hashwrite2ptr += 2;
	}
	string curhash_s = (char*)hashwrite2;

	string Blocksize_s;
	ss.str("");
	ss.clear();
	ss << blk.Head.Blocksize;
	ss >> Blocksize_s;

	string trans_count_s;
	ss.str("");
	ss.clear();
	ss << blk.trans_count;
	ss >> trans_count_s;

	unit32 blktime = blk.Head.timeStamp;
	char blktime_c[21] = { 0 };
	getTimeString(blktime_c, blktime);
	string blktime_s = blktime_c;

	string diff_s;
	ss.str("");
	ss.clear();
	ss << (unsigned long long)difficulty(blk.Head.bits);
	ss >> diff_s;

	string reward_s;
	ss.str("");
	ss.clear();
	ss << blk.blockReward;
	ss >> reward_s;

	blk_insert = "INSERT INTO blkinfo VALUES(x'"\
		+ curhash_s + "','"\
		+ blknum_s + "','"\
		+ Blocksize_s + "','"\
		+ trans_count_s + "','"\
		+ blktime_s + "','"\
		+ diff_s + "','"\
		+ reward_s + "');";

	mysql_query(&conn, blk_insert.c_str());

	return true;
}

