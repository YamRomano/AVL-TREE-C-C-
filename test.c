#include "avl.c"

typedef enum {FAILED,PASSED} TestResult;

void avl_test(int n);
int avl_test_recurse( AVLNodePtr tnode, TestResult * res );
void insert_delete_search_test(int n);
void rank_reversed_test();
void last_missing_test();
void howmany_test();

void printTree(AVLNodePtr node){
        if(!node) return;
        printTree(node->child[RIGHT]);
        printf("%d, ",node->key);
        printTree(node->child[LEFT]);
}
void avl_test(int n){
    int i=1;
    TestResult res = PASSED;
    AVLNodePtr root = NULL;
    for( i=1; i<=n; ++i )
        root = avl_insert( root, i );
    avl_test_recurse( root, &res );
    if( res == FAILED )
        printf("AVL test failed.\n");
    else
        printf("AVL test passed.\n");
    delete_avl_tree( root );
}

int avl_test_recurse( AVLNodePtr tnode, TestResult * res ){
    int h_left,h_right;
    if( !tnode )
        return -1;
    h_left = avl_test_recurse( tnode->child[LEFT], res );
    h_right = avl_test_recurse( tnode->child[RIGHT], res );
    *res =  (abs(h_left-h_right) > 1 ) ? FAILED:*res;
    if((abs(h_left-h_right)) > 1 )
        printf("failed at %d\n",tnode->key);
    return 1+max(h_left,h_right);
}


void insert_delete_search_test(int n){
    int i=1;
    AVLNodePtr node = NULL, root = NULL;
    TestResult res = PASSED;
    for( i=1; i<n; ++i ){
        int toInsert = (((int)(3/10*n)+i)%n)+1;
        root = avl_insert( root, toInsert );
    }
    avl_test_recurse( root, &res );
    for( i=1; i<n; ++i ){
        int toSearch = (((int)(5/9*n)+i)%n)+1;
        if( !((node = avl_search( root, toSearch)) && (node->key == toSearch))){
            printf("%d not found\n",toSearch);
            res = FAILED;
            break;
        }
        root = avl_delete( root, toSearch );
        if( avl_search( root, toSearch ) ){
            printf("%d not deleted\n",toSearch);
            res = FAILED;
            break;
        }
    }
    if(root!=NULL){
        res = FAILED;
        printf("not empty after delete test\n");
    }
    avl_test_recurse(root,&res);
    if( res == FAILED )
        printf("insert/delete/search test failed.\n");
    else
        printf("insert/delete/search test passed.\n");
    delete_avl_tree( root );
}

void rank_reversed_test(){
    int i=11;
    AVLNodePtr root = NULL;
    TestResult res = PASSED;
    for( i=11; i<21; ++i )
        root = avl_insert( root, i );
    if( (avl_rank( root, 20 ) != 10) || \
        (avl_rank(root,11) != 1) || \
        (avl_reversed_rank(root,12) != 9) || \
        (avl_reversed_rank(root,15) != 6) || \
        (avl_rank(root, 15) != 5)   )
        res = FAILED;
    if( res == FAILED )
        printf("rank/reversed test failed.\n");
    else
        printf("rank/reversed test passed.\n");
    delete_avl_tree( root );
}

void last_missing_test(){
    AVLNodePtr root = NULL;
    TestResult res = PASSED;
    res = (last_missing( root ) == 1)?res:FAILED;
    root = avl_insert( root, 1 );
    printf(" %d(0)\n", last_missing(root));
    res = (last_missing( root )==0)?res:FAILED;
    root = avl_insert( root, 3 );
    printf(" %d(2)\n", last_missing(root));

    res = (last_missing( root )==2)?res:FAILED;
    root = avl_insert( root, 2 );
    printf(" %d(0)\n", last_missing(root));

    res = (last_missing( root )==0)?res:FAILED;
    root = avl_insert( root, 5 );
    printf(" %d(4)\n", last_missing(root));

    res = (last_missing( root )==4)?res:FAILED;
    root = avl_insert( root, 7 );
    printf(" %d(6)\n", last_missing(root));

    res = (last_missing( root )==6)?res:FAILED;
    root = avl_insert( root, 6 );
    printf(" %d(4)\n", last_missing(root));

    res = (last_missing( root )==4)?res:FAILED;
    root = avl_insert( root, 4 );
    printf(" %d(0)\n", last_missing(root));

    res = (last_missing( root )==0)?res:FAILED;
    root = avl_insert( root, 11 );
    printf(" %d(10)\n", last_missing(root));

    res = (last_missing( root )==10)?res:FAILED;
    root = avl_insert( root, 9 );
    printf(" %d(10)\n", last_missing(root));

    res = (last_missing( root )==10)?res:FAILED;
    root = avl_insert( root, 10 );
    printf(" %d(8)\n", last_missing(root));

    res = (last_missing( root )==8)?res:FAILED;
    root = avl_insert( root, 8 );
    printf(" %d(0)\n", last_missing(root));

    res = (last_missing( root )==0)?res:FAILED;
    if( res == FAILED )
        printf("last_missing test failed.\n");
    else
        printf("last_missing test passed.\n");
    delete_avl_tree( root );
}

void howmany_test(){
    int i;
    AVLNodePtr root = NULL;
    TestResult res = PASSED;
    for( i=21; i<41; ++i )
        root = avl_insert( root, i );
    if( (avl_HowMany(root, 25,31 ) != 7) ||
        (avl_HowMany(root,28,38) != 11) ||
        (avl_HowMany(root,26,38) != 13) ||
        (avl_HowMany(root,0,24) != 4) ||
        (avl_HowMany(root, 30,80) != 11) ||
        (avl_HowMany(root, 30,30) != 1) ||
        (avl_HowMany(root,-100,100) != 20) ||
        (avl_HowMany(root,0,20) != 0) ||
        (avl_HowMany(root,41,60) != 0))
        res = FAILED;
    if( res == FAILED )
        printf("howmany test failed.\n");
    else
        printf("howmany test passed.\n");
    delete_avl_tree( root );
}

int main(){
    int n =1000000;
    avl_test(n);
    insert_delete_search_test(n);
    rank_reversed_test();
    last_missing_test();
    howmany_test();
    printf("AVL: Hello World!\n");
    return 0;
}
