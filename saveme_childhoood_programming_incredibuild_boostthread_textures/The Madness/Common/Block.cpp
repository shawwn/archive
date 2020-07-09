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

char *Block::RequestElement(unsigned short count)
{
	if(mNumAllocated == 0 && mNumFragments == 0)
		
	
	
	
	
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

		
		

		if(NumMaxContiguous >= count) {
			if(PrevMaxContiguous)
				*(char**)PrevMaxContiguous = *(char**)(MaxContiguousChain + (count-1)*mElementSize);
			else
				mFirstFragment = *(char**)(MaxContiguousChain + (count-1)*mElementSize);
			mNumFragments -= count;
			mNumAllocated += count;
			
			
			
			
			return MaxContiguousChain;
		}
	}


	/////////////////////

	if(IsFull()) {
		
		
		
		
		return NULL;
	}
	if(mNumElementsInBlock - (mNumAllocated + mNumFragments) >= count) {
		ret = mData + (mNumAllocated + mNumFragments)*mElementSize;
		mNumAllocated += count;

		
		
		
		
		return ret;
	}
	else {
		
		
		
		
		return NULL;
	}

	ret = mFirstFragment;
	mFirstFragment = *(char **)mFirstFragment;
	mNumFragments--;
	mNumAllocated++;

	
	
	
	
	
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
	if(!IsUsable(ptr)) {
		return false;
	}

	mNumAllocated--;
	if(ptr == (mData + (mNumAllocated+mNumFragments)*mElementSize)) {
		return true;
	}

	mNumFragments++;
	if(mNumFragments == 1) {
		*(char **)ptr = NULL;
		mFirstFragment = ptr;
		return true;
	}
	char *Current = mFirstFragment;
	char *Prev = NULL;
	for(int i = 0; i < mNumFragments-1; i++) {
		if(Current > ptr) {
			if(!Prev) {
				*(char **)ptr = Current;
				mFirstFragment = ptr;
			}
			else
			{
				*(char **)Prev = ptr;
				*(char **)ptr = Current;
			}
			return true;
		}
		Prev = Current;
		Current = *(char **)Current;
	}
	*(char **)ptr = *(char **)Prev;
	*(char **)Prev = ptr;
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