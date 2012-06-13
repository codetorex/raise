#include "stdafx.h"
#include "trmlwriter.h"

const TString quot = "\"";

const TString txtTrue = "true";
const TString txtFalse = "false";

TRMLWriter::TRMLWriter( TStream* output )
{
	BaseStream = output;
	UTF8Writer = new TStreamWriterUTF8(output);
	IndentLevel = 0;
	JustClosedCurl = false;
}

void TRMLWriter::WriteElementStart( const TString& element )
{
	LineBuilder.Clear();
	if (JustClosedCurl)
	{
		LineBuilder.AppendLine();
		JustClosedCurl = false;
	}
	LineBuilder.AppendChar('\t',IndentLevel);
	LineBuilder.AppendChar('"');
	LineBuilder.Append(element);
	LineBuilder.AppendChar('"');
	LineBuilder.AppendLine();
	LineBuilder.AppendChar('\t',IndentLevel);
	LineBuilder.AppendChar('{');
	LineBuilder.AppendLine();

	UTF8Writer->Write(LineBuilder);

	IndentLevel++;
}



void TRMLWriter::WriteKeyPart( const TString& key )
{
	LineBuilder.Clear();
	LineBuilder.AppendChar('\t',IndentLevel);
	LineBuilder.Append(key);
	LineBuilder.AppendChar(':');
	LineBuilder.AppendChar(' ');
}


void TRMLWriter::FinishKeyValueLine()
{
	LineBuilder.AppendChar(';');
	LineBuilder.AppendLine();

	UTF8Writer->Write(LineBuilder);
}

void TRMLWriter::WriteKeyValue( const TString& key, const TString& value )
{
	WriteKeyPart(key);
	LineBuilder.Append(value);
	FinishKeyValueLine();
}

void TRMLWriter::WriteKeyValue( const TString& key, int value )
{
	WriteKeyPart(key);
	LineBuilder.Append(value);
	FinishKeyValueLine();
}

void TRMLWriter::WriteElementEnd()
{
	if (IndentLevel == 0)
		throw Exception("No element was started");
	
	IndentLevel--;
	LineBuilder.Clear();
	LineBuilder.AppendChar('\t',IndentLevel);
	LineBuilder.AppendChar('}');
	LineBuilder.AppendLine();
	
	JustClosedCurl = true;
	UTF8Writer->Write(LineBuilder);
}

void TRMLWriter::Close()
{
	UTF8Writer->Close();
}

void TRMLWriter::SerializeObject( TType* minfo, void* object )
{
	if (minfo->ObjectName != 0)
	{
		TString* objectname;
		if (minfo->ObjectName->MemberType == MT_NONE)
		{
			objectname = &(minfo->ObjectName->Name);
		}
		else
		{
			objectname = minfo->ObjectName->GetStringPtr(object);
		}
	
		WriteElementStart(*objectname);
	}
	

	for (ui32 i=0;i<minfo->Fields.Count;i++)
	{
		TFieldInfo* curMember = minfo->Fields.Item[i];
		if (curMember->MemberType != MT_ARRAY)
		{
			WriteKeyPart(curMember->Name);
			switch( curMember->MemberType )
			{
			case MT_BOOL:
				{
					bool* boolMember = curMember->GetBoolPtr(object);
					LineBuilder.Append(*boolMember ? txtTrue : txtFalse);
				}
				break;

			case MT_INT:
				{
					int* intMember = curMember->GetIntPtr(object);
					LineBuilder.Append(*intMember);
				}
				break;

			case MT_STRING:
				{
					TString* stringMember = curMember->GetStringPtr(object);
					LineBuilder.Append(*stringMember);
				}
				break;
			}
			FinishKeyValueLine();
		}
		else
		{
			SerializeArray(curMember, curMember->GetVoidPtr(object) );
		}
	}

	if (minfo->ObjectName != 0)
	{
		WriteElementEnd();
	}
}

void TRMLWriter::SerializeArray( TFieldInfo* curMember, void* object )
{
	WriteElementStart( curMember->Name );

	switch( curMember->MemberSubtype )
	{
	case MT_OBJECT:
		TArray< void* >* arrayMember = (TArray< void* >*)object;
		for (ui32 i=0;i<arrayMember->Count;i++)
		{
			SerializeObject(curMember->ObjectInfo, arrayMember->Item[i]);
		}
		break;
	}

	WriteElementEnd();
}