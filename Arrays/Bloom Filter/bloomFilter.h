#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include "../linkedList/linkedListString.h"

class bloomFilter
{
public:
    bloomFilter(int bloomSize);
    ~bloomFilter();
    void setBit(int k, int value);
    int getBit(int k);
    int getSize();
    void add(int number);
    int check(int number);

private:
    int bloomSize;
    char *array;
};

class bloomFilterList
{
public:
    bloomFilterList(int bloomSize);
    bloomFilterList(linkedListStringNode *virus, int bloomSize);
    ~bloomFilterList();

    bloomFilterList *add(linkedListStringNode *virus);
    bloomFilter *getBloom(linkedListStringNode *virus);

private:
    int bloomSize;
    linkedListStringNode *virus;
    bloomFilter *bloom;
    bloomFilterList *next;
};

#endif