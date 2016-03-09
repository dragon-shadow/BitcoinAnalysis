#pragma once
#include "unzipBlocks.h"

using namespace std;

int main()
{
	MYSQL conn;
	leveldb::DB* tx_db;
	start_database(conn, tx_db);//同时连接MySQL数据库和LevelDB数据库
	ofstream logfile; //定义日志文件

	int para = 0;
	for (para = 20; para < 30; para++)
	{
		logfile.open("H:\\1.unzipBlock_logfile.txt", ios::app);//日志文件
		logfile << "------" << endl << "para: " << para << endl << "------" << endl;
		logfile.close();

		set_parameter(para);
		unzipBlocks(conn, tx_db);
		statistical(conn);
	}

	delete tx_db;
	mysql_close(&conn);

	return 0;
}