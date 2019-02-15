#include "tdigest.h"


void TDigest::compress() {
    // TODO: implement this one
}


double TDigest::quantile(double q) {
    if(q < 0 || q > 1) {
        return 0; // TODO
    }

    if(_centroids->size() == 0) {
        return 0; // TODO
    } else if(_centroids->size() == 1) {
        return _centroids->value(_centroids->first());
    }

    const double index = q * (_count - 1);

    double previousMean = NAN;
    double previousIndex = 0;
    int next = _centroids->floorSum(index);
    assert(next != AvlTree::NIL);
    long total = _centroids->ceilSum(next);
    const int prev = _centroids->prevNode(next);
    if(prev != AvlTree::NIL) {
        previousMean = _centroids->value(prev);
        previousIndex = total - (_centroids->count(prev) + 1.0) / 2;
    }

    while(true) {
        const double nextIndex = total + (_centroids->count(next) - 1.) / 2;
        if(nextIndex >= index) {
            if(previousMean == NAN) {
                // Index is before first centroid
                assert(total == 0);
                if(nextIndex == previousIndex) {
                    return _centroids->value(next);
                }
                // We assume a linear increase
                int next2 = _centroids->value(next);
                const double nextIndex2 = total + _centroids->count(next) + (_centroids->count(next2) - 1.) / 2;
                previousMean = (nextIndex2 * _centroids->value(next) - nextIndex * _centroids->value(next2)) / (nextIndex2 - nextIndex);
            }
            return quantile(previousIndex, index, nextIndex, previousMean, _centroids->value(next));

        } else if(_centroids->value(next) == AvlTree::NIL) {
            // Beyond last centroid
            const double nextIndex2 = _count - 1;
            const double nextMean2 = (_centroids->value(next) * (nextIndex2 - previousIndex ) - previousMean * (nextIndex2 - nextIndex)) / (nextIndex - previousIndex);
            return quantile(nextIndex, index, nextIndex2, _centroids->value(next), nextMean2);
        }
        total += _centroids->count(next);
        previousMean = _centroids->value(next);
        previousIndex = nextIndex;
        next = _centroids->nextNode(next);
    }

}

