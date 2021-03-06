#include "stdafx.h"
#include "nserverhttp.h"
#include "tlog.h"
#include "npacketbuilder.h"
#include "tfileinfo.h"
#include "tapplication.h"



void NHTTPSession::HandleReceive( const TIOStatus& err, ui32 dataReaded )
{


	if (err.NoError)
	{
		Log.Output(LG_ERR,"HTTP read error, disconnecting client");
		delete this;
		return;
	}

	Log.Output(LG_INF,"HTTP Client request");

	String header(ReceivePacket.Data,ReceivePacket.Length);


	Array<String*> headerLines = header.Split('\n');

	Log.Output(LG_INF,*headerLines.Items[0]);

	String RequestLine = *headerLines.Items[0];

	String RequestPath;

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

			String totalPath = Server->RootFolder + RequestPath;

			if (TFileInfo::Exists(totalPath))
			{
				TFileStream* fs = File::OpenRead(totalPath);

				NPacketBuilder npb( &SendPacket ); // we dont need another buffer to do this job

				ui32 fileLength = fs->Length();
				bool appendfilecontent = false;

				if (fileLength < (SendPacket.Capacity - 1024)) // small files goes in one shot
				{
					appendfilecontent = true;
				}

				String extension = TPath::GetExtension(totalPath);
				extension.ToLowerInplace();

				npb.AppendLine("HTTP/1.1 200 OK");
				npb.AppendLine(Server->ServerVersion);
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
				}
				else
				{
					ReadStream = fs;
				}

				npb.EndPacket();

				Log.Output(LG_INF,"HTTP Client response sending...");
				Socket->BeginSend(&SendPacket, &MSendCallback);

				headerLines.DeletePointers();
				return;
			}
			else
			{
				NPacketBuilder npb( &SendPacket ); // we dont need another buffer to do this job

				npb.AppendLine("HTTP/1.1 404 Not Found");
				npb.AppendLine(Server->ServerVersion);
				npb.AppendLine("Accept-Ranges: bytes");
				npb.AppendString("Content-Length: 0"); // HELLO
				npb.AppendLine("Connection: close");
				npb.AppendLine();
				npb.EndPacket();

				Log.Output(LG_INF,"HTTP Client response of 404 sending...");

				Socket->BeginSend(&SendPacket, &MSendCallback);

				headerLines.DeletePointers();
				return;
			}
		}
	}

	headerLines.DeletePointers();


	NPacketBuilder npb( &SendPacket ); // we dont need another buffer to do this job

	npb.AppendLine("HTTP/1.1 501 Not Implemented");
	npb.AppendLine(Server->ServerVersion);
	npb.AppendLine("Accept-Ranges: bytes");
	npb.AppendString("Content-Length: 0"); // HELLO
	npb.AppendLine("Connection: close");
	npb.AppendLine();
	npb.EndPacket();

	Log.Output(LG_INF,"HTTP Client unsupported request, sending response");
	Socket->BeginSend(&SendPacket, &MSendCallback);
}

void NHTTPSession::HandleSend( const TIOStatus& err )
{
	if (err.NoError)
	{
		Log.Output(LG_ERR,"HTTP write error, disconnecting client");
		delete this;
		return;
	}

	// TODO: test if error, and implement error checking stuff

	if (ReadStream == 0)
	{
		Log.Output(LG_INF,"HTTP Client response sent, disconnecting");	
		delete this;
	}
	else
	{
		ui32 Pos = ReadStream->Position();
		ui32 Len = ReadStream->Length();
		if ( Pos < Len)
		{
			double perc = ((double)Pos / (double)Len) * 100.0f;

			Log.Output(LG_INF,"HTTP sending content part %% % % / %" , sff(perc,2),sfu(Pos),sfu(Len));
			int readed = ReadStream->Read(SendBuffer,1,SendPacket.Capacity);
			SendPacket.Length = readed;

			Socket->BeginSend(&SendPacket, &MSendCallback);
		}
		else
		{
			Log.Output(LG_INF,"HTTP content fully sent closing");
			ReadStream->Close();
			ReadStream = 0;

			delete this;
			return;
		}
	}
}

NHTTPSession::~NHTTPSession()
{
	if (ReadStream)
	{
		ReadStream->Close();
	}

	Service->Disconnect(Socket);

	Log.Output(LG_INF,"HTTP Client disconnected");
}

void NHTTPSession::BeginReceiving()
{
	Log.Output(LG_INF,"HTTP Client connected");
	Socket->BeginReceive(&ReceivePacket, &MReceiveCallback  );
}

NHTTPServer::NHTTPServer( TIOService* _Service, ui16 _Port ): MyCallback(this, &NHTTPServer::HandleAccept)
{
	Service = _Service;
	Port = _Port;

	ServerName = "Basic HTTP Server";
	ServerVersion = String::Format("Server: raise/%", sfs(Application.Version.VersionText));
}