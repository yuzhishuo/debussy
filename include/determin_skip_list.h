#ifndef _DETERMIN_SKIP_LIST_H
#define _DETERMIN_SKIP_LIST_H

#include <iostream>
#include <type_traits>

#include "utility.h"

namespace debussy {

    /**
     * @brief class SkipNode.
     * the node type for DeterminSkipList to operate.
     */
    template <typename ElemType>
    class SkipNode {

        static_assert(!std::is_abstract<ElemType>::value, "unspport abstract class.");
        static_assert(
            std::is_copy_assignable<ElemType>::value
        || std::is_trivially_copy_assignable<ElemType>::value 
        || std::is_nothrow_copy_assignable<ElemType>::value, 
        "ElemType need assignable constructor function.");

    public:
        template <typename T>  friend class DeterminSkipList;
        SkipNode();
        SkipNode(const ElemType& key_input, SkipNode* right_input = nullptr,
            SkipNode* down_input = nullptr);

    DISALLOW_COPY_AND_ASSIGN(SkipNode); /** Do not all copy and assign */

    private:
        ElemType key;       /**< The key is what we use for comparing during Search,Insert,Remove.*/
        SkipNode* right;    /**< Point to the right skip node ont the same level*/
        SkipNode* down;     /**< Point to the lower level skip node where we will go down from current node */
    };

    /**
     * @brief DeterminSkipList
     * An implementaion of skip list using 1-2-3 skip list
     */
    template <typename ElemType>
    class DeterminSkipList {
    public:

        /** 
        * @brief Client should determin Max and Max + 1 value.
        * @param max    maximum
        * @param max_1  maximum - 1     
        */
        DeterminSkipList(const ElemType& max, const ElemType& max_1);
        ~DeterminSkipList();
        
        void Clear();

        /** 
        * @brief Searches for the presence of the specified element in the skip list.
        */    
        bool Search(const ElemType& value) const;
        
        /**
         * @brief If the list already has one node whose key == value
         * return false and do not do insert otherwise insert a new node with
         * its key == value and return true
         */
        bool Insert(const ElemType& value);
        /**
         * @brief If the list already has one node whose key == value
         * remove it and return true otherwise return false
         */
        bool Remove(const ElemType& value);

        /** @brief Return false if current structure of the skip list is wrong*/
        bool IsValid() const;

        /** For simplicty here do not allow copy and assign*/
        DISALLOW_COPY_AND_ASSIGN(DeterminSkipList);

    private:
        void Init();
        void ClearHelp(SkipNode<ElemType>* current);
        /**
         * Helper function for Search
         * If could not find one node with key value == item return bottom
         * otherwise return postion of node containing the key
         * */
        SkipNode<ElemType>* SearchHelp(const ElemType& value) const;
        /** Helper function for Remove, find all the nodes with key == value and modify it to other_value*/
        void FindAndModifyRemoveHelp(const ElemType& value, const ElemType& other_value);
        /** lower the head if possible during Remove help to keep skip list structure consistent*/
        void LowerHeadRemoveHelp();

    private:
        //actual data skip list store
        SkipNode<ElemType>* head;   /**< where we start*/
        SkipNode<ElemType>* bottom; /**< mark if we have down to the lowerest level*/
        SkipNode<ElemType>* tail;   /**< mark the end of each level*/

        const ElemType Max;         /**< Max > any client input key value*/
        const ElemType Max_1;       /**< Max_1 > Max*/
    };

} //ns skip_list

#include "determin_skip_list.hpp"
#endif // _DETERMIN_SKIP_LIST_H