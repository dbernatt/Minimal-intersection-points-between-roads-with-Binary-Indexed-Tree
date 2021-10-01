
using std::cout;
using std::endl;

template <class T>
class BITree
{
private:
	int size;
	T* input;
	T* bitree;

protected:
	void shift_bal(int index);
public:

	BITree();
	BITree(int n);
	BITree(T* v, int n);
	BITree(const BITree& uj);
	void updateBIT(int index, T value);
	void setInputValue(int index, T ertek);
	T getSumBIT(int index);
	void remove(int index);
	void print() const;
	void printInput() const;
	~BITree();	
};

#include "BITree.cpp"