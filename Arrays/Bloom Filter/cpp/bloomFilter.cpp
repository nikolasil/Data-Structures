#include <iostream>

#include "bloomFilter.h"

using namespace std;

bloomFilter::bloomFilter(int bloomSize, int hash_k) : hash_k(hash_k)
{
    this->bloomSize = bloomSize / sizeof(char) + (bloomSize % sizeof(char) != 0);
    this->array = new char[this->bloomSize];
    for (int i = 0; i < bloomSize; i++)
    {
        this->array[i] = 0;
    }
}

bloomFilter::~bloomFilter()
{
    delete this->array;
}

void bloomFilter::setBit(int k, int value)
{
    int position = (k / (sizeof(char) * 8));
    int shift = (k % (sizeof(char) * 8));
    if (value == 1)
    {
        this->array[position] = this->array[position] | (1 << shift);
    }
    else if (value == 0)
    {
        this->array[position] = this->array[position] & ~(1 << shift);
    }
}

int bloomFilter::getBit(int k)
{
    int position = (k / (sizeof(char) * 8));
    int shift = (k % (sizeof(char) * 8));
    if (this->array[position] & (1 << shift))
    {
        return 1;
    }
    return 0;
}

int bloomFilter::getSize()
{
    return this->bloomSize * sizeof(char) * 8;
}

int bloomFilter::getNumberOfHashes()
{
    return this->hash_k;
}

unsigned long djb2(char *str)
{
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

unsigned long sdbm(char *str)
{
    unsigned long hash = 0;
    int c;

    while (c = *str++)
    {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

unsigned long hash_i(char *str, int i)
{
    return djb2(str) + i * sdbm(str) + i * i;
}

void bloomFilter::add(int number)
{
    char numberstring[4];
    sprintf(numberstring, "%d", number);

    for (int i = 0; i < this->getNumberOfHashes(); i++)
    {
        int bit = hash_i(numberstring, i) % this->getSize();
        this->setBit(bit, 1);
    }
}

int bloomFilter::check(int number)
{
    char numberstring[4];
    sprintf(numberstring, "%d", number);

    int flag = 1;
    for (int i = 0; i < this->getNumberOfHashes(); i++)
    {
        int bit = hash_i(numberstring, i) % this->getSize();
        if (!this->getBit(bit))
        {
            flag = 0;
            break;
        }
    }
    return flag;
}

void bloomFilter::add(char *str)
{
    for (int i = 0; i < this->getNumberOfHashes(); i++)
    {
        int bit = hash_i(str, i) % this->getSize();
        this->setBit(bit, 1);
    }
}

int bloomFilter::check(char *str)
{
    int flag = 1;
    for (int i = 0; i < this->getNumberOfHashes(); i++)
    {
        int bit = hash_i(str, i) % this->getSize();
        if (!this->getBit(bit))
        {
            flag = 0;
            break;
        }
    }
    return flag;
}