#include "Block.h"
#include "LoggingInterface.h"

Block::Block(unsigned short ElementSize, unsigned short NumElements) : // ElementSize must be >= 4
	mElementSize(ElementSize), mDataSizeInBytes(ElementSize*NumElements), 
	mNumElementsInBlock(NumElements), mNumAllocated(0), mNext(NULL), mFirstFragment(NULL), mNumFragments(0)
{ 
	mData = new char[ElementSize * NumElements]; 
	if(ElementSize < 4) {
		gLog ^ "Block ERROR: Element size must be >= 4.\n";
	}
}

void Block::PrintStats()
{
	gLog % "mNumAllocated: [" % mNumAllocated % "]\n";
	gLog % "mNumFragments [" % mNumFragments % "]\n";
}

void Block::PrintChain()
{
	if(!mFirstFragment)
		return;
	char *it = mFirstFragment;
	gLog % "===== CHAIN =====\n";
	gLog % "mNumAllocated: [" % mNumAllocated % "]\n";
	gLog % "mNumFragments [" % mNumFragments % "]\n";
	gLog % "mFirstFragment: [" % Index(mFirstFragment) % "] points to [" % Index(*(char**)mFirstFragment) % "]\n";
	for(int i = 0; i < mNumFragments; i++) {
		gLog % "Chain: [" % (int)it % "] Index: [" % (int)((it-mData)/mElementSize) % "]  Contains: [" % Index(*(char**)it) % "]\n";
		it = *(char **)it;
	}
	gLog % "=================\n";
}


char *Block::RequestElement(unsigned short count)
{
	if(mNumAllocated == 0 && mNumFragments == 0)
		gLog.SetDisable(true);
	gLog % "\n";
	gLog % "-------------------START REQUEST ELEMENT--------------------\n";
	gLog % "Requesting [" % count % "] elements.\n";
	PrintStats();
	if(count == 0) {
		gLog ^ "Block ERROR: RequestFreeElement(unsigned short count[" ^ count ^ "]) count must be >0.\n";
		return NULL;
	}
	char *ret = NULL;


	if(mNumFragments)
	{
		char *PrevPtr = NULL;
		char *Current = mFirstFragment;
		char *Prev = NULL;
		char *PrevMaxContiguous = NULL;

		char *CurrentContiguousChain = mFirstFragment;
		int NumCurrentContiguous = 1;

		char *MaxContiguousChain = NULL;
		int NumMaxContiguous = 0;
		/////////////////////
		for(int i = 0; i < mNumFragments; i++) {
			if(Current - mElementSize != Prev) {
				if(NumCurrentContiguous > NumMaxContiguous) {
					MaxContiguousChain = CurrentContiguousChain;
					NumMaxContiguous = NumCurrentContiguous;
					PrevMaxContiguous = PrevPtr;
				}
				PrevPtr = Prev;
				CurrentContiguousChain = Current;
				NumCurrentContiguous = 1;
				
			}
			else {
				NumCurrentContiguous++;
			}
			Prev = Current;
			Current = *(char **)Current;
		}

		if(NumCurrentContiguous > NumMaxContiguous) {
			MaxContiguousChain = CurrentContiguousChain;
			NumMaxContiguous = NumCurrentContiguous;
			PrevMaxContiguous = PrevPtr;
		}

		gLog % "MaxContiguousPtr: Index[" % Index(MaxContiguousChain) % "] NumMaxContiguous[" % NumMaxContiguous % "]\n";
		PrintChain();

		if(NumMaxContiguous >= count) {
			if(PrevMaxContiguous)
				*(char**)PrevMaxContiguous = *(char**)(MaxContiguousChain + (count-1)*mElementSize);
			else
				mFirstFragment = *(char**)(MaxContiguousChain + (count-1)*mElementSize);
			mNumFragments -= count;
			mNumAllocated += count;
			
			gLog % "Returned contiguous: [" % Index(MaxContiguousChain) % "]\n";
			gLog % "------------------END REQUEST ELEMENT------------------------\n";
			gLog % "\n";
			return MaxContiguousChain;
		}
	}


	/////////////////////

	if(IsFull()) {
		gLog % "Block is FULL, returned NULL (Hey, that rhymed)\n";
		gLog % "------------------END REQUEST ELEMENT------------------------\n";
		gLog % "\n";
		gLog.SetDisable(false);
		return NULL;
	}
	if(mNumElementsInBlock - (mNumAllocated + mNumFragments) >= count) {
		ret = mData + (mNumAllocated + mNumFragments)*mElementSize;
		mNumAllocated += count;

		gLog % "End block return.  Return index: [" % Index(ret) % "]\n";
		gLog % "------------------END REQUEST ELEMENT------------------------\n";
		gLog % "\n";
		gLog.SetDisable(false);
		return ret;
	}
	else {
		gLog % "Returned NULL due to not enough room in the block to accomodate request.\n";
		gLog % "------------------END REQUEST ELEMENT------------------------\n";
		gLog % "\n";
		gLog.SetDisable(false);
		return NULL;
	}

	ret = mFirstFragment;
	mFirstFragment = *(char **)mFirstFragment;
	mNumFragments--;
	mNumAllocated++;

	gLog % "Return index: [" % Index(ret) % "]\n";
	PrintChain();
	gLog % "------------------END REQUEST ELEMENT------------------------\n";
	gLog % "\n";
	gLog.SetDisable(false);
	return ret;
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
	int i = 0;
	while(i < mNumFragments)
	{
		if(i)
			it = *(char**)it;
		if((it-mData) / mElementSize == ElementIndex) 
			return NULL;
		i++;
	}

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
	if(mNumAllocated == 0 && mNumFragments == 0)
		gLog.SetDisable(true);

	gLog % "\n";
	gLog % "-------------------START RELEASE ELEMENT--------------------\n";
	gLog % "Element to be released: [" % Index(ptr) % "]\n";

	if(!IsUsable(ptr)) {
		gLog % "ptr is not usable!\n";
		gLog % "------------------END RELEASE ELEMENT------------------------\n";
		gLog % "\n";
		gLog.SetDisable(false);
		return false;
	}

	mNumAllocated--;
	if(ptr == (mData + (mNumAllocated+mNumFragments)*mElementSize)) {
		gLog % "Released from end of block, no fragments created.\n";
		gLog % "------------------END RELEASE ELEMENT------------------------\n";
		gLog % "\n";
		gLog.SetDisable(false);
		return true;
	}

	mNumFragments++;
	if(mNumFragments == 1) {
		*(char **)ptr = NULL;
		mFirstFragment = ptr;
		
		gLog % "First fragment created [" % Index(mFirstFragment) % "] containing {" % Index(*(char**)mFirstFragment) % "}\n";
		gLog % "------------------END RELEASE ELEMENT------------------------\n";
		gLog % "\n";
		gLog.SetDisable(false);
		return true;
	}

	//////////////////
	mNumFragments--;
	gLog % "````\n";
	gLog % "Chain before alteration:\n";
	PrintChain();
	gLog % "````\n";
	mNumFragments++;
	//////////////////

	char *Current = mFirstFragment;
	char *Prev = NULL;
	for(int i = 0; i < mNumFragments-1; i++) {
		if(Current > ptr) {
			gLog % "Prev[" % Index(Prev) % "] Current[" % Index(Current) % "] ptr[" % Index(ptr) % "]\n";
			if(Prev)	{ gLog % "Prev{" % Index(*(char**)Prev) % "}"; }
			if(Current) { gLog % " Current{" % Index(*(char**)Current) % "}"; }
			if(ptr)		{ gLog % " ptr{" % Index(*(char**)ptr) % "}"; }
			gLog % "\n";
			gLog % "===changed===\n";


			if(!Prev) {
				*(char **)ptr = Current;
				mFirstFragment = ptr;
				
			}
			else
			{
				*(char **)Prev = ptr;
				*(char **)ptr = Current;
			}
			//*(char **)ptr = Current;
			//*(char **)Current = ptr;


			if(Prev)	{ gLog % "Prev{" % Index(*(char**)Prev) % "}"; }
			if(Current) { gLog % " Current{" % Index(*(char**)Current) % "}"; }
			if(ptr)		{ gLog % " ptr{" % Index(*(char**)ptr) % "}"; }
			gLog % "\n";
			gLog % "Chain after alteration:\n";
			PrintChain();
			gLog % "------------------END RELEASE ELEMENT------------------------\n";
			gLog % "\n";
			return true;
		}
		Prev = Current;
		Current = *(char **)Current;
	}

	gLog % "Current was always LESS THAN ptr.\n";
	gLog % "Prev[" % Index(Prev) % "] Current[" % Index(Current) % "]\n";
	gLog % "Setting ptr[" % Index(ptr) % "]'s value to *(char **)Prev's, which is {" % Index(*(char**)Prev) % "}\n";
	gLog % "Setting Prev[" % Index(Prev) % "]'s value to ptr, which is [" % Index(ptr) % "]\n";
	*(char **)ptr = *(char **)Prev;
	*(char **)Prev = ptr;
	//if(*(char**)Prev < mFirstFragment) 
		//mFirstFragment = ptr;
	gLog % "Chain after alteration:\n";
	PrintChain();

	//*(char **)ptr = mFirstFragment;
	//mFirstFragment = ptr;

	gLog % "------------------END RELEASE ELEMENT------------------------\n";
	gLog % "\n";
	gLog.SetDisable(false);
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