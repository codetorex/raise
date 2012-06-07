#include "stdafx.h"
#include "txmlwriter.h"


void TXMLWriter::SerializeArray( TMember* curMember, void* object )
{

}

void TXMLWriter::SerializeObject( TMemberInfo* minfo, void* object )
{
	if(minfo->ObjectName)
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

	TArrayEnumerator<TMember*> ae(minfo->Members);
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
	}
}
