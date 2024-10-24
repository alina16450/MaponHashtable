#include "Map.h"
#include "MapIterator.h"
#include <math.h>
#include <iostream>

int Map::h(TKey e)const{ 
	int val = abs(e);
	return(val % cap);
}

int nextPrime(int p){ 
	bool isPrime = false;
	for (int i = p * 2; i < p * 4; i++) {
		bool temp = true;
		while (temp == true) {
			for (int j = 2; j < i; j++) {
				if (i % j == 0) {
					temp = false;
					break;
				}
			}
			if (temp == true) {
				isPrime = true;
				break;
			}
		}
		if (isPrime == true)
			return i;
	}
}

void Map::changeFirstEmpty(){ 
	while (firstEmpty < cap && pairs[firstEmpty].second != NULL_TVALUE)
		firstEmpty++;
}

Map::Map() {
	cap = 11;
	firstEmpty = 0;
	pairs = new TElem[cap];
	next = new int[cap];
	for (int i = 0; i < cap; i++) {
		pairs[i].first = -1;
		pairs[i].second = NULL_TVALUE;
		next[i] = -1;
	}
}

Map::~Map() {
	cap = 0;
	delete[] pairs;
	delete[] next;
}

TValue Map::add(TKey c, TValue v) {
	if (size() + 1 >= cap) { 
		// Expand the capacity and rehash all elements
		int oldCap = cap;
		cap = nextPrime(cap); //nextprime multiplies oldcap by two and then finds the next prime number
		TElem* tempPairs = new TElem[cap];
		int* tempNext = new int[cap];

		// Initialize the new arrays
		for (int i = 0; i < cap; i++) {
			tempPairs[i].first = -1;
			tempPairs[i].second = NULL_TVALUE;
			tempNext[i] = -1;
		}

		// Rehash existing elements
		int j = 0;
		int tempFirstEmpty = 0;

		while (j < oldCap) {
			if (pairs[j].first != -1 || pairs[j].second != NULL_TVALUE) {
				int tempIndex = h(pairs[j].first);
				if (tempPairs[tempIndex].first == -1 && tempPairs[tempIndex].second == NULL_TVALUE) {
					tempPairs[tempIndex].first = pairs[j].first;
					tempPairs[tempIndex].second = pairs[j].second;
					if (tempIndex == tempFirstEmpty) {
						while (tempFirstEmpty < cap && tempPairs[tempFirstEmpty].second != NULL_TVALUE)
							tempFirstEmpty++;
					}
				}
				else {

					int i = h(pairs[j].first);  //search alg for key in tempPairs
					int a = i;
					while (i != -1 && tempPairs[i].first != pairs[j].first && pairs[j].second != NULL_TVALUE) {
						a = i;
						i = tempNext[i];
					}

					if (i == -1 && tempPairs[a].second != NULL_TVALUE) {
						tempPairs[tempFirstEmpty].first = pairs[j].first;
						tempPairs[tempFirstEmpty].second = pairs[j].second;
						int tempCurrent = tempIndex;
						while (tempNext[tempCurrent] != -1) {
							tempCurrent = tempNext[tempCurrent];
						}
						tempNext[tempCurrent] = tempFirstEmpty;

						while (tempFirstEmpty < cap && tempPairs[tempFirstEmpty].second != NULL_TVALUE)
							tempFirstEmpty++;

					}
					else {
						tempPairs[i].second = pairs[j].second;
						while (tempFirstEmpty < cap && tempPairs[tempFirstEmpty].second != NULL_TVALUE)
							tempFirstEmpty++;
					}
				}
			}
			j++;
		}

		delete[] pairs;
		delete[] next;
		pairs = tempPairs;
		next = tempNext;

		// Update firstEmpty
		firstEmpty = 0;
		while (firstEmpty < cap && (pairs[firstEmpty].first != -1 || pairs[firstEmpty].second != NULL_TVALUE)) {
			firstEmpty++;
		}
	}

	// Add new element or update existing element
	int index = h(c);
	if (pairs[index].first == -1 && pairs[index].second == NULL_TVALUE) {
		// If the slot is empty, insert the new element
		pairs[index].first = c;
		pairs[index].second = v;
		if (index == firstEmpty) {
			changeFirstEmpty();
		}
		return NULL_TVALUE;
	}
	else {
		// If the key already exists, update its value
		if (searchKey(c) != NULL_TVALUE) {
			int temp = searchKey(c);//
			TValue tv = pairs[temp].second;
			pairs[temp].second = v;
			return tv;
		}
		else {
			// If the key does not exist, insert it at firstEmpty
			pairs[firstEmpty].first = c;
			pairs[firstEmpty].second = v;
			int current = index;
			while (next[current] != -1)
				current = next[current];
			next[current] = firstEmpty;
			changeFirstEmpty();
			return NULL_TVALUE;
		}
	}
}

int Map::searchKey(TKey c) const{ 
	int i = h(c);
	while (i != -1 && pairs[i].first != c) {
		i = next[i];
	}
	if (i == -1) {
		return NULL_TVALUE;
	}
	return i;

}

TValue Map::search(TKey c) const { 
	int i = h(c);
	while (i != -1 && pairs[i].first != c) {
		i = next[i];
	}
	if (i == -1 && pairs[i].second != NULL_TVALUE)
		return NULL_TVALUE;
	else
		return pairs[i].second;
}

TValue Map::remove(TKey c) { 
	int i = h(c);
	int j = -1; //to keep track of prev of i
	while (i != -1 && pairs[i].first != c) {
		j = i;
		i = next[i];
	}
	if (i == -1 || pairs[i].second == NULL_TVALUE) { //value was not found in pairs
		return NULL_TVALUE;
	}
	else{
		TElem v = pairs[i];  
		bool done = false;
		while (done == false) {
			int p = next[i];
			int pp = i;
			while (p != -1 && h(pairs[p].first) != i) {
				pp = p;
				p = next[p];
			}
			if (p == -1)
				done = true;
			else {
				pairs[i] = pairs[p];
				i = p;
				j = pp;
			}
			if (j == -1) {
				int x = 0;
				while (x < cap && next[x] != i) {
					x++;
				}
				if (x < cap) {
					j = x;
				}
			}
			if (j != -1)
				next[j] = next[i];
			pairs[i].first = -1;
			pairs[i].second = NULL_TVALUE;
			next[i] = -1;
			if (i < firstEmpty)
				firstEmpty = 1;
		}
		return v.second;
	}
}

int Map::size() const { 
	 int count = 0;
    for (int i = 0; i < cap; i++) {
        if (pairs[i].first != -1 || pairs[i].second != NULL_TVALUE) {
            count++;
        }
    }
    return count;
}

bool Map::isEmpty() const 
{
	return(size() == 0);
}

MapIterator Map::iterator() const 
{
	return MapIterator(*this);
}
