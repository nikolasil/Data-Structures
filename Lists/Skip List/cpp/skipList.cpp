#include <iostream>

#include "skipList.h"
#include "../../citizenRecords/citizen.h"
#include "../linkedList/linkedListString.h"

using namespace std;

skipListNode::skipListNode(int id, citizenRecord *citizen) : id(id), citizen(citizen)
{
    this->next = NULL;
    this->down = NULL;
    this->citizen = NULL;
}

skipListNode::skipListNode(skipListNode *node)
{
    this->id = node->getId();
    this->citizen = node->getCitizen();
    this->next = NULL;
    this->down = node;
}

skipListNode::~skipListNode()
{
}

void skipListNode::destroy()
{
    if (this != NULL)
    {
        skipListNode *temp = this->next;
        while (temp != NULL)
        {
            skipListNode *next = temp->next;
            delete temp;
            temp = next;
        }
        delete this;
    }
}

void skipListNode::print()
{
    skipListNode *temp = this;
    while (temp != NULL)
    {
        cout << temp->id << " ";
        // cout << "[" << temp->id << "," << temp << "," << temp->down << "]";
        temp = temp->next;
    }
    cout << endl;
}

void skipListNode::setNext(skipListNode *temp)
{
    this->next = temp;
}

void skipListNode::setDown(skipListNode *temp)
{
    this->down = temp;
}

void skipListNode::setId(int id)
{
    this->id = id;
}

void skipListNode::setCitizen(citizenRecord *citizen)
{
    this->citizen = citizen;
}

skipListNode *skipListNode::getNext()
{
    return this->next;
}

skipListNode *skipListNode::getDown()
{
    return this->down;
}

int skipListNode::getId()
{
    return this->id;
}

citizenRecord *skipListNode::getCitizen()
{
    return this->citizen;
}

void skipListNode::add(skipListNode *new_node)
{
    skipListNode *curr = this;
    while (curr->next != NULL && curr->next->id < new_node->id)
    {
        curr = curr->next;
    }
    new_node->next = curr->next;
    curr->next = new_node;
}

// skipListLevel methods implemantation

skipListLevel::skipListLevel(skipListLevel *prevLevel, int l) : myLevel(l)
/* when we make a new level we must make the negative and positive infinity points
to the down levels negative and positive infinity  */
{
    this->list = new skipListNode(NEG_INF, NULL);
    this->pos_inf = new skipListNode(POS_INF, NULL);
    this->list->setNext(this->pos_inf);

    this->list->setDown(prevLevel->getNegInf());
    this->pos_inf->setDown(prevLevel->getPosInf());

    this->downLevel = prevLevel;
    this->upLevel = NULL;
}

skipListLevel::skipListLevel() : myLevel(0) /* this costructor in beeing called only for the 0 level */
{
    this->list = new skipListNode(NEG_INF, NULL);
    this->pos_inf = new skipListNode(POS_INF, NULL);
    this->list->setNext(this->pos_inf);

    this->downLevel = NULL;
    this->upLevel = NULL;
}

skipListLevel::~skipListLevel()
{
}

void skipListLevel::setUpLevel(skipListLevel *up)
{
    this->upLevel = up;
}

void skipListLevel::setDownLevel(skipListLevel *down)
{
    this->downLevel = down;
}

skipListLevel *skipListLevel::getUpLevel()
{
    return this->upLevel;
}

skipListLevel *skipListLevel::getDownLevel()
{
    return this->downLevel;
}

int skipListLevel::getMyLevel()
{
    return this->myLevel;
}

skipListNode *skipListLevel::getList()
{
    return this->list;
}

skipListNode *skipListLevel::getPosInf()
{
    return this->pos_inf;
}

skipListNode *skipListLevel::getNegInf()
{
    return this->list;
}

void skipListLevel::print() /* print only the list of the level */
{
    this->list->print();
}

// skipList methods implemantation

int skipList::coinFlip() /* returns 0 or 1 with 50% change each */
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand((time_t)ts.tv_nsec); // tv_nsec to change the value with nanoseconds so the function dont return the same thing every time

    return rand() % 2;
}

skipList::skipList() /* create the skip list with level 0 that has negative infinity
                        and posotive infinty nodes */
{
    this->floor = new skipListLevel();
    this->ceiling = this->floor;
}

skipList::~skipList() /* delete the all the skip list levels and nodes */
{
    skipListLevel *temp = this->ceiling;
    while (temp != NULL)
    {
        temp->getList()->destroy();
        skipListLevel *next = temp->getDownLevel();
        delete temp;
        temp = next;
    }
}

void skipList::add(int id, citizenRecord *citizen) /* add the id in the skip list
                            if it is the same id no insertion will happen*/
{
    skipListLevel *currLevel = this->ceiling;
    skipListNode *currNode = ceiling->getNegInf();
    skipListNode *nextNode = currNode->getNext();

    skipListNode *prevCreated = NULL;
    int create = 0;
    int heightOfNewNode = 0;
    while (this->coinFlip() && (this->ceiling->getMyLevel() >= heightOfNewNode) && this->ceiling->getMyLevel() < maxLevels - 1)
    {
        heightOfNewNode++;
    }

    while (1)
    {
        if (id < nextNode->getId())
        {
            if (currLevel->getMyLevel() <= heightOfNewNode)
            { // create a node
                if (create)
                {
                    skipListNode *downNode = new skipListNode(id, citizen);
                    prevCreated->setDown(downNode);
                    currNode->setNext(downNode);
                    downNode->setNext(nextNode);
                    prevCreated = downNode;
                }
                else if (!create) // if it is the first time
                {
                    prevCreated = new skipListNode(id, citizen);
                    currNode->setNext(prevCreated);
                    prevCreated->setNext(nextNode);
                    if (currLevel == this->ceiling && heightOfNewNode == this->ceiling->getMyLevel() + 1)
                    { // make a new level
                        skipListLevel *newLevel = new skipListLevel(this->ceiling, this->ceiling->getMyLevel() + 1);
                        this->ceiling->setUpLevel(newLevel);
                        this->ceiling = newLevel;
                        this->ceiling->getList()->add(new skipListNode(prevCreated));
                    }
                    create = 1;
                }
            }

            if (currNode->getDown() != NULL)
            { // if there is a down level
                // step down
                currNode = currNode->getDown();
                nextNode = currNode->getNext();
                currLevel = currLevel->getDownLevel();
            }
            else
            {
                if (!create)
                {
                    prevCreated = new skipListNode(id, citizen);
                    currNode->setNext(prevCreated);
                    prevCreated->setNext(nextNode);
                }
                return;
            }
        }
        else if (id > nextNode->getId())
        {
            currNode = nextNode;
            nextNode = nextNode->getNext();
        }
        else
        {
            return;
        }
    }
}

void skipList::remove(int id)
{
    skipListLevel *currLevel = this->ceiling;
    skipListNode *currNode = ceiling->getNegInf();
    skipListNode *nextNode = currNode->getNext();

    while (1)
    {
        if (id < nextNode->getId())
        {
            if (currNode->getDown() != NULL)
            { // if there is a down level
                // step down
                currNode = currNode->getDown();
                nextNode = currNode->getNext();
                currLevel = currLevel->getDownLevel();
            }
            else
            {
                return;
            }
        }
        else if (id > nextNode->getId())
        {
            currNode = nextNode;
            nextNode = nextNode->getNext();
        }
        else
        {
            int deleteLevel = 0;
            if (currNode == this->ceiling->getNegInf() && nextNode->getNext() == this->ceiling->getPosInf())
            {
                deleteLevel = 1;
            }
            skipListNode *toDelete = nextNode;
            currNode->setNext(nextNode->getNext());
            nextNode = toDelete->getDown();
            delete toDelete;
            if (deleteLevel && this->ceiling != this->floor)
            {
                skipListLevel *temp = this->ceiling->getDownLevel();
                this->ceiling->getList()->destroy();
                delete this->ceiling;
                this->ceiling = temp;
            }
            if (nextNode != NULL)
            {
                currNode = currNode->getDown();
                while (1)
                {
                    if (currNode->getNext() == nextNode)
                    {
                        break;
                    }
                    currNode = currNode->getNext();
                }
                currLevel = currLevel->getDownLevel();
            }
            else
            {
                return;
            }
        }
    }
}

skipListNode *skipList::search(int id, char top_bottom = 't')
{
    skipListLevel *currLevel = this->ceiling;
    skipListNode *currNode = ceiling->getNegInf();
    skipListNode *nextNode = currNode->getNext();

    while (1)
    {
        if (id < nextNode->getId())
        {
            if (currNode->getDown() != NULL)
            { // if there is a down level
                // step down
                currNode = currNode->getDown();
                nextNode = currNode->getNext();
                currLevel = currLevel->getDownLevel();
            }
            else
            {
                return NULL;
            }
        }
        else if (id > nextNode->getId())
        {
            currNode = nextNode;
            nextNode = nextNode->getNext();
        }
        else
        {
            if (top_bottom == 'b')
            {
                while (1)
                {
                    if (nextNode->getDown() == NULL)
                    {
                        break;
                    }
                    nextNode = nextNode->getDown();
                }
            }
            return nextNode;
        }
    }
}

skipListLevel *skipList::getCeiling()
{
    return this->ceiling;
}

skipListLevel *skipList::getFloor()
{
    return this->floor;
}

void skipList::print() /* print all the levels one by one */
{
    skipListLevel *temp = this->ceiling;
    while (temp != NULL)
    {
        temp->print();
        temp = temp->getDownLevel();
    }
}

// skipList methods implemantation

skipList_Lists::skipList_Lists(linkedListStringNode *virus)
{
    this->virus = virus;
    this->vaccinated = new skipList();
    this->notVaccinated = new skipList();
    this->next = NULL;
}

skipList_Lists::~skipList_Lists()
{
    if (this->vaccinated != NULL)
    {
        delete this->vaccinated;
    }
    if (this->notVaccinated != NULL)
    {
        delete this->notVaccinated;
    }
    if (this->next != NULL)
    {
        delete this->next;
    }
}

skipList_Lists *skipList_Lists::add(linkedListStringNode *virus)
{
    if (this->virus == NULL)
    {
        this->virus = virus;
        this->vaccinated = new skipList();
        this->notVaccinated = new skipList();
        return this;
    }
    skipList_Lists *new_node = new skipList_Lists(virus);
    new_node->next = this;
    return new_node;
}

skipList *skipList_Lists::getVaccinated(linkedListStringNode *virus)
{
    skipList_Lists *temp = this;
    while (temp != NULL)
    {
        if (temp->virus->getString().compare(virus->getString()) == 0)
        {
            return temp->vaccinated;
        }
        temp = temp->next;
    }
    return NULL;
}

skipList *skipList_Lists::getNotVaccinated(linkedListStringNode *virus)
{
    skipList_Lists *temp = this;
    while (temp != NULL)
    {
        if (temp->virus->getString().compare(virus->getString()) == 0)
        {
            return temp->notVaccinated;
        }
        temp = temp->next;
    }
    return NULL;
}