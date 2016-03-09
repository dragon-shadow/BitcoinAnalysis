#pragma once
#include <Windows.h>
#include <mysql.h>
#include <vector>
#include <string>
#include <algorithm>
#include "block.h"


vector<unit32> entitychange_v;//何时清空
unit32 entitycnt;//何时清空

unit8 coinbase_h[20] = { 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30 };

string upent_section;
string getent;
string upent;
string del;

void input_entity_mysql(vector<string> &address_bv, MYSQL &conn)
{
	//cout << "进入input_entity_mysql" << endl;
	MYSQL_RES *result = NULL;
	MYSQL_ROW row;
	
	unit8 address_b[20] = { 0 };
	unit8 address_h[41] = { 0 };
	unit8 *address_h_ptr = NULL;
	string address_s;
	
	//cout << mask++ << endl;//5

	unit32 incount = address_bv.size();
	
	//cout << "incount"<<incount << endl;

	if (incount == 1)
	{
		memcpy(address_b, address_bv[0].c_str(), 20);
		if (memcmp(address_b, coinbase_h, 20) != 0)// 判断是否是挖矿奖励，如果不是挖矿奖励
		{
			address_h_ptr = address_h;
			for (int n = 0; n < 20; n++)
			{
				sprintf_s((char*)address_h_ptr, 3, "%02x", address_b[n]);
				address_h_ptr += 2;
			}
			address_s = (char*)address_h;

			unit8 temp1[4] = { 0 };
			unit8 temp2[4] = { 0 };
			unit32 entity = 0;

			getent = "SELECT entity FROM addressinfo WHERE address = x'" + address_s + "';";
			//cout << getent << endl;
			mysql_query(&conn, getent.c_str());
			result = mysql_store_result(&conn);
			row = mysql_fetch_row(result);

			memcpy(temp1, row[0], 4);
			temp2[3] = temp1[0];
			temp2[2] = temp1[1];
			temp2[1] = temp1[2];
			temp2[0] = temp1[3];
			memcpy(&entity, temp2, 4);

			mysql_free_result(result);

			entitychange_v.push_back(entity);//记录受影响的entity
		}
	}
	else
	{
		unit8 temp1[4] = { 0 };
		unit8 temp2[4] = { 0 };
		unit32 entity = 0;

		vector<unit32> entity_v;

		for (unit32 i = 0; i < incount; i++)
		{
			memcpy(address_b, address_bv[i].c_str(), 20);
			address_h_ptr = address_h;
			for (int n = 0; n < 20; n++)
			{
				sprintf_s((char*)address_h_ptr, 3, "%02x", address_b[n]);
				address_h_ptr += 2;
			}
			address_s = (char*)address_h;

			getent = "SELECT entity FROM addressinfo WHERE address = x'" + address_s + "';";
			//cout << getent << endl;
			mysql_query(&conn, getent.c_str());
			result = mysql_store_result(&conn);
			row = mysql_fetch_row(result);

			memcpy(temp1, row[0], 4);
			temp2[3] = temp1[0];
			temp2[2] = temp1[1];
			temp2[1] = temp1[2];
			temp2[0] = temp1[3];
			memcpy(&entity, temp2, 4);

			mysql_free_result(result);

			entity_v.push_back(entity);
		}
		sort(entity_v.begin(), entity_v.end());

		bool unequal = 0; // 判断是否都属于一个entity
		for (int i = 1; i < entity_v.size(); i++)
		{
			if (entity_v[0] != entity_v[i])
			{
				unequal = 1;
				break;
			}
		}

		if (unequal) // 更新entity代号，都等于entity_v[0]
		{
			unit8 entity_0[9] = { 0 };
			sprintf_s((char*)entity_0, 9, "%08x", entity_v[0]);
			string entity_0_s = (char*)entity_0;
			upent_section = "UPDATE addressinfo SET entity = x'" + entity_0_s + "' WHERE entity = x'";

			unit8 entity_buf[9] = { 0 };
			string entity_buf_s;
			for (unit32 i = 1; i < entity_v.size(); i++)
			{
				if (entity_v[i - 1] != entity_v[i])
				{
					sprintf_s((char*)entity_buf, 9, "%08x", entity_v[i]);
					entity_buf_s = (char*)entity_buf;
					upent = upent_section + entity_buf_s + "';";
					mysql_query(&conn, upent.c_str());

					//删除多余的entity DELETE FROM Persons WHERE LastName='Griffin'
					del = "DELETE FROM entityinfo WHERE entity = x'" + entity_buf_s + "';";
					mysql_query(&conn, del.c_str());
				}
			}
		}
		entitychange_v.push_back(entity_v[0]);//记录受影响的entity
	}	
	address_bv.clear();
}


void insert_entity_mysql(unit32 entitycnt, MYSQL &conn)
{
	string insert;
	string entitycnt_s;
	unit8 entitycnt_c[9] = { 0 };
	
	sprintf_s((char*)entitycnt_c, 9, "%08x", entitycnt);
	//entitycnt_s.clear();
	entitycnt_s = (char*)entitycnt_c;

	//insert.clear();//beta
	insert = "INSERT INTO entityinfo(entity) VALUES(x'" + entitycnt_s + "');";
	//cout << insert << endl;
	mysql_query(&conn, insert.c_str());
}


void output_entity_mysql(vector<string> &address_bv, unit32 &entitycnt, MYSQL &conn)
{
	MYSQL_RES *result = NULL;
	MYSQL_ROW row;

	unit8 temp1[4] = { 0 };
	unit8 temp2[4] = { 0 };
	unit32 entity = 0;
	unit8 entity_buf[9] = { 0 };
	string entity_buf_s;
	
	unit8 address_b[20] = { 0 };
	unit8 address_h[41] = { 0 };
	unit8 *address_h_ptr = NULL;
	string address_s;

	for (unit32 i = 0; i < address_bv.size(); i++)
	{
		memcpy(address_b, address_bv[i].c_str(), 20);
		address_h_ptr = address_h;
		for (int n = 0; n < 20; n++)
		{
			sprintf_s((char*)address_h_ptr, 3, "%02x", address_b[n]);
			address_h_ptr += 2;
		}
		address_s = (char*)address_h;


		getent = "SELECT entity FROM addressinfo where address = x'" + address_s + "';";
		//cout << getent << endl;
		mysql_query(&conn, getent.c_str());

		result = mysql_store_result(&conn);
		row = mysql_fetch_row(result);

		memcpy(temp1, row[0], 4);
		temp2[3] = temp1[0];
		temp2[2] = temp1[1];
		temp2[1] = temp1[2];
		temp2[0] = temp1[3];
		memcpy(&entity, temp2, 4);

		mysql_free_result(result);

		if (0 == entity)
		{
			entitycnt++;
			//unit8 buf[9] = { 0 };//beta
			sprintf_s((char*)entity_buf, 9, "%08x", entitycnt);
			//string buf_s = (char*)buf;
			entity_buf_s = (char*)entity_buf;
			//upent.clear();
			upent = "UPDATE addressinfo SET entity = x'" + entity_buf_s + "' WHERE address = x'" + address_s + "';";
			//cout << upent << endl;
			mysql_query(&conn, upent.c_str());
			
			insert_entity_mysql(entitycnt, conn);//插入新entity

			entitychange_v.push_back(entitycnt);//记录受影响的entity
		}
		else
		{
			entitychange_v.push_back(entitycnt);//记录受影响的entity
		}
	}
	address_bv.clear();
}


// ("UPDATE entityinfo, addressinfo SET entityinfo.addcnt = (SELECT count(*) FROM addressinfo WHERE entityinfo.entity = addressinfo.entity),	entityinfo.inval = (SELECT sum(inval) FROM addressinfo WHERE entityinfo.entity = addressinfo.entity), entityinfo.outval = (SELECT sum(outval) FROM addressinfo WHERE entityinfo.entity = addressinfo.entity), entityinfo.firsttm = (SELECT min(firsttm) FROM addressinfo WHERE entityinfo.entity = addressinfo.entity), entityinfo.lasttm = (SELECT max(lasttm) FROM addressinfo WHERE entityinfo.entity = addressinfo.entity), entityinfo.bal = (SELECT sum(bal) FROM addressinfo WHERE entityinfo.entity = addressinfo.entity), entityinfo.maxbal = (SELECT sum(maxbal) FROM addressinfo WHERE entityinfo.entity = addressinfo.entity), entityinfo.incnt = (SELECT sum(incnt) FROM addressinfo WHERE entityinfo.entity = addressinfo.entity), entityinfo.outcnt = (SELECT sum(outcnt) FROM addressinfo WHERE entityinfo.entity = addressinfo.entity) WHERE entityinfo.entity = addressinfo.entity;");
void update_entity_mysql(vector<unit32> &ent_change_v, MYSQL &conn)
{
	string update;
	sort(ent_change_v.begin(), ent_change_v.end());
	ent_change_v.erase(unique(ent_change_v.begin(), ent_change_v.end()), ent_change_v.end());
	
	unit8 ent_cha[9] = { 0 };
	string ent_cha_s;
	for (unit32 i = 0; i < ent_change_v.size(); i++)
	{
		if (i % 1000 == 0)
		{
			cout << i << endl;// 输出计数器，表示运行情况
		}
		sprintf_s((char*)ent_cha, 9, "%08x", ent_change_v[i]);
		ent_cha_s = (char*)ent_cha;

		update = \
			"UPDATE entityinfo SET addcnt = (SELECT count(*) FROM addressinfo WHERE entity=x'" + ent_cha_s + "'),"\
			+ "inval = (SELECT sum(inval) FROM addressinfo WHERE entity=x'" + ent_cha_s + "'),"\
			+ "outval = (SELECT sum(outval) FROM addressinfo WHERE entity=x'" + ent_cha_s + "'),"\
			+ "firsttm = (SELECT min(firsttm) FROM addressinfo WHERE entity=x'" + ent_cha_s + "'),"\
			+ "lasttm = (SELECT max(lasttm) FROM addressinfo WHERE entity=x'" + ent_cha_s + "'),"\
			+ "bal = (SELECT sum(bal) FROM addressinfo WHERE entity=x'" + ent_cha_s + "'),"\
			+ "maxbal = (SELECT sum(maxbal) FROM addressinfo WHERE entity=x'" + ent_cha_s + "'),"\
			+ "incnt = (SELECT sum(incnt) FROM addressinfo WHERE entity=x'" + ent_cha_s + "'),"\
			+ "outcnt = (SELECT sum(outcnt) FROM addressinfo WHERE entity=x'" + ent_cha_s + "')"\
			+ " WHERE entity=x'" + ent_cha_s + "';";

		mysql_query(&conn, update.c_str());
	}
	
	ent_change_v.clear();
}