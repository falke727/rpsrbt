/* check.cpp */

#include <check.hpp>

unsigned checkClassifyResult(list<Result>*& seq, list< list<Result> > & other)
{
	list< list<Result> >::iterator it, end;
	it = other.begin(), end = other.end();

	/* size check */
	while (it != end) {
		if (seq->size() != it->size()) { return 1; }
		++it;
	}

	/* iterator for the Result of sequential search */
	list<Result>::iterator sIt, sEnd = seq->end();

	/* iterator for the Results of proposed algorithms */
	list<Result>::iterator pIt;

	it = other.begin();
	while (it != end) {
		sIt = seq->begin();
		pIt = it->begin();
		while (sIt != sEnd) {
			if (sIt->getValue() != pIt->getValue()) {
				cout << sIt->getPacket() << ": " << sIt->getValue() << endl;
				cout << pIt->getPacket() << ": " << pIt->getValue() << endl;
				return 1;
			}
			++sIt;
			++pIt;
		}
		++it;
	}

	return 0;
}
