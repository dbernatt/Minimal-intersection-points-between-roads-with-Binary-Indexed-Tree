
#include <cstdlib>

template <class T>
BITree<T>::BITree() { 
	// Alapertelmezett inicializalo konstruktor
	size = 1;
	bitree = (T *) malloc(sizeof(T));
	input = (T *) malloc(sizeof(T));
}

template <class T>
BITree<T>::BITree(T* v, int n) : size(n+1){ 
	// Inicializalo kontruktor
	// Frissiti es feltolti a binarisan indexelt fat a v sorozat alapjan
	
	input = (T *) malloc( n * sizeof(T));
	bitree = (T *) malloc( size * sizeof(T));

	for (int i = 0; i < size; ++i)
	{
		bitree[i] = 0;
	}

	for(int i = 0; i < n; i++){
		input[i] = v[i];
		updateBIT(i, v[i]);
	}
}

template<class T>
BITree<T>::BITree(int n) : size(n+1){
	// Inicializallo kontruktor, lenullazza a tombot
	input = (T *) malloc(n * sizeof(T));
	bitree = (T *) malloc(size * sizeof(T));
	for(int  i = 0; i < size; i++){
		bitree[i] = 0;
	}
}

template<class T>
BITree<T>::BITree(const BITree& uj) : size(uj.size){ // Masolo konstruktor
	// Lemasolja a bemeneti osztalyt a jelenlegibe
    bitree = (T *) malloc( size * sizeof(T));

    for (int i = 0; i < size; ++i){
    	bitree[i] = 0;
    }

    for(int i = 1; i < size; i++){
        bitree[i] = uj.bitree[i];
    }
}

template<class T>
void BITree<T>::updateBIT(int index, T ertek){
	// Frissiti a binarisan indexelt fat az index pozicion kezdodo ertek ertekkel
	index++;
	while(index <= size){
		bitree[index] += ertek;
		index += (index & (-index));
	}
}

template<class T>
T BITree<T>::getSumBIT(int index){
    // Visszateriti az index pozicioig levo osszeget a fabol
    T sum = 0; 
    index++;
 
    while (index>0)
    {
        sum += bitree[index]; 
        index -= (index & (-index));
    }
    return sum;
}

template<class T>
void BITree<T>::setInputValue(int index, T ertek){	
	// Modositja a bemeneti sorozatot es vele egyutt a binarisan indexelt fat is
	input[index] = ertek;
	update(index, ertek);
}

template<class T>
void BITree<T>::print() const{
	// Kiirja a binarisan indexelt fa sorozatot
	for(int i = 1; i < size; i++){
		cout << bitree[i] << " ";
	}
	cout << endl;
}

template<class T>
void BITree<T>::printInput() const{
	// Kiirja a bemeneti sorozatot
	for(int i = 0; i < size-1; i++){
		cout << input[i] << " ";
	}
	cout << endl;
}

template<class T>
void BITree<T>::shift_bal(int index){
	// Eltolja az elemeket 1-el balra

	for(int i = index; i < size-2; i++){
		input[i] = input[i+1];
	}
}

template<class T>
void BITree<T>::remove(int index){
	// Toroli az index pozicion levo elemet
	// Majd frissiti a binarisan indexelt fa sorozatot

	const char* ures;
	const char* rosszIndex;
	if(size <= 1){
		throw ures;
	}
	if(index >= 0 && index < size-1){
		shift_bal(index);
		size--;
		for (int i = 0; i < size; ++i){
			bitree[i] = 0;
		}
		for(int i = 0; i < size-1; i++){
			updateBIT(i, input[i]);
		}
	}
	else{
		throw rosszIndex;
	}
}


template <class T>
BITree<T>::~BITree() {
	// Felszabaditja a lefoglalt sorozatokat

	delete[] input;
	delete[] bitree;
}