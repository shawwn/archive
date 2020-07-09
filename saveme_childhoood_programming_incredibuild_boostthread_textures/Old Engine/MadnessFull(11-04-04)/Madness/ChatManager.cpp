#include "dxstdafx.h"
#include "ChatManager.h"
#include "ClientNetworking.h"
#include "ParsingInterface.h"

bool ChatManager::Say(const wstring &Message)
{
	if(!gNetworking->Running())
		return false;

	Packet pack;
	pack.Build(CMSG_CHAT_SAY);
	pack.AddBytes((Byte *)WideStringToString(Message).c_str(),(int)WideStringToString(Message).length()+1);
	gNetworking->WriteData(&pack);
	return true;
}