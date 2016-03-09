#pragma once
#include <mysql.h>
#include <leveldb/db.h>
#include <leveldb/cache.h>
#include <leveldb/filter_policy.h>
#include "EntityProcess.h"

using namespace std;



// 用来计算区块难度（为简化，取近似值），来源维基
inline float fast_log(float val)
{
	int * const exp_ptr = reinterpret_cast <int *>(&val);
	int x = *exp_ptr;
	const int log_2 = ((x >> 23) & 255) - 128;
	x &= ~(255 << 23);
	x += 127 << 23;
	*exp_ptr = x;
	val = ((-1.0f / 3) * val + 2) * val - 2.0f / 3;
	return ((val + log_2) * 0.69314718f);
}
// 用来计算区块难度（为简化，取近似值），来源维基
float difficulty(unsigned int bits)
{
	static double max_body = fast_log(0x00ffff), scaland = fast_log(256);
	return exp(max_body - fast_log(bits & 0x00ffffff) + scaland * (0x1d - ((bits & 0xff000000) >> 24)));
}

void getTimeString(char time_p[21], unsigned int ts)
{
	time_t t(ts);
	struct tm *gtm = gmtime(&t);
	strftime(time_p, 20, "%Y-%m-%d %H:%M:%S", gtm);
}

const char user[] = "root";         //username
const char pswd[] = "asd123456";    //password
const char host[] = "localhost";    //or"127.0.0.1"
const char table[] = "test1";       //database
unsigned int port = 3306;           //server port


void start_database(MYSQL &conn, leveldb::DB* &tx_db)
{
	//打开MySQL实例
	mysql_init(&conn);
	mysql_real_connect(&conn, host, user, pswd, table, 0, NULL, 0);
	string unsafemode("SET SQL_SAFE_UPDATES = 0;");
	mysql_query(&conn, unsafemode.c_str());
	
	//打开LevelDB实例
	leveldb::Options options;
	options.create_if_missing = true;
	options.write_buffer_size = 64 * 1024 * 1024; // 128 MB
	options.max_open_files = 128;
	options.block_cache = leveldb::NewLRUCache(256 * 1024 * 1024);  // 100MB cache
	options.filter_policy = leveldb::NewBloomFilterPolicy(4);
	options.compression = leveldb::kNoCompression;

	leveldb::DB::Open(options, "G:/tx_leveldb", &tx_db);
}

int Transaction2MySQL(Transaction &tx, unit64 invalue_sum, unit64 outvalue_sum, string txtime_s, MYSQL &conn)
{
	//cout << mask++ << endl;

	//将本交易的信息存入MySQL
	//统计需要插入MySQL的信息，生成insert语句
	string blknum_s;
	ss.str("");
	ss.clear();
	ss << tx.blocknumber;
	ss >> blknum_s;

	//cout<<"txtime: ";
	//unit32 txtime = 0;
	//char txtime_c[20] = { 0 };
	//getTimeString(txtime_c, tx.txtime);
	//string txtime_s = txtime_c;
	//cout << txtime << endl;

	string txcounter_s;
	ss.str("");
	ss.clear();
	ss << tx.txcounter;
	ss >> txcounter_s;

	string rawlength_s;
	ss.str("");
	ss.clear();
	ss << tx.length;
	ss >> rawlength_s;

	unit8 txhash[32] = { 0 }; // 反转HASH值，符合主流规范
	for (int k = 31, j = 0; k >= 0; --k, ++j)
		txhash[j] = tx.curhash[k];
	
	unit8 txhash_h[65] = { 0 }; // 将16进制hash值
	unit8 *txhash_h_ptr = txhash_h;
	for (int n = 0; n < 32; n++)
	{
		sprintf_s((char*)txhash_h_ptr, 3, "%02x", txhash[n]); // 用法不明
		txhash_h_ptr += 2;
	}
	string txhash_s;
	txhash_s.assign((char*)txhash_h, 64);

	string incounter_s;
	ss.str("");
	ss.clear();
	ss << tx.in_count;
	ss >> incounter_s;

	string outcounter_s;
	ss.str("");
	ss.clear();
	ss << tx.out_count;
	ss >> outcounter_s;

	if (outvalue_sum > invalue_sum)
		invalue_sum = outvalue_sum;

	string invalue_sum_s;
	ss.str("");
	ss.clear();
	ss << invalue_sum;
	ss >> invalue_sum_s;

	string outvalue_sum_s;
	ss.str("");
	ss.clear();
	ss << outvalue_sum;
	ss >> outvalue_sum_s;

	unit64 txfee = invalue_sum - outvalue_sum;
	string txfee_s;
	ss.str("");
	ss.clear();
	ss << txfee;
	ss >> txfee_s;

	//string part1("INSERT INTO txinfo VALUES(x'");
	//string part2("','");
	//string part3("');");
	string insert;
	insert = "INSERT INTO txinfo VALUES(x'"\
		+ txhash_s + "','"\
		+ txcounter_s + "','"\
		+ blknum_s + "','"\
		+ txtime_s + "','"\
		+ rawlength_s + "','"\
		+ txfee_s + "','"\
		+ incounter_s + "','"\
		+ invalue_sum_s + "','"\
		+ outcounter_s + "','"\
		+ outvalue_sum_s + "');";
	//maskfile << insert << endl;
	//cout << insert << endl;
	mysql_query(&conn, insert.c_str());
	//cout << "txinfo插入完成" << endl;

	return 1;
}

string updata;

string coinbase_s("3030303030303030303030303030303030303030"); //coinbase 的虚构地址 

int input_transaction_mysql(unit8 address_b[20], unit64 value, string txtime_s, MYSQL &conn)
{
	unit8 address_h[41] = { 0 };
	unit8 *address_h_ptr = address_h;

	for (int n = 0; n < 20; n++)
	{
		sprintf_s((char*)address_h_ptr, 3, "%02x", address_b[n]); //
		address_h_ptr += 2;
	}
	
	string address_s;
	address_s.assign((char*)address_h, 40);

	if (address_s.compare(coinbase_s) == 0) // 判断是否是挖矿奖励
	{
		return 1;
	}
	else
	{
		string value_s;
		ss.str("");
		ss.clear();
		ss << value;
		ss >> value_s;

		updata = "UPDATE addressinfo SET inval=inval+" + value_s + ",lasttm='" + txtime_s + "',bal=outval-inval,incnt=incnt+1 WHERE address=x'" + address_s + "';";
		//maskfile << updata << endl;
		mysql_query(&conn, updata.c_str());
	}
	return 1;
}

int output_transaction_mysql(unit8 address_b[20], unit64 value, string txtime_s, MYSQL &conn)
{
	unit8 address_h[41] = { 0 };
	unit8 *address_h_ptr = address_h;

	for (int n = 0; n < 20; n++)
	{
		sprintf_s((char*)address_h_ptr, 3, "%02x", address_b[n]); //
		address_h_ptr += 2;
	}

	string address_s;
	address_s.assign((char*)address_h, 40);

	string value_s;
	ss.str("");
	ss.clear();
	ss << value;
	ss >> value_s;

	//updata.clear();
	updata = "INSERT INTO addressinfo(address,outval,firsttm,lasttm,bal) VALUES(x'" + address_s + "','" + value_s + "','" + txtime_s + "','" + txtime_s + "',outval-inval) ON DUPLICATE KEY UPDATE outval=outval+" + value_s + ",lasttm='" + txtime_s + "',bal=outval-inval,outcnt=outcnt+1;";
	//maskfile << updata << endl;
	mysql_query(&conn, updata.c_str());

	//updata.clear();
	updata = "UPDATE addressinfo SET maxbal=bal WHERE bal>maxbal AND address=x'" + address_s + "';";
	//maskfile << updata << endl;
	mysql_query(&conn, updata.c_str());

	return 1;
}


//*********************************************************
//*********************************************************

unit8 coinbase_b[32] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//读取input中hash值对应交易的address和value
void ReadOutput(unit8 txhash[32], unit32 index, unit8 preout[28], leveldb::DB* &tx_db)
{
	if (memcmp(txhash, coinbase_b, 32) == 0) // 如果为系统奖励，设置为全“0”的虚拟地址
	{
		memset(preout, 48, 20); // 虚构coinbase地址全‘0’ 48位ascii码
		unit64 value = 0;
		memcpy((preout + 20), &value, 8);
	}
	else
	{
		string key_s;
		string value_s;

		key_s.assign((char*)txhash, 32);
		tx_db->Get(leveldb::ReadOptions(), key_s, &value_s);
		
		unit32 value_len = value_s.length();
		unit8 *pretx = new unit8[value_len + 1];
		value_s.copy((char*)pretx, value_len);

		unit32 incounter = 0; // 取出incounter
		memcpy(&incounter, (pretx + 16), 4);
		memcpy(preout, (pretx + 20 + incounter * 28 + 4 + index * 28), 28);
		
		delete[] pretx;
	}
}


void TransactionProcess(Transaction &tx, MYSQL &conn, leveldb::DB* &tx_db)
{
	// 当前交易的hash值，首先将hash转换成字符，用来初始化string，作为文件名
	unit8 txhash[32] = { 0 }; // 反转HASH值，符合主流规范
	for (int k = 31, j = 0; k >= 0; --k, ++j)
	{
		txhash[j] = tx.curhash[k];
	}

	/* 计算二进制文件的长度，不包含length本身
	 文件结构
	//binlength	4 （计算长度不包含binlength）
	----------
	blocknumber	4
	txtime		4
	txcounter	4
	length		4
	curhash		32(不存了)
	----------
	incounter	4
	----------	重复
	prehash		20
	index		8
	----------
	outcounter	4
	----------	重复
	address		20
	value		8
	----------
	*/
	/*------------------写入数据库---------------------*/
	//cout << mask++ << endl;//0
	string key_s;
	string value_s;
	unit8 key_c[32] = { 0 };
	unit8 *value_c = NULL;
	int value_c_ptr = 0;

	//unit32 value_len = 4 + 4 + 4 + 4 + 32 + 4 + tx.in_count * (20 + 8) + 4 + tx.out_count * (20 + 8);//修改过
	unit32 value_len = 4 + 4 + 4 + 4 + 4 + tx.in_count * (20 + 8) + 4 + tx.out_count * (20 + 8);//修改过
	value_c = new unit8[value_len + 1];//申请存放交易空间

	memcpy(value_c + value_c_ptr, &tx.blocknumber, 4); value_c_ptr += 4;
	memcpy(value_c + value_c_ptr, &tx.txtime, 4); value_c_ptr += 4;
	memcpy(value_c + value_c_ptr, &tx.txcounter, 4); value_c_ptr += 4;
	memcpy(value_c + value_c_ptr, &tx.length, 4); value_c_ptr += 4;
	
	//memcpy(value_c + value_c_ptr, txhash, 32); value_c_ptr += 32;
	memcpy(key_c, txhash, 32);//hash值作为主键
	
	char txtime_c[21] = { 0 };
	getTimeString(txtime_c, tx.txtime);
	string txtime_s = txtime_c;

	vector<string> input_address_bv;
	vector<string> output_address_bv;

	// input部分
	memcpy(value_c + value_c_ptr, &tx.in_count, 4); value_c_ptr += 4;
	unit8 preout[28] = { 0 };
	unit8 preaddress[20] = { 0 };
	string preaddress_s;
	unit64 invalue = 0;
	unit64 invalue_sum = 0;//统计input值之和
	
	for (unit32 i = 0; i < tx.in_count; ++i)
	{
		for (int k = 31, j = 0; k >= 0; --k, ++j)//翻转hash值
		{
			txhash[j] = tx.ins[i].hash[k];
		}
		
		//cout << mask++ << endl;//1

		ReadOutput(txhash, tx.ins[i].index, preout, tx_db);//直接找出input对应的address和value
		
		memcpy(value_c + value_c_ptr, preout, 28); value_c_ptr += 28;
		
		memcpy(preaddress, preout, 20);
		memcpy(&invalue, preout+20, 8);
		invalue_sum = invalue_sum + invalue;
		
		//cout << mask++ << endl;//2

		input_transaction_mysql(preaddress, invalue, txtime_s, conn);
		
		//cout << mask++ << endl;//3

		preaddress_s.assign((char*)preaddress, 20);
		input_address_bv.push_back(preaddress_s);
	}
	
	//cout << mask++ << endl;//4

	input_entity_mysql(input_address_bv, conn);//处理input部分entity

	// output部分
	memcpy(value_c + value_c_ptr, &tx.out_count, 4); value_c_ptr += 4;
	
	string outputaddress_s;
	unit64 outvalue_sum = 0;//统计input值之和
	
	for (unit32 i = 0; i < tx.out_count; ++i)
	{
		memcpy(value_c + value_c_ptr, tx.outs[i].address, 20); value_c_ptr += 20;

		memcpy(value_c + value_c_ptr, &tx.outs[i].val, 8); value_c_ptr += 8;
		outvalue_sum = outvalue_sum + tx.outs[i].val;
		//cout << mask++ << endl;//6

		//cout << "before output_transaction_mysql" << endl;

		output_transaction_mysql(tx.outs[i].address, tx.outs[i].val, txtime_s, conn);
		
		//cout << "output_transaction_mysql" << endl;

		//cout << mask++ << endl;//7

		outputaddress_s.assign((char*)tx.outs[i].address, 20);
		output_address_bv.push_back(outputaddress_s);
	}
	
	//cout << mask++ << endl;//8
	//cout << "before output_entity_mysql" << endl;
	output_entity_mysql(output_address_bv, entitycnt, conn);//处理output部分entity
	
	//cout << mask++ << endl;

	key_s.assign((char*)key_c, 32);
	value_s.assign((char*)value_c, value_len);
	
	//cout << "before tx_db->Put" << endl;

	tx_db->Put(leveldb::WriteOptions(), key_s, value_s);//完整的交易已经存入LevelDB
	
	//cout << "tx_db->Put" << endl;

	delete[] value_c;

	//**************************************************
	//将本交易的信息存入MySQL
	Transaction2MySQL(tx, invalue_sum, outvalue_sum, txtime_s, conn);
}
