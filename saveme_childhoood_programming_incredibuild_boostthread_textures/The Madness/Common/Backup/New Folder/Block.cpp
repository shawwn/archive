#include "Block.h"
#include "LoggingInterface.h"
using namespace std;

bool PtrSorter(char *p1, char *p2) { return p1 < p2; }

Block::Block(unsigned short ElementSize, unsigned short NumElements) : // ElementSize must be >= 4
	mElementSize(ElementSize), mDataSizeInBytes(ElementSize*NumElements), 
	mNumElementsInBlock(NumElements), mNumAllocated(0), mNext(NULL), mFirstFragment(NULL), mNumFragments(0)
{ 
	mData = new char[ElementSize * NumElements]; 
	if(ElementSize < 4) {
		gLog ^ "Block ERROR: Element size must be >= 4.\n";
	}
	mNumFragments = 0;
}

void Block::PrintChain()
{
	if(!mFirstFragment)
		return;
	char *asdf = mFirstFragment;
	gLog << "----------------\n";
	gLog << "mNumAllocated: [" << mNumAllocated << "]\n";
	gLog << "mNumFragments [" << mNumFragments << "]\n";
	gLog << "mFirstFragment: [" << (int)mFirstFragment << "] points to [" << (int)*(char**)mFirstFragment << "]\n";
	for(int i = 0; i < mNumFragments; i++) {
		gLog << "Chain: [" << (int)asdf << "] Index: [" << (int)((asdf-mData)/mElementSize) << "]  Contains: [" << Index(*(char**)asdf) << "]\n";
		asdf = *(char **)asdf;
	}
	gLog << "----------------\n";
}


char *Block::RequestElement(unsigned short count)
{
	if(count == 0) {
		gLog ^ "Block ERROR: RequestFreeElement(unsigned short count[" ^ count ^ "]) count must be >0.\n";
		return NULL;
	}

	/*mFragments.sort(PtrSorter);
	unsigned short MaxContiguous = 0;
	unsigned short CurContiguous = 1;
	char *MaxContiguousPtr = NULL;
	char *PrevContiguousPtr = NULL;
	char *StartContiguousPtr = *mFragments.begin();
	for(list<char *>::const_iterator it = mFragments.begin(); it != mFragments.end(); it++) {
		if(PrevContiguousPtr + mElementSize != *it) {
			if(CurContiguous > MaxContiguous) {
				MaxContiguous = CurContiguous;
				MaxContiguousPtr = StartContiguousPtr;
				StartContiguousPtr = *it;
				CurContiguous = 1;
			}
			else
				CurContiguous++;
		}
	}*/

	unsigned short MaxContiguous = 0;
	unsigned short CurrContiguous = 1;
	char *Frag = mFirstFragment;
	char *Prev = mFirstFragment;
	char *MaxContiguousPtr = NULL;
	char *MaxPrevPtr = NULL;
	char *CurPrevPtr = NULL;
	char *CurrContiguousPtr = mFirstFragment;
	
	gLog << "RequestElement requesting for [" << count << "] elements.  Current chain:\n";
	PrintChain();

	if(mNumFragments)
	{
		for(int i = 0; i < mNumFragments; i++) {
			if(Frag - mElementSize != Prev) {
				if(CurrContiguous > MaxContiguous) {
					MaxContiguousPtr = CurrContiguousPtr;
					MaxContiguous = CurrContiguous;
			gLog << "omgwtf1 MaxPrevPtr was [" << (int)MaxPrevPtr << "] Index[" << (int)((MaxPrevPtr-mData)/mElementSize) << "] but now is ";
					MaxPrevPtr = CurPrevPtr;
			gLog << "[" << (int)MaxPrevPtr << "] Index[" << (int)((MaxPrevPtr-mData)/mElementSize) << "] Frag[" << Index(Frag) << "]\n";
				}
				CurrContiguous = 1;
				CurrContiguousPtr = Frag;
				CurPrevPtr = Prev;
			}
			else
			{
				CurrContiguous++;
			}
			Prev = Frag;
			Frag = *(char **)Frag;
		}

		if(CurrContiguous > MaxContiguous && mNumFragments) {
			MaxContiguousPtr = CurrContiguousPtr;
			MaxContiguous = CurrContiguous;
			gLog << "omgwtf2 MaxPrevPtr was [" << (int)MaxPrevPtr << "] Index[" << (int)((MaxPrevPtr-mData)/mElementSize) << "] but now is ";
			MaxPrevPtr = CurPrevPtr;
			gLog << "[" << (int)MaxPrevPtr << "] Index[" << (int)((MaxPrevPtr-mData)/mElementSize) << "] Frag[" << Index(Frag) << "]\n";
		}

		gLog << "MaxContiguous [" << MaxContiguous << "]\n";

		if(MaxContiguous >= count) {
			mNumFragments -= count;
			mNumAllocated += count;

			char *ptr = MaxContiguousPtr + mElementSize*(count-1);
			if(MaxContiguousPtr == mFirstFragment) {
				if(!mNumFragments) 
					mFirstFragment = NULL;
				else
					mFirstFragment = *(char **)ptr;
			}
			else {
				*(char**)MaxPrevPtr = ptr+mElementSize;
				gLog << "MaxPrevPtr[" << (int)MaxPrevPtr << "] set to [" << (int)(ptr+mElementSize) << "]\n";
			}

			gLog << "Contiguous return.  MaxPrevPtr[" << (int)MaxPrevPtr << "] Contains Index [" << (int)((*(char**)MaxPrevPtr-mData)/mElementSize) << "]\n";
			gLog << "MaxContiguousPtr[" << (int)MaxContiguousPtr << "] Index[" << (int)((MaxContiguousPtr-mData)/mElementSize) << "]\n";
			gLog << "ptr[" << (int)ptr << "] Index[" << (int)((ptr-mData)/mElementSize) << "]\n";
			gLog << "Chain is now:\n";
			PrintChain();
			gLog << "\n";

			return MaxContiguousPtr;
		}
	}

	if(IsFull())
		return NULL;
	if(mNumElementsInBlock - (mNumAllocated + mNumFragments) < count) 
		return NULL;

	mNumAllocated += count;
	gLog << "End block return.  Chain is now:\n";
	PrintChain();
	gLog << "\n";
	return mData + (mNumAllocated + mNumFragments)*mElementSize;

	/*char *ret = NULL;
	if(!mNumFragments || count > 1) { 
		if(IsFull()) 
			return NULL;
		if(mNumElementsInBlock - (mNumAllocated + mNumFragments) >= count) {
			ret = mData + (mNumAllocated + mNumFragments)*mElementSize;
			mNumAllocated += count;
			gLog << "End block return.  Chain is now:\n";
			PrintChain();
			gLog << "\n";
			return ret;
		}
		else
			return NULL;
	}
	ret = mFirstFragment;
	mFirstFragment = *(char **)mFirstFragment;
	mNumFragments--;
	mNumAllocated++;

	gLog << "Fragment return.  Chain is now:\n";
	PrintChain();
	gLog << "\n";
	return ret;*/
}
char *Block::At(unsigned short ElementIndex)
{
	if(ElementIndex > mNumFragments + mNumAllocated)
		return NULL;
	if(!mFirstFragment)
		return mData + ElementIndex*mElementSize;
	char *it = mFirstFragment;
	int i = 0;
	while(i < mNumFragments)
	{
		if(i)
			it = *(char**)it;
		if((it-mData) / mElementSize == ElementIndex) 
			return NULL;
		i++;
	}

	//for(list<char *>::const_iterator it = mFragments.begin(); it != mFragments.end(); it++) {
		//if((*it - mData) / mElementSize == ElementIndex)
			//return NULL;
	//}

    return mData + ElementIndex*mElementSize;	
}

/* Same as above function, just a const version */
char const *Block::At(unsigned short ElementIndex) const
{
	if(ElementIndex > mNumFragments + mNumAllocated)
		return NULL;
	if(!mFirstFragment)
		return mData + ElementIndex*mElementSize;
	char *it = mFirstFragment;
	for(int i = 0; i < mNumFragments; i++)
	{
		if(i) {
			it = *(char**)it;
		}
		if((it-mData) / mElementSize == ElementIndex) 
			return NULL;
	}

/*	for(list<char *>::const_iterator it = mFragments.begin(); it != mFragments.end(); it++) {
		if((*it - mData) / mElementSize == ElementIndex)
			return NULL;
	}*/

    return mData + ElementIndex*mElementSize;	
}

bool Block::IsUsable(char const *ptr) const
{
	if(!Contains(ptr) || ((ptr-mData) % mElementSize))
		return false;
	return (At((unsigned short)(ptr-mData) / mElementSize) != NULL);
}

void Block::Reset()
{
	mFirstFragment = NULL;
	mNumFragments = 0;
	mNumAllocated = 0;
}

bool Block::ReleaseElement(char *ptr)
{
	if(Index(ptr) == 373 || Index(ptr) == 374) {
		gLog << "omg\n";
	}
	gLog << "ReleaseElement called... \n";
	if(!Contains(ptr)) {
		gLog << "OMGNINJA NOT USABLE.\n";
		//gLog ^ "Wtf\n";
		return false;
	}

	gLog << "Usable... \n";

	mNumAllocated--;
	if(ptr == (mData + (mNumAllocated+mNumFragments)*mElementSize))
		return true;
	gLog << "Pointer not at end.\n";

	//mFragments.push_back(ptr);
	//mNumFragments++;


	// [X5X1X] [XxX2X] [XX34X]
	
	if(!mFirstFragment) {
		if(mNumFragments > 0) {
			gLog << "****ASDF ASDF******\n";
			PrintChain();
			gLog << "\n\n\n\n\n\n\n";
		}
		mNumFragments = 1;
		//*(char **)ptr = NULL;
		mFirstFragment = ptr;
		gLog << "**(((**First fragment created**)))**  Chain is now:\n";
		PrintChain();
		gLog << "\n";
		return true;
	}

	char *Frag = mFirstFragment;
	char *Prev = NULL;
 	for(int i = 0; i < mNumFragments; i++) {
		if(Frag > ptr) {
			if(!Prev) {
				*(char **)ptr = Frag;
				mFirstFragment = ptr;
				
			}
			else
			{
				*(char **)Prev = ptr;
				*(char **)ptr = Frag;
			}
			mNumFragments++;

			gLog << "Modified: \n";
			if(Prev)
				gLog << "Prev[" << Index(Prev) << "] contains[" << Index(*(char**)Prev) << "]\n";
			gLog << "ptr[" << Index(ptr) << "] contains[" << Index(*(char**)ptr) << "]\n";
			if(Frag)
				gLog << "Frag [" << Index(Frag) << "]\n";
			gLog << "Chain is now:\n";
			PrintChain();
			gLog << "\n";

			return true;
		}
		Prev = Frag;
		Frag = *(char **)Frag;
	}

	if(Prev)
		gLog << "Index of Prev is [" << Index(Prev) << "]\n";
	else
		gLog << "Prev is NULL.\n";
	gLog << "Index of ptr is [" << Index(ptr) << "]\n";
	gLog << "*(char **)ptr = *(char **)Prev;\n";
	gLog << "*(char **)Prev = ptr;\n";
	if(Prev)
		gLog << "Prev contained [" << Index(*(char **)Prev) << "]\n";
	else
		gLog << "Prev is NULL.\n";
	*(char **)ptr = *(char **)Prev;
	*(char **)Prev = ptr;

	gLog << "ptr now contains[" << Index(*(char**)ptr) << "]\n";
	gLog << "Prev now contains[" << Index(*(char**)Prev) << "]\n";

	gLog << "Modified: \n";
	/*if(mNumFragments == 1) {
		gLog << "mNumFragments is 1.\n";
		if(Prev)
			gLog << "Index of Prev is [" << Index(Prev) << "]\n";
		else
			gLog << "Prev is NULL.\n";
		gLog << "Index of ptr is [" << Index(ptr) << "]\n";
		gLog << "*(char **)ptr = *(char **)Prev;\n";
		if(Prev)
			gLog << "Prev contained [" << Index(*(char **)Prev) << "]\n";
		else
			gLog << "Prev is NULL.\n";
		gLog << "mFirstFragment WAS [" << Index(mFirstFragment) << "]\n";
		*(char **)ptr = *(char **)Prev;
		*(char **)Prev = ptr;
		//*(char **)ptr = Prev;
		//mFirstFragment = ptr;
		gLog << "Now it is [" << Index(mFirstFragment) << "]\n";
	}
	else
	{
		if(Prev)
			gLog << "Index of Prev is [" << Index(Prev) << "]\n";
		else
			gLog << "Prev is NULL.\n";
		gLog << "Index of ptr is [" << Index(ptr) << "]\n";
		gLog << "*(char **)ptr = *(char **)Prev;\n";
		gLog << "*(char **)Prev = ptr;\n";
		if(Prev)
			gLog << "Prev contained [" << Index(*(char **)Prev) << "]\n";
		else
			gLog << "Prev is NULL.\n";
		*(char **)ptr = *(char **)Prev;
		*(char **)Prev = ptr;

		gLog << "ptr now contains[" << Index(*(char**)ptr) << "]\n";
		gLog << "Prev now contains[" << Index(*(char**)Prev) << "]\n";
	}
	*/
	mNumFragments++;
	gLog << "Chain added.  Chain is now:\n";
	PrintChain();
	gLog << "\n";

	return true;
}

Block *BlockManager::Allocate() {
	Block *ret;
	if(mEmptyBlock) {
		ret = mEmptyBlock;
		mBlocks.push_back(mEmptyBlock);
		mEmptyBlock = NULL;
		return ret;
	}
	ret = new Block(mElementSize,mNumElementsPerBlock);
	if(!ret) {
		gLog ^ "BlockManager ERROR: Could not allocate new block.\n";
		return NULL;
	}
	mBlocks.push_back(ret);
	return ret;
}

bool BlockManager::Deallocate(Block *b, Iterator Known) {
	if(!b) {
		// insert assert here
	}

	mBlocks.erase(Known);

	if(mEmptyBlock) {
		SAFE_DELETE(b);
		return false;
	}
	mEmptyBlock = b;
	return true;
}

bool BlockManager::Deallocate(Block *b) {
	if(!b) {
		// insert assert here
	}

	for(Iterator it = mBlocks.begin(); it != mBlocks.end(); it++)  {
		if((*it) == b) {
			mBlocks.erase(it);
			break;
		}
	}

	if(mEmptyBlock) {
		SAFE_DELETE(b);
		return false;
	}
	mEmptyBlock = b;
	return true;
}

char *BlockManager::RequestElement(unsigned short count)
{
	if(count == 0 || count > mNumElementsPerBlock) {
		gLog ^ "BlockManager ERROR: RequestElement count must be != 0 && < mNumElementsPerBlock.\n";
		return NULL;
	}
	if(!mFirstFree) {
		mFirstFree = Allocate();
		return mFirstFree->RequestElement(count);
	}
	char *ret;
	while(!(ret = mFirstFree->RequestElement(count))) {
		if(!mFirstFree->mNext) {
			mFirstFree = Allocate();
			return mFirstFree->RequestElement(count);
		}
		mFirstFree = mFirstFree->mNext;
	}

	return ret;
}

bool BlockManager::ReleaseElement(char *ptr)
{
	if(!mFirstFree)
		return false;
	if(mFirstFree->ReleaseElement(ptr))
		return true;

	Block *it = mFirstFree;
	Block *prev = NULL;
	bool WasFull;
	while(it) {
		WasFull = it->IsFull();
		if(it->ReleaseElement(ptr)) {
			if(WasFull) {
				it->mNext = mFirstFree;
				mFirstFree = it;
			}
			if(it->IsEmpty()) {
				prev->mNext = it->mNext;
				Deallocate(it);
			}
			return true;

		}
		prev = it;
		it = it->mNext;
	}

	for(Iterator it = mBlocks.begin(); it != mBlocks.end(); it++) {
		Block *b = *it;
		if(b->ReleaseElement(ptr)) {
			if(b->IsEmpty()) {
				Deallocate(b);
				return true;
			}

			b->mNext = mFirstFree;
			mFirstFree = b;
			return true;
		}
	}
	return false;
}