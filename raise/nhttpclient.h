#ifndef NHTTPCLIENT_H
#define NHTTPCLIENT_H

// TODO: fix these shit

/*#include "nasynchronous.h"
#include "npacketbuilder.h"

class NRequestHTTP
{
public:
	TString Host;
	TString RequestPath;

	TString Header;

	void BuildHeader()
	{
		// TODO: make this nicer
		TStringBuilder sb;
		sb.Append("GET ");
		sb.Append(RequestPath);
		sb.Append(" HTTP/1.1\r\n");
		sb.Append("Host: ");
		sb.Append(Host);
		sb.Append("\r\n");
		sb.Append("\r\n");
		Header = sb.ToString();
	}

	
	TFileStream* Output;
};



NRequestHTTP* hrq;// = new NRequestHTTP();

class NClientHTTP: public NService
{
public:
	NClientHTTP()
	{
		Name = "Basic HTTP Client";
	}


	void Accepted( NSocket* Client ) 
	{
		Log.Output(LG_INF,"HTTP Client connected");

		Client->DataObject = hrq;
		
		NPacketBuilder npb( &Client->SendBuffer );
		npb.AppendString(hrq->Header);
		npb.EndPacket();

		Client->Send( &Client->SendBuffer );
		// Send request here
	}

	void Disconnected( NSocket* Client ) 
	{
		Log.Output(LG_INF,"HTTP Client disconnected");
	}

	void Received( NSocket* Client, NPacket* Packet ) 
	{
		Log.Output(LG_INF,"HTTP got response");
		hrq->Output->Write( Packet->Data,1,Packet->Length);
	}

	void Sent( NSocket* Client, NPacket* Packet ) 
	{
		Log.Output(LG_INF,"HTTP request sent");
	}
};

void LoginScene::ConnectToASite( const TString& sitenam )
{

}

void LoginScene::ConnectTo4Chan()
{
	NHTTPClient* cli = new NHTTPClient(IOService);

	cli->Connect("test");

	/*hrq->Host = "4chan.org";

	LPHOSTENT lpHostEntry = gethostbyname((char*)hrq->Host.Data);
	if (lpHostEntry == 0)
	{
		Log.Output(LG_ERR,"Cant resolve hostname %", sfs(hrq->Host));
		return;
	}
	
	hrq->RequestPath = "/";
	hrq->Output = new TFileStream("c:\\outputtt.html",fm_Write);
	hrq->BuildHeader();

	in_addr* k = (LPIN_ADDR)lpHostEntry->h_addr_list;

	NEndPoint np;
	np.Address = NAddress4("141.101.126.243");
	np.Port = 81;

	Async->Connect(np,testHTTP);/
}

*/

/*class NHTTPClient
{
public:
	NSocket* Socket;
	TIOService* Service;

	NUri Uri;
	TString Header;


	byte ReceiveBuffer[8192];
	NPacket ReceivePacket;

	byte SendBuffer[8192];
	NPacket SendPacket;

	TFileStream* Output;


	NHTTPClient(TIOService* _service)
	{
		Service = _service;
		Socket = _service->CreateSocketTCP();
		ReceivePacket.Use(ReceiveBuffer,8192);
		SendPacket.Use(SendBuffer,8192);

		Output = 0;
	}

	~NHTTPClient()
	{
		if (Socket)
		{
			Service->Disconnect(Socket);
		}

		if (Output)
		{
			Output->Close();
		}

		Log.Output(LG_INF,"HTTP Client disconnected");
	}

	void Connect(const TString& _uri)
	{
		NEndPoint ep;
		ep.Address = "141.101.124.244";
		ep.Port = 80;

		Uri.Host = "www.4chan.org";
		Uri.AbsolutePath = "/";
		Uri.Scheme = "http";

		Output = new TFileStream("c://outpuutt.htm", fm_Write);

		// uri thing is not implemented currently
		Socket->BeginConnect( ep , GetHandler(this,&NHTTPClient::HandleConnected)); // TODO: make better handler? like this one creates pointers all the time but not tracks when it needed to be deleted. we could use something better with unions of different delegates
	}

	void SendRequest()
	{
		TStringBuilder sb;
		sb.Append("GET ");
		sb.Append(Uri.AbsolutePath);
		sb.Append(" HTTP/1.1\r\n");
		sb.Append("Host: ");
		sb.Append(Uri.Host);
		sb.Append("\r\n");
		sb.Append("Connection: close\r\n");
		sb.Append("\r\n");
		Header = sb.ToString();

		NPacketBuilder npb( &SendPacket );
		npb.AppendString(Header);
		npb.EndPacket();

		Socket->BeginSend(&SendPacket, GetHandler(this,&NHTTPClient::HandleSend));
	}



	void HandleReceive(const SystemError& err, ui32 dataReaded)
	{
		if (err.OSErrorID != 0)
		{
			Log.Output(LG_ERR, "Error on receive: %", sfs(err.Message));
			delete this;
			return;
		}

		Log.Output(LG_INF, "HTTP response received % bytes", sfu(dataReaded));

		Output->Write(ReceivePacket.Data,1,ReceivePacket.Length);

		Log.Output(LG_INF, "HTTP response written to file, requesting next part");
		StartReceive();
	}

	void HandleSend(const SystemError& err)
	{
		if (err.OSErrorID != 0)
		{
			Log.Output(LG_ERR, "Error on sending data: %", sfs(err.Message));
			delete this;
			return;
		}
		Log.Output(LG_INF, "HTTP request sent waiting for reply...");
		StartReceive();
	}

	void HandleConnected(const SystemError& err)
	{
		if (err.OSErrorID != 0) // TODO: change this
		{
			Log.Output(LG_ERR, "Error on connect: %", sfs(err.Message));
			delete this;
			return;
		}


		Log.Output(LG_INF, "HTTP client connected sending request...");
		SendRequest();
	}

private:
	void StartReceive()
	{
		Socket->BeginReceive(&ReceivePacket, GetHandler(this,&NHTTPClient::HandleReceive));
	}

};*/

#endif


