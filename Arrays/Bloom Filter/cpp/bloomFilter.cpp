#include <iostream>

#include "bloomFilter.h"
#include "../linkedList/linkedListString.h"

using namespace std;

bloomFilter::bloomFilter(int bloomSize)
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

unsigned long djb2(char *str)
{
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
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
    int K = 16;
    for (int i = 0; i < K; i++)
    {
        int bit = hash_i(numberstring, i) % this->getSize();
        this->setBit(bit, 1);
        // cout << "hash_" << i << " of " << numberstring << " is " << bit << endl;
    }
}

int bloomFilter::check(int number)
{
    char numberstring[4];
    sprintf(numberstring, "%d", number);
    int K = 16;
    int flag = 1;
    for (int i = 0; i < K; i++)
    {
        int bit = hash_i(numberstring, i) % this->getSize();
        if (!this->getBit(bit))
        {
            flag = 0;
            break;
        }
        // cout << "hash_" << i << " of " << numberstring << " is " << bit << endl;
    }
    return flag;
}

bloomFilterList::bloomFilterList(int bloomSize) : bloomSize(bloomSize)
{
    this->next = NULL;
    this->bloom = NULL;
    this->virus = NULL;
}

bloomFilterList::bloomFilterList(linkedListStringNode *virus, int bloomSize) : bloomSize(bloomSize), virus(virus)
{
    this->bloom = new bloomFilter(this->bloomSize);
    this->next = NULL;
}

bloomFilterList::~bloomFilterList()
{
    if (this->bloom != NULL)
    {
        delete this->bloom;
    }
    if (this->next != NULL)
    {
        delete this->next;
    }
}

bloomFilterList *bloomFilterList::add(linkedListStringNode *virus)
{
    if (this->virus == NULL)
    {
        this->virus = virus;
        this->bloom = new bloomFilter(this->bloomSize);
        return this;
    }
    bloomFilterList *new_node = new bloomFilterList(virus, this->bloomSize);
    new_node->next = this;
    return new_node;
}

bloomFilter *bloomFilterList::getBloom(linkedListStringNode *virus)
{
    bloomFilterList *temp = this;
    while (temp != NULL)
    {
        if (temp->virus->getString().compare(virus->getString()) == 0)
        {
            return temp->bloom;
        }
        temp = temp->next;
    }
    return NULL;
}