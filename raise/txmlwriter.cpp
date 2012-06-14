#include "stdafx.h"
#include "txmlwriter.h"


void TXMLWriter::SerializeArray( TFieldInfo* curMember, void* object )
{

}

void TXMLWriter::SerializeObject( TType* minfo, void* object )
{

	WriteStartElement(minfo->Name);

	throw NotImplementedException();

	WriteEndElement();

	/*if(minfo->ObjectName)
	{
		TString* objectName;
		if (minfo->ObjectName->MemberType == MT_NONE)
		{
			objectName = &(minfo->ObjectName->Name);
		}
		else
		{
			objectName = minfo->ObjectName->GetStringPtr(object);
		}

		WriteStartElement(*objectName);
	}

	throw NotImplementedException();

	TArrayEnumerator<TFieldInfo*> ae(minfo->Fields);
	while(ae.MoveNext())
	{
		if (ae.Current->MemberType != MT_ARRAY)
		{
			WriteStartElement(ae.Current->Name);
			switch(ae.Current->MemberType)
			{
			case MT_BOOL:

				break;
			}
			WriteEndElement();
		}
	}

	if (minfo->ObjectName)
	{
		WriteEndElement();
	}*/
}
