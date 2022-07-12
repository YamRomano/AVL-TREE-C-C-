// Do not change the next 13 lines
#include "avl.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum Position { LEFT, RIGHT } Position;  // TIP: use this in your code!

int max2(int x, int y) {
    return (x < y) ? y : x;
}

int abs(int x) {
    return (x < 0) ? -x : x;
}

// Fill your info here:
/****
  Student1 name: YAM ROMANO

  Student1 ID: 327054847

****/

//MY DEFINE
#define LEFTSON (tnode->child[LEFT])
#define LEFTSON_H (tnode->child[LEFT] ? tnode->child[LEFT]->height:NULLHEIGHT)
#define LEFTSON_S (tnode->child[LEFT] ? tnode->child[LEFT]->size:0) 

#define RIGHTSON (tnode->child[RIGHT])
#define RIGHTSON_H (tnode->child[RIGHT] ? tnode->child[RIGHT]->height:NULLHEIGHT)
#define RIGHTSON_S (tnode->child[RIGHT] ? tnode->child[RIGHT]->size:0)

#define NULLHEIGHT -1

int abs(int x);
int max2(int x, int y);

//MY DECLARATIONS
AVLNodePtr leftRotate(AVLNodePtr tnode);
AVLNodePtr rightRotate(AVLNodePtr tnode);
int isBalanced(AVLNodePtr tnode);
AVLNodePtr findMin(AVLNodePtr tnode);
AVLNodePtr findMax(AVLNodePtr tnode);
AVLNodePtr potanPapa(AVLNodePtr tnode, int k);
AVLNodePtr potanPapaAux(AVLNodePtr tnode, int k);
int lastMisRec(AVLNodePtr tnode, AVLNodePtr max, AVLNodePtr min, AVLNodePtr papa);
int rank(AVLNodePtr tnode, int k, int exist);
AVLNodePtr insert(AVLNodePtr tnode, int k);
AVLNodePtr delete(AVLNodePtr tnode, int k);

// to delete 
void my_test();
void preOrder(struct Node* root);


// Operations

//==================== AVL SEARCH =====================
/*
This recursive function gets root node and key we would like to find
It's checking if the current root key is larger tnan the given one on each level.
TIME COMPLEXITY WORST CASE: O(Log(n))
 */
AVLNodePtr avl_search(AVLNodePtr tnode, int k)
{
    if (!tnode)
        return NULL;
    if (tnode->key == k)
        return tnode;
    if (tnode->key > k)
        return avl_search(LEFTSON, k);
    else
        return avl_search(RIGHTSON, k);
}
//==================== AVL INSERT =====================
/*
This function using next functions: avl_search(O(Log(n)) - once , avl_insert(O(Log(n)) - once.
This Aux function gets the root and key of the node we would liike to insert.
It's checking if the tree is empty, and if the key is already exists.
If both are false, it's calling to insert function to add the k node.
TIME COMPLEXITY WORST CASE: avl_search-once:O(Log(n) +  avl_insert(O(Log(n)) - once = O(2Log(n))~ O(Log(n)).
*/
AVLNodePtr avl_insert(AVLNodePtr tnode, int k)
{
    if (!tnode)
        return new_avl_node(k);
    if (avl_search(tnode, k))
    {
        printf("This key is already exists in the tree.");
        return -1;
    }
    return insert(tnode, k);
}
/*
This function using next functions: isBalanced(O(1), rightRotate(O(1)), leftRotate(O(1)), max2(O(1)).
This recursive function gets root node and key (thats not in the tree) we would like to add.
Then, it's finding the righ place for the k node , adding new node and adjusts it's members.
The function is also checking if the current level is balanced and performing rotations if needed.
The recurtion is going over each level so at the exit from the function we can be sure that the tree is balanced as should.
TIME COMPLEXITY WORST CASE:is balanced(O(1)) + recurtion depth (at worst case in each level we would perform rotation, while
    the complaxity of rotation is O(1): O(Log(n) =  O(Log(n))).
 */
AVLNodePtr insert(AVLNodePtr tnode, int k) {

    if (k < tnode->key)
        LEFTSON = avl_insert(LEFTSON, k);
    else if (k > tnode->key)
        RIGHTSON = avl_insert(RIGHTSON, k);
    else
        return tnode;
    tnode->height = (1 + max2(LEFTSON_H, RIGHTSON_H));
    tnode->size = (1 + (LEFTSON_S + RIGHTSON_S));

    int balanced = isBalanced(tnode);

    //-------LEFT-LEFT CASE-----------
    if ((balanced > 1) && (k < LEFTSON->key))
        return rightRotate(tnode);
    //-------RIGHT-RIGHT CASE---------
    if ((balanced < -1) && (k > RIGHTSON->key))
        return leftRotate(tnode);
    //-------LEFT-RIGHT CASE----------
    if ((balanced > 1) && (k > LEFTSON->key))
    {
        LEFTSON = leftRotate(LEFTSON);
        return rightRotate(tnode);
    }
    //-------RIGHT-LEFT CASE-----------
    if ((balanced < -1) && (k < RIGHTSON->key))
    {
        RIGHTSON = rightRotate(RIGHTSON);
        return leftRotate(tnode);
    }
    return tnode;// NO ROTATIONS NEEDED - THE TREE IS BALANCED
}
//==================== AVL DELETE =====================
/*
This function using next functions: findMax-once / findMin- once (complets the recurtion to the bottom so doesn't
    encreases the time complexity), isBalanced(O(1), rightRotate(O(1)), leftRotate(O(1)), max2(O(1)).
This recursive function gets root node and key we would like to delete from the tree.
It's checking if the tree is empty and is the key already exists.
Then, it's finding the k node (that exists), and checking wich case is it : leaf case or more then one child case.
Then, it's deleting the k node as explainde below.
The function is also checking if the current level is balanced and performing rotations if needed.
The recurtion os going over each level so at the exit from the function we can be sure that the tree is balanced as should.
TIME COMPLEXITY WORST CASE: is balanced(O(1)) + recurtion depth (at worst case in each level we would perform rotation, while
    the complaxity of rotation is O(1) and findMax-once / findMin- once: O(Log(n) =  O(Log(n))).
 */
AVLNodePtr delete(AVLNodePtr tnode, int k)
{
    //THE SEARCH FOR THE WANTED NODE
    if (!tnode)
        return tnode;

    if (k < tnode->key)
        LEFTSON = avl_delete(LEFTSON, k);
    else if (k > tnode->key)
        RIGHTSON = avl_delete(RIGHTSON, k);
    //IF WE FOUND THE WANTED NODE
    else {
        if (!LEFTSON || !RIGHTSON)//IF THE K NODE HAS ONE OR LESS CHILDREN
        {
            AVLNodePtr temp = LEFTSON ? LEFTSON : RIGHTSON;//WICH CHILD IS EXISTS
            if (!temp) //IF K NODE IS A LEAF
            {
                temp = tnode;
                tnode = NULL;
            }
            else  //CONECT THE CHILS TO K NODE FATHER AND DELETE THE K NODE
            {
                *tnode = *temp;
                free(temp);
            }
        }
        else//IF THE K NODE HAS MORE THAN ONE CHILD
        {
            //WE WILL CHECK WICH SIDE IS HIGHER TO PERFORM LESS ROTATIONS IN BALANCE
            if (LEFTSON_H >= RIGHTSON_H)
            {
                AVLNodePtr temp = findMax(LEFTSON); /* ALL THE LET SIDE TREE IS SMALLER THAN K NODE SO WE WILL FIND THE MAX OF IT
                                        SO IT WOULD PERFORM AS NEW ROOT OF THIS SUB-TREE (LARGER THAN LEFT SIDE AND SMALLER THAN THE RIGHT SIDE)*/
                tnode->key = temp->key;
                LEFTSON = avl_delete(LEFTSON, temp->key);// LEAF CASE
            }
            else//SIMTRIC TO THE LEFT SIDE
            {
                AVLNodePtr temp = findMin(RIGHTSON);
                tnode->key = temp->key;
                RIGHTSON = avl_delete(RIGHTSON, temp->key);// LEAF CASE
            }
        }

    }
    // CHECK IF THERE IS STILL A TREE AFTER DELLITING K NODE
    if (!tnode)
        return tnode;

    // COMPUTES NEW HIGHT OF CURRENT ROOT
    tnode->height = max2(LEFTSON_H, RIGHTSON_H) + 1;

    // CHECKS IF CURRENT LEVEL IS BALANCED
    int balanced = isBalanced(tnode);

    //-------RIGHT-RIGHT CASE---------
    if (balanced > 1 && isBalanced(LEFTSON) >= 0)
        return rightRotate(tnode);

    //-------LEFT-LEFT CASE---------
    if (balanced > 1 && isBalanced(LEFTSON) < 0)
    {
        LEFTSON = leftRotate(LEFTSON);
        return rightRotate(tnode);
    }
    //-------LEFT-RIGHT CASE----------
    if (balanced < -1 && isBalanced(RIGHTSON) <= 0)
        return leftRotate(tnode);

    //-------RIGHT-LEFT CASE-----------
    if (balanced < -1 && isBalanced(RIGHTSON) > 0)
    {
        RIGHTSON = rightRotate(RIGHTSON);
        return leftRotate(tnode);
    }
    else
        tnode->size--;// EATHER WAY REDUCE THE CURRENT ROOT SIZE

    return tnode;// NO ROTATIONS NEEDED - THE TREE IS BALANCED
}
/*
This AUX function using next functions: avl_search(O(Log(n)) - once , avl_delete(O(Log(n)) - once.
This Aux function gets the root and key of the node we would like to delete.
It's checking if the tree is empty, and if the key is already exists.
If both are false, it's calling to insert function to add the k node.
TIME COMPLEXITY WORST CASE: avl_search-once:O(Log(n) +  avl_delete(O(Log(n)) - once = O(2Log(n))~ O(Log(n)).
*/
AVLNodePtr avl_delete(AVLNodePtr tnode, int k) {
    if (!avl_search(tnode, k))
    {
        printf("This key doesn't exist in the tree.");
        return -1;
    }
    return delete(tnode, k);
}
//========================================= NEW AVL NODE =========================================================
/*
This function doesn't using other functions.
This function gets the key of the node we would like to create.
It's adjusting new node's parameters and returns the referance to this new node.
TIME COMPLEXITY WORST CASE: O(6) ~ O(1).
*/
AVLNodePtr new_avl_node(int k) {
    AVLNodePtr avl = (AVLNodePtr)malloc(sizeof(AVLNode));
    avl->height = 0;
    avl->size = 1;
    avl->child[LEFT] = NULL;
    avl->child[RIGHT] = NULL;
    avl->key = k;
    return avl;
}
//==================== DELETE AVL TREE =====================
/*
This recursive function doesn't using other functions.
This function gets the root of the tree we would like to delete.
It's going to the bottom and freeing yeach sub-root so we wouldn't lost data.
TIME COMPLEXITY WORST CASE: O(n), we should delete each node separately.
*/
void delete_avl_tree(AVLNodePtr tnode) {
    if (!tnode)
        return;
    delete_avl_tree(LEFTSON);
    delete_avl_tree(RIGHTSON);
    free(tnode);
}

// Queries
//==================== LAST NISSING =====================
/*
This AUX function using next functions: findMax (O(Log(n)))- once, findMin (O(Log(n))) - once, lastMisRec (O(Log(n))).
This function gets the root of the tree.
It's finding the largest node and smallest node and checking if the difference between their keys less than tree size.
If ir doesnt, the function calling to lastMisRec to find the hole.
TIME COMPLEXITY WORST CASE: findMax O(Log(n)) + findMin O(Log(n)) + lastMisRec O(Log(n)) = O(3Log(n)) ~ O(Log(n)).
*/
int last_missing(AVLNodePtr tnode)
{
    int missing = 0;
    if (!tnode) return 1;
    AVLNodePtr max = findMax(tnode);
    AVLNodePtr min = findMin(tnode);
    if (!max && !min)
        return (tnode->key - 1);

    if (tnode->size >= ((max->key) - (min->key) + 1))// IF IT'S TRUE, THERE IS NO HOLES IN THE TREE
    {
        return (min->key - 1);
    }
    else // NOW WE KNOW THERE IS MINIMUM ONE HOLE, SO WE SHOULD FIND WHERE.
    {

        missing = lastMisRec(tnode, max, min, tnode);
        return missing;
    }
}
/*
This recursive function using next functions: abs (O(1)).
This function gets the root of the tree and referance to max node in the current sub-tree.
It's going to the bottom while checking each on each level if there is a hole in the right sub-tree (we want the "LAST missing")
    and if there isn't, we will look in the left sub-tree.
TIME COMPLEXITY WORST CASE: O(Log(n)) as the recurtion depth.
*/
int lastMisRec(AVLNodePtr tnode, AVLNodePtr max, AVLNodePtr min, AVLNodePtr papa)
{

    if (!tnode) return 0;// Empty tree case
    if (((abs(max->key - tnode->key)) - 1) >= (RIGHTSON_S))
    {
        if ((tnode->size == 1) && (tnode->key < papa->key))//Left Leaf Case//
        {
            if ((papa->key - tnode->key) == 1 && (abs(tnode->key - min->key)) > 1) return ((min->key) + 1);
            if ((papa->key - tnode->key) == 1 && (abs(tnode->key - min->key)) == 1) return ((papa->key) + 1);
            if ((papa->key - tnode->key) > 1) return ((tnode->key) + 1);

        }

        if ((tnode->size == 1) && (tnode->key > papa->key))//Right Leaf Case//
        {  
            if ((tnode->key - papa->key) == 1 && (abs(tnode->key - max->key)) > 1) return ((max->key) - 1);
            if ((tnode->key - papa->key) == 1 && (abs(tnode->key - max->key)) == 1) return ((papa->key) - 1);
            if ((tnode->key - papa->key) > 1) return ((tnode->key) - 1);
        }

        if ((tnode->size == 1) && (tnode->key > papa->key)) return ((tnode->key) - 1);//Right Leaf Case//+1
        if ((tnode->size == 1) && (tnode->key > papa->key)) return ((tnode->key) - 1);//Right Leaf Case//+1

        if (!RIGHTSON) return ((tnode->key) - 1);
        lastMisRec(RIGHTSON, max, tnode, tnode);
    }
    else
    {
        if (!LEFTSON) return ((tnode->key) - 1);
        lastMisRec(LEFTSON, tnode, min, tnode);
    }
}
//==================== AVL RANK =====================
/*
This Aux function using next functions: avl_search (O(Log(n))), potanPapaAux (O(Log(n))), rank (O(Log(n))) .
This function gets the root of the tree and key of the node we would like to know it's rank.
We check if the k node is i the tree. If it is, we calling to rank function with k.
If k node doesn't in the tree, we look for it's potantional father node, checking if k would be it's lef son or right son and calling
    to rank function with father key.
TIME COMPLEXITY WORST CASE: avl_search O(Log(n)) + potanPapaAux O(Log(n)) + rank O(Log(n)) =  O(3Log(n)) ~ O(Log(n)).
*/
int avl_rank(AVLNodePtr tnode, int k)
{
    int exist = 1;
    if (!avl_search(tnode, k))
    {
        exist = 0;
        AVLNodePtr papa = potanPapaAux(tnode, k);
        if (k < papa->key) return rank(tnode, papa->key, exist);
        else return rank(tnode, papa->key, 0) + 1;
    }
    return rank(tnode, k, 1);
}
/*
This recursive function doesn't use other functions.
This function gets the root of the tree key of wanted node and if Knode is in the tree.
It's going to the bottom while checking each on each level if the key larger/ smaller than the current root key,
    and adding leftson rank (the number of nodes smaller than k) each time we go right in the tree .
It's also checking if we got to the right node and returning it's leftson rank (number of nodes smaller than k).
TIME COMPLEXITY WORST CASE: O(Log(n)) as the recurtion depth.
*/
int rank(AVLNodePtr tnode, int k, int exist)
{
    if (!tnode)  return 0;
    //FOUND THE K NODE
    if (k == tnode->key && exist) return (LEFTSON_S + 1);// IS THE TNODE SMALLER OR LARGER THAN K 
    else if (k == tnode->key && !exist) return (LEFTSON_S);
    //TURNING LEFT
    if (k > tnode->key)  return (rank(RIGHTSON, k, exist) + LEFTSON_S + 1);
    //TURNING RIGHT
    else return  (rank(LEFTSON, k, exist));
}
//==================== AVL REVERESED RANK =====================
/*
This function based on avl_rank function(O(Log(n))).
It's also using avl_search (O(Log(n))) - once.
We check if the wanted node is in the tree so we would know if its own value counts in the rank calculatons.
We know the number of node in the tree, so we substract regular rank from the tree size.
TIME COMPLEXITY WORST CASE: avl_search (O(Log(n))) - once + avl_rank O(Log(n)) =  O(2Log(n)) ~ O(Log(n)).
*/
int avl_reversed_rank(AVLNodePtr tnode, int k)
{
    if ((!avl_search(tnode, k)))
        return ((tnode->size) - avl_rank(tnode, k));
    return ((tnode->size) - avl_rank(tnode, k) + 1);
}
//==================== AVL HOW MANY =====================
/*
This function based on avl_rank function (O(Log(n))).
It's also using avl_search (O(Log(n))) - twice and abs O(1).
We check if the wanted node is in the tree so we would know if its own value counts in the "how many" calculatons.
If both nodes are in the tree, we return the differance between their ranks.
Else, we return the differace between fathers ranks.
TIME COMPLEXITY WORST CASE: Log(n) * abs O(1) + 2 * avl_search (O(Log(n))) + avl_rank O(Log(n)) =  O(4Log(n)) ~ O(Log(n)).
*/
int avl_HowMany(AVLNodePtr tnode, int x, int y)
{
    if (x > y)//SWAP SO Y BIGGER THAN X
    {
        int temp = x;
        x = y;
        y = temp;
    }
    int papaR = 0;
    // x not in the tree
    if (!avl_search(tnode, x))
    {
        papaR = avl_rank(tnode, x);
        return abs(avl_rank(tnode, y) - papaR);

    }
    //y not in the tree
    if (!avl_search(tnode, y))
    {
        papaR = avl_rank(tnode, y);
        return abs(avl_rank(tnode, x) - papaR) + 1;
    }

    return (1 + avl_rank(tnode, y) - avl_rank(tnode, x));
}
/**************************************************************************************************
*                                   MY ADDITIONAL FUNCTIONS
***************************************************************************************************/

//========================== POTANTIONAL PAPA =============================
/*
This function based on potanPapa function (O(Log(n))).
It's also using findMin (O(Log(n))) and findMax (O(Log(n))).
We the min node and max node in the tree, and calling to potanPapa.
TIME COMPLEXITY WORST CASE: findMin (O(Log(n))) + findMax (O(Log(n))) + potanPapa O(Log(n)) =  O(3Log(n)) ~ O(Log(n)).
*/
AVLNodePtr potanPapaAux(AVLNodePtr tnode, int k)
{
    AVLNodePtr max = findMax(tnode);
    AVLNodePtr min = findMin(tnode);
    return potanPapa(tnode, k, min, max);
}
/*
This recursive function doesnt use other functions.
It's searching for the first node, in rute from the root to the place K should be , that could be potantial father.
Also, important to see that the max of left sub-tree us the root itself.
TIME COMPLEXITY WORST CASE:  O(Log(n)) as the recurtion depth.
*/
AVLNodePtr potanPapa(AVLNodePtr tnode, int k, AVLNodePtr min, AVLNodePtr max)
{
    if (!tnode) return NULL;
    if (k > max->key) return max;
    if (k < min->key) return min;
    if (!LEFTSON && tnode->key > k) return tnode;
    if (!RIGHTSON && tnode->key < k) return tnode;
    if (tnode->key > k) potanPapa(LEFTSON, k, min, tnode);
    else if ((LEFTSON ? LEFTSON->key : 0) < k) potanPapa(RIGHTSON, k, tnode, max);
}
//========================== LEFT ROTATION =============================
/*
This function using next functions: max2 O(1).
It's performs swap algorithm of left rotation as studied at the lectures.
Also, it's adjusting all the member values at current level.
TIME COMPLEXITY WORST CASE: ~ O(1).
*/
AVLNodePtr leftRotate(AVLNodePtr tnode)
{
    AVLNodePtr rightSon = RIGHTSON;
    AVLNodePtr rightLeftSon = rightSon->child[LEFT];
    rightSon->child[LEFT] = tnode;
    RIGHTSON = rightLeftSon;
    tnode->height = (max2(LEFTSON_H, RIGHTSON_H) + 1);
    rightSon->height = max2(rightSon->child[LEFT] ? rightSon->child[LEFT]->height : NULLHEIGHT, rightSon->child[RIGHT] ? rightSon->child[RIGHT]->height : NULLHEIGHT) + 1;
    tnode->size = (LEFTSON_S + RIGHTSON_S + 1);
    rightSon->size = ((rightSon->child[LEFT] ? rightSon->child[LEFT]->size : 0) + (rightSon->child[RIGHT] ? rightSon->child[RIGHT]->size : 0) + 1);
    return rightSon;
}

//========================== RIGHT ROTATION =============================
/*
This function using next functions: max2 O(1).
It's performs swap algorithm of right rotation as studied at the lectures.
Also, it's adjusting all the member values at current level.
TIME COMPLEXITY WORST CASE: ~ O(1).
*/
AVLNodePtr rightRotate(AVLNodePtr tnode)
{
    AVLNodePtr leftSon = LEFTSON;
    AVLNodePtr leftRightSon = leftSon->child[RIGHT];
    leftSon->child[RIGHT] = tnode;
    LEFTSON = leftRightSon;
    tnode->height = (max2(LEFTSON_H, RIGHTSON_H) + 1);
    leftSon->height = max2((leftSon->child[LEFT] ? leftSon->child[LEFT]->height : NULLHEIGHT), (leftSon->child[RIGHT] ? leftSon->child[RIGHT]->height : NULLHEIGHT)) + 1;
    tnode->size = (LEFTSON_S + RIGHTSON_S + 1);
    leftSon->size = (((leftSon->child[LEFT] ? leftSon->child[LEFT]->size : 0) + (leftSon->child[RIGHT] ? leftSon->child[RIGHT]->size : 0)) + 1);
    return leftSon;
}
//========================== AVL BALANCED =============================
/*
This function returns hight differances between left sub-tree and right sub-tree, if exists.
TIME COMPLEXITY WORST CASE: ~ O(1).
*/
int isBalanced(AVLNodePtr tnode)
{
    if (!tnode)
        return 0;
    return ((LEFTSON_H)-(RIGHTSON_H));
}

//========================== FIND MINIMUM / FIND MAXIMUM =============================
/*
This function returns the leftmost node.
TIME COMPLEXITY WORST CASE: O(Log(n)).
*/
AVLNodePtr findMin(AVLNodePtr tnode)
{
    if (!tnode) return tnode;
    while (LEFTSON)
    {
        tnode = LEFTSON;
    }
    return tnode;
}
/*
This function returns the rightmost node.
TIME COMPLEXITY WORST CASE : O(Log(n)).
*/
AVLNodePtr findMax(AVLNodePtr tnode)
{
    if (!tnode) return tnode;
    while (RIGHTSON)
    {
        tnode = RIGHTSON;
    }
    return tnode;
}
/*
typedef enum { FAILED, PASSED } TestResult;

void avl_test(int n);
int avl_test_recurse(AVLNodePtr tnode, TestResult* res);
void insert_delete_search_test(int n);
void rank_reversed_test();
void last_missing_test();
void howmany_test();

void printTree(AVLNodePtr node) {
    if (!node) return;
    printTree(node->child[RIGHT]);
    printf("%d, ", node->key);
    printTree(node->child[LEFT]);
}
void avl_test(int n) {
    int i = 1;
    TestResult res = PASSED;
    AVLNodePtr root = NULL;
    for (i = 1; i <= n; ++i)
        root = avl_insert(root, i);
    avl_test_recurse(root, &res);
    if (res == FAILED)
        printf("AVL test failed.\n");
    else
        printf("AVL test passed.\n");
    delete_avl_tree(root);
}

int avl_test_recurse(AVLNodePtr tnode, TestResult* res) {
    int h_left, h_right;
    if (!tnode)
        return -1;
    h_left = avl_test_recurse(tnode->child[LEFT], res);
    h_right = avl_test_recurse(tnode->child[RIGHT], res);
    *res = (abs(h_left - h_right) > 1) ? FAILED : *res;
    if ((abs(h_left - h_right)) > 1)
        printf("failed at %d\n", tnode->key);
    return 1 + max(h_left, h_right);
}


void insert_delete_search_test(int n) {
    int i = 1;
    AVLNodePtr node = NULL, root = NULL;
    TestResult res = PASSED;
    for (i = 1; i < n; ++i) {
        int toInsert = (((int)(3 / 10 * n) + i) % n) + 1;
        root = avl_insert(root, toInsert);
    }
    avl_test_recurse(root, &res);
    for (i = 1; i < n; ++i) {
        int toSearch = (((int)(5 / 9 * n) + i) % n) + 1;
        if (!((node = avl_search(root, toSearch)) && (node->key == toSearch))) {
            printf("%d not found\n", toSearch);
            res = FAILED;
            break;
        }
        root = avl_delete(root, toSearch);
        if (avl_search(root, toSearch)) {
            printf("%d not deleted\n", toSearch);
            res = FAILED;
            break;
        }
    }
    if (root != NULL) {
        res = FAILED;
        printf("not empty after delete test\n");
    }
    avl_test_recurse(root, &res);
    if (res == FAILED)
        printf("insert/delete/search test failed.\n");
    else
        printf("insert/delete/search test passed.\n");
    delete_avl_tree(root);
}

void rank_reversed_test() {
    int i = 11;
    AVLNodePtr root = NULL;
    TestResult res = PASSED;
    for (i = 11; i < 21; ++i)
        root = avl_insert(root, i);
    if ((avl_rank(root, 20) != 10) || \
        (avl_rank(root, 11) != 1) || \
        (avl_reversed_rank(root, 12) != 9) || \
        (avl_reversed_rank(root, 15) != 6) || \
        (avl_rank(root, 15) != 5))
        res = FAILED;
    if (res == FAILED)
        printf("rank/reversed test failed.\n");
    else
        printf("rank/reversed test passed.\n");
    delete_avl_tree(root);
}

void last_missing_test() {
    AVLNodePtr root = NULL;
    TestResult res = PASSED;
    res = (last_missing(root) == 1) ? res : FAILED;
    root = avl_insert(root, 1);
    res = (last_missing(root) == 0) ? res : FAILED;
    root = avl_insert(root, 3);
    res = (last_missing(root) == 2) ? res : FAILED;
    root = avl_insert(root, 2);

    res = (last_missing(root) == 0) ? res : FAILED;
    root = avl_insert(root, 5);

    res = (last_missing(root) == 4) ? res : FAILED;
    root = avl_insert(root, 7);

    res = (last_missing(root) == 6) ? res : FAILED;
    root = avl_insert(root, 6);

    res = (last_missing(root) == 4) ? res : FAILED;
    root = avl_insert(root, 4);

    res = (last_missing(root) == 0) ? res : FAILED;
    root = avl_insert(root, 11);

    res = (last_missing(root) == 10) ? res : FAILED;
    root = avl_insert(root, 9);

    res = (last_missing(root) == 10) ? res : FAILED;
    root = avl_insert(root, 10);

    res = (last_missing(root) == 8) ? res : FAILED;
    root = avl_insert(root, 8);

    res = (last_missing(root) == 0) ? res : FAILED;
    if (res == FAILED)
        printf("last_missing test failed.\n");
    else
        printf("last_missing test passed.\n");
    delete_avl_tree(root);
}

void howmany_test() {
    int i;
    AVLNodePtr root = NULL;
    TestResult res = PASSED;
    for (i = 21; i < 41; ++i)
        root = avl_insert(root, i);
    if ((avl_HowMany(root, 25, 31) != 7) ||
        (avl_HowMany(root, 28, 38) != 11) ||
        (avl_HowMany(root, 26, 38) != 13) ||
        (avl_HowMany(root, 0, 24) != 4) ||
        (avl_HowMany(root, 30, 80) != 11) ||
        (avl_HowMany(root, 30, 30) != 1) ||
        (avl_HowMany(root, -100, 100) != 20) ||
        (avl_HowMany(root, 0, 20) != 0) ||
        (avl_HowMany(root, 41, 60) != 0))
        res = FAILED;
    if (res == FAILED)
        printf("howmany test failed.\n");
    else
        printf("howmany test passed.\n");
    delete_avl_tree(root);
}


void insert_delete_search_test_2();
void rank_reversed_test_2();
void last_missing_test_2();
void howmany_test_2();


void insert_delete_search_test_2() {
    int i = 1;
    AVLNodePtr node = NULL, root = NULL;
    TestResult res = PASSED;
    for (i = 1; i < 300; ++i)
        root = avl_insert(root, i);
    for (i = 1; i < 300; ++i) {
        if (!((node = avl_search(root, i)) && (node->key == i))) {
            res = FAILED;
            break;
        }
        root = avl_delete(root, i);
        if (avl_search(root, i)) {
            res = FAILED;
            break;
        }
    }
    if (res == FAILED)
        printf("insert/delete/search test 2 failed.\n");
    else
        printf("insert/delete/search test 2 passed.\n");
    delete_avl_tree(root);
}
void rank_reversed_test_2() {
    int i = 11;
    AVLNodePtr node = NULL, root = NULL;
    TestResult res = PASSED;
    for (i = 1; i < 300; ++i)
        root = avl_insert(root, i);
    if ((avl_rank(root, 100) != 100) || \
        (avl_rank(root, 11) != 11) || \
        (avl_reversed_rank(root, 291) != 9) || \
        (avl_reversed_rank(root, 299) != 1) || \
        (avl_rank(root, 15) != 15))
        res = FAILED;
    if (res == FAILED)
        printf("rank/reversed test 2 failed.\n");
    else
        printf("rank/reversed test 2 passed.\n");
    delete_avl_tree(root);
}

void last_missing_test_2() {
    AVLNodePtr root = NULL;
    TestResult res = PASSED;
    root = avl_insert(root, 8);
    res = (last_missing(root) == 7) ? res : FAILED;
    root = avl_insert(root, 7);
    res = (last_missing(root) == 6) ? res : FAILED;
    root = avl_insert(root, 5);
    res = (last_missing(root) == 6) ? res : FAILED;
    root = avl_insert(root, 6);
    res = (last_missing(root) == 4) ? res : FAILED;
    root = avl_insert(root, 2);
    res = (last_missing(root) == 4) ? res : FAILED;
    root = avl_insert(root, 4);
    res = (last_missing(root) == 3) ? res : FAILED;
    root = avl_insert(root, 3);
    res = (last_missing(root) == 1) ? res : FAILED;
    root = avl_insert(root, 1);
    res = (last_missing(root) == 0) ? res : FAILED;
    if (res == FAILED)
        printf("last_missing test 2 failed.\n");
    else
        printf("last_missing test 2 passed.\n");
    delete_avl_tree(root);
}

         
void howmany_test_2() {
    int i = 11;
    AVLNodePtr node = NULL, root = NULL;
    TestResult res = PASSED;
    for (i = 49; i < 79; ++i)
        root = avl_insert(root, i);
    printf("HM is %d (0)\n", avl_HowMany(root, 25, 31));
    printf("HM is %d (11)\n", avl_HowMany(root, 50, 60));
    printf("HM is %d (23)\n", avl_HowMany(root, 53, 75));
    printf("HM is %d (4)\n", avl_HowMany(root, 10, 52));
    printf("HM is %d (10) the answer should be 9.. there are 9 nodes between 70 and 90:70,71,72,73,74,75,76,77,78\n", avl_HowMany(root, 70, 90)); 
    printf("HM is %d (0)\n", avl_HowMany(root, 80, 81));
    printf("HM is %d (1)\n", avl_HowMany(root, 49, 49));

    if ((avl_HowMany(root, 25, 31) != 0) || \
        (avl_HowMany(root, 50, 60) != 11) || \
        (avl_HowMany(root, 53, 75) != 23) || \
        (avl_HowMany(root, 10, 52) != 4) || \
        (avl_HowMany(root, 70, 90) != 10) || \
        (avl_HowMany(root, 80, 81) != 0) || \
        (avl_HowMany(root, 49, 49) != 1))
        res = FAILED;
    if (res == FAILED)
        printf("howMany test 2 failed.\n");
    else
        printf("howMany test 2 passed.\n");
    delete_avl_tree(root);
}



int main() {
    int n = 1000000;
    avl_test(n);
    insert_delete_search_test(n);
    rank_reversed_test();
    last_missing_test();
    howmany_test();
    
    last_missing_test_2();
    howmany_test_2();
    insert_delete_search_test_2();
    rank_reversed_test_2();

    printf("AVL: Hello World!\n");
    return 0;
}                   */