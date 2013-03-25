#ifndef RDBCONTEXT_H
#define RDBCONTEXT_H

#include "tstring.h"

/**
 * RaiseDB is in memory database system.
 * Optimized to use as much as memory available.
 * Uses disk cache when not enough memory available.
 * Records every transaction to hard drive to maintain persistence.
 * NoSQL only API access.
 */

enum RDBTypes
{
	RDB_SMALLINT,
	RDB_INTEGER,
	RDB_BIGINT,
	RDB_DECIMAL,
	RDB_NUMERIC,
	RDB_REAL,
	RDB_DOUBLE,
	RDB_SERIAL,
	RDB_CHAR,
	RDB_VARCHAR,
	RDB_TEXT,
};

class RDBBFieldType
{
public:
	TString ToString(void* dataPtr);
	void Parse(const TString& value, void* output);
};

class RDBField
{
public:
	TString Name;
	int TypeId;
	int Length;
};

class RDBRelation
{
public:
	TString FirstSet;
	TString FirstField;

	TString SecondSet;
	TString SecondField;

	int RelationType;
};

class RDBObjectMap
{
public:
	RDBField* Field;
	int DataOffset;
};

class RDBObject
{
public:

};

class RDBScheme
{
public:
	TArray< RDBField* > Fields;
	TArray< RDBRelation* > Relations;
	TArray< RDBObjectMap* > Maps;

	int MinSize;
};

class RDBContext
{
public:
	
};

class TestUser
{
public:
	int UserId;
	TString Username;
	TString Password;
	TString Email;

	static RDBScheme UserScheme;

};

class TestDatabase: public RDBContext
{
public:
	RDBSet<TestUser> Users;

};

#endif