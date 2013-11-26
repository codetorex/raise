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
	String ToString(void* dataPtr);
	void Parse(const String& value, void* output);
};

class RDBField
{
public:
	String Name;
	int TypeId;
	int Length;
};

class RDBRelation
{
public:
	String FirstSet;
	String FirstField;

	String SecondSet;
	String SecondField;

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
	Array< RDBField* > Fields;
	Array< RDBRelation* > Relations;
	Array< RDBObjectMap* > Maps;

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
	String Username;
	String Password;
	String Email;

	static RDBScheme UserScheme;

};

class TestDatabase: public RDBContext
{
public:
	RDBSet<TestUser> Users;

};

#endif