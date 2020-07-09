#ifndef HANDLERINTERFACE_H
#define HANDLERINTERFACE_H

#include <map>
#include "Types.h"
#include "PacketHandler.h"

class HandlerInterface
{
private:
	std::map<DoubleWord,PacketHandler *> mHandlers;
	std::vector<PacketHandler *> mConsecutive;
	typedef std::map<DoubleWord,PacketHandler *>::iterator Iterator;
	inline Iterator Begin() { return mHandlers.begin(); }
	inline Iterator End() { return mHandlers.end(); }
public:
	inline void RegisterHandler(DoubleWord OpCode, PacketHandler *Handler) { 
		mHandlers[OpCode] = Handler; 
		for(UINT i = 0; i < NumHandlers(); i++) { if(mConsecutive[i] == Handler) { return; } }
		mConsecutive.push_back(Handler);
	}
	inline PacketHandler *Find(DoubleWord OpCode) { Iterator it = mHandlers.find(OpCode); return (it != End() ? (*it).second : NULL); }
	inline PacketHandler *FindHandler(UINT Index) { return (Index < NumHandlers() ? mConsecutive[Index] : NULL); }

	inline UINT NumHandlers() { return (UINT)mConsecutive.size(); }
};
extern HandlerInterface gHandlers;

#endif