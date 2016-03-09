#pragma once
#include "unzipBlocks.h"

using namespace std;

int main()
{
	MYSQL conn;
	leveldb::DB* tx_db;
	start_database(conn, tx_db);//ͬʱ����MySQL���ݿ��LevelDB���ݿ�
	ofstream logfile; //������־�ļ�

	int para = 0;
	for (para = 20; para < 30; para++)
	{
		logfile.open("H:\\1.unzipBlock_logfile.txt", ios::app);//��־�ļ�
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