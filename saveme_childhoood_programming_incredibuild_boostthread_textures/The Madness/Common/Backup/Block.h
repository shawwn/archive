#ifndef BLOCK_H
#define BLOCK_H

#include <list>


#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif    

class Block
{
private:
	char *mData;
	char *mFirstFragment;
	unsigned short mNumFragments;
	unsigned short mNumAllocated;

	unsigned short mNumElementsInBlock;
	unsigned short mElementSize;
	unsigned long mDataSizeInBytes;
	void PrintStats();
	void PrintChain();
	inline int Index(char *ptr) { if(!ptr) { return -1111; } return (int)(ptr-mData)/mElementSize; }
public:
	Block *mNext;

	Block(unsigned short ElementSize, unsigned short NumElements);
	~Block() { SAFE_DELETE_ARRAY(mData); }

	char *RequestElement(unsigned short count = 1);
	bool ReleaseElement(char *ptr); 

	char *At(unsigned short ElementIndex);
	char const *At(unsigned short ElementIndex) const;

	bool IsUsable(char const *ptr) const;
	void Reset();
	
	inline bool Contains(char const *ptr) const { if( (ptr >= mData) && (ptr < (mData + mDataSizeInBytes)) ) { return true; } return false; }
	inline bool IsFull() const { return (mNumAllocated == mNumElementsInBlock); }
	inline bool IsEmpty() const { return !mNumAllocated; }

	inline unsigned short GetNumFragments() { return mNumFragments; }
	inline unsigned short GetElementSize() { return mElementSize; }
	inline unsigned short GetNumAllocated() { return mNumAllocated; }
	inline unsigned short GetNumFree() { return mNumElementsInBlock - mNumAllocated; }
	inline unsigned short GetTotal() { return mNumElementsInBlock; }
};

class BlockManager
{
private:
	typedef std::list<Block *>::iterator Iterator;

	std::list<Block *> mBlocks;
	Block *mFirstFree;
	Block *mEmptyBlock;
	int mElementSize;
	int mNumElementsPerBlock;

	Block *Allocate();
	bool Deallocate(Block *b, Iterator Known);
	bool Deallocate(Block *b);
	
	char *RequestElement(unsigned short count = 1);
	bool ReleaseElement(char *ptr);
public:
	BlockManager(int ElementSize, int NumElementsPerBlock) : mFirstFree(NULL), mEmptyBlock(NULL), mElementSize(ElementSize), mNumElementsPerBlock(NumElementsPerBlock) { }
	~BlockManager() { for(Iterator it = mBlocks.begin(); it != mBlocks.end(); it++) { SAFE_DELETE(*it); } SAFE_DELETE(mEmptyBlock); }

	void Reset() {
		Iterator it = mBlocks.begin();
		if(it != mBlocks.end()) {
			(*mBlocks.begin())->Reset();
			it++;
		}
		if(it != mBlocks.end()) {
			mFirstFree = (*it);
			it = mBlocks.erase(it);
		}
		for(it; it != mBlocks.end(); it++)
		{
			Deallocate((*it),it);
			mBlocks.erase(it);
		}	
	}
	
	template<class T> T* Construct(unsigned short count = 1) {
		if(sizeof(T) != mElementSize)
			return NULL;
		if(count > mNumElementsPerBlock)
			return NULL;
		char *elem = RequestElement(count);
		if(!elem) { mBlocks.push_back(Allocate()); elem = RequestElement(count); }
		if(!elem) { return NULL; }
		T* constructed = new(elem) T[count];
		return constructed;
	}

	template<class T> void Deconstruct(T* ptr) { ptr->~T();  ReleaseElement((char *)ptr);  }

	inline void GetStats(int &NumBlocks, int &NumAllocated, int &NumFragments, int &NumFree, int &NumTotal) { 
		NumBlocks = (int)mBlocks.size(); NumAllocated = 0; NumFragments = 0; NumFree = 0; NumTotal = 0;
		for(Iterator it = mBlocks.begin(); it != mBlocks.end(); it++) {
			NumAllocated += (*it)->GetNumAllocated();
			NumFragments += (*it)->GetNumFragments();
			NumFree += (*it)->GetNumFree();
			NumTotal += (*it)->GetTotal();
		}
	}
	inline int GetElementSize() { return mElementSize; }
	inline int GetNumElementsPerBlock() { return mNumElementsPerBlock; }
};

#endif