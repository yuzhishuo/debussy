#include "determin_skip_list.h"

using namespace debussy; 

template <typename ElemType>
SkipNode<ElemType>::SkipNode() : right(nullptr), down(nullptr) {}

template <typename ElemType>
SkipNode<ElemType>::SkipNode(const ElemType& key_input, SkipNode* right_input,
    SkipNode* down_input)
    : key(key_input), right(right_input), down(down_input) {}

template <typename ElemType>
DeterminSkipList<ElemType>::DeterminSkipList(const ElemType& max, const ElemType& max_1)
    : head(nullptr), bottom(nullptr), tail(nullptr), Max(max), Max_1(max_1)
{
    Init();
}

template <typename ElemType>
DeterminSkipList<ElemType>::~DeterminSkipList()
{
    if (head) {
        Clear();
    }
}

template <typename ElemType>
void DeterminSkipList<ElemType>::Init()
{
    //init head bottom and tail see paper p372 figure 6
    bottom = new SkipNode<ElemType>;
    bottom->right = bottom->down = bottom;

    tail = new SkipNode<ElemType>(Max_1);
    tail->right = tail;

    head = new SkipNode<ElemType>(Max, tail, bottom);
}

template <typename ElemType>
void DeterminSkipList<ElemType>::Clear()
{
    ClearHelp(head->down);
    delete head;
    delete bottom;
    delete tail;
    head = nullptr;
    bottom = nullptr;
    tail = nullptr;
}

template <typename ElemType>
void DeterminSkipList<ElemType>::ClearHelp(SkipNode<ElemType>* current)
{
    SkipNode<ElemType>* temp;
    if (current == bottom)
        return;
    ClearHelp(current->down);
    while (current != tail) {
        temp = current;
        current = current->right;
        delete temp;
    }
}

template <typename ElemType>
inline bool DeterminSkipList<ElemType>::Search(const ElemType& value) const
{
    return ((SearchHelp(value) == bottom) ? false : true);
}

template <typename ElemType>
SkipNode<ElemType>* DeterminSkipList<ElemType>::
    SearchHelp(const ElemType& value) const
{
    SkipNode<ElemType>* current;

    current = head;
    while (current != bottom) {
        while (value > current->key)
            current = current->right;
        if (current->down == bottom)
            return ((value == current->key) ? current : bottom);
        current = current->down;
    }
    return nullptr;
}

/**
 * Insert a key value to the skip list.
 * if that the key value has already existed.
 * return false and will not insert.
 * else insert and return true.
 */
template <typename ElemType>
bool DeterminSkipList<ElemType>::Insert(const ElemType& value)
{
    SkipNode<ElemType>* current, * new_node;

    current = head;
    bottom->key = value;
    bool can_insert = true;
    while (current != bottom) {
        while (value > current->key)
            current = current->right;
        // if exists one elem whose key == value return false do not need insert
        if ((current->down == bottom) && (value == current->key)) {
            can_insert = false; //can not insert but should check if need to adjust head before return
            break;
        }

        // if gap size is 3 or at bottom level and must insert
        // then promote the middle element
        if ((current->down == bottom)
            || (current->key > current->down->right->right->key)) {
            new_node = new SkipNode<ElemType>;
            new_node->right = current->right;
            new_node->down = current->down->right->right;
            current->right = new_node;
            new_node->key = current->key;
            current->key = current->down->right->key;
        }
        current = current->down;
    }
    // if need to raise the height of the skip list, raise it
    if (head->right != tail) {
        new_node = new SkipNode<ElemType>;
        new_node->down = head;
        new_node->right = tail;
        new_node->key = Max;
        head = new_node;
    }

    return can_insert;
}
template <typename ElemType>
void DeterminSkipList<ElemType>::FindAndModifyRemoveHelp(const ElemType& value, const ElemType& other_value)
{
    SkipNode<ElemType>* current;

    current = head;
    while (current != bottom) {
        while (value > current->key)
            current = current->right;
        if (value == current->key)
            current->key = other_value;
        current = current->down;
    }

}
template <typename ElemType>
void DeterminSkipList<ElemType>::LowerHeadRemoveHelp()
{
    SkipNode<ElemType>* temp;
    if (head->down->right == tail) {
        temp = head->down;
        head->down = temp->down;
        delete temp;
    }
}
template <typename ElemType>
bool DeterminSkipList<ElemType>::Remove(const ElemType& value)
{
    //empty list can not remove
    if (head->down == bottom)
        return false;
    //current node, previous node of current, start mark the begining of each level when the node goes down
    //if previous finally equal to start means have not advanced toward right on this level
    //next save the start position of the next level for current node
    SkipNode<ElemType>* current, * previous, * temp, * next;

    current = head->down;
    int visit_num = 0;
    bool can_remove = true;
    for (;;) {  //loop and will return when curent->down == bottom
        previous = nullptr;
        while (value > current->key) {   //try advance toward right as far as possible
            previous = current;
            current = current->right;
        }
        //visit num will mark if a node is a height 1 node
        if (value == current->key)
            visit_num++;

        //if on level 1 try to remove if possible and return
        if (current->down == bottom) {
            if (visit_num == 0) {       //can not find do not  need to remove
                LowerHeadRemoveHelp(); ///might need to lower the head
                can_remove = false;
                return false;
            }
            else { //need to remove
                if (visit_num == 1) {  //if node height == 1 just remove it
                    //delete need to consider down fild of the upper level
                    //so swap current and current->right
                    temp = current->right; //to be deleted
                    current->key = current->right->key;
                    current->right = temp->right;
                    delete temp;
                    LowerHeadRemoveHelp(); ///might need to lower the head
                }
                else {
                    //if current node height > 1, swap the key with neighbour than delete the neighbour
                    //here swap with left neigbour,notice we must have advaced right on
                    //this level otherwise it must be a height 1 node.
                    //we will find all the node with key value current->key and modify it
                    //to previous->key
                    LowerHeadRemoveHelp(); //need to make sure the sturcture correct first
                    FindAndModifyRemoveHelp(current->key, previous->key);
                    previous->right = current->right;
                    delete current;
                }
                return true;
            }
        }
        //on other levels ,not level 1
        //save the postion to go down on the lower level
        next = current->down;  //the next level will be unchaged when dealing the current level
        //current->key == current->down->right->key means the gap we want to down
        //is of size only 1
        if (current->key == current->down->right->key) {
            //the first gap case, consider current gap and the next
            if (previous == nullptr) {  //have not advanced on this level
                //if the next gap has 2 or more one level lower element
                //one element need to lower down,the other one need to raise one level,
                //just swap do not need to delte space
                if (current->right->key > current->right->down->right->key) {
                    current->key = current->right->down->key;
                    current->right->down = current->right->down->right;
                }
                else {
                    //one element need to lower down,need to delete space
                    temp = current->right;
                    current->key = temp->key;
                    current->right = temp->right;
                    delete temp;
                }
            }
            else {    //not the first so consider the privious gap and the current
                if (previous->key > previous->down->right->key) {
                    temp = previous->down->right;
                    if (temp->right->key != previous->key)
                        temp = temp->right;
                    previous->key = temp->key;
                    current->down = temp->right;
                }
                else {
                    previous->key = current->key;
                    previous->right = current->right;
                    delete current;
                }
            }
        }
        current = next; //go to the next level (one level lower)
    }
}

template <typename ElemType>
bool DeterminSkipList<ElemType>::IsValid() const
{
    SkipNode<ElemType>* current, * temp, * temp2;
    int gap_size;

    current = head;
    while (current->down != bottom) {
        temp = current;
        while (temp != tail) {
            temp2 = temp->down;
            gap_size = 0;
            while (temp2->key != temp->key) {
                gap_size++;
                temp2 = temp2->right;
            }
            if (gap_size < 1 || gap_size > 3)
                return false;
            temp = temp->right;
        }
        current = current->down;
    }
    return true;
}