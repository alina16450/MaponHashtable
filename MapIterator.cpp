#include "Map.h"
#include "MapIterator.h"
#include <exception>
using namespace std;

void MapIterator::move()
{
	while (current < map.cap && map.pairs[current].second == NULL_TVALUE)
		current++;
}

MapIterator::MapIterator(const Map& d) : map(d)
{
	current = 0;
	move();
}


void MapIterator::first() {
	current = 0;
	move();
}


void MapIterator::next() {
	if (!valid())
		throw std::exception();
	current++;
	move();
}


TElem MapIterator::getCurrent() {
	if (!valid())
		throw std::exception();
	return map.pairs[current];
}


bool MapIterator::valid() const {
	return(current < map.cap);
}
