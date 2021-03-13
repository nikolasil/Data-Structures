#define BLOOMFITLER_H
#ifndef BLOOMFILTER_H

class bloomFilter
{
public:
    bloomFilter(int bloomSize, int hash_k);
    ~bloomFilter();

    void setBit(int k, int value);
    int getBit(int k);

    int getSize();
    int getNumberOfHashes();

    void add(int number);
    int check(int number);
    void add(char *str);
    int check(char *str);

private:
    int bloomSize;
    int hash_k;
    char *array;
};

#endif