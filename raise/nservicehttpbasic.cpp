#include "stdafx.h"
#include "nservicehttpbasic.h"
#include "npacketbuilder.h"
#include "tfileinfo.h"
#include "texception.h"
#include "tlog.h"
#include "tapplication.h"


NServiceHTTP::NServiceHTTP()
{
	Name = "Basic HTTP Server";
	ServerVersion = TString::Format("Server: raise/%", sfs(Application.Version.VersionText));
}

void NServiceHTTP::Connected( NSocket* Client )
{
	Log.Output(LG_INF,"HTTP Client connected");
}

void NServiceHTTP::Disconnected( NSocket* Client )
{
	Log.Output(LG_INF,"HTTP Client disconnected");
}

void NServiceHTTP::Received( NSocket* Client, NPacket* Packet )
{
	Log.Output(LG_INF,"HTTP Client request");

	TString header(Packet->Data,Packet->Length);

	TArray<ch32> SplitChars;
	SplitChars.Add('\n');


	TArray<TString*> headerLines = header.Split(SplitChars);

	Log.Output(LG_INF,*headerLines.Item[0]);

	TString RequestLine = *headerLines.Item[0];
	
	TString RequestPath;

	if (RequestLine.StartsWith("GET"))
	{
		int a = RequestLine.IndexOf(" ",4);
		if (a > 0)
		{
			int b = RequestLine.IndexOf("?");
			if (b == -1)
			{
				b = a - 4;
			}
			else
			{
				b -= 4;
			}
			RequestPath = RequestLine.Substring(4,b);
			if (RequestPath == "/")
			{
				RequestPath = "/index.htm";
			}

			TString totalPath = RootFolder + RequestPath;
			
			if (TFileInfo::Exists(totalPath))
			{
				TFileStream* fs = File::OpenRead(totalPath);

				NPacketBuilder npb( &Client->SendBuffer ); // we dont need another buffer to do this job

				ui32 fileLength = fs->Length();
				bool appendfilecontent = false;

				if (fileLength < (Client->SendBuffer.Capacity - 1024)) // small files goes in one shot
				{
					appendfilecontent = true;
				}
				
				TString extension = TPath::GetExtension(totalPath);
				extension.ToLowerInplace();

				npb.AppendLine("HTTP/1.1 200 OK");
				npb.AppendLine(ServerVersion);
				npb.AppendLine("Accept-Ranges: bytes");
				npb.AppendString("Content-Length: "); // HELLO
				npb.AppendString(fileLength);
				npb.AppendLine();

				npb.AppendLine("Connection: close");
				/*npb.AppendString("Content-Type: ");
				if (extension.StartsWith(".htm"))
				{
					npb.AppendLine("text/html; charset=UTF-8");
				}
				else if (extension == ".jpg")
				{
					npb.AppendLine("image/jpeg");
				}
				else if (extension == ".png")
				{
					npb.AppendLine("image/png");
				}
				else if (extension == ".ico")
				{
					npb.AppendLine("image/x-icon");
				}*/

				npb.AppendLine();
				
				if (appendfilecontent)
				{
					npb.AddStreamContent(fs);
					Client->DataObject = 0;
				}
				else
				{
					Client->DataObject = fs;
				}

				npb.EndPacket();

				Log.Output(LG_INF,"HTTP Client response sending...");
				Server->Send(Client,&Client->SendBuffer);

				headerLines.DeletePointers();
				return;
			}
			else
			{
				NPacketBuilder npb( &Client->SendBuffer ); // we dont need another buffer to do this job

				npb.AppendLine("HTTP/1.1 404 Not Found");
				npb.AppendLine(ServerVersion);
				npb.AppendLine("Accept-Ranges: bytes");
				npb.AppendString("Content-Length: 0"); // HELLO
				npb.AppendLine("Connection: close");
				npb.AppendLine();
				npb.EndPacket();

				Log.Output(LG_INF,"HTTP Client response of 404 sending...");
				Server->Send(Client,&Client->SendBuffer);
				

				headerLines.DeletePointers();
				return;
			}
		}
	}

	headerLines.DeletePointers();


	NPacketBuilder npb( &Client->SendBuffer ); // we dont need another buffer to do this job

	npb.AppendLine("HTTP/1.1 501 Not Implemented");
	npb.AppendLine(ServerVersion);
	npb.AppendLine("Accept-Ranges: bytes");
	npb.AppendString("Content-Length: 0"); // HELLO
	npb.AppendLine("Connection: close");
	npb.AppendLine();
	npb.EndPacket();

	Log.Output(LG_INF,"HTTP Client unsupported request, sending response");
	Server->Send(Client,&Client->SendBuffer);
	


	/*NPacketBuilder npb;

	npb.AppendLine("HTTP/1.1 200 OK");
	npb.AppendLine(ServerVersion);
	npb.AppendLine("Last-Modified: Wed, 08 Jan 2003 23:11:55 GMT");
	npb.AppendLine("Accept-Ranges: bytes");
	npb.AppendLine("Content-Length: 5"); // HELLO
	npb.AppendLine("Connection: close");
	npb.AppendLine("Content-Type: text/html; charset=UTF-8");
	npb.AppendLine();
	npb.AppendString("HELLO");

	NPacket* response = npb.ToPacket();*/

	//Server->Send(Client,response);
	//Log.Output(LG_INF,"HTTP Client response sending...");
}

void NServiceHTTP::Sent( NSocket* Client, NPacket* Packet )
{
	if (Client->DataObject == 0)
	{
		/*TString tests(Packet->Data,Packet->Length);

		Log.Output(LG_INF,tests);*/
		Log.Output(LG_INF,"HTTP Client response sent, disconnecting");
		Server->Disconnect(Client,false);
	}
	else
	{
		TFileStream* fs = (TFileStream*)Client->DataObject;

		if (fs->Position() < fs->Length())
		{
			Log.Output(LG_INF,"HTTP sending content part");
			int readed = fs->Read(Client->SendByteBuffer,1,Client->SendBuffer.Capacity);
			Client->SendBuffer.Length = readed;
			Server->Send(Client,&Client->SendBuffer);
		}
		else
		{
			Log.Output(LG_INF,"HTTP content fully sent closing");
			fs->Close();
			Client->DataObject = 0;
			Server->Disconnect(Client,false);
		}
	}
	
	
}
