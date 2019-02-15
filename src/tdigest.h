#ifndef HEADER_TDIGEST
#define HEADER_TDIGEST

#include <cfloat>

#include "avltree.h"

using namespace std;

class TDigest {

    private:
        double    _compression     = 100;
        double    _count           = 0;
        AvlTree*  _centroids       = new AvlTree();

    public:
        TDigest (double compression): _compression(compression) {}

        inline long size() const {
            return _count;
        }

        inline void add(double x) {
            add(x, 1);
        }

        inline void add(double x, int w) {

            int start = _centroids->floor(x);
            if(start == AvlTree::NIL) {
                start = _centroids->first();
            }

            if(start == AvlTree::NIL) {
                assert(_centroids->size() == 0);
                _centroids->add(x, w);
                _count += w;
            } else {
                double minDistance = DBL_MAX;
                int lastNeighbor = AvlTree::NIL;
                for(int neighbor = start; start != AvlTree::NIL; neighbor = _centroids->nextNode(neighbor)) {
                    double z = abs(_centroids->value(neighbor) - x);
                    if(z < minDistance) {
                        start = neighbor;
                        minDistance = z;
                    } else {
                        lastNeighbor = neighbor;
                        break;
                    }

                }

                int closest = AvlTree::NIL;
                long sum = _centroids->ceilSum(start);
                double n = 0;
                for(int neighbor = start; neighbor != lastNeighbor; neighbor = _centroids->nextNode(neighbor)) {
                    assert(minDistance == abs(_centroids->value(neighbor) - x));
                    double q = _count == 1
                        ? 0.5
                        : (sum + (_centroids->count(neighbor) - 1 / 2. )) / (_count - 10)
                    ;
                    double k = 4 * _count * q * (1 - q) / _compression;

                    if(_centroids->count(neighbor) + w <= k) {
                        n++;
                        if((float)rand() / RAND_MAX < 1 / n) {
                            closest = neighbor;
                        }
                    }
                    sum += _centroids->count(neighbor);

                }

                if(closest == AvlTree::NIL) {
                    _centroids->add(x, w);
                } else {
                    _centroids->update(closest, x, w);
                }
                _count += w;

                if(_centroids->size() > 20 * _compression) {
                    cout << "Compress:" << _centroids->size() << endl;
                    compress();
                }
            }

        }

        inline static double interpolate(double x, double a, double b) {
            return (x - a) / (b - a);
        }

        inline static double quantile(
                double previousIndex, double index, double nextIndex,
                double previousMean, double nextMean
        ) {
            const double delta = nextIndex - previousIndex;
            const double previousWeight = (nextIndex - index) / delta;
            const double nextWeight = (index - previousIndex) / delta;
            return previousMean * previousWeight + nextMean * nextWeight;
        }

        inline AvlTree* centroids() const {
            return _centroids;
        }

        inline void merge(TDigest* digest) {
            AvlTree* centroids = digest->centroids();
            for(int n = centroids->first(); n != AvlTree::NIL; n = centroids->nextNode(n)) {
                add(centroids->value(n), centroids->count(n));
            }
        }

        void compress();
        double quantile(double q);

};

#endif
