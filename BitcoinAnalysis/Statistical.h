#pragma once
#include <Windows.h>
#include <mysql.h>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include "hash.h"
using namespace std;

int statistical(MYSQL &conn)
{
	MYSQL_RES *result;
	MYSQL_ROW row;

	/*统计时间段,从文件中读取输入参数*/
	ifstream timefile("H:\\para\\5.statistic.txt");
	char temp_time_c[20] = { 0 };

	string timerange;// $time$
	timefile.getline(temp_time_c, 8);
	timerange = temp_time_c;
	cout << "***" << "统计时间段：" << timerange << "***" << endl;

	string before;
	string after;
	timefile.getline(temp_time_c, 20);
	before = temp_time_c;
	cout << "  before:" << before << endl;
	timefile.getline(temp_time_c, 20);
	after = temp_time_c;
	cout << "  after:" << after << endl;

	//打开输出结果文件
	string filepath("G:\\Statistic\\");
	string filename;
	filename = timerange + ".txt";
	filepath = filepath + filename;
	ofstream record;
	record.open(filepath.c_str(), ios::app);//统计结果记录

	char result_c[500] = { 0 };
	string result_s;

	time_t t = time(0);
	char time_c[64] = { 0 };
	strftime(time_c, sizeof(time_c), "%Y-%m-%d %X %A", localtime(&t));
	string OStime = time_c;
	record << "-------------" << OStime << "-------------" << endl;
	record << "***" << "统计时间段：" << timerange << "***" << endl;
	
	/*------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	/*关于address的相关统计*/
	record << "~~~~~~~~~~~~~~~~~~" << endl;		cout << "~~~~~~~~~~~~~~~~~~" << endl;
	record << "1. address" << endl;				cout << "1. address" << endl;
	record << "~~~~~~~~~~~~~~~~~~" << endl;		cout << "~~~~~~~~~~~~~~~~~~" << endl;

	string address[29];
	//地址总数，停用地址数，活跃地址数
	address[1] = "SELECT count(*) FROM test1.addressinfo;";
	address[2] = "SELECT count(*) FROM test1.addressinfo WHERE unix_timestamp(lasttm) < unix_timestamp('" + before + "');";// $time$
	address[3] = "SELECT count(*) FROM test1.addressinfo WHERE unix_timestamp(lasttm) > unix_timestamp('" + after + "');";// $time$
	//收入
	address[4] = "SELECT avg(outval) FROM test1.addressinfo;";
	address[5] = "SELECT stddev_pop(outval) FROM test1.addressinfo;";
	address[6] = "SELECT max(outval) FROM test1.addressinfo;";
	address[7] = "SELECT min(outval) FROM test1.addressinfo;";
	//支出
	address[8] = "SELECT avg(inval) FROM test1.addressinfo;";
	address[9] = "SELECT stddev_pop(inval) FROM test1.addressinfo;";
	address[10] = "SELECT max(inval) FROM test1.addressinfo;";
	address[11] = "SELECT min(inval) FROM test1.addressinfo;";
	//余额
	address[12] = "SELECT avg(bal) FROM test1.addressinfo;";
	address[13] = "SELECT stddev_pop(bal) FROM test1.addressinfo;";
	address[14] = "SELECT max(bal) FROM test1.addressinfo;";
	address[15] = "SELECT min(bal) FROM test1.addressinfo;";
	//最大余额
	address[16] = "SELECT avg(maxbal) FROM test1.addressinfo;";
	address[17] = "SELECT stddev_pop(maxbal) FROM test1.addressinfo;";
	address[18] = "SELECT max(maxbal) FROM test1.addressinfo;";
	address[19] = "SELECT min(maxbal) FROM test1.addressinfo;";
	//生存时间
	address[20] = "SELECT avg((unix_timestamp(lasttm) - unix_timestamp(firsttm))/86400) FROM test1.addressinfo;";
	address[21] = "SELECT stddev_pop((unix_timestamp(lasttm) - unix_timestamp(firsttm))/86400) FROM test1.addressinfo;";
	address[22] = "SELECT max((unix_timestamp(lasttm) - unix_timestamp(firsttm))/86400) FROM test1.addressinfo;";
	address[23] = "SELECT min((unix_timestamp(lasttm) - unix_timestamp(firsttm))/86400) FROM test1.addressinfo;";
	//参与交易数
	address[24] = "SELECT avg(incnt + outcnt) FROM test1.addressinfo;";
	address[25] = "SELECT stddev_pop(incnt + outcnt) FROM test1.addressinfo;";
	address[26] = "SELECT max(incnt + outcnt) FROM test1.addressinfo;";
	address[27] = "SELECT min(incnt + outcnt) FROM test1.addressinfo;";
	//停用address余额总和
	address[28] = "SELECT IFNULL(sum(bal),0) FROM test1.addressinfo WHERE unix_timestamp(lasttm) < unix_timestamp('" + before + "');";
	
	for (unit8 i = 1; i < 29; i++)
	{
		mysql_query(&conn, address[i].c_str());
		result = mysql_store_result(&conn);
		if (row = mysql_fetch_row(result))
		{
			memset(result_c, 0, 500);
			//result_s.clear();

			strcpy(result_c, row[0]);
			result_s = result_c;
			record << result_s << '\t';
		}
		mysql_free_result(result);
	}
	record << endl;

	/*------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	//关于new address的统计
	record << "~~~~~~~~~~~~~~~~~~" << endl;		cout << "~~~~~~~~~~~~~~~~~~" << endl;
	record << "2. new address" << endl;			cout << "2. new address" << endl;
	record << "~~~~~~~~~~~~~~~~~~" << endl;		cout << "~~~~~~~~~~~~~~~~~~" << endl;

	string new_add[22];
	string new_add_time("WHERE unix_timestamp(firsttm) > unix_timestamp('" + after + "');");// $time$
	//新生地址总数
	new_add[1] = "SELECT count(*) FROM test1.addressinfo ";
	//收入
	new_add[2] = "SELECT avg(outval) FROM test1.addressinfo ";
	new_add[3] = "SELECT stddev_pop(outval) FROM test1.addressinfo ";
	new_add[4] = "SELECT max(outval) FROM test1.addressinfo ";
	new_add[5] = "SELECT min(outval) FROM test1.addressinfo ";
	//支出
	new_add[6] = "SELECT avg(inval) FROM test1.addressinfo ";
	new_add[7] = "SELECT stddev_pop(inval) FROM test1.addressinfo ";
	new_add[8] = "SELECT max(inval) FROM test1.addressinfo ";
	new_add[9] = "SELECT min(inval) FROM test1.addressinfo ";
	//余额
	new_add[10] = "SELECT avg(bal) FROM test1.addressinfo ";
	new_add[11] = "SELECT stddev_pop(bal) FROM test1.addressinfo ";
	new_add[12] = "SELECT max(bal) FROM test1.addressinfo ";
	new_add[13] = "SELECT min(bal) FROM test1.addressinfo ";
	//最大余额
	new_add[14] = "SELECT avg(maxbal) FROM test1.addressinfo ";
	new_add[15] = "SELECT stddev_pop(maxbal) FROM test1.addressinfo ";
	new_add[16] = "SELECT max(maxbal) FROM test1.addressinfo ";
	new_add[17] = "SELECT min(maxbal) FROM test1.addressinfo ";
	//参与交易数
	new_add[18] = "SELECT avg(incnt + outcnt) FROM test1.addressinfo ";
	new_add[19] = "SELECT stddev_pop(incnt + outcnt) FROM test1.addressinfo ";
	new_add[20] = "SELECT max(incnt + outcnt) FROM test1.addressinfo ";
	new_add[21] = "SELECT min(incnt + outcnt) FROM test1.addressinfo ";

	string get;
	for (unit8 i = 1; i < 22; i++)
	{
		//get.clear();
		get = new_add[i] + new_add_time;

		mysql_query(&conn, get.c_str());
		result = mysql_store_result(&conn);
		if (row = mysql_fetch_row(result))
		{
			memset(result_c, 0, 500);
			//result_s.clear();

			strcpy(result_c, row[0]);
			result_s = result_c;
			record << result_s << '\t';
		}
		mysql_free_result(result);
	}
	record << endl;

	/*------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	//关于no stop address的统计
	record << "~~~~~~~~~~~~~~~~~~" << endl;	cout << "~~~~~~~~~~~~~~~~~~" << endl;
	record << "3. no stop address" << endl;	cout << "3. no stop address" << endl;
	record << "~~~~~~~~~~~~~~~~~~" << endl;	cout << "~~~~~~~~~~~~~~~~~~" << endl;

	string unstop_add[26];
	string unstop_add_time("WHERE unix_timestamp(lasttm) > unix_timestamp('" + before + "');");// $time$
	//非停用地址总数
	unstop_add[1] = "SELECT count(*) FROM test1.addressinfo ";
	//收入
	unstop_add[2] = "SELECT avg(outval) FROM test1.addressinfo ";
	unstop_add[3] = "SELECT stddev_pop(outval) FROM test1.addressinfo ";
	unstop_add[4] = "SELECT max(outval) FROM test1.addressinfo ";
	unstop_add[5] = "SELECT min(outval) FROM test1.addressinfo ";
	//支出
	unstop_add[6] = "SELECT avg(inval) FROM test1.addressinfo ";
	unstop_add[7] = "SELECT stddev_pop(inval) FROM test1.addressinfo ";
	unstop_add[8] = "SELECT max(inval) FROM test1.addressinfo ";
	unstop_add[9] = "SELECT min(inval) FROM test1.addressinfo ";
	//余额
	unstop_add[10] = "SELECT avg(bal) FROM test1.addressinfo ";
	unstop_add[11] = "SELECT stddev_pop(bal) FROM test1.addressinfo ";
	unstop_add[12] = "SELECT max(bal) FROM test1.addressinfo ";
	unstop_add[13] = "SELECT min(bal) FROM test1.addressinfo ";
	//最大余额
	unstop_add[14] = "SELECT avg(maxbal) FROM test1.addressinfo ";
	unstop_add[15] = "SELECT stddev_pop(maxbal) FROM test1.addressinfo ";
	unstop_add[16] = "SELECT max(maxbal) FROM test1.addressinfo ";
	unstop_add[17] = "SELECT min(maxbal) FROM test1.addressinfo ";
	//生存时间
	unstop_add[18] = "SELECT avg((unix_timestamp(lasttm) - unix_timestamp(firsttm))/86400) FROM test1.addressinfo ";
	unstop_add[19] = "SELECT stddev_pop((unix_timestamp(lasttm) - unix_timestamp(firsttm))/86400) FROM test1.addressinfo ";
	unstop_add[20] = "SELECT max((unix_timestamp(lasttm) - unix_timestamp(firsttm))/86400) FROM test1.addressinfo ";
	unstop_add[21] = "SELECT min((unix_timestamp(lasttm) - unix_timestamp(firsttm))/86400) FROM test1.addressinfo ";
	//参与交易数
	unstop_add[22] = "SELECT avg(incnt + outcnt) FROM test1.addressinfo ";
	unstop_add[23] = "SELECT stddev_pop(incnt + outcnt) FROM test1.addressinfo ";
	unstop_add[24] = "SELECT max(incnt + outcnt) FROM test1.addressinfo ";
	unstop_add[25] = "SELECT min(incnt + outcnt) FROM test1.addressinfo ";

	for (unit8 i = 1; i < 26; i++)
	{
		//get.clear();
		get = unstop_add[i] + unstop_add_time;

		mysql_query(&conn, get.c_str());
		result = mysql_store_result(&conn);
		if (row = mysql_fetch_row(result))
		{
			memset(result_c, 0, 500);
			//result_s.clear();

			strcpy(result_c, row[0]);
			result_s = result_c;
			record << result_s << '\t';
		}
		mysql_free_result(result);
	}
	record << endl;

	/*------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	//关于entity的统计
	record << "~~~~~~~~~~~~~~~~~~" << endl;		cout << "~~~~~~~~~~~~~~~~~~" << endl;
	record << "4. entity" << endl;				cout << "4. entity" << endl;
	record << "~~~~~~~~~~~~~~~~~~" << endl;		cout << "~~~~~~~~~~~~~~~~~~" << endl;

	string entity[32];
	//地址总数，停用实体数，活跃实体数
	entity[1] = "SELECT count(*) FROM test1.entityinfo;";
	entity[2] = "SELECT count(*) FROM test1.entityinfo WHERE unix_timestamp(lasttm) < unix_timestamp('" + before + "');";// $time$
	entity[3] = "SELECT count(*) FROM test1.entityinfo WHERE unix_timestamp(lasttm) > unix_timestamp('" + after + "');";// $time$
	//包含的地址数
	entity[4] = "SELECT avg(addcnt) FROM test1.entityinfo;";
	entity[5] = "SELECT stddev_pop(addcnt) FROM test1.entityinfo;";
	entity[6] = "SELECT max(addcnt) FROM test1.entityinfo;";
	entity[7] = "SELECT min(addcnt) FROM test1.entityinfo;";
	//收入
	entity[8] = "SELECT avg(outval) FROM test1.entityinfo;";
	entity[9] = "SELECT stddev_pop(outval) FROM test1.entityinfo;";
	entity[10] = "SELECT max(outval) FROM test1.entityinfo;";
	entity[11] = "SELECT min(outval) FROM test1.entityinfo;";
	//支出
	entity[12] = "SELECT avg(inval) FROM test1.entityinfo;";
	entity[13] = "SELECT stddev_pop(inval) FROM test1.entityinfo;";
	entity[14] = "SELECT max(inval) FROM test1.entityinfo;";
	entity[15] = "SELECT min(inval) FROM test1.entityinfo;";
	//余额
	entity[16] = "SELECT avg(bal) FROM test1.entityinfo;";
	entity[17] = "SELECT stddev_pop(bal) FROM test1.entityinfo;";
	entity[18] = "SELECT max(bal) FROM test1.entityinfo;";
	entity[19] = "SELECT min(bal) FROM test1.entityinfo;";
	//最大余额
	entity[20] = "SELECT avg(maxbal) FROM test1.entityinfo;";
	entity[21] = "SELECT stddev_pop(maxbal) FROM test1.entityinfo;";
	entity[22] = "SELECT max(maxbal) FROM test1.entityinfo;";
	entity[23] = "SELECT min(maxbal) FROM test1.entityinfo;";
	//生存时间
	entity[24] = "SELECT avg((unix_timestamp(lasttm)-unix_timestamp(firsttm))/86400) FROM test1.entityinfo;";
	entity[25] = "SELECT stddev_pop((unix_timestamp(lasttm)-unix_timestamp(firsttm))/86400) FROM test1.entityinfo;";
	entity[26] = "SELECT max((unix_timestamp(lasttm)-unix_timestamp(firsttm))/86400) FROM test1.entityinfo;";
	entity[27] = "SELECT min((unix_timestamp(lasttm)-unix_timestamp(firsttm))/86400) FROM test1.entityinfo;";
	//参与交易数
	entity[28] = "SELECT avg(incnt + outcnt) FROM test1.entityinfo;";
	entity[29] = "SELECT stddev_pop(incnt + outcnt) FROM test1.entityinfo;";
	entity[30] = "SELECT max(incnt + outcnt) FROM test1.entityinfo;";
	entity[31] = "SELECT min(incnt + outcnt) FROM test1.entityinfo;";

	for (unit8 i = 1; i < 32; i++)
	{
		mysql_query(&conn, entity[i].c_str());
		result = mysql_store_result(&conn);
		if (row = mysql_fetch_row(result))
		{
			memset(result_c, 0, 500);
			//result_s.clear();

			strcpy(result_c, row[0]);
			result_s = result_c;
			record << result_s << '\t';
		}
		mysql_free_result(result);
	}
	record << endl;
	
	/*------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	//关于new entity的统计
	record << "~~~~~~~~~~~~~~~~~~" << endl;		cout << "~~~~~~~~~~~~~~~~~~" << endl;
	record << "5. new entity" << endl;			cout << "5. new entity" << endl;
	record << "~~~~~~~~~~~~~~~~~~" << endl;		cout << "~~~~~~~~~~~~~~~~~~" << endl;

	string new_entity[26];
	string new_entity_time("WHERE unix_timestamp(firsttm) > unix_timestamp('" + after + "');");// $time$
	//新实体总数
	new_entity[1] = "SELECT count(*) FROM test1.entityinfo ";
	//包含的地址数
	new_entity[2] = "SELECT avg(addcnt) FROM test1.entityinfo ";
	new_entity[3] = "SELECT stddev_pop(addcnt) FROM test1.entityinfo ";
	new_entity[4] = "SELECT max(addcnt) FROM test1.entityinfo ";
	new_entity[5] = "SELECT min(addcnt) FROM test1.entityinfo ";
	//收入
	new_entity[6] = "SELECT avg(outval) FROM test1.entityinfo ";
	new_entity[7] = "SELECT stddev_pop(outval) FROM test1.entityinfo ";
	new_entity[8] = "SELECT max(outval) FROM test1.entityinfo ";
	new_entity[9] = "SELECT min(outval) FROM test1.entityinfo ";
	//支出
	new_entity[10] = "SELECT avg(inval) FROM test1.entityinfo ";
	new_entity[11] = "SELECT stddev_pop(inval) FROM test1.entityinfo ";
	new_entity[12] = "SELECT max(inval) FROM test1.entityinfo ";
	new_entity[13] = "SELECT min(inval) FROM test1.entityinfo ";
	//余额
	new_entity[14] = "SELECT avg(bal) FROM test1.entityinfo ";
	new_entity[15] = "SELECT stddev_pop(bal) FROM test1.entityinfo ";
	new_entity[16] = "SELECT max(bal) FROM test1.entityinfo ";
	new_entity[17] = "SELECT min(bal) FROM test1.entityinfo ";
	//最大余额
	new_entity[18] = "SELECT avg(maxbal) FROM test1.entityinfo ";
	new_entity[19] = "SELECT stddev_pop(maxbal) FROM test1.entityinfo ";
	new_entity[20] = "SELECT max(maxbal) FROM test1.entityinfo ";
	new_entity[21] = "SELECT min(maxbal) FROM test1.entityinfo ";
	//参与交易数
	new_entity[22] = "SELECT avg(incnt + outcnt) FROM test1.entityinfo ";
	new_entity[23] = "SELECT stddev_pop(incnt + outcnt) FROM test1.entityinfo ";
	new_entity[24] = "SELECT max(incnt + outcnt) FROM test1.entityinfo ";
	new_entity[25] = "SELECT min(incnt + outcnt) FROM test1.entityinfo ";

	for (unit8 i = 1; i < 26; i++)
	{
		//get.clear();
		get = new_entity[i] + new_entity_time;

		mysql_query(&conn, get.c_str());
		result = mysql_store_result(&conn);
		if (row = mysql_fetch_row(result))
		{
			memset(result_c, 0, 500);
			//result_s.clear();

			strcpy(result_c, row[0]);
			result_s = result_c;
			record << result_s << '\t';
		}
		mysql_free_result(result);
	}
	record << endl;
	
	/*------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	//关于no stop entity的统计
	record << "~~~~~~~~~~~~~~~~~~" << endl;		cout << "~~~~~~~~~~~~~~~~~~" << endl;
	record << "6. no stop entity" << endl;		cout << "6. no stop entity" << endl;
	record << "~~~~~~~~~~~~~~~~~~" << endl;		cout << "~~~~~~~~~~~~~~~~~~" << endl;

	string unstop_entity[30];
	string unstop_entity_time("WHERE unix_timestamp(lasttm) > unix_timestamp('" + before + "');");// $time$
	//非停用实体数
	unstop_entity[1] = "SELECT count(*) FROM test1.entityinfo ";
	//包含的地址数
	unstop_entity[2] = "SELECT avg(addcnt) FROM test1.entityinfo ";
	unstop_entity[3] = "SELECT stddev_pop(addcnt) FROM test1.entityinfo ";
	unstop_entity[4] = "SELECT max(addcnt) FROM test1.entityinfo ";
	unstop_entity[5] = "SELECT min(addcnt) FROM test1.entityinfo ";
	//收入
	unstop_entity[6] = "SELECT avg(outval) FROM test1.entityinfo ";
	unstop_entity[7] = "SELECT stddev_pop(outval) FROM test1.entityinfo ";
	unstop_entity[8] = "SELECT max(outval) FROM test1.entityinfo ";
	unstop_entity[9] = "SELECT min(outval) FROM test1.entityinfo ";
	//支出
	unstop_entity[10] = "SELECT avg(inval) FROM test1.entityinfo ";
	unstop_entity[11] = "SELECT stddev_pop(inval) FROM test1.entityinfo ";
	unstop_entity[12] = "SELECT max(inval) FROM test1.entityinfo ";
	unstop_entity[13] = "SELECT min(inval) FROM test1.entityinfo ";
	//余额
	unstop_entity[14] = "SELECT avg(bal) FROM test1.entityinfo ";
	unstop_entity[15] = "SELECT stddev_pop(bal) FROM test1.entityinfo ";
	unstop_entity[16] = "SELECT max(bal) FROM test1.entityinfo ";
	unstop_entity[17] = "SELECT min(bal) FROM test1.entityinfo ";
	//最大余额
	unstop_entity[18] = "SELECT avg(maxbal) FROM test1.entityinfo ";
	unstop_entity[19] = "SELECT stddev_pop(maxbal) FROM test1.entityinfo ";
	unstop_entity[20] = "SELECT max(maxbal) FROM test1.entityinfo ";
	unstop_entity[21] = "SELECT min(maxbal) FROM test1.entityinfo ";
	//生存时间
	unstop_entity[22] = "SELECT avg((unix_timestamp(lasttm) - unix_timestamp(firsttm))/86400) FROM test1.entityinfo ";
	unstop_entity[23] = "SELECT stddev_pop((unix_timestamp(lasttm) - unix_timestamp(firsttm))/86400) FROM test1.entityinfo ";
	unstop_entity[24] = "SELECT max((unix_timestamp(lasttm) - unix_timestamp(firsttm))/86400) FROM test1.entityinfo ";
	unstop_entity[25] = "SELECT min((unix_timestamp(lasttm) - unix_timestamp(firsttm))/86400) FROM test1.entityinfo ";
	//参与交易数
	unstop_entity[26] = "SELECT avg(incnt + outcnt) FROM test1.entityinfo ";
	unstop_entity[27] = "SELECT stddev_pop(incnt + outcnt) FROM test1.entityinfo ";
	unstop_entity[28] = "SELECT max(incnt + outcnt) FROM test1.entityinfo ";
	unstop_entity[29] = "SELECT min(incnt + outcnt) FROM test1.entityinfo ";

	for (unit8 i = 1; i < 30; i++)
	{
		//get.clear();
		get = unstop_entity[i] + unstop_entity_time;

		mysql_query(&conn, get.c_str());
		result = mysql_store_result(&conn);
		if (row = mysql_fetch_row(result))
		{
			memset(result_c, 0, 500);
			//result_s.clear();

			strcpy(result_c, row[0]);
			result_s = result_c;
			record << result_s << '\t';
		}
		mysql_free_result(result);
	}
	record << endl;
	
	/*------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	//关于transaction的统计
	record << "~~~~~~~~~~~~~~~~~~" << endl;		cout << "~~~~~~~~~~~~~~~~~~" << endl;
	record << "7. transaction" << endl;			cout << "7. transaction" << endl;
	record << "~~~~~~~~~~~~~~~~~~" << endl;		cout << "~~~~~~~~~~~~~~~~~~" << endl;

	string transaction[22];
	string transaction_time("WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "');");// $time$
	//交易总数
	transaction[0] = "SELECT count(*) FROM test1.txinfo;";
	//新生成交易书
	transaction[1] = "SELECT count(*) FROM test1.txinfo ";
	//交易金额
	transaction[2] = "SELECT avg(outvalue) FROM test1.txinfo ";
	transaction[3] = "SELECT stddev_pop(outvalue) FROM test1.txinfo ";
	transaction[4] = "SELECT max(outvalue) FROM test1.txinfo ";
	transaction[5] = "SELECT min(outvalue) FROM test1.txinfo ";
	//交易文件大小
	transaction[6] = "SELECT avg(txsize) FROM test1.txinfo ";
	transaction[7] = "SELECT stddev_pop(txsize) FROM test1.txinfo ";
	transaction[8] = "SELECT max(txsize) FROM test1.txinfo ";
	transaction[9] = "SELECT min(txsize) FROM test1.txinfo ";
	//交易费用
	transaction[10] = "SELECT avg(txfee) FROM test1.txinfo ";
	transaction[11] = "SELECT stddev_pop(txfee) FROM test1.txinfo ";
	transaction[12] = "SELECT max(txfee) FROM test1.txinfo ";
	transaction[13] = "SELECT min(txfee) FROM test1.txinfo ";
	//输入地址数量
	transaction[14] = "SELECT avg(incnt) FROM test1.txinfo ";
	transaction[15] = "SELECT stddev_pop(incnt) FROM test1.txinfo ";
	transaction[16] = "SELECT max(incnt) FROM test1.txinfo ";
	transaction[17] = "SELECT min(incnt) FROM test1.txinfo ";
	//输出地址数量
	transaction[18] = "SELECT avg(outcnt) FROM test1.txinfo ";
	transaction[19] = "SELECT stddev_pop(outcnt) FROM test1.txinfo ";
	transaction[20] = "SELECT max(outcnt) FROM test1.txinfo ";
	transaction[21] = "SELECT min(outcnt) FROM test1.txinfo ";

	mysql_query(&conn, transaction[0].c_str());
	result = mysql_store_result(&conn);
	if (row = mysql_fetch_row(result))
	{
		memset(result_c, 0, 500);
		//result_s.clear();

		strcpy(result_c, row[0]);
		result_s = result_c;
		record << result_s << '\t';
	}
	mysql_free_result(result);

	for (unit8 i = 1; i < 22; i++)
	{
		//get.clear();
		get = transaction[i] + transaction_time;

		mysql_query(&conn, get.c_str());
		result = mysql_store_result(&conn);
		if (row = mysql_fetch_row(result))
		{
			memset(result_c, 0, 500);
			//result_s.clear();

			strcpy(result_c, row[0]);
			result_s = result_c;
			record << result_s << '\t';
		}
		mysql_free_result(result);
	}
	record << endl;
	
	/*------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	//关于block的统计
	record << "~~~~~~~~~~~~~~~~~~" << endl;		cout << "~~~~~~~~~~~~~~~~~~" << endl;
	record << "8. block" << endl;				cout << "8. block" << endl;
	record << "~~~~~~~~~~~~~~~~~~" << endl;		cout << "~~~~~~~~~~~~~~~~~~" << endl;

	string block[22];
	string block_time("WHERE unix_timestamp(blktime) > unix_timestamp('" + after + "');");// $time$
	//块总数
	block[0] = "SELECT count(*) FROM test1.blkinfo;";
	//新生成块数
	block[1] = "SELECT count(*) FROM test1.blkinfo ";
	//包含交易数
	block[2] = "SELECT avg(txcnt) FROM test1.blkinfo ";
	block[3] = "SELECT stddev_pop(txcnt) FROM test1.blkinfo ";
	block[4] = "SELECT max(txcnt) FROM test1.blkinfo ";
	block[5] = "SELECT min(txcnt) FROM test1.blkinfo ";
	//块难度
	block[6] = "SELECT avg(diff) FROM test1.blkinfo ";
	block[7] = "SELECT stddev_pop(diff) FROM test1.blkinfo ";
	block[8] = "SELECT max(diff) FROM test1.blkinfo ";
	block[9] = "SELECT min(diff) FROM test1.blkinfo ";
	//块文件大小
	block[10] = "SELECT avg(blksize) FROM test1.blkinfo ";
	block[11] = "SELECT stddev_pop(blksize) FROM test1.blkinfo ";
	block[12] = "SELECT max(blksize) FROM test1.blkinfo ";
	block[13] = "SELECT min(blksize) FROM test1.blkinfo ";
	//交易费用
	block[14] = "SELECT avg(reward) FROM test1.blkinfo ";
	block[15] = "SELECT stddev_pop(reward) FROM test1.blkinfo ";
	block[16] = "SELECT max(reward) FROM test1.blkinfo ";
	block[17] = "SELECT min(reward) FROM test1.blkinfo ";

	mysql_query(&conn, block[0].c_str());
	result = mysql_store_result(&conn);
	if (row = mysql_fetch_row(result))
	{
		memset(result_c, 0, 500);
		//result_s.clear();

		strcpy(result_c, row[0]);
		result_s = result_c;
		record << result_s << '\t';
	}
	mysql_free_result(result);

	for (unit8 i = 1; i < 18; i++)
	{
		//get.clear();
		get = block[i] + block_time;

		mysql_query(&conn, get.c_str());
		result = mysql_store_result(&conn);
		if (row = mysql_fetch_row(result))
		{
			memset(result_c, 0, 500);
			//result_s.clear();

			strcpy(result_c, row[0]);
			result_s = result_c;
			record << result_s << '\t';
		}
		mysql_free_result(result);
	}
	record << endl;

	/*------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	//关于other的统计
	record << "~~~~~~~~~~~~~~~~~~" << endl;		cout << "~~~~~~~~~~~~~~~~~~" << endl;
	record << "9. other" << endl;				cout << "9. other" << endl;
	record << "~~~~~~~~~~~~~~~~~~" << endl;		cout << "~~~~~~~~~~~~~~~~~~" << endl;
	// entity地址数量的分布
	string NumOfAddOfEntity[31];
	
	NumOfAddOfEntity[0] = "SELECT count(*) FROM entityinfo WHERE addcnt>=1 AND addcnt<2;";
	NumOfAddOfEntity[1] = "SELECT count(*) FROM entityinfo WHERE addcnt>=2 AND addcnt<3;";
	NumOfAddOfEntity[2] = "SELECT count(*) FROM entityinfo WHERE addcnt>=3 AND addcnt<4;";
	NumOfAddOfEntity[3] = "SELECT count(*) FROM entityinfo WHERE addcnt>=4 AND addcnt<5;";
	NumOfAddOfEntity[4] = "SELECT count(*) FROM entityinfo WHERE addcnt>=5 AND addcnt<6;";
	NumOfAddOfEntity[5] = "SELECT count(*) FROM entityinfo WHERE addcnt>=6 AND addcnt<7;";
	NumOfAddOfEntity[6] = "SELECT count(*) FROM entityinfo WHERE addcnt>=7 AND addcnt<8;";
	NumOfAddOfEntity[7] = "SELECT count(*) FROM entityinfo WHERE addcnt>=8 AND addcnt<9;";
	NumOfAddOfEntity[8] = "SELECT count(*) FROM entityinfo WHERE addcnt>=9 AND addcnt<10;";
	NumOfAddOfEntity[9] = "SELECT count(*) FROM entityinfo WHERE addcnt>=10 AND addcnt<20;";
	NumOfAddOfEntity[10] = "SELECT count(*) FROM entityinfo WHERE addcnt>=20 AND addcnt<30;";
	NumOfAddOfEntity[11] = "SELECT count(*) FROM entityinfo WHERE addcnt>=30 AND addcnt<40;";
	NumOfAddOfEntity[12] = "SELECT count(*) FROM entityinfo WHERE addcnt>=40 AND addcnt<50;";
	NumOfAddOfEntity[13] = "SELECT count(*) FROM entityinfo WHERE addcnt>=50 AND addcnt<60;";
	NumOfAddOfEntity[14] = "SELECT count(*) FROM entityinfo WHERE addcnt>=60 AND addcnt<70;";
	NumOfAddOfEntity[15] = "SELECT count(*) FROM entityinfo WHERE addcnt>=70 AND addcnt<80;";
	NumOfAddOfEntity[16] = "SELECT count(*) FROM entityinfo WHERE addcnt>=80 AND addcnt<90;";
	NumOfAddOfEntity[17] = "SELECT count(*) FROM entityinfo WHERE addcnt>=90 AND addcnt<100;";
	NumOfAddOfEntity[18] = "SELECT count(*) FROM entityinfo WHERE addcnt>=100 AND addcnt<200;";
	NumOfAddOfEntity[19] = "SELECT count(*) FROM entityinfo WHERE addcnt>=200 AND addcnt<300;";
	NumOfAddOfEntity[20] = "SELECT count(*) FROM entityinfo WHERE addcnt>=300 AND addcnt<400;";
	NumOfAddOfEntity[21] = "SELECT count(*) FROM entityinfo WHERE addcnt>=400 AND addcnt<500;";
	NumOfAddOfEntity[22] = "SELECT count(*) FROM entityinfo WHERE addcnt>=500 AND addcnt<1000;";
	NumOfAddOfEntity[23] = "SELECT count(*) FROM entityinfo WHERE addcnt>=1000 AND addcnt<2000;";
	NumOfAddOfEntity[24] = "SELECT count(*) FROM entityinfo WHERE addcnt>=2000 AND addcnt<3000;";
	NumOfAddOfEntity[25] = "SELECT count(*) FROM entityinfo WHERE addcnt>=3000 AND addcnt<4000;";
	NumOfAddOfEntity[26] = "SELECT count(*) FROM entityinfo WHERE addcnt>=4000 AND addcnt<5000;";
	NumOfAddOfEntity[27] = "SELECT count(*) FROM entityinfo WHERE addcnt>=5000 AND addcnt<10000;";
	NumOfAddOfEntity[28] = "SELECT count(*) FROM entityinfo WHERE addcnt>=10000 AND addcnt<50000;";
	NumOfAddOfEntity[29] = "SELECT count(*) FROM entityinfo WHERE addcnt>=50000 AND addcnt<100000;";
	NumOfAddOfEntity[30] = "SELECT count(*) FROM entityinfo WHERE addcnt>=100000;";

	record << "~~~~~~~~" << endl;
	record << "9.2 entity地址数量的分布" << endl; cout << "   9.2 entity地址数量的分布" << endl;
	for (unit8 i = 0; i < 31; i++)
	{
		mysql_query(&conn, NumOfAddOfEntity[i].c_str());
		result = mysql_store_result(&conn);
		if (row = mysql_fetch_row(result))
		{
			memset(result_c, 0, 500);
			//result_s.clear();

			strcpy(result_c, row[0]);
			result_s = result_c;
			record << result_s << '\t';
		}
		mysql_free_result(result);
	}
	record << endl;
	
	/*------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	// address收入分布
	string OutvalOfAdd[48];

	OutvalOfAdd[0] = "SELECT count(*) FROM addressinfo WHERE outval>=0*100000000 AND outval<1*100000000;";
	OutvalOfAdd[1] = "SELECT count(*) FROM addressinfo WHERE outval>=1*100000000 AND outval<2*100000000;";
	OutvalOfAdd[2] = "SELECT count(*) FROM addressinfo WHERE outval>=2*100000000 AND outval<3*100000000;";
	OutvalOfAdd[3] = "SELECT count(*) FROM addressinfo WHERE outval>=3*100000000 AND outval<4*100000000;";
	OutvalOfAdd[4] = "SELECT count(*) FROM addressinfo WHERE outval>=4*100000000 AND outval<5*100000000;";
	OutvalOfAdd[5] = "SELECT count(*) FROM addressinfo WHERE outval>=5*100000000 AND outval<6*100000000;";
	OutvalOfAdd[6] = "SELECT count(*) FROM addressinfo WHERE outval>=6*100000000 AND outval<7*100000000;";
	OutvalOfAdd[7] = "SELECT count(*) FROM addressinfo WHERE outval>=7*100000000 AND outval<8*100000000;";
	OutvalOfAdd[8] = "SELECT count(*) FROM addressinfo WHERE outval>=8*100000000 AND outval<9*100000000;";
	OutvalOfAdd[9] = "SELECT count(*) FROM addressinfo WHERE outval>=9*100000000 AND outval<10*100000000;";
	OutvalOfAdd[10] = "SELECT count(*) FROM addressinfo WHERE outval>=10*100000000 AND outval<20*100000000;";
	OutvalOfAdd[11] = "SELECT count(*) FROM addressinfo WHERE outval>=20*100000000 AND outval<30*100000000;";
	OutvalOfAdd[12] = "SELECT count(*) FROM addressinfo WHERE outval>=30*100000000 AND outval<40*100000000;";
	OutvalOfAdd[13] = "SELECT count(*) FROM addressinfo WHERE outval>=40*100000000 AND outval<50*100000000;";
	OutvalOfAdd[14] = "SELECT count(*) FROM addressinfo WHERE outval>=50*100000000 AND outval<60*100000000;";
	OutvalOfAdd[15] = "SELECT count(*) FROM addressinfo WHERE outval>=60*100000000 AND outval<70*100000000;";
	OutvalOfAdd[16] = "SELECT count(*) FROM addressinfo WHERE outval>=70*100000000 AND outval<80*100000000;";
	OutvalOfAdd[17] = "SELECT count(*) FROM addressinfo WHERE outval>=80*100000000 AND outval<90*100000000;";
	OutvalOfAdd[18] = "SELECT count(*) FROM addressinfo WHERE outval>=90*100000000 AND outval<100*100000000;";
	OutvalOfAdd[19] = "SELECT count(*) FROM addressinfo WHERE outval>=100*100000000 AND outval<1000*100000000;";
	OutvalOfAdd[20] = "SELECT count(*) FROM addressinfo WHERE outval>=1000*100000000 AND outval<2000*100000000;";
	OutvalOfAdd[21] = "SELECT count(*) FROM addressinfo WHERE outval>=2000*100000000 AND outval<3000*100000000;";
	OutvalOfAdd[22] = "SELECT count(*) FROM addressinfo WHERE outval>=3000*100000000 AND outval<4000*100000000;";
	OutvalOfAdd[23] = "SELECT count(*) FROM addressinfo WHERE outval>=4000*100000000 AND outval<5000*100000000;";
	OutvalOfAdd[24] = "SELECT count(*) FROM addressinfo WHERE outval>=5000*100000000 AND outval<6000*100000000;";
	OutvalOfAdd[25] = "SELECT count(*) FROM addressinfo WHERE outval>=6000*100000000 AND outval<7000*100000000;";
	OutvalOfAdd[26] = "SELECT count(*) FROM addressinfo WHERE outval>=7000*100000000 AND outval<8000*100000000;";
	OutvalOfAdd[27] = "SELECT count(*) FROM addressinfo WHERE outval>=8000*100000000 AND outval<9000*100000000;";
	OutvalOfAdd[28] = "SELECT count(*) FROM addressinfo WHERE outval>=9000*100000000 AND outval<10000*100000000;";
	OutvalOfAdd[29] = "SELECT count(*) FROM addressinfo WHERE outval>=10000*100000000 AND outval<20000*100000000;";
	OutvalOfAdd[30] = "SELECT count(*) FROM addressinfo WHERE outval>=20000*100000000 AND outval<30000*100000000;";
	OutvalOfAdd[31] = "SELECT count(*) FROM addressinfo WHERE outval>=30000*100000000 AND outval<40000*100000000;";
	OutvalOfAdd[32] = "SELECT count(*) FROM addressinfo WHERE outval>=40000*100000000 AND outval<50000*100000000;";
	OutvalOfAdd[33] = "SELECT count(*) FROM addressinfo WHERE outval>=50000*100000000 AND outval<60000*100000000;";
	OutvalOfAdd[34] = "SELECT count(*) FROM addressinfo WHERE outval>=60000*100000000 AND outval<70000*100000000;";
	OutvalOfAdd[35] = "SELECT count(*) FROM addressinfo WHERE outval>=70000*100000000 AND outval<80000*100000000;";
	OutvalOfAdd[36] = "SELECT count(*) FROM addressinfo WHERE outval>=80000*100000000 AND outval<90000*100000000;";
	OutvalOfAdd[37] = "SELECT count(*) FROM addressinfo WHERE outval>=90000*100000000 AND outval<100000*100000000;";
	OutvalOfAdd[38] = "SELECT count(*) FROM addressinfo WHERE outval>=100000*100000000 AND outval<200000*100000000;";
	OutvalOfAdd[39] = "SELECT count(*) FROM addressinfo WHERE outval>=200000*100000000 AND outval<300000*100000000;";
	OutvalOfAdd[40] = "SELECT count(*) FROM addressinfo WHERE outval>=300000*100000000 AND outval<400000*100000000;";
	OutvalOfAdd[41] = "SELECT count(*) FROM addressinfo WHERE outval>=400000*100000000 AND outval<500000*100000000;";
	OutvalOfAdd[42] = "SELECT count(*) FROM addressinfo WHERE outval>=500000*100000000 AND outval<600000*100000000;";
	OutvalOfAdd[43] = "SELECT count(*) FROM addressinfo WHERE outval>=600000*100000000 AND outval<700000*100000000;";
	OutvalOfAdd[44] = "SELECT count(*) FROM addressinfo WHERE outval>=700000*100000000 AND outval<800000*100000000;";
	OutvalOfAdd[45] = "SELECT count(*) FROM addressinfo WHERE outval>=800000*100000000 AND outval<900000*100000000;";
	OutvalOfAdd[46] = "SELECT count(*) FROM addressinfo WHERE outval>=900000*100000000 AND outval<1000000*100000000;";
	OutvalOfAdd[47] = "SELECT count(*) FROM addressinfo WHERE outval>=1000000*100000000;";

	record << "~~~~~~~~" << endl;
	record << "9.3 address收入分布" << endl; cout << "   9.3 address收入分布" << endl;
	for (unit8 i = 0; i < 48; i++)
	{
		mysql_query(&conn, OutvalOfAdd[i].c_str());
		result = mysql_store_result(&conn);
		if (row = mysql_fetch_row(result))
		{
			memset(result_c, 0, 500);
			//result_s.clear();

			strcpy(result_c, row[0]);
			result_s = result_c;
			record << result_s << '\t';
		}
		mysql_free_result(result);
	}
	record << endl;
	
	/*------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	// entity收入分布
	string OutvalOfEntity[48];

	OutvalOfEntity[0] = "SELECT count(*) FROM entityinfo WHERE outval>=0*100000000 AND outval<1*100000000;";
	OutvalOfEntity[1] = "SELECT count(*) FROM entityinfo WHERE outval>=1*100000000 AND outval<2*100000000;";
	OutvalOfEntity[2] = "SELECT count(*) FROM entityinfo WHERE outval>=2*100000000 AND outval<3*100000000;";
	OutvalOfEntity[3] = "SELECT count(*) FROM entityinfo WHERE outval>=3*100000000 AND outval<4*100000000;";
	OutvalOfEntity[4] = "SELECT count(*) FROM entityinfo WHERE outval>=4*100000000 AND outval<5*100000000;";
	OutvalOfEntity[5] = "SELECT count(*) FROM entityinfo WHERE outval>=5*100000000 AND outval<6*100000000;";
	OutvalOfEntity[6] = "SELECT count(*) FROM entityinfo WHERE outval>=6*100000000 AND outval<7*100000000;";
	OutvalOfEntity[7] = "SELECT count(*) FROM entityinfo WHERE outval>=7*100000000 AND outval<8*100000000;";
	OutvalOfEntity[8] = "SELECT count(*) FROM entityinfo WHERE outval>=8*100000000 AND outval<9*100000000;";
	OutvalOfEntity[9] = "SELECT count(*) FROM entityinfo WHERE outval>=9*100000000 AND outval<10*100000000;";
	OutvalOfEntity[10] = "SELECT count(*) FROM entityinfo WHERE outval>=10*100000000 AND outval<20*100000000;";
	OutvalOfEntity[11] = "SELECT count(*) FROM entityinfo WHERE outval>=20*100000000 AND outval<30*100000000;";
	OutvalOfEntity[12] = "SELECT count(*) FROM entityinfo WHERE outval>=30*100000000 AND outval<40*100000000;";
	OutvalOfEntity[13] = "SELECT count(*) FROM entityinfo WHERE outval>=40*100000000 AND outval<50*100000000;";
	OutvalOfEntity[14] = "SELECT count(*) FROM entityinfo WHERE outval>=50*100000000 AND outval<60*100000000;";
	OutvalOfEntity[15] = "SELECT count(*) FROM entityinfo WHERE outval>=60*100000000 AND outval<70*100000000;";
	OutvalOfEntity[16] = "SELECT count(*) FROM entityinfo WHERE outval>=70*100000000 AND outval<80*100000000;";
	OutvalOfEntity[17] = "SELECT count(*) FROM entityinfo WHERE outval>=80*100000000 AND outval<90*100000000;";
	OutvalOfEntity[18] = "SELECT count(*) FROM entityinfo WHERE outval>=90*100000000 AND outval<100*100000000;";
	OutvalOfEntity[19] = "SELECT count(*) FROM entityinfo WHERE outval>=100*100000000 AND outval<1000*100000000;";
	OutvalOfEntity[20] = "SELECT count(*) FROM entityinfo WHERE outval>=1000*100000000 AND outval<2000*100000000;";
	OutvalOfEntity[21] = "SELECT count(*) FROM entityinfo WHERE outval>=2000*100000000 AND outval<3000*100000000;";
	OutvalOfEntity[22] = "SELECT count(*) FROM entityinfo WHERE outval>=3000*100000000 AND outval<4000*100000000;";
	OutvalOfEntity[23] = "SELECT count(*) FROM entityinfo WHERE outval>=4000*100000000 AND outval<5000*100000000;";
	OutvalOfEntity[24] = "SELECT count(*) FROM entityinfo WHERE outval>=5000*100000000 AND outval<6000*100000000;";
	OutvalOfEntity[25] = "SELECT count(*) FROM entityinfo WHERE outval>=6000*100000000 AND outval<7000*100000000;";
	OutvalOfEntity[26] = "SELECT count(*) FROM entityinfo WHERE outval>=7000*100000000 AND outval<8000*100000000;";
	OutvalOfEntity[27] = "SELECT count(*) FROM entityinfo WHERE outval>=8000*100000000 AND outval<9000*100000000;";
	OutvalOfEntity[28] = "SELECT count(*) FROM entityinfo WHERE outval>=9000*100000000 AND outval<10000*100000000;";
	OutvalOfEntity[29] = "SELECT count(*) FROM entityinfo WHERE outval>=10000*100000000 AND outval<20000*100000000;";
	OutvalOfEntity[30] = "SELECT count(*) FROM entityinfo WHERE outval>=20000*100000000 AND outval<30000*100000000;";
	OutvalOfEntity[31] = "SELECT count(*) FROM entityinfo WHERE outval>=30000*100000000 AND outval<40000*100000000;";
	OutvalOfEntity[32] = "SELECT count(*) FROM entityinfo WHERE outval>=40000*100000000 AND outval<50000*100000000;";
	OutvalOfEntity[33] = "SELECT count(*) FROM entityinfo WHERE outval>=50000*100000000 AND outval<60000*100000000;";
	OutvalOfEntity[34] = "SELECT count(*) FROM entityinfo WHERE outval>=60000*100000000 AND outval<70000*100000000;";
	OutvalOfEntity[35] = "SELECT count(*) FROM entityinfo WHERE outval>=70000*100000000 AND outval<80000*100000000;";
	OutvalOfEntity[36] = "SELECT count(*) FROM entityinfo WHERE outval>=80000*100000000 AND outval<90000*100000000;";
	OutvalOfEntity[37] = "SELECT count(*) FROM entityinfo WHERE outval>=90000*100000000 AND outval<100000*100000000;";
	OutvalOfEntity[38] = "SELECT count(*) FROM entityinfo WHERE outval>=100000*100000000 AND outval<200000*100000000;";
	OutvalOfEntity[39] = "SELECT count(*) FROM entityinfo WHERE outval>=200000*100000000 AND outval<300000*100000000;";
	OutvalOfEntity[40] = "SELECT count(*) FROM entityinfo WHERE outval>=300000*100000000 AND outval<400000*100000000;";
	OutvalOfEntity[41] = "SELECT count(*) FROM entityinfo WHERE outval>=400000*100000000 AND outval<500000*100000000;";
	OutvalOfEntity[42] = "SELECT count(*) FROM entityinfo WHERE outval>=500000*100000000 AND outval<600000*100000000;";
	OutvalOfEntity[43] = "SELECT count(*) FROM entityinfo WHERE outval>=600000*100000000 AND outval<700000*100000000;";
	OutvalOfEntity[44] = "SELECT count(*) FROM entityinfo WHERE outval>=700000*100000000 AND outval<800000*100000000;";
	OutvalOfEntity[45] = "SELECT count(*) FROM entityinfo WHERE outval>=800000*100000000 AND outval<900000*100000000;";
	OutvalOfEntity[46] = "SELECT count(*) FROM entityinfo WHERE outval>=900000*100000000 AND outval<1000000*100000000;";
	OutvalOfEntity[47] = "SELECT count(*) FROM entityinfo WHERE outval>=1000000*100000000;";

	record << "~~~~~~~~" << endl;
	record << "9.4 entity收入分布" << endl; cout << "   9.4 entity收入分布" << endl;
	for (unit8 i = 0; i < 48; i++)
	{
		mysql_query(&conn, OutvalOfEntity[i].c_str());
		result = mysql_store_result(&conn);
		if (row = mysql_fetch_row(result))
		{
			memset(result_c, 0, 500);
			//result_s.clear();

			strcpy(result_c, row[0]);
			result_s = result_c;
			record << result_s << '\t';
		}
		mysql_free_result(result);
	}
	record << endl;

	/*------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	// address当前余额分布
	string BalOfAdd[58];
	
	BalOfAdd[0] = "SELECT count(*) FROM addressinfo WHERE bal>=0*100000000 AND bal<0.01*100000000;";
	BalOfAdd[1] = "SELECT count(*) FROM addressinfo WHERE bal>=0.01*100000000 AND bal<0.1*100000000;";
	BalOfAdd[2] = "SELECT count(*) FROM addressinfo WHERE bal>=0.1*100000000 AND bal<0.2*100000000;";
	BalOfAdd[3] = "SELECT count(*) FROM addressinfo WHERE bal>=0.2*100000000 AND bal<0.3*100000000;";
	BalOfAdd[4] = "SELECT count(*) FROM addressinfo WHERE bal>=0.3*100000000 AND bal<0.4*100000000;";
	BalOfAdd[5] = "SELECT count(*) FROM addressinfo WHERE bal>=0.4*100000000 AND bal<0.5*100000000;";
	BalOfAdd[6] = "SELECT count(*) FROM addressinfo WHERE bal>=0.5*100000000 AND bal<0.6*100000000;";
	BalOfAdd[7] = "SELECT count(*) FROM addressinfo WHERE bal>=0.6*100000000 AND bal<0.7*100000000;";
	BalOfAdd[8] = "SELECT count(*) FROM addressinfo WHERE bal>=0.7*100000000 AND bal<0.8*100000000;";
	BalOfAdd[9] = "SELECT count(*) FROM addressinfo WHERE bal>=0.8*100000000 AND bal<0.9*100000000;";
	BalOfAdd[10] = "SELECT count(*) FROM addressinfo WHERE bal>=0.9*100000000 AND bal<1*100000000;";
	BalOfAdd[11] = "SELECT count(*) FROM addressinfo WHERE bal>=1*100000000 AND bal<2*100000000;";
	BalOfAdd[12] = "SELECT count(*) FROM addressinfo WHERE bal>=2*100000000 AND bal<3*100000000;";
	BalOfAdd[13] = "SELECT count(*) FROM addressinfo WHERE bal>=3*100000000 AND bal<4*100000000;";
	BalOfAdd[14] = "SELECT count(*) FROM addressinfo WHERE bal>=4*100000000 AND bal<5*100000000;";
	BalOfAdd[15] = "SELECT count(*) FROM addressinfo WHERE bal>=5*100000000 AND bal<6*100000000;";
	BalOfAdd[16] = "SELECT count(*) FROM addressinfo WHERE bal>=6*100000000 AND bal<7*100000000;";
	BalOfAdd[17] = "SELECT count(*) FROM addressinfo WHERE bal>=7*100000000 AND bal<8*100000000;";
	BalOfAdd[18] = "SELECT count(*) FROM addressinfo WHERE bal>=8*100000000 AND bal<9*100000000;";
	BalOfAdd[19] = "SELECT count(*) FROM addressinfo WHERE bal>=9*100000000 AND bal<10*100000000;";
	BalOfAdd[20] = "SELECT count(*) FROM addressinfo WHERE bal>=10*100000000 AND bal<20*100000000;";
	BalOfAdd[21] = "SELECT count(*) FROM addressinfo WHERE bal>=20*100000000 AND bal<30*100000000;";
	BalOfAdd[22] = "SELECT count(*) FROM addressinfo WHERE bal>=30*100000000 AND bal<40*100000000;";
	BalOfAdd[23] = "SELECT count(*) FROM addressinfo WHERE bal>=40*100000000 AND bal<50*100000000;";
	BalOfAdd[24] = "SELECT count(*) FROM addressinfo WHERE bal>=50*100000000 AND bal<60*100000000;";
	BalOfAdd[25] = "SELECT count(*) FROM addressinfo WHERE bal>=60*100000000 AND bal<70*100000000;";
	BalOfAdd[26] = "SELECT count(*) FROM addressinfo WHERE bal>=70*100000000 AND bal<80*100000000;";
	BalOfAdd[27] = "SELECT count(*) FROM addressinfo WHERE bal>=80*100000000 AND bal<90*100000000;";
	BalOfAdd[28] = "SELECT count(*) FROM addressinfo WHERE bal>=90*100000000 AND bal<100*100000000;";
	BalOfAdd[29] = "SELECT count(*) FROM addressinfo WHERE bal>=100*100000000 AND bal<1000*100000000;";
	BalOfAdd[30] = "SELECT count(*) FROM addressinfo WHERE bal>=1000*100000000 AND bal<2000*100000000;";
	BalOfAdd[31] = "SELECT count(*) FROM addressinfo WHERE bal>=2000*100000000 AND bal<3000*100000000;";
	BalOfAdd[32] = "SELECT count(*) FROM addressinfo WHERE bal>=3000*100000000 AND bal<4000*100000000;";
	BalOfAdd[33] = "SELECT count(*) FROM addressinfo WHERE bal>=4000*100000000 AND bal<5000*100000000;";
	BalOfAdd[34] = "SELECT count(*) FROM addressinfo WHERE bal>=5000*100000000 AND bal<6000*100000000;";
	BalOfAdd[35] = "SELECT count(*) FROM addressinfo WHERE bal>=6000*100000000 AND bal<7000*100000000;";
	BalOfAdd[36] = "SELECT count(*) FROM addressinfo WHERE bal>=7000*100000000 AND bal<8000*100000000;";
	BalOfAdd[37] = "SELECT count(*) FROM addressinfo WHERE bal>=8000*100000000 AND bal<9000*100000000;";
	BalOfAdd[38] = "SELECT count(*) FROM addressinfo WHERE bal>=9000*100000000 AND bal<10000*100000000;";
	BalOfAdd[39] = "SELECT count(*) FROM addressinfo WHERE bal>=10000*100000000 AND bal<20000*100000000;";
	BalOfAdd[40] = "SELECT count(*) FROM addressinfo WHERE bal>=20000*100000000 AND bal<30000*100000000;";
	BalOfAdd[41] = "SELECT count(*) FROM addressinfo WHERE bal>=30000*100000000 AND bal<40000*100000000;";
	BalOfAdd[42] = "SELECT count(*) FROM addressinfo WHERE bal>=40000*100000000 AND bal<50000*100000000;";
	BalOfAdd[43] = "SELECT count(*) FROM addressinfo WHERE bal>=50000*100000000 AND bal<60000*100000000;";
	BalOfAdd[44] = "SELECT count(*) FROM addressinfo WHERE bal>=60000*100000000 AND bal<70000*100000000;";
	BalOfAdd[45] = "SELECT count(*) FROM addressinfo WHERE bal>=70000*100000000 AND bal<80000*100000000;";
	BalOfAdd[46] = "SELECT count(*) FROM addressinfo WHERE bal>=80000*100000000 AND bal<90000*100000000;";
	BalOfAdd[47] = "SELECT count(*) FROM addressinfo WHERE bal>=90000*100000000 AND bal<100000*100000000;";
	BalOfAdd[48] = "SELECT count(*) FROM addressinfo WHERE bal>=100000*100000000 AND bal<200000*100000000;";
	BalOfAdd[49] = "SELECT count(*) FROM addressinfo WHERE bal>=200000*100000000 AND bal<300000*100000000;";
	BalOfAdd[50] = "SELECT count(*) FROM addressinfo WHERE bal>=300000*100000000 AND bal<400000*100000000;";
	BalOfAdd[51] = "SELECT count(*) FROM addressinfo WHERE bal>=400000*100000000 AND bal<500000*100000000;";
	BalOfAdd[52] = "SELECT count(*) FROM addressinfo WHERE bal>=500000*100000000 AND bal<600000*100000000;";
	BalOfAdd[53] = "SELECT count(*) FROM addressinfo WHERE bal>=600000*100000000 AND bal<700000*100000000;";
	BalOfAdd[54] = "SELECT count(*) FROM addressinfo WHERE bal>=700000*100000000 AND bal<800000*100000000;";
	BalOfAdd[55] = "SELECT count(*) FROM addressinfo WHERE bal>=800000*100000000 AND bal<900000*100000000;";
	BalOfAdd[56] = "SELECT count(*) FROM addressinfo WHERE bal>=900000*100000000 AND bal<1000000*100000000;";
	BalOfAdd[57] = "SELECT count(*) FROM addressinfo WHERE bal>=1000000*100000000;";

	record << "~~~~~~~~" << endl;
	record << "9.5 address当前余额分布" << endl; cout << "   9.5 address当前余额分布" << endl;
	for (unit8 i = 0; i < 58; i++)
	{
		mysql_query(&conn, BalOfAdd[i].c_str());
		result = mysql_store_result(&conn);
		if (row = mysql_fetch_row(result))
		{
			memset(result_c, 0, 500);
			//result_s.clear();

			strcpy(result_c, row[0]);
			result_s = result_c;
			record << result_s << '\t';
		}
		mysql_free_result(result);
	}
	record << endl;

	// entity当前余额分布
	string BalOfEntity[58];

	BalOfEntity[0] = "SELECT count(*) FROM entityinfo WHERE bal>=0*100000000 AND bal<0.01*100000000;";
	BalOfEntity[1] = "SELECT count(*) FROM entityinfo WHERE bal>=0.01*100000000 AND bal<0.1*100000000;";
	BalOfEntity[2] = "SELECT count(*) FROM entityinfo WHERE bal>=0.1*100000000 AND bal<0.2*100000000;";
	BalOfEntity[3] = "SELECT count(*) FROM entityinfo WHERE bal>=0.2*100000000 AND bal<0.3*100000000;";
	BalOfEntity[4] = "SELECT count(*) FROM entityinfo WHERE bal>=0.3*100000000 AND bal<0.4*100000000;";
	BalOfEntity[5] = "SELECT count(*) FROM entityinfo WHERE bal>=0.4*100000000 AND bal<0.5*100000000;";
	BalOfEntity[6] = "SELECT count(*) FROM entityinfo WHERE bal>=0.5*100000000 AND bal<0.6*100000000;";
	BalOfEntity[7] = "SELECT count(*) FROM entityinfo WHERE bal>=0.6*100000000 AND bal<0.7*100000000;";
	BalOfEntity[8] = "SELECT count(*) FROM entityinfo WHERE bal>=0.7*100000000 AND bal<0.8*100000000;";
	BalOfEntity[9] = "SELECT count(*) FROM entityinfo WHERE bal>=0.8*100000000 AND bal<0.9*100000000;";
	BalOfEntity[10] = "SELECT count(*) FROM entityinfo WHERE bal>=0.9*100000000 AND bal<1*100000000;";
	BalOfEntity[11] = "SELECT count(*) FROM entityinfo WHERE bal>=1*100000000 AND bal<2*100000000;";
	BalOfEntity[12] = "SELECT count(*) FROM entityinfo WHERE bal>=2*100000000 AND bal<3*100000000;";
	BalOfEntity[13] = "SELECT count(*) FROM entityinfo WHERE bal>=3*100000000 AND bal<4*100000000;";
	BalOfEntity[14] = "SELECT count(*) FROM entityinfo WHERE bal>=4*100000000 AND bal<5*100000000;";
	BalOfEntity[15] = "SELECT count(*) FROM entityinfo WHERE bal>=5*100000000 AND bal<6*100000000;";
	BalOfEntity[16] = "SELECT count(*) FROM entityinfo WHERE bal>=6*100000000 AND bal<7*100000000;";
	BalOfEntity[17] = "SELECT count(*) FROM entityinfo WHERE bal>=7*100000000 AND bal<8*100000000;";
	BalOfEntity[18] = "SELECT count(*) FROM entityinfo WHERE bal>=8*100000000 AND bal<9*100000000;";
	BalOfEntity[19] = "SELECT count(*) FROM entityinfo WHERE bal>=9*100000000 AND bal<10*100000000;";
	BalOfEntity[20] = "SELECT count(*) FROM entityinfo WHERE bal>=10*100000000 AND bal<20*100000000;";
	BalOfEntity[21] = "SELECT count(*) FROM entityinfo WHERE bal>=20*100000000 AND bal<30*100000000;";
	BalOfEntity[22] = "SELECT count(*) FROM entityinfo WHERE bal>=30*100000000 AND bal<40*100000000;";
	BalOfEntity[23] = "SELECT count(*) FROM entityinfo WHERE bal>=40*100000000 AND bal<50*100000000;";
	BalOfEntity[24] = "SELECT count(*) FROM entityinfo WHERE bal>=50*100000000 AND bal<60*100000000;";
	BalOfEntity[25] = "SELECT count(*) FROM entityinfo WHERE bal>=60*100000000 AND bal<70*100000000;";
	BalOfEntity[26] = "SELECT count(*) FROM entityinfo WHERE bal>=70*100000000 AND bal<80*100000000;";
	BalOfEntity[27] = "SELECT count(*) FROM entityinfo WHERE bal>=80*100000000 AND bal<90*100000000;";
	BalOfEntity[28] = "SELECT count(*) FROM entityinfo WHERE bal>=90*100000000 AND bal<100*100000000;";
	BalOfEntity[29] = "SELECT count(*) FROM entityinfo WHERE bal>=100*100000000 AND bal<1000*100000000;";
	BalOfEntity[30] = "SELECT count(*) FROM entityinfo WHERE bal>=1000*100000000 AND bal<2000*100000000;";
	BalOfEntity[31] = "SELECT count(*) FROM entityinfo WHERE bal>=2000*100000000 AND bal<3000*100000000;";
	BalOfEntity[32] = "SELECT count(*) FROM entityinfo WHERE bal>=3000*100000000 AND bal<4000*100000000;";
	BalOfEntity[33] = "SELECT count(*) FROM entityinfo WHERE bal>=4000*100000000 AND bal<5000*100000000;";
	BalOfEntity[34] = "SELECT count(*) FROM entityinfo WHERE bal>=5000*100000000 AND bal<6000*100000000;";
	BalOfEntity[35] = "SELECT count(*) FROM entityinfo WHERE bal>=6000*100000000 AND bal<7000*100000000;";
	BalOfEntity[36] = "SELECT count(*) FROM entityinfo WHERE bal>=7000*100000000 AND bal<8000*100000000;";
	BalOfEntity[37] = "SELECT count(*) FROM entityinfo WHERE bal>=8000*100000000 AND bal<9000*100000000;";
	BalOfEntity[38] = "SELECT count(*) FROM entityinfo WHERE bal>=9000*100000000 AND bal<10000*100000000;";
	BalOfEntity[39] = "SELECT count(*) FROM entityinfo WHERE bal>=10000*100000000 AND bal<20000*100000000;";
	BalOfEntity[40] = "SELECT count(*) FROM entityinfo WHERE bal>=20000*100000000 AND bal<30000*100000000;";
	BalOfEntity[41] = "SELECT count(*) FROM entityinfo WHERE bal>=30000*100000000 AND bal<40000*100000000;";
	BalOfEntity[42] = "SELECT count(*) FROM entityinfo WHERE bal>=40000*100000000 AND bal<50000*100000000;";
	BalOfEntity[43] = "SELECT count(*) FROM entityinfo WHERE bal>=50000*100000000 AND bal<60000*100000000;";
	BalOfEntity[44] = "SELECT count(*) FROM entityinfo WHERE bal>=60000*100000000 AND bal<70000*100000000;";
	BalOfEntity[45] = "SELECT count(*) FROM entityinfo WHERE bal>=70000*100000000 AND bal<80000*100000000;";
	BalOfEntity[46] = "SELECT count(*) FROM entityinfo WHERE bal>=80000*100000000 AND bal<90000*100000000;";
	BalOfEntity[47] = "SELECT count(*) FROM entityinfo WHERE bal>=90000*100000000 AND bal<100000*100000000;";
	BalOfEntity[48] = "SELECT count(*) FROM entityinfo WHERE bal>=100000*100000000 AND bal<200000*100000000;";
	BalOfEntity[49] = "SELECT count(*) FROM entityinfo WHERE bal>=200000*100000000 AND bal<300000*100000000;";
	BalOfEntity[50] = "SELECT count(*) FROM entityinfo WHERE bal>=300000*100000000 AND bal<400000*100000000;";
	BalOfEntity[51] = "SELECT count(*) FROM entityinfo WHERE bal>=400000*100000000 AND bal<500000*100000000;";
	BalOfEntity[52] = "SELECT count(*) FROM entityinfo WHERE bal>=500000*100000000 AND bal<600000*100000000;";
	BalOfEntity[53] = "SELECT count(*) FROM entityinfo WHERE bal>=600000*100000000 AND bal<700000*100000000;";
	BalOfEntity[54] = "SELECT count(*) FROM entityinfo WHERE bal>=700000*100000000 AND bal<800000*100000000;";
	BalOfEntity[55] = "SELECT count(*) FROM entityinfo WHERE bal>=800000*100000000 AND bal<900000*100000000;";
	BalOfEntity[56] = "SELECT count(*) FROM entityinfo WHERE bal>=900000*100000000 AND bal<1000000*100000000;";
	BalOfEntity[57] = "SELECT count(*) FROM entityinfo WHERE bal>=1000000*100000000;";

	record << "~~~~~~~~" << endl;
	record << "9.6 entity当前余额分布" << endl; cout << "   9.6 entity当前余额分布" << endl;
	for (unit8 i = 0; i < 58; i++)
	{
		mysql_query(&conn, BalOfEntity[i].c_str());
		result = mysql_store_result(&conn);
		if (row = mysql_fetch_row(result))
		{
			memset(result_c, 0, 500);
			//result_s.clear();

			strcpy(result_c, row[0]);
			result_s = result_c;
			record << result_s << '\t';
		}
		mysql_free_result(result);
	}
	record << endl;

	// address历史最大余额分布
	string MaxbalOfAdd[58];

	MaxbalOfAdd[0] = "SELECT count(*) FROM addressinfo WHERE maxbal>=0*100000000 AND maxbal<0.01*100000000;";
	MaxbalOfAdd[1] = "SELECT count(*) FROM addressinfo WHERE maxbal>=0.01*100000000 AND maxbal<0.1*100000000;";
	MaxbalOfAdd[2] = "SELECT count(*) FROM addressinfo WHERE maxbal>=0.1*100000000 AND maxbal<0.2*100000000;";
	MaxbalOfAdd[3] = "SELECT count(*) FROM addressinfo WHERE maxbal>=0.2*100000000 AND maxbal<0.3*100000000;";
	MaxbalOfAdd[4] = "SELECT count(*) FROM addressinfo WHERE maxbal>=0.3*100000000 AND maxbal<0.4*100000000;";
	MaxbalOfAdd[5] = "SELECT count(*) FROM addressinfo WHERE maxbal>=0.4*100000000 AND maxbal<0.5*100000000;";
	MaxbalOfAdd[6] = "SELECT count(*) FROM addressinfo WHERE maxbal>=0.5*100000000 AND maxbal<0.6*100000000;";
	MaxbalOfAdd[7] = "SELECT count(*) FROM addressinfo WHERE maxbal>=0.6*100000000 AND maxbal<0.7*100000000;";
	MaxbalOfAdd[8] = "SELECT count(*) FROM addressinfo WHERE maxbal>=0.7*100000000 AND maxbal<0.8*100000000;";
	MaxbalOfAdd[9] = "SELECT count(*) FROM addressinfo WHERE maxbal>=0.8*100000000 AND maxbal<0.9*100000000;";
	MaxbalOfAdd[10] = "SELECT count(*) FROM addressinfo WHERE maxbal>=0.9*100000000 AND maxbal<1*100000000;";
	MaxbalOfAdd[11] = "SELECT count(*) FROM addressinfo WHERE maxbal>=1*100000000 AND maxbal<2*100000000;";
	MaxbalOfAdd[12] = "SELECT count(*) FROM addressinfo WHERE maxbal>=2*100000000 AND maxbal<3*100000000;";
	MaxbalOfAdd[13] = "SELECT count(*) FROM addressinfo WHERE maxbal>=3*100000000 AND maxbal<4*100000000;";
	MaxbalOfAdd[14] = "SELECT count(*) FROM addressinfo WHERE maxbal>=4*100000000 AND maxbal<5*100000000;";
	MaxbalOfAdd[15] = "SELECT count(*) FROM addressinfo WHERE maxbal>=5*100000000 AND maxbal<6*100000000;";
	MaxbalOfAdd[16] = "SELECT count(*) FROM addressinfo WHERE maxbal>=6*100000000 AND maxbal<7*100000000;";
	MaxbalOfAdd[17] = "SELECT count(*) FROM addressinfo WHERE maxbal>=7*100000000 AND maxbal<8*100000000;";
	MaxbalOfAdd[18] = "SELECT count(*) FROM addressinfo WHERE maxbal>=8*100000000 AND maxbal<9*100000000;";
	MaxbalOfAdd[19] = "SELECT count(*) FROM addressinfo WHERE maxbal>=9*100000000 AND maxbal<10*100000000;";
	MaxbalOfAdd[20] = "SELECT count(*) FROM addressinfo WHERE maxbal>=10*100000000 AND maxbal<20*100000000;";
	MaxbalOfAdd[21] = "SELECT count(*) FROM addressinfo WHERE maxbal>=20*100000000 AND maxbal<30*100000000;";
	MaxbalOfAdd[22] = "SELECT count(*) FROM addressinfo WHERE maxbal>=30*100000000 AND maxbal<40*100000000;";
	MaxbalOfAdd[23] = "SELECT count(*) FROM addressinfo WHERE maxbal>=40*100000000 AND maxbal<50*100000000;";
	MaxbalOfAdd[24] = "SELECT count(*) FROM addressinfo WHERE maxbal>=50*100000000 AND maxbal<60*100000000;";
	MaxbalOfAdd[25] = "SELECT count(*) FROM addressinfo WHERE maxbal>=60*100000000 AND maxbal<70*100000000;";
	MaxbalOfAdd[26] = "SELECT count(*) FROM addressinfo WHERE maxbal>=70*100000000 AND maxbal<80*100000000;";
	MaxbalOfAdd[27] = "SELECT count(*) FROM addressinfo WHERE maxbal>=80*100000000 AND maxbal<90*100000000;";
	MaxbalOfAdd[28] = "SELECT count(*) FROM addressinfo WHERE maxbal>=90*100000000 AND maxbal<100*100000000;";
	MaxbalOfAdd[29] = "SELECT count(*) FROM addressinfo WHERE maxbal>=100*100000000 AND maxbal<1000*100000000;";
	MaxbalOfAdd[30] = "SELECT count(*) FROM addressinfo WHERE maxbal>=1000*100000000 AND maxbal<2000*100000000;";
	MaxbalOfAdd[31] = "SELECT count(*) FROM addressinfo WHERE maxbal>=2000*100000000 AND maxbal<3000*100000000;";
	MaxbalOfAdd[32] = "SELECT count(*) FROM addressinfo WHERE maxbal>=3000*100000000 AND maxbal<4000*100000000;";
	MaxbalOfAdd[33] = "SELECT count(*) FROM addressinfo WHERE maxbal>=4000*100000000 AND maxbal<5000*100000000;";
	MaxbalOfAdd[34] = "SELECT count(*) FROM addressinfo WHERE maxbal>=5000*100000000 AND maxbal<6000*100000000;";
	MaxbalOfAdd[35] = "SELECT count(*) FROM addressinfo WHERE maxbal>=6000*100000000 AND maxbal<7000*100000000;";
	MaxbalOfAdd[36] = "SELECT count(*) FROM addressinfo WHERE maxbal>=7000*100000000 AND maxbal<8000*100000000;";
	MaxbalOfAdd[37] = "SELECT count(*) FROM addressinfo WHERE maxbal>=8000*100000000 AND maxbal<9000*100000000;";
	MaxbalOfAdd[38] = "SELECT count(*) FROM addressinfo WHERE maxbal>=9000*100000000 AND maxbal<10000*100000000;";
	MaxbalOfAdd[39] = "SELECT count(*) FROM addressinfo WHERE maxbal>=10000*100000000 AND maxbal<20000*100000000;";
	MaxbalOfAdd[40] = "SELECT count(*) FROM addressinfo WHERE maxbal>=20000*100000000 AND maxbal<30000*100000000;";
	MaxbalOfAdd[41] = "SELECT count(*) FROM addressinfo WHERE maxbal>=30000*100000000 AND maxbal<40000*100000000;";
	MaxbalOfAdd[42] = "SELECT count(*) FROM addressinfo WHERE maxbal>=40000*100000000 AND maxbal<50000*100000000;";
	MaxbalOfAdd[43] = "SELECT count(*) FROM addressinfo WHERE maxbal>=50000*100000000 AND maxbal<60000*100000000;";
	MaxbalOfAdd[44] = "SELECT count(*) FROM addressinfo WHERE maxbal>=60000*100000000 AND maxbal<70000*100000000;";
	MaxbalOfAdd[45] = "SELECT count(*) FROM addressinfo WHERE maxbal>=70000*100000000 AND maxbal<80000*100000000;";
	MaxbalOfAdd[46] = "SELECT count(*) FROM addressinfo WHERE maxbal>=80000*100000000 AND maxbal<90000*100000000;";
	MaxbalOfAdd[47] = "SELECT count(*) FROM addressinfo WHERE maxbal>=90000*100000000 AND maxbal<100000*100000000;";
	MaxbalOfAdd[48] = "SELECT count(*) FROM addressinfo WHERE maxbal>=100000*100000000 AND maxbal<200000*100000000;";
	MaxbalOfAdd[49] = "SELECT count(*) FROM addressinfo WHERE maxbal>=200000*100000000 AND maxbal<300000*100000000;";
	MaxbalOfAdd[50] = "SELECT count(*) FROM addressinfo WHERE maxbal>=300000*100000000 AND maxbal<400000*100000000;";
	MaxbalOfAdd[51] = "SELECT count(*) FROM addressinfo WHERE maxbal>=400000*100000000 AND maxbal<500000*100000000;";
	MaxbalOfAdd[52] = "SELECT count(*) FROM addressinfo WHERE maxbal>=500000*100000000 AND maxbal<600000*100000000;";
	MaxbalOfAdd[53] = "SELECT count(*) FROM addressinfo WHERE maxbal>=600000*100000000 AND maxbal<700000*100000000;";
	MaxbalOfAdd[54] = "SELECT count(*) FROM addressinfo WHERE maxbal>=700000*100000000 AND maxbal<800000*100000000;";
	MaxbalOfAdd[55] = "SELECT count(*) FROM addressinfo WHERE maxbal>=800000*100000000 AND maxbal<900000*100000000;";
	MaxbalOfAdd[56] = "SELECT count(*) FROM addressinfo WHERE maxbal>=900000*100000000 AND maxbal<1000000*100000000;";
	MaxbalOfAdd[57] = "SELECT count(*) FROM addressinfo WHERE maxbal>=1000000*100000000;";

	record << "~~~~~~~~" << endl;
	record << "9.7 address历史最大余额分布" << endl; cout << "   9.7 address历史最大余额分布" << endl;
	for (unit8 i = 0; i < 58; i++)
	{
		mysql_query(&conn, MaxbalOfAdd[i].c_str());
		result = mysql_store_result(&conn);
		if (row = mysql_fetch_row(result))
		{
			memset(result_c, 0, 500);
			//result_s.clear();

			strcpy(result_c, row[0]);
			result_s = result_c;
			record << result_s << '\t';
		}
		mysql_free_result(result);
	}
	record << endl;

	// entity历史最大余额分布
	string MaxbalOfEntity[58];

	MaxbalOfEntity[0] = "SELECT count(*) FROM entityinfo WHERE maxbal>=0*100000000 AND maxbal<0.01*100000000;";
	MaxbalOfEntity[1] = "SELECT count(*) FROM entityinfo WHERE maxbal>=0.01*100000000 AND maxbal<0.1*100000000;";
	MaxbalOfEntity[2] = "SELECT count(*) FROM entityinfo WHERE maxbal>=0.1*100000000 AND maxbal<0.2*100000000;";
	MaxbalOfEntity[3] = "SELECT count(*) FROM entityinfo WHERE maxbal>=0.2*100000000 AND maxbal<0.3*100000000;";
	MaxbalOfEntity[4] = "SELECT count(*) FROM entityinfo WHERE maxbal>=0.3*100000000 AND maxbal<0.4*100000000;";
	MaxbalOfEntity[5] = "SELECT count(*) FROM entityinfo WHERE maxbal>=0.4*100000000 AND maxbal<0.5*100000000;";
	MaxbalOfEntity[6] = "SELECT count(*) FROM entityinfo WHERE maxbal>=0.5*100000000 AND maxbal<0.6*100000000;";
	MaxbalOfEntity[7] = "SELECT count(*) FROM entityinfo WHERE maxbal>=0.6*100000000 AND maxbal<0.7*100000000;";
	MaxbalOfEntity[8] = "SELECT count(*) FROM entityinfo WHERE maxbal>=0.7*100000000 AND maxbal<0.8*100000000;";
	MaxbalOfEntity[9] = "SELECT count(*) FROM entityinfo WHERE maxbal>=0.8*100000000 AND maxbal<0.9*100000000;";
	MaxbalOfEntity[10] = "SELECT count(*) FROM entityinfo WHERE maxbal>=0.9*100000000 AND maxbal<1*100000000;";
	MaxbalOfEntity[11] = "SELECT count(*) FROM entityinfo WHERE maxbal>=1*100000000 AND maxbal<2*100000000;";
	MaxbalOfEntity[12] = "SELECT count(*) FROM entityinfo WHERE maxbal>=2*100000000 AND maxbal<3*100000000;";
	MaxbalOfEntity[13] = "SELECT count(*) FROM entityinfo WHERE maxbal>=3*100000000 AND maxbal<4*100000000;";
	MaxbalOfEntity[14] = "SELECT count(*) FROM entityinfo WHERE maxbal>=4*100000000 AND maxbal<5*100000000;";
	MaxbalOfEntity[15] = "SELECT count(*) FROM entityinfo WHERE maxbal>=5*100000000 AND maxbal<6*100000000;";
	MaxbalOfEntity[16] = "SELECT count(*) FROM entityinfo WHERE maxbal>=6*100000000 AND maxbal<7*100000000;";
	MaxbalOfEntity[17] = "SELECT count(*) FROM entityinfo WHERE maxbal>=7*100000000 AND maxbal<8*100000000;";
	MaxbalOfEntity[18] = "SELECT count(*) FROM entityinfo WHERE maxbal>=8*100000000 AND maxbal<9*100000000;";
	MaxbalOfEntity[19] = "SELECT count(*) FROM entityinfo WHERE maxbal>=9*100000000 AND maxbal<10*100000000;";
	MaxbalOfEntity[20] = "SELECT count(*) FROM entityinfo WHERE maxbal>=10*100000000 AND maxbal<20*100000000;";
	MaxbalOfEntity[21] = "SELECT count(*) FROM entityinfo WHERE maxbal>=20*100000000 AND maxbal<30*100000000;";
	MaxbalOfEntity[22] = "SELECT count(*) FROM entityinfo WHERE maxbal>=30*100000000 AND maxbal<40*100000000;";
	MaxbalOfEntity[23] = "SELECT count(*) FROM entityinfo WHERE maxbal>=40*100000000 AND maxbal<50*100000000;";
	MaxbalOfEntity[24] = "SELECT count(*) FROM entityinfo WHERE maxbal>=50*100000000 AND maxbal<60*100000000;";
	MaxbalOfEntity[25] = "SELECT count(*) FROM entityinfo WHERE maxbal>=60*100000000 AND maxbal<70*100000000;";
	MaxbalOfEntity[26] = "SELECT count(*) FROM entityinfo WHERE maxbal>=70*100000000 AND maxbal<80*100000000;";
	MaxbalOfEntity[27] = "SELECT count(*) FROM entityinfo WHERE maxbal>=80*100000000 AND maxbal<90*100000000;";
	MaxbalOfEntity[28] = "SELECT count(*) FROM entityinfo WHERE maxbal>=90*100000000 AND maxbal<100*100000000;";
	MaxbalOfEntity[29] = "SELECT count(*) FROM entityinfo WHERE maxbal>=100*100000000 AND maxbal<1000*100000000;";
	MaxbalOfEntity[30] = "SELECT count(*) FROM entityinfo WHERE maxbal>=1000*100000000 AND maxbal<2000*100000000;";
	MaxbalOfEntity[31] = "SELECT count(*) FROM entityinfo WHERE maxbal>=2000*100000000 AND maxbal<3000*100000000;";
	MaxbalOfEntity[32] = "SELECT count(*) FROM entityinfo WHERE maxbal>=3000*100000000 AND maxbal<4000*100000000;";
	MaxbalOfEntity[33] = "SELECT count(*) FROM entityinfo WHERE maxbal>=4000*100000000 AND maxbal<5000*100000000;";
	MaxbalOfEntity[34] = "SELECT count(*) FROM entityinfo WHERE maxbal>=5000*100000000 AND maxbal<6000*100000000;";
	MaxbalOfEntity[35] = "SELECT count(*) FROM entityinfo WHERE maxbal>=6000*100000000 AND maxbal<7000*100000000;";
	MaxbalOfEntity[36] = "SELECT count(*) FROM entityinfo WHERE maxbal>=7000*100000000 AND maxbal<8000*100000000;";
	MaxbalOfEntity[37] = "SELECT count(*) FROM entityinfo WHERE maxbal>=8000*100000000 AND maxbal<9000*100000000;";
	MaxbalOfEntity[38] = "SELECT count(*) FROM entityinfo WHERE maxbal>=9000*100000000 AND maxbal<10000*100000000;";
	MaxbalOfEntity[39] = "SELECT count(*) FROM entityinfo WHERE maxbal>=10000*100000000 AND maxbal<20000*100000000;";
	MaxbalOfEntity[40] = "SELECT count(*) FROM entityinfo WHERE maxbal>=20000*100000000 AND maxbal<30000*100000000;";
	MaxbalOfEntity[41] = "SELECT count(*) FROM entityinfo WHERE maxbal>=30000*100000000 AND maxbal<40000*100000000;";
	MaxbalOfEntity[42] = "SELECT count(*) FROM entityinfo WHERE maxbal>=40000*100000000 AND maxbal<50000*100000000;";
	MaxbalOfEntity[43] = "SELECT count(*) FROM entityinfo WHERE maxbal>=50000*100000000 AND maxbal<60000*100000000;";
	MaxbalOfEntity[44] = "SELECT count(*) FROM entityinfo WHERE maxbal>=60000*100000000 AND maxbal<70000*100000000;";
	MaxbalOfEntity[45] = "SELECT count(*) FROM entityinfo WHERE maxbal>=70000*100000000 AND maxbal<80000*100000000;";
	MaxbalOfEntity[46] = "SELECT count(*) FROM entityinfo WHERE maxbal>=80000*100000000 AND maxbal<90000*100000000;";
	MaxbalOfEntity[47] = "SELECT count(*) FROM entityinfo WHERE maxbal>=90000*100000000 AND maxbal<100000*100000000;";
	MaxbalOfEntity[48] = "SELECT count(*) FROM entityinfo WHERE maxbal>=100000*100000000 AND maxbal<200000*100000000;";
	MaxbalOfEntity[49] = "SELECT count(*) FROM entityinfo WHERE maxbal>=200000*100000000 AND maxbal<300000*100000000;";
	MaxbalOfEntity[50] = "SELECT count(*) FROM entityinfo WHERE maxbal>=300000*100000000 AND maxbal<400000*100000000;";
	MaxbalOfEntity[51] = "SELECT count(*) FROM entityinfo WHERE maxbal>=400000*100000000 AND maxbal<500000*100000000;";
	MaxbalOfEntity[52] = "SELECT count(*) FROM entityinfo WHERE maxbal>=500000*100000000 AND maxbal<600000*100000000;";
	MaxbalOfEntity[53] = "SELECT count(*) FROM entityinfo WHERE maxbal>=600000*100000000 AND maxbal<700000*100000000;";
	MaxbalOfEntity[54] = "SELECT count(*) FROM entityinfo WHERE maxbal>=700000*100000000 AND maxbal<800000*100000000;";
	MaxbalOfEntity[55] = "SELECT count(*) FROM entityinfo WHERE maxbal>=800000*100000000 AND maxbal<900000*100000000;";
	MaxbalOfEntity[56] = "SELECT count(*) FROM entityinfo WHERE maxbal>=900000*100000000 AND maxbal<1000000*100000000;";
	MaxbalOfEntity[57] = "SELECT count(*) FROM entityinfo WHERE maxbal>=1000000*100000000;";

	record << "~~~~~~~~" << endl;
	record << "9.8 entity历史最大余额分布" << endl; cout << "   9.8 entity历史最大余额分布" << endl;
	for (unit8 i = 0; i < 58; i++)
	{
		mysql_query(&conn, MaxbalOfEntity[i].c_str());
		result = mysql_store_result(&conn);
		if (row = mysql_fetch_row(result))
		{
			memset(result_c, 0, 500);
			//result_s.clear();

			strcpy(result_c, row[0]);
			result_s = result_c;
			record << result_s << '\t';
		}
		mysql_free_result(result);
	}
	record << endl;

	// address参与交易数
	string TxcntOfAdd[25];

	TxcntOfAdd[0] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=1 AND (incnt+outcnt)<2;";
	TxcntOfAdd[1] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=2 AND (incnt+outcnt)<3;";
	TxcntOfAdd[2] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=3 AND (incnt+outcnt)<4;";
	TxcntOfAdd[3] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=4 AND (incnt+outcnt)<5;";
	TxcntOfAdd[4] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=5 AND (incnt+outcnt)<6;";
	TxcntOfAdd[5] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=6 AND (incnt+outcnt)<7;";
	TxcntOfAdd[6] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=7 AND (incnt+outcnt)<8;";
	TxcntOfAdd[7] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=8 AND (incnt+outcnt)<9;";
	TxcntOfAdd[8] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=9 AND (incnt+outcnt)<10;";
	TxcntOfAdd[9] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=10 AND (incnt+outcnt)<100;";
	TxcntOfAdd[10] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=100 AND (incnt+outcnt)<200;";
	TxcntOfAdd[11] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=200 AND (incnt+outcnt)<300;";
	TxcntOfAdd[12] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=300 AND (incnt+outcnt)<400;";
	TxcntOfAdd[13] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=400 AND (incnt+outcnt)<500;";
	TxcntOfAdd[14] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=500 AND (incnt+outcnt)<600;";
	TxcntOfAdd[15] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=600 AND (incnt+outcnt)<700;";
	TxcntOfAdd[16] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=700 AND (incnt+outcnt)<800;";
	TxcntOfAdd[17] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=800 AND (incnt+outcnt)<900;";
	TxcntOfAdd[18] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=900 AND (incnt+outcnt)<1000;";
	TxcntOfAdd[19] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=1000 AND (incnt+outcnt)<5000;";
	TxcntOfAdd[20] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=5000 AND (incnt+outcnt)<10000;";
	TxcntOfAdd[21] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=10000 AND (incnt+outcnt)<50000;";
	TxcntOfAdd[22] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=50000 AND (incnt+outcnt)<100000;";
	TxcntOfAdd[23] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=100000 AND (incnt+outcnt)<500000;";
	TxcntOfAdd[24] = "SELECT count(*) FROM addressinfo WHERE (incnt+outcnt)>=500000;";

	record << "~~~~~~~~" << endl;
	record << "9.9 address参与交易数" << endl; cout << "   9.9 address参与交易数" << endl;
	for (unit8 i = 0; i < 25; i++)
	{
		mysql_query(&conn, TxcntOfAdd[i].c_str());
		result = mysql_store_result(&conn);
		if (row = mysql_fetch_row(result))
		{
			memset(result_c, 0, 500);
			//result_s.clear();

			strcpy(result_c, row[0]);
			result_s = result_c;
			record << result_s << '\t';
		}
		mysql_free_result(result);
	}
	record << endl;

	// entity参与交易数
	string TxcntOfEntity[25];

	TxcntOfEntity[0] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=1 AND (incnt+outcnt)<2;";
	TxcntOfEntity[1] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=2 AND (incnt+outcnt)<3;";
	TxcntOfEntity[2] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=3 AND (incnt+outcnt)<4;";
	TxcntOfEntity[3] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=4 AND (incnt+outcnt)<5;";
	TxcntOfEntity[4] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=5 AND (incnt+outcnt)<6;";
	TxcntOfEntity[5] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=6 AND (incnt+outcnt)<7;";
	TxcntOfEntity[6] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=7 AND (incnt+outcnt)<8;";
	TxcntOfEntity[7] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=8 AND (incnt+outcnt)<9;";
	TxcntOfEntity[8] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=9 AND (incnt+outcnt)<10;";
	TxcntOfEntity[9] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=10 AND (incnt+outcnt)<100;";
	TxcntOfEntity[10] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=100 AND (incnt+outcnt)<200;";
	TxcntOfEntity[11] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=200 AND (incnt+outcnt)<300;";
	TxcntOfEntity[12] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=300 AND (incnt+outcnt)<400;";
	TxcntOfEntity[13] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=400 AND (incnt+outcnt)<500;";
	TxcntOfEntity[14] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=500 AND (incnt+outcnt)<600;";
	TxcntOfEntity[15] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=600 AND (incnt+outcnt)<700;";
	TxcntOfEntity[16] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=700 AND (incnt+outcnt)<800;";
	TxcntOfEntity[17] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=800 AND (incnt+outcnt)<900;";
	TxcntOfEntity[18] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=900 AND (incnt+outcnt)<1000;";
	TxcntOfEntity[19] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=1000 AND (incnt+outcnt)<5000;";
	TxcntOfEntity[20] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=5000 AND (incnt+outcnt)<10000;";
	TxcntOfEntity[21] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=10000 AND (incnt+outcnt)<50000;";
	TxcntOfEntity[22] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=50000 AND (incnt+outcnt)<100000;";
	TxcntOfEntity[23] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=100000 AND (incnt+outcnt)<500000;";
	TxcntOfEntity[24] = "SELECT count(*) FROM entityinfo WHERE (incnt+outcnt)>=500000;";

	record << "~~~~~~~~" << endl;
	record << "9.10 entity参与交易数" << endl; cout << "   9.10 entity参与交易数" << endl;
	for (unit8 i = 0; i < 25; i++)
	{
		mysql_query(&conn, TxcntOfEntity[i].c_str());
		result = mysql_store_result(&conn);
		if (row = mysql_fetch_row(result))
		{
			memset(result_c, 0, 500);
			//result_s.clear();

			strcpy(result_c, row[0]);
			result_s = result_c;
			record << result_s << '\t';
		}
		mysql_free_result(result);
	}
	record << endl;

	// 交易金额分布
	string OutvalOfTx[48];

	OutvalOfTx[0] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=0*100000000 AND outvalue<1*100000000;";
	OutvalOfTx[1] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=1*100000000 AND outvalue<2*100000000;";
	OutvalOfTx[2] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=2*100000000 AND outvalue<3*100000000;";
	OutvalOfTx[3] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=3*100000000 AND outvalue<4*100000000;";
	OutvalOfTx[4] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=4*100000000 AND outvalue<5*100000000;";
	OutvalOfTx[5] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=5*100000000 AND outvalue<6*100000000;";
	OutvalOfTx[6] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=6*100000000 AND outvalue<7*100000000;";
	OutvalOfTx[7] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=7*100000000 AND outvalue<8*100000000;";
	OutvalOfTx[8] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=8*100000000 AND outvalue<9*100000000;";
	OutvalOfTx[9] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=9*100000000 AND outvalue<10*100000000;";
	OutvalOfTx[10] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=10*100000000 AND outvalue<20*100000000;";
	OutvalOfTx[11] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=20*100000000 AND outvalue<30*100000000;";
	OutvalOfTx[12] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=30*100000000 AND outvalue<40*100000000;";
	OutvalOfTx[13] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=40*100000000 AND outvalue<50*100000000;";
	OutvalOfTx[14] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=50*100000000 AND outvalue<60*100000000;";
	OutvalOfTx[15] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=60*100000000 AND outvalue<70*100000000;";
	OutvalOfTx[16] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=70*100000000 AND outvalue<80*100000000;";
	OutvalOfTx[17] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=80*100000000 AND outvalue<90*100000000;";
	OutvalOfTx[18] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=90*100000000 AND outvalue<100*100000000;";
	OutvalOfTx[19] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=100*100000000 AND outvalue<1000*100000000;";
	OutvalOfTx[20] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=1000*100000000 AND outvalue<2000*100000000;";
	OutvalOfTx[21] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=2000*100000000 AND outvalue<3000*100000000;";
	OutvalOfTx[22] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=3000*100000000 AND outvalue<4000*100000000;";
	OutvalOfTx[23] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=4000*100000000 AND outvalue<5000*100000000;";
	OutvalOfTx[24] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=5000*100000000 AND outvalue<6000*100000000;";
	OutvalOfTx[25] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=6000*100000000 AND outvalue<7000*100000000;";
	OutvalOfTx[26] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=7000*100000000 AND outvalue<8000*100000000;";
	OutvalOfTx[27] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=8000*100000000 AND outvalue<9000*100000000;";
	OutvalOfTx[28] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=9000*100000000 AND outvalue<10000*100000000;";
	OutvalOfTx[29] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=10000*100000000 AND outvalue<20000*100000000;";
	OutvalOfTx[30] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=20000*100000000 AND outvalue<30000*100000000;";
	OutvalOfTx[31] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=30000*100000000 AND outvalue<40000*100000000;";
	OutvalOfTx[32] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=40000*100000000 AND outvalue<50000*100000000;";
	OutvalOfTx[33] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=50000*100000000 AND outvalue<60000*100000000;";
	OutvalOfTx[34] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=60000*100000000 AND outvalue<70000*100000000;";
	OutvalOfTx[35] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=70000*100000000 AND outvalue<80000*100000000;";
	OutvalOfTx[36] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=80000*100000000 AND outvalue<90000*100000000;";
	OutvalOfTx[37] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=90000*100000000 AND outvalue<100000*100000000;";
	OutvalOfTx[38] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=100000*100000000 AND outvalue<200000*100000000;";
	OutvalOfTx[39] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=200000*100000000 AND outvalue<300000*100000000;";
	OutvalOfTx[40] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=300000*100000000 AND outvalue<400000*100000000;";
	OutvalOfTx[41] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=400000*100000000 AND outvalue<500000*100000000;";
	OutvalOfTx[42] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=500000*100000000 AND outvalue<600000*100000000;";
	OutvalOfTx[43] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=600000*100000000 AND outvalue<700000*100000000;";
	OutvalOfTx[44] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=700000*100000000 AND outvalue<800000*100000000;";
	OutvalOfTx[45] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=800000*100000000 AND outvalue<900000*100000000;";
	OutvalOfTx[46] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=900000*100000000 AND outvalue<1000000*100000000;";
	OutvalOfTx[47] = "SELECT count(*) FROM txinfo WHERE unix_timestamp(txtime) > unix_timestamp('" + after + "') AND outvalue>=1000000*100000000;";

	record << "~~~~~~~~" << endl;
	record << "交易金额分布" << endl; cout << "   9.3 address收入分布" << endl;
	for (unit8 i = 0; i < 48; i++)
	{
		mysql_query(&conn, OutvalOfTx[i].c_str());
		result = mysql_store_result(&conn);
		if (row = mysql_fetch_row(result))
		{
			memset(result_c, 0, 500);
			//result_s.clear();

			strcpy(result_c, row[0]);
			result_s = result_c;
			record << result_s << '\t';
		}
		mysql_free_result(result);
	}
	record << endl;

	// 交易费用分布
	string TxfeeOfTX[58];

	TxfeeOfTX[0] = "SELECT count(*) FROM txinfo WHERE maxbal>=0*100000000 AND maxbal<0.01*100000000;";
	TxfeeOfTX[1] = "SELECT count(*) FROM txinfo WHERE maxbal>=0.01*100000000 AND maxbal<0.1*100000000;";
	TxfeeOfTX[2] = "SELECT count(*) FROM txinfo WHERE maxbal>=0.1*100000000 AND maxbal<0.2*100000000;";
	TxfeeOfTX[3] = "SELECT count(*) FROM txinfo WHERE maxbal>=0.2*100000000 AND maxbal<0.3*100000000;";
	TxfeeOfTX[4] = "SELECT count(*) FROM txinfo WHERE maxbal>=0.3*100000000 AND maxbal<0.4*100000000;";
	TxfeeOfTX[5] = "SELECT count(*) FROM txinfo WHERE maxbal>=0.4*100000000 AND maxbal<0.5*100000000;";
	TxfeeOfTX[6] = "SELECT count(*) FROM txinfo WHERE maxbal>=0.5*100000000 AND maxbal<0.6*100000000;";
	TxfeeOfTX[7] = "SELECT count(*) FROM txinfo WHERE maxbal>=0.6*100000000 AND maxbal<0.7*100000000;";
	TxfeeOfTX[8] = "SELECT count(*) FROM txinfo WHERE maxbal>=0.7*100000000 AND maxbal<0.8*100000000;";
	TxfeeOfTX[9] = "SELECT count(*) FROM txinfo WHERE maxbal>=0.8*100000000 AND maxbal<0.9*100000000;";
	TxfeeOfTX[10] = "SELECT count(*) FROM txinfo WHERE maxbal>=0.9*100000000 AND maxbal<1*100000000;";
	TxfeeOfTX[11] = "SELECT count(*) FROM txinfo WHERE maxbal>=1*100000000 AND maxbal<2*100000000;";
	TxfeeOfTX[12] = "SELECT count(*) FROM txinfo WHERE maxbal>=2*100000000 AND maxbal<3*100000000;";
	TxfeeOfTX[13] = "SELECT count(*) FROM txinfo WHERE maxbal>=3*100000000 AND maxbal<4*100000000;";
	TxfeeOfTX[14] = "SELECT count(*) FROM txinfo WHERE maxbal>=4*100000000 AND maxbal<5*100000000;";
	TxfeeOfTX[15] = "SELECT count(*) FROM txinfo WHERE maxbal>=5*100000000 AND maxbal<6*100000000;";
	TxfeeOfTX[16] = "SELECT count(*) FROM txinfo WHERE maxbal>=6*100000000 AND maxbal<7*100000000;";
	TxfeeOfTX[17] = "SELECT count(*) FROM txinfo WHERE maxbal>=7*100000000 AND maxbal<8*100000000;";
	TxfeeOfTX[18] = "SELECT count(*) FROM txinfo WHERE maxbal>=8*100000000 AND maxbal<9*100000000;";
	TxfeeOfTX[19] = "SELECT count(*) FROM txinfo WHERE maxbal>=9*100000000 AND maxbal<10*100000000;";
	TxfeeOfTX[20] = "SELECT count(*) FROM txinfo WHERE maxbal>=10*100000000 AND maxbal<20*100000000;";
	TxfeeOfTX[21] = "SELECT count(*) FROM txinfo WHERE maxbal>=20*100000000 AND maxbal<30*100000000;";
	TxfeeOfTX[22] = "SELECT count(*) FROM txinfo WHERE maxbal>=30*100000000 AND maxbal<40*100000000;";
	TxfeeOfTX[23] = "SELECT count(*) FROM txinfo WHERE maxbal>=40*100000000 AND maxbal<50*100000000;";
	TxfeeOfTX[24] = "SELECT count(*) FROM txinfo WHERE maxbal>=50*100000000 AND maxbal<60*100000000;";
	TxfeeOfTX[25] = "SELECT count(*) FROM txinfo WHERE maxbal>=60*100000000 AND maxbal<70*100000000;";
	TxfeeOfTX[26] = "SELECT count(*) FROM txinfo WHERE maxbal>=70*100000000 AND maxbal<80*100000000;";
	TxfeeOfTX[27] = "SELECT count(*) FROM txinfo WHERE maxbal>=80*100000000 AND maxbal<90*100000000;";
	TxfeeOfTX[28] = "SELECT count(*) FROM txinfo WHERE maxbal>=90*100000000 AND maxbal<100*100000000;";
	TxfeeOfTX[29] = "SELECT count(*) FROM txinfo WHERE maxbal>=100*100000000 AND maxbal<1000*100000000;";
	TxfeeOfTX[30] = "SELECT count(*) FROM txinfo WHERE maxbal>=1000*100000000 AND maxbal<2000*100000000;";
	TxfeeOfTX[31] = "SELECT count(*) FROM txinfo WHERE maxbal>=2000*100000000 AND maxbal<3000*100000000;";
	TxfeeOfTX[32] = "SELECT count(*) FROM txinfo WHERE maxbal>=3000*100000000 AND maxbal<4000*100000000;";
	TxfeeOfTX[33] = "SELECT count(*) FROM txinfo WHERE maxbal>=4000*100000000 AND maxbal<5000*100000000;";
	TxfeeOfTX[34] = "SELECT count(*) FROM txinfo WHERE maxbal>=5000*100000000 AND maxbal<6000*100000000;";
	TxfeeOfTX[35] = "SELECT count(*) FROM txinfo WHERE maxbal>=6000*100000000 AND maxbal<7000*100000000;";
	TxfeeOfTX[36] = "SELECT count(*) FROM txinfo WHERE maxbal>=7000*100000000 AND maxbal<8000*100000000;";
	TxfeeOfTX[37] = "SELECT count(*) FROM txinfo WHERE maxbal>=8000*100000000 AND maxbal<9000*100000000;";
	TxfeeOfTX[38] = "SELECT count(*) FROM txinfo WHERE maxbal>=9000*100000000 AND maxbal<10000*100000000;";
	TxfeeOfTX[39] = "SELECT count(*) FROM txinfo WHERE maxbal>=10000*100000000 AND maxbal<20000*100000000;";
	TxfeeOfTX[40] = "SELECT count(*) FROM txinfo WHERE maxbal>=20000*100000000 AND maxbal<30000*100000000;";
	TxfeeOfTX[41] = "SELECT count(*) FROM txinfo WHERE maxbal>=30000*100000000 AND maxbal<40000*100000000;";
	TxfeeOfTX[42] = "SELECT count(*) FROM txinfo WHERE maxbal>=40000*100000000 AND maxbal<50000*100000000;";
	TxfeeOfTX[43] = "SELECT count(*) FROM txinfo WHERE maxbal>=50000*100000000 AND maxbal<60000*100000000;";
	TxfeeOfTX[44] = "SELECT count(*) FROM txinfo WHERE maxbal>=60000*100000000 AND maxbal<70000*100000000;";
	TxfeeOfTX[45] = "SELECT count(*) FROM txinfo WHERE maxbal>=70000*100000000 AND maxbal<80000*100000000;";
	TxfeeOfTX[46] = "SELECT count(*) FROM txinfo WHERE maxbal>=80000*100000000 AND maxbal<90000*100000000;";
	TxfeeOfTX[47] = "SELECT count(*) FROM txinfo WHERE maxbal>=90000*100000000 AND maxbal<100000*100000000;";
	TxfeeOfTX[48] = "SELECT count(*) FROM txinfo WHERE maxbal>=100000*100000000 AND maxbal<200000*100000000;";
	TxfeeOfTX[49] = "SELECT count(*) FROM txinfo WHERE maxbal>=200000*100000000 AND maxbal<300000*100000000;";
	TxfeeOfTX[50] = "SELECT count(*) FROM txinfo WHERE maxbal>=300000*100000000 AND maxbal<400000*100000000;";
	TxfeeOfTX[51] = "SELECT count(*) FROM txinfo WHERE maxbal>=400000*100000000 AND maxbal<500000*100000000;";
	TxfeeOfTX[52] = "SELECT count(*) FROM txinfo WHERE maxbal>=500000*100000000 AND maxbal<600000*100000000;";
	TxfeeOfTX[53] = "SELECT count(*) FROM txinfo WHERE maxbal>=600000*100000000 AND maxbal<700000*100000000;";
	TxfeeOfTX[54] = "SELECT count(*) FROM txinfo WHERE maxbal>=700000*100000000 AND maxbal<800000*100000000;";
	TxfeeOfTX[55] = "SELECT count(*) FROM txinfo WHERE maxbal>=800000*100000000 AND maxbal<900000*100000000;";
	TxfeeOfTX[56] = "SELECT count(*) FROM txinfo WHERE maxbal>=900000*100000000 AND maxbal<1000000*100000000;";
	TxfeeOfTX[57] = "SELECT count(*) FROM txinfo WHERE maxbal>=1000000*100000000;";

	record << "~~~~~~~~" << endl;
	record << "9.7 address历史最大余额分布" << endl; cout << "   9.7 address历史最大余额分布" << endl;
	for (unit8 i = 0; i < 58; i++)
	{
		mysql_query(&conn, MaxbalOfAdd[i].c_str());
		result = mysql_store_result(&conn);
		if (row = mysql_fetch_row(result))
		{
			memset(result_c, 0, 500);
			//result_s.clear();

			strcpy(result_c, row[0]);
			result_s = result_c;
			record << result_s << '\t';
		}
		mysql_free_result(result);
	}
	record << endl;

	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/

	/*关于address生存时间的统计*/
	/*
	string address[13];
	address[0] = "AND livetm = 0;";
	address[1] = "AND livetm > 0*86400 AND livetm <1*86400;";
	address[2] = "AND livetm >= 1*86400 AND livetm <2*86400;";
	address[3] = "AND livetm >= 2*86400 AND livetm <4*86400;";
	address[4] = "AND livetm >= 4*86400 AND livetm <8*86400;";
	address[5] = "AND livetm >= 8*86400 AND livetm <16*86400;";
	address[6] = "AND livetm >= 16*86400 AND livetm <32*86400;";
	address[7] = "AND livetm >= 32*86400 AND livetm <64*86400;";
	address[8] = "AND livetm >= 64*86400 AND livetm <128*86400;";
	address[9] = "AND livetm >= 128*86400 AND livetm <256*86400;";
	address[10] = "AND livetm >= 256*86400 AND livetm <512*86400;";
	address[11] = "AND livetm >= 512*86400 AND livetm <1024*86400;";
	address[12] = "AND livetm >= 1024*86400;";
	string address_time;
	record.open("I:\\Statistic2\\address-live-time.txt", ios::app);//统计结果记录
	for (unit8 i = 0; i < 71; i++)
	{
	after = TIMERANGE[i];
	before = TIMERANGE[i + 1];
	address_time = "SELECT count(*) FROM test1.addressinfo WHERE unix_timestamp(firsttm) >= unix_timestamp('"\
	+ after + "') AND unix_timestamp(firsttm) < unix_timestamp('"\
	+ before + "') ";
	for (unit8 j = 0; j < 13; j++)
	{
	get = address_time + address[j];
	mysql_query(&conn, get.c_str());
	result = mysql_store_result(&conn);
	if (row = mysql_fetch_row(result))
	{
	memset(result_c, 0, 500);
	//result_s.clear();

	strcpy(result_c, row[0]);
	result_s = result_c;
	record << result_s << '\t';
	}
	mysql_free_result(result);
	}
	record << endl;
	}
	record.close();

	//关于entity生存时间的统计
	string entity[13];
	entity[0] = "AND livetm = 0;";
	entity[1] = "AND livetm > 0*86400 AND livetm <1*86400;";
	entity[2] = "AND livetm >= 1*86400 AND livetm <2*86400;";
	entity[3] = "AND livetm >= 2*86400 AND livetm <4*86400;";
	entity[4] = "AND livetm >= 4*86400 AND livetm <8*86400;";
	entity[5] = "AND livetm >= 8*86400 AND livetm <16*86400;";
	entity[6] = "AND livetm >= 16*86400 AND livetm <32*86400;";
	entity[7] = "AND livetm >= 32*86400 AND livetm <64*86400;";
	entity[8] = "AND livetm >= 64*86400 AND livetm <128*86400;";
	entity[9] = "AND livetm >= 128*86400 AND livetm <256*86400;";
	entity[10] = "AND livetm >= 256*86400 AND livetm <512*86400;";
	entity[11] = "AND livetm >= 512*86400 AND livetm <1024*86400;";
	entity[12] = "AND livetm >= 1024*86400;";
	string entity_time;
	record.open("I:\\Statistic2\\entity-live-time.txt", ios::app);//统计结果记录
	for (unit8 i = 0; i < 71; i++)
	{
	after = TIMERANGE[i];
	before = TIMERANGE[i + 1];
	entity_time = "SELECT count(*) FROM test1.entityinfo WHERE unix_timestamp(firsttm) >= unix_timestamp('"\
	+ after + "') AND unix_timestamp(firsttm) < unix_timestamp('"\
	+ before + "') ";
	for (unit8 j = 0; j < 13; j++)
	{
	get = entity_time + entity[j];
	mysql_query(&conn, get.c_str());
	result = mysql_store_result(&conn);
	if (row = mysql_fetch_row(result))
	{
	memset(result_c, 0, 500);
	//result_s.clear();

	strcpy(result_c, row[0]);
	result_s = result_c;
	record << result_s << '\t';
	}
	mysql_free_result(result);
	}
	record << endl;
	}
	record.close();
	*/
	//关于transaction的统计
	string tx_time;// $time$
	//交易金额
	string tx_amount[36];
	tx_amount[0] = "AND outvalue>=0*100000000 AND outvalue<10*100000000;";
	tx_amount[1] = "AND outvalue>=10*100000000 AND outvalue<20*100000000;";
	tx_amount[2] = "AND outvalue>=20*100000000 AND outvalue<30*100000000;";
	tx_amount[3] = "AND outvalue>=30*100000000 AND outvalue<40*100000000;";
	tx_amount[4] = "AND outvalue>=40*100000000 AND outvalue<50*100000000;";
	tx_amount[5] = "AND outvalue>=50*100000000 AND outvalue<60*100000000;";
	tx_amount[6] = "AND outvalue>=60*100000000 AND outvalue<70*100000000;";
	tx_amount[7] = "AND outvalue>=70*100000000 AND outvalue<80*100000000;";
	tx_amount[8] = "AND outvalue>=80*100000000 AND outvalue<90*100000000;";
	tx_amount[9] = "AND outvalue>=90*100000000 AND outvalue<100*100000000;";
	tx_amount[10] = "AND outvalue>=100*100000000 AND outvalue<120*100000000;";
	tx_amount[11] = "AND outvalue>=120*100000000 AND outvalue<140*100000000;";
	tx_amount[12] = "AND outvalue>=140*100000000 AND outvalue<160*100000000;";
	tx_amount[13] = "AND outvalue>=160*100000000 AND outvalue<180*100000000;";
	tx_amount[14] = "AND outvalue>=180*100000000 AND outvalue<200*100000000;";
	tx_amount[15] = "AND outvalue>=200*100000000 AND outvalue<220*100000000;";
	tx_amount[16] = "AND outvalue>=220*100000000 AND outvalue<240*100000000;";
	tx_amount[17] = "AND outvalue>=240*100000000 AND outvalue<260*100000000;";
	tx_amount[18] = "AND outvalue>=260*100000000 AND outvalue<280*100000000;";
	tx_amount[19] = "AND outvalue>=280*100000000 AND outvalue<300*100000000;";
	tx_amount[20] = "AND outvalue>=300*100000000 AND outvalue<320*100000000;";
	tx_amount[21] = "AND outvalue>=320*100000000;";

	tx_amount[22] = "AND outvalue>=320*100000000 AND outvalue<340*100000000;";
	tx_amount[23] = "AND outvalue>=340*100000000 AND outvalue<360*100000000;";
	tx_amount[24] = "AND outvalue>=360*100000000 AND outvalue<380*100000000;";
	tx_amount[25] = "AND outvalue>=380*100000000 AND outvalue<400*100000000;";
	tx_amount[26] = "AND outvalue>=420*100000000 AND outvalue<440*100000000;";
	tx_amount[27] = "AND outvalue>=440*100000000 AND outvalue<460*100000000;";
	tx_amount[28] = "AND outvalue>=460*100000000 AND outvalue<480*100000000;";
	tx_amount[29] = "AND outvalue>=480*100000000 AND outvalue<500*100000000;";
	tx_amount[30] = "AND outvalue>=500*100000000 AND outvalue<520*100000000;";
	tx_amount[31] = "AND outvalue>=520*100000000 AND outvalue<540*100000000;";
	tx_amount[32] = "AND outvalue>=540*100000000 AND outvalue<560*100000000;";
	tx_amount[33] = "AND outvalue>=560*100000000 AND outvalue<580*100000000;";
	tx_amount[34] = "AND outvalue>=580*100000000 AND outvalue<600*100000000;";
	tx_amount[35] = "AND outvalue>=600*100000000;";

	record.open("I:\\Statistic2\\tx-amount-2.txt", ios::app);//统计结果记录
	for (unit8 i = 0; i < 71; i++)
	{
		tx_time = "SELECT count(*) FROM test1.txinfo WHERE unix_timestamp(txtime) >= unix_timestamp('"\
			+ after + "') AND unix_timestamp(txtime) < unix_timestamp('"\
			+ before + "') ";
		for (unit8 j = 22; j < 36; j++)
		{
			get = tx_time + tx_amount[j];
			mysql_query(&conn, get.c_str());
			result = mysql_store_result(&conn);
			if (row = mysql_fetch_row(result))
			{
				memset(result_c, 0, 500);
				//result_s.clear();

				strcpy(result_c, row[0]);
				result_s = result_c;
				record << result_s << '\t';
			}
			mysql_free_result(result);
		}
		record << endl;
	}
	record.close();

	//交易文件大小
	string tx_size[33];
	tx_size[0] = "AND txsize>=0 AND txsize<128;";
	tx_size[1] = "AND txsize>=128 AND txsize<256;";
	tx_size[2] = "AND txsize>=256 AND txsize<384;";
	tx_size[3] = "AND txsize>=384 AND txsize<512;";
	tx_size[4] = "AND txsize>=512 AND txsize<640;";
	tx_size[5] = "AND txsize>=640 AND txsize<768;";
	tx_size[6] = "AND txsize>=768 AND txsize<896;";
	tx_size[7] = "AND txsize>=896 AND txsize<1024;";
	tx_size[8] = "AND txsize>=1024 AND txsize<1152;";
	tx_size[9] = "AND txsize>=1152 AND txsize<1280;";
	tx_size[10] = "AND txsize>=1280 AND txsize<1408;";
	tx_size[11] = "AND txsize>=1408 AND txsize<1536;";
	tx_size[12] = "AND txsize>=1536 AND txsize<1664;";
	tx_size[13] = "AND txsize>=1664 AND txsize<1792;";
	tx_size[14] = "AND txsize>=1792 AND txsize<1920;";
	tx_size[15] = "AND txsize>=1920 AND txsize<2048;";
	tx_size[16] = "AND txsize>=2048 AND txsize<2176;";
	tx_size[17] = "AND txsize>=2176 AND txsize<2304;";
	tx_size[18] = "AND txsize>=2304 AND txsize<2432;";
	tx_size[19] = "AND txsize>=2432 AND txsize<2560;";
	tx_size[20] = "AND txsize>=2560 AND txsize<2688;";
	tx_size[21] = "AND txsize>=2688 AND txsize<2816;";
	tx_size[22] = "AND txsize>=2816 AND txsize<2944;";
	tx_size[23] = "AND txsize>=2944 AND txsize<3072;";
	tx_size[24] = "AND txsize>=3072 AND txsize<3200;";
	tx_size[25] = "AND txsize>=3200 AND txsize<3328;";
	tx_size[26] = "AND txsize>=3328 AND txsize<3456;";
	tx_size[27] = "AND txsize>=3456 AND txsize<3584;";
	tx_size[28] = "AND txsize>=3584 AND txsize<3712;";
	tx_size[29] = "AND txsize>=3712 AND txsize<3840;";
	tx_size[30] = "AND txsize>=3840 AND txsize<3968;";
	tx_size[31] = "AND txsize>=3968 AND txsize<4096;";
	tx_size[32] = "AND txsize>=4096;";

	record.open("I:\\Statistic2\\tx-size-2.txt", ios::app);//统计结果记录
	for (unit8 i = 0; i < 71; i++)
	{
		tx_time = "SELECT count(*) FROM test1.txinfo WHERE unix_timestamp(txtime) >= unix_timestamp('"\
			+ after + "') AND unix_timestamp(txtime) < unix_timestamp('"\
			+ before + "') ";
		for (unit8 j = 0; j < 33; j++)
		{
			get = tx_time + tx_size[j];
			mysql_query(&conn, get.c_str());
			result = mysql_store_result(&conn);
			if (row = mysql_fetch_row(result))
			{
				memset(result_c, 0, 500);
				//result_s.clear();

				strcpy(result_c, row[0]);
				result_s = result_c;
				record << result_s << '\t';
			}
			mysql_free_result(result);
		}
		record << endl;
	}
	record.close();
	/*
	//交易费用
	string tx_fee[17];
	tx_fee[0] = "AND txfee>=1000 AND txfee<5000;";
	tx_fee[1] = "AND txfee>=5000 AND txfee<10000;";
	tx_fee[2] = "AND txfee>=10000 AND txfee<20000;";
	tx_fee[3] = "AND txfee>=20000 AND txfee<40000;";
	tx_fee[4] = "AND txfee>=40000 AND txfee<80000;";
	tx_fee[5] = "AND txfee>=80000 AND txfee<100000;";
	tx_fee[6] = "AND txfee>=100000 AND txfee<140000;";
	tx_fee[7] = "AND txfee>=140000 AND txfee<180000;";
	tx_fee[8] = "AND txfee>=180000 AND txfee<220000;";
	tx_fee[9] = "AND txfee>=220000 AND txfee<260000;";
	tx_fee[10] = "AND txfee>=260000 AND txfee<300000;";
	tx_fee[11] = "AND txfee>=300000 AND txfee<340000;";
	tx_fee[12] = "AND txfee>=340000 AND txfee<380000;";
	tx_fee[13] = "AND txfee>=380000 AND txfee<420000;";
	tx_fee[14] = "AND txfee>=420000 AND txfee<460000;";
	tx_fee[15] = "AND txfee>=460000 AND txfee<500000;";
	tx_fee[16] = "AND txfee>=500000;";

	record.open("I:\\Statistic2\\tx-fee.txt", ios::app);//统计结果记录
	for (unit8 i = 0; i < 71; i++)
	{
	after = TIMERANGE[i];
	before = TIMERANGE[i + 1];
	tx_time = "SELECT count(*) FROM test1.txinfo WHERE unix_timestamp(txtime) >= unix_timestamp('"\
	+ after + "') AND unix_timestamp(txtime) < unix_timestamp('"\
	+ before + "') ";
	for (unit8 j = 0; j < 17; j++)
	{
	get = tx_time + tx_fee[j];
	mysql_query(&conn, get.c_str());
	result = mysql_store_result(&conn);
	if (row = mysql_fetch_row(result))
	{
	memset(result_c, 0, 500);
	//result_s.clear();

	strcpy(result_c, row[0]);
	result_s = result_c;
	record << result_s << '\t';
	}
	mysql_free_result(result);
	}
	record << endl;
	}
	record.close();

	//输入地址数量
	string tx_incnt[12];
	tx_incnt[0] = "AND incnt>=1 AND incnt<2;";
	tx_incnt[1] = "AND incnt>=2 AND incnt<3;";
	tx_incnt[2] = "AND incnt>=3 AND incnt<4;";
	tx_incnt[3] = "AND incnt>=4 AND incnt<5;";
	tx_incnt[4] = "AND incnt>=5 AND incnt<6;";
	tx_incnt[5] = "AND incnt>=6 AND incnt<7;";
	tx_incnt[6] = "AND incnt>=7 AND incnt<8;";
	tx_incnt[7] = "AND incnt>=8 AND incnt<9;";
	tx_incnt[8] = "AND incnt>=9 AND incnt<10;";
	tx_incnt[9] = "AND incnt>=10 AND incnt<15;";
	tx_incnt[10] = "AND incnt>=15 AND incnt<20;";
	tx_incnt[11] = "AND incnt>=20;";

	record.open("I:\\Statistic2\\tx-incnt.txt", ios::app);//统计结果记录
	for (unit8 i = 0; i < 71; i++)
	{
	after = TIMERANGE[i];
	before = TIMERANGE[i + 1];
	tx_time = "SELECT count(*) FROM test1.txinfo WHERE unix_timestamp(txtime) >= unix_timestamp('"\
	+ after + "') AND unix_timestamp(txtime) < unix_timestamp('"\
	+ before + "') ";
	for (unit8 j = 0; j < 12; j++)
	{
	get = tx_time + tx_incnt[j];
	mysql_query(&conn, get.c_str());
	result = mysql_store_result(&conn);
	if (row = mysql_fetch_row(result))
	{
	memset(result_c, 0, 500);
	//result_s.clear();

	strcpy(result_c, row[0]);
	result_s = result_c;
	record << result_s << '\t';
	}
	mysql_free_result(result);
	}
	record << endl;
	}
	record.close();

	//输出地址数量
	string tx_outcnt[12];
	tx_outcnt[0] = "AND outcnt>=1 AND outcnt<2;";
	tx_outcnt[1] = "AND outcnt>=2 AND outcnt<3;";
	tx_outcnt[2] = "AND outcnt>=3 AND outcnt<4;";
	tx_outcnt[3] = "AND outcnt>=4 AND outcnt<5;";
	tx_outcnt[4] = "AND outcnt>=5 AND outcnt<6;";
	tx_outcnt[5] = "AND outcnt>=6 AND outcnt<7;";
	tx_outcnt[6] = "AND outcnt>=7 AND outcnt<8;";
	tx_outcnt[7] = "AND outcnt>=8 AND outcnt<9;";
	tx_outcnt[8] = "AND outcnt>=9 AND outcnt<10;";
	tx_outcnt[9] = "AND outcnt>=10 AND outcnt<15;";
	tx_outcnt[10] = "AND outcnt>=15 AND outcnt<20;";
	tx_outcnt[11] = "AND outcnt>=20;";
	record.open("I:\\Statistic2\\tx-outcnt.txt", ios::app);//统计结果记录
	for (unit8 i = 0; i < 71; i++)
	{
	after = TIMERANGE[i];
	before = TIMERANGE[i + 1];
	tx_time = "SELECT count(*) FROM test1.txinfo WHERE unix_timestamp(txtime) >= unix_timestamp('"\
	+ after + "') AND unix_timestamp(txtime) < unix_timestamp('"\
	+ before + "') ";
	for (unit8 j = 0; j < 12; j++)
	{
	get = tx_time + tx_outcnt[j];
	mysql_query(&conn, get.c_str());
	result = mysql_store_result(&conn);
	if (row = mysql_fetch_row(result))
	{
	memset(result_c, 0, 500);
	//result_s.clear();

	strcpy(result_c, row[0]);
	result_s = result_c;
	record << result_s << '\t';
	}
	mysql_free_result(result);
	}
	record << endl;
	}
	record.close();

	//关于block的统计
	string block_time;// $time$
	//包含交易数
	string blk_txcnt[17];
	blk_txcnt[0] = "AND txcnt>=1 AND txcnt<2;";
	blk_txcnt[1] = "AND txcnt>=2 AND txcnt<5;";
	blk_txcnt[2] = "AND txcnt>=5 AND txcnt<10;";
	blk_txcnt[3] = "AND txcnt>=10 AND txcnt<20;";
	blk_txcnt[4] = "AND txcnt>=20 AND txcnt<40;";
	blk_txcnt[5] = "AND txcnt>=40 AND txcnt<80;";
	blk_txcnt[6] = "AND txcnt>=80 AND txcnt<100;";
	blk_txcnt[7] = "AND txcnt>=100 AND txcnt<200;";
	blk_txcnt[8] = "AND txcnt>=200 AND txcnt<300;";
	blk_txcnt[9] = "AND txcnt>=300 AND txcnt<400;";
	blk_txcnt[10] = "AND txcnt>=400 AND txcnt<500;";
	blk_txcnt[11] = "AND txcnt>=500 AND txcnt<600;";
	blk_txcnt[12] = "AND txcnt>=600 AND txcnt<700;";
	blk_txcnt[13] = "AND txcnt>=700 AND txcnt<800;";
	blk_txcnt[14] = "AND txcnt>=800 AND txcnt<900;";
	blk_txcnt[15] = "AND txcnt>=900 AND txcnt<1000;";
	blk_txcnt[16] = "AND txcnt>=1000;";
	record.open("I:\\Statistic2\\blk-txcnt.txt", ios::app);//统计结果记录
	for (unit8 i = 0; i < 71; i++)
	{
	after = TIMERANGE[i];
	before = TIMERANGE[i + 1];
	block_time = "SELECT count(*) FROM test1.blkinfo WHERE unix_timestamp(blktime) >= unix_timestamp('"\
	+ after + "') AND unix_timestamp(blktime) < unix_timestamp('"\
	+ before + "') ";
	for (unit8 j = 0; j < 17; j++)
	{
	get = block_time + blk_txcnt[j];
	mysql_query(&conn, get.c_str());
	result = mysql_store_result(&conn);
	if (row = mysql_fetch_row(result))
	{
	memset(result_c, 0, 500);
	//result_s.clear();

	strcpy(result_c, row[0]);
	result_s = result_c;
	record << result_s << '\t';
	}
	mysql_free_result(result);
	}
	record << endl;
	}
	record.close();

	//块文件大小
	string blk_size[16];
	blk_size[0] = "AND blksize>=185 AND blksize<200;";
	blk_size[1] = "AND blksize>=200 AND blksize<300;";
	blk_size[2] = "AND blksize>=300 AND blksize<400;";
	blk_size[3] = "AND blksize>=400 AND blksize<500;";
	blk_size[4] = "AND blksize>=500 AND blksize<1000;";
	blk_size[5] = "AND blksize>=1000 AND blksize<5000;";
	blk_size[6] = "AND blksize>=5000 AND blksize<10000;";
	blk_size[7] = "AND blksize>=10000 AND blksize<50000;";
	blk_size[8] = "AND blksize>=50000 AND blksize<100000;";
	blk_size[9] = "AND blksize>=100000 AND blksize<150000;";
	blk_size[10] = "AND blksize>=150000 AND blksize<200000;";
	blk_size[11] = "AND blksize>=200000 AND blksize<250000;";
	blk_size[12] = "AND blksize>=250000 AND blksize<300000;";
	blk_size[13] = "AND blksize>=300000 AND blksize<350000;";
	blk_size[14] = "AND blksize>=350000 AND blksize<400000;";
	blk_size[15] = "AND blksize>=400000;";
	record.open("I:\\Statistic2\\blk-size.txt", ios::app);//统计结果记录
	for (unit8 i = 0; i < 71; i++)
	{
	after = TIMERANGE[i];
	before = TIMERANGE[i + 1];
	block_time = "SELECT count(*) FROM test1.blkinfo WHERE unix_timestamp(blktime) >= unix_timestamp('"\
	+ after + "') AND unix_timestamp(blktime) < unix_timestamp('"\
	+ before + "') ";
	for (unit8 j = 0; j < 16; j++)
	{
	get = block_time + blk_size[j];
	mysql_query(&conn, get.c_str());
	result = mysql_store_result(&conn);
	if (row = mysql_fetch_row(result))
	{
	memset(result_c, 0, 500);
	//result_s.clear();

	strcpy(result_c, row[0]);
	result_s = result_c;
	record << result_s << '\t';
	}
	mysql_free_result(result);
	}
	record << endl;
	}
	record.close();

	//交易费用
	string block_time;
	string blk_reward[41];
	blk_reward[0] = "AND reward=0;";
	blk_reward[1] = "AND reward>0 AND reward<500;";
	blk_reward[2] = "AND reward>=500 AND reward<1000;";
	blk_reward[3] = "AND reward>=1000 AND reward<2000;";
	blk_reward[4] = "AND reward>=2000 AND reward<4000;";
	blk_reward[5] = "AND reward>=4000 AND reward<6000;";
	blk_reward[6] = "AND reward>=6000 AND reward<8000;";
	blk_reward[7] = "AND reward>=8000 AND reward<10000;";
	blk_reward[8] = "AND reward>=10000 AND reward<15000;";
	blk_reward[9] = "AND reward>=15000 AND reward<20000;";
	blk_reward[10] = "AND reward>=20000 AND reward<25000;";
	blk_reward[11] = "AND reward>=25000 AND reward<30000;";
	blk_reward[12] = "AND reward>=30000 AND reward<35000;";
	blk_reward[13] = "AND reward>=35000 AND reward<40000;";
	blk_reward[14] = "AND reward>=40000 AND reward<45000;";
	blk_reward[15] = "AND reward>=45000 AND reward<50000;";
	blk_reward[16] = "AND reward>=50000 AND reward<55000;";
	blk_reward[17] = "AND reward>=55000 AND reward<60000;";
	blk_reward[18] = "AND reward>=60000 AND reward<65000;";
	blk_reward[19] = "AND reward>=65000 AND reward<70000;";
	blk_reward[20] = "AND reward>=70000 AND reward<75000;";
	blk_reward[21] = "AND reward>=75000 AND reward<80000;";
	blk_reward[22] = "AND reward>=80000 AND reward<85000;";
	blk_reward[23] = "AND reward>=85000 AND reward<90000;";
	blk_reward[24] = "AND reward>=90000 AND reward<95000;";
	blk_reward[25] = "AND reward>=95000 AND reward<100000;";
	blk_reward[26] = "AND reward>=100000 AND reward<500000;";
	blk_reward[27] = "AND reward>=500000 AND reward<1000000;";
	blk_reward[28] = "AND reward>=1000000 AND reward<2000000;";
	blk_reward[29] = "AND reward>=2000000 AND reward<4000000;";
	blk_reward[30] = "AND reward>=4000000 AND reward<8000000;";
	blk_reward[31] = "AND reward>=8000000 AND reward<10000000;";
	blk_reward[32] = "AND reward>=10000000 AND reward<15000000;";
	blk_reward[33] = "AND reward>=15000000 AND reward<20000000;";
	blk_reward[34] = "AND reward>=20000000 AND reward<25000000;";
	blk_reward[35] = "AND reward>=25000000 AND reward<30000000;";
	blk_reward[36] = "AND reward>=30000000 AND reward<35000000;";
	blk_reward[37] = "AND reward>=35000000 AND reward<40000000;";
	blk_reward[38] = "AND reward>=40000000 AND reward<45000000;";
	blk_reward[39] = "AND reward>=45000000 AND reward<50000000;";
	blk_reward[40] = "AND reward>=50000000;";
	record.open("I:\\Statistic2\\blk-reward.txt", ios::app);//统计结果记录
	for (unit8 i = 0; i < 71; i++)
	{
	after = TIMERANGE[i];
	before = TIMERANGE[i + 1];
	block_time = "SELECT count(*) FROM test1.blkinfo WHERE unix_timestamp(blktime) >= unix_timestamp('"\
	+ after + "') AND unix_timestamp(blktime) < unix_timestamp('"\
	+ before + "') ";
	for (unit8 j = 0; j < 41; j++)
	{
	get = block_time + blk_reward[j];
	mysql_query(&conn, get.c_str());
	result = mysql_store_result(&conn);
	if (row = mysql_fetch_row(result))
	{
	memset(result_c, 0, 500);
	//result_s.clear();

	strcpy(result_c, row[0]);
	result_s = result_c;
	record << result_s << '\t';
	}
	mysql_free_result(result);
	}
	record << endl;
	}
	record.close();

	string add_txcnt[17];
	add_txcnt[0] = "AND (incnt+outcnt)>=1 AND (incnt+outcnt)<2;";
	add_txcnt[1] = "AND (incnt+outcnt)>=2 AND (incnt+outcnt)<4;";
	add_txcnt[2] = "AND (incnt+outcnt)>=4 AND (incnt+outcnt)<6;";
	add_txcnt[3] = "AND (incnt+outcnt)>=6 AND (incnt+outcnt)<8;";
	add_txcnt[4] = "AND (incnt+outcnt)>=8 AND (incnt+outcnt)<10;";
	add_txcnt[5] = "AND (incnt+outcnt)>=10 AND (incnt+outcnt)<50;";
	add_txcnt[6] = "AND (incnt+outcnt)>=50 AND (incnt+outcnt)<100;";
	add_txcnt[7] = "AND (incnt+outcnt)>=100 AND (incnt+outcnt)<200;";
	add_txcnt[8] = "AND (incnt+outcnt)>=200 AND (incnt+outcnt)<300;";
	add_txcnt[9] = "AND (incnt+outcnt)>=300 AND (incnt+outcnt)<400;";
	add_txcnt[10] = "AND (incnt+outcnt)>=400 AND (incnt+outcnt)<500;";
	add_txcnt[11] = "AND (incnt+outcnt)>=500 AND (incnt+outcnt)<600;";
	add_txcnt[12] = "AND (incnt+outcnt)>=600 AND (incnt+outcnt)<700;";
	add_txcnt[13] = "AND (incnt+outcnt)>=700 AND (incnt+outcnt)<800;";
	add_txcnt[14] = "AND (incnt+outcnt)>=800 AND (incnt+outcnt)<900;";
	add_txcnt[15] = "AND (incnt+outcnt)>=900 AND (incnt+outcnt)<1000;";
	add_txcnt[16] = "AND (incnt+outcnt)>=1000;";
	string address_time;
	record.open("I:\\Statistic2\\add-txcnt.txt", ios::app);//统计结果记录
	for (unit8 i = 0; i < 71; i++)
	{
	after = TIMERANGE[i];
	before = TIMERANGE[i + 1];
	address_time = "SELECT count(*) FROM test1.addressinfo WHERE unix_timestamp(firsttm) >= unix_timestamp('"\
	+ after + "') AND unix_timestamp(firsttm) < unix_timestamp('"\
	+ before + "') ";
	for (unit8 j = 0; j < 17; j++)
	{
	get = address_time + add_txcnt[j];
	mysql_query(&conn, get.c_str());
	result = mysql_store_result(&conn);
	if (row = mysql_fetch_row(result))
	{
	memset(result_c, 0, 500);
	//result_s.clear();

	strcpy(result_c, row[0]);
	result_s = result_c;
	record << result_s << '\t';
	}
	mysql_free_result(result);
	}
	record << endl;
	}
	record.close();

	//关于entity生存时间的统计
	string ety_txcnt[27];
	ety_txcnt[0] = "AND (incnt+outcnt)>=1 AND (incnt+outcnt)<2;";
	ety_txcnt[1] = "AND (incnt+outcnt)>=2 AND (incnt+outcnt)<4;";
	ety_txcnt[2] = "AND (incnt+outcnt)>=4 AND (incnt+outcnt)<6;";
	ety_txcnt[3] = "AND (incnt+outcnt)>=6 AND (incnt+outcnt)<8;";
	ety_txcnt[4] = "AND (incnt+outcnt)>=8 AND (incnt+outcnt)<10;";
	ety_txcnt[5] = "AND (incnt+outcnt)>=10 AND (incnt+outcnt)<50;";
	ety_txcnt[6] = "AND (incnt+outcnt)>=50 AND (incnt+outcnt)<100;";
	ety_txcnt[7] = "AND (incnt+outcnt)>=100 AND (incnt+outcnt)<200;";
	ety_txcnt[8] = "AND (incnt+outcnt)>=200 AND (incnt+outcnt)<300;";
	ety_txcnt[9] = "AND (incnt+outcnt)>=300 AND (incnt+outcnt)<400;";
	ety_txcnt[10] = "AND (incnt+outcnt)>=400 AND (incnt+outcnt)<500;";
	ety_txcnt[11] = "AND (incnt+outcnt)>=500 AND (incnt+outcnt)<600;";
	ety_txcnt[12] = "AND (incnt+outcnt)>=600 AND (incnt+outcnt)<700;";
	ety_txcnt[13] = "AND (incnt+outcnt)>=700 AND (incnt+outcnt)<800;";
	ety_txcnt[14] = "AND (incnt+outcnt)>=800 AND (incnt+outcnt)<900;";
	ety_txcnt[15] = "AND (incnt+outcnt)>=900 AND (incnt+outcnt)<1000;";
	ety_txcnt[16] = "AND (incnt+outcnt)>=1000 AND (incnt+outcnt)<1100;";
	ety_txcnt[17] = "AND (incnt+outcnt)>=1100 AND (incnt+outcnt)<1200;";
	ety_txcnt[18] = "AND (incnt+outcnt)>=1200 AND (incnt+outcnt)<1300;";
	ety_txcnt[19] = "AND (incnt+outcnt)>=1300 AND (incnt+outcnt)<1400;";
	ety_txcnt[20] = "AND (incnt+outcnt)>=1400 AND (incnt+outcnt)<1500;";
	ety_txcnt[21] = "AND (incnt+outcnt)>=1500 AND (incnt+outcnt)<1600;";
	ety_txcnt[22] = "AND (incnt+outcnt)>=1600 AND (incnt+outcnt)<1700;";
	ety_txcnt[23] = "AND (incnt+outcnt)>=1700 AND (incnt+outcnt)<1800;";
	ety_txcnt[24] = "AND (incnt+outcnt)>=1800 AND (incnt+outcnt)<1900;";
	ety_txcnt[25] = "AND (incnt+outcnt)>=1900 AND (incnt+outcnt)<2000;";
	ety_txcnt[26] = "AND (incnt+outcnt)>=2000;";
	string entity_time;
	record.open("I:\\Statistic2\\ety-txcnt.txt", ios::app);//统计结果记录
	for (unit8 i = 0; i < 71; i++)
	{
	after = TIMERANGE[i];
	before = TIMERANGE[i + 1];
	entity_time = "SELECT count(*) FROM test1.entityinfo WHERE unix_timestamp(firsttm) >= unix_timestamp('"\
	+ after + "') AND unix_timestamp(firsttm) < unix_timestamp('"\
	+ before + "') ";
	for (unit8 j = 0; j < 27; j++)
	{
	get = entity_time + ety_txcnt[j];
	mysql_query(&conn, get.c_str());
	result = mysql_store_result(&conn);
	if (row = mysql_fetch_row(result))
	{
	memset(result_c, 0, 500);
	//result_s.clear();

	strcpy(result_c, row[0]);
	result_s = result_c;
	record << result_s << '\t';
	}
	mysql_free_result(result);
	}
	record << endl;
	}
	record.close();
	*/

	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/
	//mysql_close(&conn);

	cout << "~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "statistic process OK :)!" << endl;

	//getchar();
	return 1;
}
