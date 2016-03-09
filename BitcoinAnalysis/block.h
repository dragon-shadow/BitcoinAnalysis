#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "hash.h"

#define MAGIC_BLOCK 0xd9b4bef9 //block协议定义的“魔法数字”
#define ONE_BTC 100000000

//int mask = 0;
//ofstream maskfile; //定义日志文件

stringstream ss;

//协议定义的脚本操作符
enum ScriptOpcodes
{
	OP_0 = 0x00,
	OP_PUSHDATA1 = 0x4c,
	OP_PUSHDATA2 = 0x4d,
	OP_PUSHDATA4 = 0x4e,
	OP_1NEGATE = 0x4f,
	OP_RESERVED = 0x50,
	OP_1 = 0x51,
	OP_2 = 0x52,
	OP_3 = 0x53,
	OP_4 = 0x54,
	OP_5 = 0x55,
	OP_6 = 0x56,
	OP_7 = 0x57,
	OP_8 = 0x58,
	OP_9 = 0x59,
	OP_10 = 0x5a,
	OP_11 = 0x5b,
	OP_12 = 0x5c,
	OP_13 = 0x5d,
	OP_14 = 0x5e,
	OP_15 = 0x5f,
	OP_16 = 0x60,
	OP_NOP = 0x61,
	OP_VER = 0x62,
	OP_IF = 0x63,
	OP_NOTIF = 0x64,
	OP_VERIF = 0x65,
	OP_VERNOTIF = 0x66,
	OP_ELSE = 0x67,
	OP_ENDIF = 0x68,
	OP_VERIFY = 0x69,
	OP_RETURN = 0x6a,
	OP_TOALTSTACK = 0x6b,
	OP_FROMALTSTACK = 0x6c,
	OP_2DROP = 0x6d,
	OP_2DUP = 0x6e,
	OP_3DUP = 0x6f,
	OP_2OVER = 0x70,
	OP_2ROT = 0x71,
	OP_2SWAP = 0x72,
	OP_IFDUP = 0x73,
	OP_DEPTH = 0x74,
	OP_DROP = 0x75,
	OP_DUP = 0x76,
	OP_NIP = 0x77,
	OP_OVER = 0x78,
	OP_PICK = 0x79,
	OP_ROLL = 0x7a,
	OP_ROT = 0x7b,
	OP_SWAP = 0x7c,
	OP_TUCK = 0x7d,
	OP_CAT = 0x7e,	// Currently disabled
	OP_SUBSTR = 0x7f,	// Currently disabled
	OP_LEFT = 0x80,	// Currently disabled
	OP_RIGHT = 0x81,	// Currently disabled
	OP_SIZE = 0x82,	// Currently disabled
	OP_INVERT = 0x83,	// Currently disabled
	OP_AND = 0x84,	// Currently disabled
	OP_OR = 0x85,	// Currently disabled
	OP_XOR = 0x86,	// Currently disabled
	OP_EQUAL = 0x87,
	OP_EQUALVERIFY = 0x88,
	OP_RESERVED1 = 0x89,
	OP_RESERVED2 = 0x8a,
	OP_1ADD = 0x8b,
	OP_1SUB = 0x8c,
	OP_2MUL = 0x8d,	// Currently disabled
	OP_2DIV = 0x8e,	// Currently disabled
	OP_NEGATE = 0x8f,
	OP_ABS = 0x90,
	OP_NOT = 0x91,
	OP_0NOTEQUAL = 0x92,
	OP_ADD = 0x93,
	OP_SUB = 0x94,
	OP_MUL = 0x95,	// Currently disabled
	OP_DIV = 0x96,	// Currently disabled
	OP_MOD = 0x97,	// Currently disabled
	OP_LSHIFT = 0x98,	// Currently disabled
	OP_RSHIFT = 0x99,	// Currently disabled
	OP_BOOLAND = 0x9a,
	OP_BOOLOR = 0x9b,
	OP_NUMEQUAL = 0x9c,
	OP_NUMEQUALVERIFY = 0x9d,
	OP_NUMNOTEQUAL = 0x9e,
	OP_LESSTHAN = 0x9f,
	OP_GREATERTHAN = 0xa0,
	OP_LESSTHANOREQUAL = 0xa1,
	OP_GREATERTHANOREQUAL = 0xa2,
	OP_MIN = 0xa3,
	OP_MAX = 0xa4,
	OP_WITHIN = 0xa5,
	OP_RIPEMD160 = 0xa6,
	OP_SHA1 = 0xa7,
	OP_SHA256 = 0xa8,
	OP_HASH160 = 0xa9,
	OP_HASH256 = 0xaa,
	OP_CODESEPARATOR = 0xab,
	OP_CHECKSIG = 0xac,
	OP_CHECKSIGVERIFY = 0xad,
	OP_CHECKMULTISIG = 0xae,
	OP_CHECKMULTISIGVERIFY = 0xaf,
	OP_NOP1 = 0xb0,
	OP_NOP2 = 0xb1,
	OP_NOP3 = 0xb2,
	OP_NOP4 = 0xb3,
	OP_NOP5 = 0xb4,
	OP_NOP6 = 0xb5,
	OP_NOP7 = 0xb6,
	OP_NOP8 = 0xb7,
	OP_NOP9 = 0xb8,
	OP_NOP10 = 0xb9,
	OP_SMALLINTEGER = 0xfa,
	OP_PUBKEYS = 0xfb,
	OP_PUBKEYHASH = 0xfd,
	OP_PUBKEY = 0xfe,
	OP_INVALIDOPCODE = 0xff
};

// Read one byte from the block-chain input stream.
inline unit8 readU8(char* transSec, int &transPos)
{
	unit8 ret = 0;
	memcpy(&ret, transSec + transPos, 1);
	transPos += 1;
	return ret;
}

// Read two bytes from the block-chain input stream.
inline unit16 readU16(char* transSec, int &transPos)
{
	unit16 ret = 0;
	memcpy(&ret, transSec + transPos, 2);
	transPos += 2;
	return ret;
}

// Read four bytes from the block-chain input stream.
inline unit32 readU32(char* transSec, int &transPos)
{
	unit32 ret = 0;
	memcpy(&ret, transSec + transPos, 4);
	transPos += 4;
	return ret;
}

// Read eight bytes from the block-chain input stream.
inline unit64 readU64(char* transSec, int &transPos)
{
	unit64 ret = 0;
	memcpy(&ret, transSec + transPos, 8);
	transPos += 8;
	return ret;
}

// Return the current stream pointer representing a 32byte hash and advance the read pointer accordingly
inline void readHash(unit8 hash[32], char* transSec, int &transPos)
{
	memcpy(hash, transSec + transPos, 32);
	transPos += 32;
}

// reads a variable length integer. 疑似bug，readU32
// See the documentation from here:  https://en.bitcoin.it/wiki/Protocol_specification#Variable_length_integer
inline unit32 readVarLen(char* transSec, int &transPos)
{
	unit32 ret = 0;
	unit8 v = readU8(transSec, transPos);
	if (v < 0xfd)
	{
		ret = (unit32)v;
		return ret;
	}
	else if (v == 0xfd)
	{
		unit16 v = readU16(transSec, transPos);
		ret = (unit32)v;
		return ret;
	}
	else if (v == 0xfe)
	{
		unit32 v = readU32(transSec, transPos);
		ret = (unit32)v;
		return ret;

	}
	else if (v == 0xff)
	{
		//实际没有这种情况
	}
	return false;
}

// Get the current read buffer address and advance the stream buffer by this length;
// used to get the address of input/output scripts
inline unit8* readBufferAdvance(char* transSec, int &transPos, unit32 readLength)
{
	unit8 *ret = new unit8[readLength];
	memcpy(ret, transSec + transPos, readLength);
	transPos += readLength;
	return ret;
}

/*定义Bitcoin Transaction*/
struct Txin // Transaction 的 input 部分
{
	unit8  hash[32]; // preHash上一笔交易的HASH值
	unit32 index; // 上一笔交易中的第几个output
	unit32 script_length; //
	unit8* script;
	unit32 seq;
	// 以下并非原始数据，不直接包含在blockchain中
	unit8  address[20];
};

struct Txout
{
	unit64 val;
	unit32 script_length;
	unit8* script;
	// 以下并非原始数据，不直接包含在blockchain中
	unit8  address[20];
};

struct Transaction
{
	unit32 ver;
	unit32 in_count; //可变长度变量
	std::vector<Txin> ins;
	unit32 out_count; //可变长度变量
	std::vector<Txout> outs;
	unit32 lock_time;
	// 以下并非原始数据，不直接包含在blockchain中
	unit32 blocknumber;
	unit16 txnumber;
	unit32 txcounter;
	unit32 length; // 交易长度
	unit32 txtime; // 记录交易发生时间
	unit8  curhash[32];
};

/*定义Bitcoin Block*/
struct Blkheader
{
	unit32 magic; //标记值
	unit32 Blocksize; //整个block的大小
	unit32 Version;
	unit8  preBlockHash[32]; //上一个block
	unit8  merkRoot[32]; //交易hash值
	unit32 timeStamp;
	unit32 bits; // 难度系数
	unit32 nonce; // 随机数
};

struct Block
{
	Blkheader Head;
	unit32 trans_count;
	std::vector<Transaction> trans;
	// 以下并非原始数据，不直接包含在blockchain中
	unit8  curhash[32]; // The computed block hash
	unit32 blocknumber; // Index of this block, the genesis block is considered zero
	unit64 blockReward;				// Block redward in BTC
	bool   warning;					// there was a warning issued while processing this block.
};

// 计算两次SHA256值
void twiceSHA256(const void *input, unit32 size, unit8 output[32])
{
	unit8 mid_temp[32] = { 0 };
	BLOCKCHAIN_SHA256::computeSHA256(input, size, mid_temp);	// Compute the SHA256 hash of the RIPEMD16 hash + the one byte header (for a checksum)
	BLOCKCHAIN_SHA256::computeSHA256(mid_temp, 32, output); // now compute the SHA256 hash of the previously computed SHA256 hash (for a checksum)
}

bool outAddress(const unit8 *Script, unit32 ScriptLength, unit8 address[20])
{
	bool decodeOK = 0;
	if (ScriptLength == 67 && Script[0] == 65 && Script[66] == OP_CHECKSIG)
	{
		unit8 publicKey[65] = { 0 }; // The 65 bytes long ECDSA public key; first byte will always be 0x4 followed by two 32 byte components
		memcpy(publicKey, Script + 1, 65);
		unit8 AddressAndCheck[25] = { 0 }; // 网络版本号 0 + 地址 20bytes + 校验和 4 bytes 
		BLOCKCHAIN_BITCOIN_ADDRESS::bitcoinPublicKeyToAddress(publicKey, AddressAndCheck);
		memcpy(address, AddressAndCheck + 1, 20); // 只取其中“地址 20bytes”全网唯一
		decodeOK = 1; goto OK; //解析成功
	}
	else if (ScriptLength == 66 && Script[65] == OP_CHECKSIG)
	{
		unit8 publicKey[65] = { 0 }; // The 65 bytes long ECDSA public key; first byte will always be 0x4 followed by two 32 byte components
		memcpy(publicKey, Script, 65);
		unit8 AddressAndCheck[25] = { 0 }; // 网络版本号 0 + 地址 20bytes + 校验和 4 bytes 
		BLOCKCHAIN_BITCOIN_ADDRESS::bitcoinPublicKeyToAddress(publicKey, AddressAndCheck);
		memcpy(address, AddressAndCheck + 1, 20); // 只取其中“地址 20bytes”全网唯一
		decodeOK = 1; goto OK; //解析成功

	}
	else if (ScriptLength >= 25 &&
		Script[0] == OP_DUP &&
		Script[1] == OP_HASH160 &&
		Script[2] == 20)
	{
		memcpy(address, Script + 3, 20); // 直接就是RipeMD160 20byte地址
		decodeOK = 1; goto OK; // 解析成功
	}
	else if (ScriptLength == 23 &&
		Script[0] == OP_HASH160 &&
		Script[1] == 20)
	{
		memcpy(address, Script + 2, 20);;
		decodeOK = 1; goto OK; // 解析成功
	}
	else if (ScriptLength == 23 &&
		Script[0] == 20 &&
		Script[21] == OP_NOP2 &&
		Script[22] == OP_DROP)
	{
		memcpy(address, Script + 1, 20);
		decodeOK = 1; goto OK; // 解析成功
	}
	else if (ScriptLength == 35 &&
		Script[0] == 33 &&
		Script[34] == OP_CHECKSIG)
	{
		BLOCKCHAIN_RIPEMD160::computeRIPEMD160(Script + 1, 33, address);
		decodeOK = 1; goto OK; // 解析成功
	}
	else if (ScriptLength == 33 && Script[0] == 32)
	{
		BLOCKCHAIN_RIPEMD160::computeRIPEMD160(Script + 1, 32, address);
		decodeOK = 1; goto OK; // 解析成功
	}
	else if (ScriptLength == 37 && Script[0] == 36)
	{
		BLOCKCHAIN_RIPEMD160::computeRIPEMD160(Script + 1, 36, address);
		decodeOK = 1; goto OK; // 解析成功
	}
	else if (ScriptLength == 22 && Script[0] == OP_RETURN && Script[1] == 20)
	{
		memcpy(address, Script + 2, 20);
		decodeOK = 1; goto OK; // 解析成功
	}
	else if (ScriptLength > 35 &&
		(Script[0] == OP_1 || Script[0] == OP_2 || Script[0] == OP_3) &&
		Script[ScriptLength - 1] == OP_CHECKMULTISIG)
	{
		BLOCKCHAIN_RIPEMD160::computeRIPEMD160(Script, ScriptLength, address);
		decodeOK = 1; goto OK; // 解析成功
	}
	else if (ScriptLength < 20)
	{
		memset(address, 49, 20); // 空地址，全部置'1'(全'0'为coinbase，系统奖励)，可能bug，记录因为脚本错误而丢失的比特币
		decodeOK = 1; goto OK; //解析成功
	}
	else
	{
		// Ok..we are going to scan for this pattern.. OP_DUP, OP_HASH160, 0x14 then exactly 20 bytes after 0x88,0xAC
		// 25...
		if (ScriptLength > 25)
		{
			unit32 endIndex = ScriptLength - 25;

			for (unit32 i = 0; i < endIndex; i++)
			{
				const unit8 *scan = &Script[i];
				if (scan[0] == OP_DUP &&
					scan[1] == OP_HASH160 &&
					scan[2] == 20 &&
					scan[23] == OP_EQUALVERIFY &&
					scan[24] == OP_CHECKSIG)
				{
					memcpy(address, scan + 3, 20); // 直接就是RipeMD160 20byte地址, 正确脚本从scan+3开始
					decodeOK = 1; goto OK;// 解析成功
				}
			}
		}
		BLOCKCHAIN_RIPEMD160::computeRIPEMD160(Script, ScriptLength, address);
	}
OK:return decodeOK;
}

void printHash(unit8 h[32])
{
	for (int k = 31; k >= 0; --k)
	{
		cout << hex << setfill('0') << setw(2) << (int)h[k] << dec;
	}
	cout << endl;
}

void printAddress(unit8 a[20])
{
	for (int k = 0; k <20; ++k)
	{
		cout << hex << setfill('0') << setw(2) << (int)a[k] << dec;
	}
	cout << endl;
}