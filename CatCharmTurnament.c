/* COP 3502C PA5 This program is written by: Jayden Rodriguez Lugo */


//Binary Search tree as a live leaderboard

//libraries
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//defined varibles
#define MAX_NAME 25 // max name of a cat
#define NUM_TRAITS 5 // number of total traits a cat has

//data structs

//cat struct
typedef struct Cat {
    char *name; //dynamically allocate space for a cat’s name
    char *breed; //dynamically allocate space for a cat’s breed
    int charm; //unique charm score for each cat
    int traits[NUM_TRAITS]; //scores corresponding to each index in TRAIT_NAMES
} Cat;

typedef struct BST_Node {
    Cat *cat;
    struct BST_Node *left;
    struct BST_Node *right;
    int subtree_size; // stores the current size of this subtree’s root (not height)
} BST_Node;

//trates for given cats
const char *TRAIT_NAMES[NUM_TRAITS] = {"friendly", "grumpy", "playful", "lazy","curious"};

//func prototypes
//make BST, delete node, insert in BST, search(remove this when these functions are added)
void queries(int userIn, BST_Node *catTree);
BST_Node *create_Node();//creates cat node
BST_Node *Insert_Cat(BST_Node *root, BST_Node *element, int depth);//inserts cat into the Binary search tree
Cat* createSingleCat();//creates one cat
int findKthSmallest(BST_Node *root, int k);//finds the kth smallest element within the tree
void print_inOrder(BST_Node *root);//prints elements in the tree using in-order
void freeTree(BST_Node *root);//frees the elements within the tree using post-order
BST_Node *case2Delete(BST_Node *root, char *catName);//deletes the specified cat based on the name
BST_Node *case5Delete(BST_Node *root, int tIndex, int tValue, int *count);//deletes cats with specified traits
Cat* findCat(BST_Node *root,char *catName);//finds the cat by name
BST_Node *deleteCat(BST_Node *root, Cat *cat);//deleted specified cat from the tree
char** filterByTrait(BST_Node *root, int traitIndex, int traitValue, int *resultSize);//filters cat by traits
void collectTraits(BST_Node *root, int traitIndex, int traitValue, int *resultSize, char** results, int *i);//helper function for filterbyTraits

//Aux funcs for deleting the node
BST_Node *findNode(BST_Node *root, Cat *cat);//returns pointer to a node in given tree that stores the specified cat
BST_Node *nodeParent(BST_Node *root, BST_Node *node);//finds parent of a given node in a given binary tree
BST_Node *minVal(BST_Node *root);//find the min value in given tree(min val will either be root or be found in left subtree)
BST_Node *maxVal(BST_Node *root); //finds max value in given tree(right most leaf)
int isLeaf(BST_Node *root);//checks if a node is a leaf node or not
//checks if the tree has only a left or right child
int hasOnlyLeftChild(BST_Node *root);
int hasOnlyRightChild(BST_Node *root);

//main func
int main(){
    int userIn = 0;
    BST_Node *catTree = NULL;//initializes tree
    //gets user input for the amount of inputs for the program
    scanf("%d", &userIn);
    //handles the rest of the users queries
    queries(userIn, catTree);
    //frees memory used
    freeTree(catTree);
    //printf("code Ran!\n");
}//end of main

//handles users queries
void queries(int userIn,BST_Node *catTree){
    //for the users queries
    int q = 0;
    int startDepth = 0;
    int NumOfCats = 0;
    //printf("userIn = %d\n", userIn);
    //scanf("%d", &q);
    for(int i = 0; i < userIn; i++){
        scanf("%d", &q);
        //printf("current q %d\n", q);
        //cases for users input
        switch(q){
            case 1: {
                //inserts cat into the tree
                BST_Node *catNode = create_Node();//creates new node
                //printf("Entering insert_cat\n");
                catTree =  Insert_Cat(catTree,catNode,startDepth);//insterts cat into tree
                NumOfCats++;
                //startDepth = 1;//once tree has root, depth will begin at 1
                break;
            }
            case 2:{
                //deletes with the three-case aooriach
                char catName[MAX_NAME];
                scanf("%s", catName);
                catTree = case2Delete(catTree,catName);
                //catTree->subtree_size--;
                NumOfCats--;
                break;
            }
            case 3:{
                //finds the k-th smallest element in the tree
                //if k = 1, return the smallest element, if k = 3 then return the third smallest element
                int k = 0;
                scanf("%d", &k);//gets input for kth element
                //find the smallest
                if(findKthSmallest(catTree,k) == 0){
                    printf("NO SMALLEST ELEMENT FOUND\n");
                }//end of if
                break;
            }
          
            case 4:{
                 //filters out cats based on specific trait
                int tIndex = 0;
                int tValue = 0;
                int resultSize = NumOfCats;
                scanf("%d %d", &tIndex, &tValue);//takes in input for the wanted values
                //prints the cats based of user specifications
                char** case4 = filterByTrait(catTree,tIndex,tValue, &resultSize);
                //if cats where found, print them out
                if(case4 == NULL){
                    printf("NONE FOUND\n");
                }else{
                    printf("%s: ", TRAIT_NAMES[tIndex]);
                    for(int i = 0; i < resultSize; i++){
                        printf("%s ", case4[i]);
                    }//end of loop
                    //frees dynamic memory for later uses
                    for(int i = 0; i < resultSize; i++){
                        free(case4[i]);
                    }//end of loop
                    free(case4);
                }
                printf("\n");
                break;
            }
            case 5:{
                //Given a trait index and a trait value, remove any node that has these constraints
                int tIndex = 0;
                int tValue = 0;
                int count = 0;
                scanf("%d %d", &tIndex, &tValue);//takes in input for the wanted values
                catTree = case5Delete(catTree, tIndex, tValue, &count);
                if(count == 0){
                    printf("NONE REMOVED\n");
                }else{
                    printf("%d\n",count);
                }
                break;
            }
            case 6:
                //prints elements in tree via in-order traversal
                if(catTree != NULL){//if tree is not empty
                    //printf("cat %s\n", catTree->cat->name);
                    print_inOrder(catTree);
                }else{//when empty
                    printf("EMPTY\n");
                }//end of file
                break;
            default:
            //free space :D, if your reading this I hope your days going good, keep on keeping on 
                break;
        }//end of switch case
    }//end of for loop
}//end of func


//creates one cat
Cat* createSingleCat(){
    //allocates memory for a single cat
    Cat *SingleCat = (Cat*)malloc(sizeof(Cat));
    char tempName[MAX_NAME];
    char tempBreed[MAX_NAME];
    int len;

    scanf("%s", tempName);
    len = strlen(tempName) + 1;
    //allocates memory for name
    SingleCat->name = (char*)malloc(len * sizeof(char));
    strcpy(SingleCat->name, tempName);

    scanf("%s", tempBreed);
    len = strlen(tempBreed) + 1;
    //allocates memory for breed
    SingleCat->breed = (char*)malloc(len * sizeof(char));
    strcpy(SingleCat->breed, tempBreed);
    //user input for charms
    scanf("%d", &SingleCat->charm);
    //gets input for the traits
    for(int i = 0; i < NUM_TRAITS; i++){
        int t = 0;
        scanf("%d", &t);
        //if input is not 1 or 0
        if(t != 1 && t != 0){
            i--;//stay on this index
        }else{
            SingleCat->traits[i] = t;
        }//end of else if
    }
    return SingleCat;//returns the cat
}//end of func

//creates cat node
BST_Node *create_Node(){
    //creates a temp node
    BST_Node *temp;
    Cat *newcat = createSingleCat();//creates the cat;
    temp = malloc(sizeof(BST_Node));
    temp->cat = newcat;//inputs cat into the node
    temp->left = NULL;
    temp->right = NULL;
    temp->subtree_size = 1;//default subtree_size
    //printf("Exiting create_node\n");
    return temp;//returns node
}//end of func

//inserts cat into the Binary search tree
BST_Node *Insert_Cat(BST_Node *root, BST_Node *element, int depth){
    //when the list is empty
    if(root == NULL){
        printf("Insert: %d\n", depth);
        return element;
    }else{
        //insert to the right
        if(strcmp(root->cat->name, element->cat->name) < 0){
            if(root->right != NULL){//if the right node exists
                //continue down the right path
                root->right = Insert_Cat(root->right, element, depth + 1);
            }else{//if no right node,insert element
                root->right = element;
                printf("Insert: %d\n", depth);
            }//end of nested else if
        }else if(strcmp(root->cat->name, element->cat->name) > 0){//inserts to the left
            if(root->left != NULL){//if the left node exists
                //continue down the left path
                root->left = Insert_Cat(root->left, element, depth + 1);
            }else{//if no left node,insert element
                root->left = element;
                printf("Insert: %d\n", depth);
            }//end of nested else if
        }else if(strcmp(root->cat->name, element->cat->name) == 0){//when both have same name
            int rootCount = 0; 
            int elementCount = 0;
            //gets to see witch cat has more traits
            for(int i = 0; i < NUM_TRAITS; i++){
                if(root->cat->traits[i] == 1){
                    //when roots cat has a trait
                    rootCount++;
                }
                if(element->cat->traits[i] == 1){
                    //when elements cat has a trait
                    elementCount++;
                }//end of nested  ifs
            }//end of for loop
            //if the element has more traits then the root
            if(rootCount < elementCount){
               //replace the value
               root->cat = element->cat;
               printf("Replaced\n");
            }
        }//end of inner else if
    }//end of else if

    //calculates the subtree size for the given node
    int leftTree = 0;
    int rightTree = 0;
    if(root->left != NULL){
        leftTree = root->left->subtree_size;
    }//end of if
    if(root->right != NULL){
        rightTree = root->right->subtree_size;
    }//end of if
    root->subtree_size = 1 + leftTree + rightTree;
    //printf("exiting\n");
    return root;
}//end of func

//deletes the specified cat based on the name
BST_Node *case2Delete(BST_Node *root, char *catName){
    //when the root exists
    if(root != NULL){
        //printf("In case 2 func\n");
        Cat *tempCat = findCat(root, catName);
        //makes sure the cat is found
        if(tempCat != NULL){
            root = deleteCat(root, tempCat);//deletes the node
            free(tempCat);//frees memory used for the tempCar
            //printf("root after is %s\n", root->cat->name);
            printf("Deletion Complete\n");
            return root;
        }//end of func
    }//end of if
    //printf("Exiting case 2\n");
    return NULL;
}///end of func

//finds the cat
Cat* findCat(BST_Node *root, char *catName){
    //printf("entered findCat\n");
    if(root == NULL){
        return NULL;
    }//end of if

    //check if current node matches
    if(strcmp(root->cat->name, catName) == 0){
        //printf("Exiting 1\n");
        return root->cat;
    }//end of if

    //searches the left tree
    if(strcmp(root->cat->name, catName) > 0){
        //printf("Exiting 1\n");
        return findCat(root->left, catName);
    }
    //searches the right tree
    return findCat(root->right, catName);
}//end of func

//deleted specified cat from the tree
BST_Node *deleteCat(BST_Node *root, Cat *cat){
    //find the node
   BST_Node *deletedNode = findNode(root, cat);
   if(deletedNode == NULL){
        //cat not found, return
        return root;
   }
   //find the parent node of the selected cat 
   BST_Node *catParent = nodeParent(root, deletedNode);
   //case 1: leaf node deletion
   if(isLeaf(deletedNode)){
    //printf("case 1");
        if(catParent == NULL){
            //when only one node
            free(root);
            return NULL;
        }
        //deletedNode is left child
        if(strcmp(deletedNode->cat->name, cat->name) >= 0){
            free(catParent->left);
            catParent->left = NULL;
        }else{
            //deletedNode is right child
            free(catParent->right);
            catParent->right = NULL;
        }//end of else if
        return root;//exit func
   }//end of case 1

   //case 2: only left child
   if(hasOnlyLeftChild(deletedNode)){
    //printf("Case 2 a\n");
        //if no parent
        if(catParent == NULL){
            //delete the root
            BST_Node *savedNode = deletedNode->left;
            free(deletedNode);
            return savedNode;
        }//if node is left node
        if(strcmp(catParent->cat->name, cat->name) > 0){
            //delete the left node
            BST_Node *savedNode = catParent->left;
            catParent->left = catParent->left->left;
            free(savedNode);
        }else{//if node is right node
            //delete the right node
            BST_Node *savedNode = catParent->right;
            catParent->right = catParent->right->right;
            free(savedNode);
        }
        return root;//returns the tree
   }//end of case 2-a

   //case 2-b only right 
   if(hasOnlyRightChild(deletedNode)){
    //printf("Case 2 b\n");
        //if no parent
        if(catParent == NULL){
            //delete the root
            BST_Node *savedNode = deletedNode->right;
            free(deletedNode);
            return savedNode;
        }//if node is left node
        if(strcmp(catParent->cat->name, cat->name) > 0){
            //delete the left node
            BST_Node *savedNode = catParent->left;
            catParent->left = catParent->left->left;
            free(savedNode);
        }else{//if node is right node
            //delete the right node
            BST_Node *savedNode = catParent->right;
            catParent->right = catParent->right->right;
            free(savedNode);
        }
        return root;//returns the tree
   }//end of case 2-b

   //case 3 two childern
   BST_Node *new_Deleted_cat = minVal(root->right);
   Cat *Saved_Cat = new_Deleted_cat->cat;
   root = deleteCat(root,Saved_Cat);//deletes successor node
   deletedNode->cat = Saved_Cat;//put minVal value into original node
   
   //calculates new size for case 3
   printf("Case 3\n");
   int leftSize = 0;
   int rightSize = 0;
   if(root->left != NULL){
        leftSize = root->left->subtree_size;
        //printf("left size %d\n", leftSize);
   }
   if(root->right != NULL){
        rightSize = root->right->subtree_size;
        //printf("right size %d\n", rightSize);
   }
   root->subtree_size = 1 + leftSize + rightSize;
   return root;
}//end of func

//finds the kth smallest element within the tree
int findKthSmallest(BST_Node *root, int k){
    if(root != NULL){
        int leftSize = 0;
        //stores the left nodes side if exists 
        if(root->left != NULL){
            leftSize = root->left->subtree_size;
        }
        //if the roots subtree size is equal to K
        if(leftSize + 1 == k){
            //print the cat and return
            printf("%s %s %d\n", root->cat->name, root->cat->breed, root->cat->charm);
            return 1;
        }//end of if
        if(k <= leftSize){
            //if the index is less than the current roots subtree size
            findKthSmallest(root->left, k);//go left
            return 1;
        }else{
            //if the index is less than the current roots subtree size
            findKthSmallest(root->right,k - leftSize - 1);//go right
            return 1;
        }//end of else if
    }//end of if
    return 0;
}//end of func


//AUX function for filterByTraits
void collectTraits(BST_Node *root, int traitIndex, int traitValue, int *resultSize, char** results, int *i){
    //when node is not NULL
    if(root != NULL){
        //go left in-order (left-root-right)
        collectTraits(root->left, traitIndex,traitValue,resultSize,results, i);
        //when the cats traits matches
        if(root->cat->traits[traitIndex] == traitValue){
            //preform the deep copy to the results char string
            int len = strlen(root->cat->name) + 1;
            results[*i] = (char*)malloc(len * sizeof(char));
            strcpy(results[*i], root->cat->name);
            (*i)++;
        }//end of if
        //go right
        collectTraits(root->right, traitIndex,traitValue,resultSize,results, i);
    }//end of if
}//end of func

//filters cats by their traits
char** filterByTrait(BST_Node *root, int traitIndex, int traitValue, int *resultSize){
    //when tree empty
    if(root == NULL){
        return NULL;
    }//end of if
    int total = resultSize;
    char **results = (char**)malloc(total * sizeof(char*));
    //inputs cats into the dynamic char array
    int count = 0;
    collectTraits(root, traitIndex,traitValue,resultSize,results, &count);
    //if no cat found
    if(count == 0){
        //free memory and return NULL
        free(results);
        return NULL;
    }
    //for resizing the array
    char** sizeChange = realloc(results, count * sizeof(char*));
    if(sizeChange != NULL){
        results = sizeChange;
    }
    //set the resultSize to the number of cats with the traits counted
    *resultSize = count;
    return results;
}//end of func

//deletes cats with specified traits
BST_Node *case5Delete(BST_Node *root, int tIndex, int tValue, int *count){
    if(root == NULL){
        //when there is nothing within the root
        return NULL;
    }
    //travels through post order (left-right-root)
    root->left = case5Delete(root->left, tIndex, tValue, count);
    root->right = case5Delete(root->right,tIndex,tValue, count);

    //when cat is found
    if(root->cat->traits[tIndex] == tValue){
        //delete and return updated tree
        //printf("Match! %s\n", root->cat->name);
        (*count)++;
        return deleteCat(root, root->cat);
    }
    //returns root
    return root;
}//end of func


//prints elements in the tree using in-order (left-root-right)
void print_inOrder(BST_Node *root){
    if(root != NULL){
        print_inOrder(root->left);//moves to left node
        printf("%s %d %d\n", root->cat->name, root->cat->charm, root->subtree_size);//prints current
        print_inOrder(root->right);//moves to right node
    }//end of func
}//end of func


//Auxiliary functions for node deleting func

//returns pointer to a node in given tree that stores the specified cat
BST_Node *findNode(BST_Node *root, Cat *cat){
    if(root != NULL){
        //found the cat at the current root
        if(strcmp(root->cat->name, cat->name) == 0){
            return root;
        }//end of if
        //go left based on the names
        if(strcmp(root->cat->name, cat->name) > 0){
            return findNode(root->left, cat);
        }else{
            //else go right
            return findNode(root->right,cat);
        }//end of inner else if
    }else{
        //no cat found
        return NULL;
    }//end of else if
}//end of func

//finds parent of a given node in a given binary tree
BST_Node *nodeParent(BST_Node *root, BST_Node *node){
    //if root is NULL or if its the same as the node
    if(root == NULL || root == node){
        return NULL;//no parent found
    }//end of if

    //if the left or the right nodes are equal to the given node
    if(root->left == node || root->right == node){
        return root;//return current root
    }//end of if

    //look for node's parent in the left
    if(strcmp(root->cat->name, node->cat->name) > 0){
        return nodeParent(root->left,node);
    }
    //look for node's parent in the right 
    return nodeParent(root->right,node);
}//end of func

//find the min value in given tree(min val will either be root or be found in left subtree)
BST_Node *minVal(BST_Node *root){
    //if root is NULL
    if(root == NULL){
        return NULL;
    }//end of if
    //if left child is NULL
    if(root->left == NULL){
        return root;
    }else{
        //continue left until the left most leaf is found
        return minVal(root->left);
    }//end of else if
}//end of func

//finds max value in given tree(right most leaf)
BST_Node *maxVal(BST_Node *root){
    //if root is NULL
    if(root == NULL){
        return NULL;
    }//end of if
    //if right child is NULL
    if(root->right == NULL){
        return root;
    }else{
        //continue left until the left most leaf is found
        return minVal(root->right);
    }//end of else if
}//end of func

//checks if a node is a leaf node or not
int isLeaf(BST_Node *root){
    return (root->left == NULL && root->right == NULL);
}//end of func

//checks if the tree has only a left or right child
int hasOnlyLeftChild(BST_Node *root){
    return (root->left != NULL && root->right == NULL);
}
int hasOnlyRightChild(BST_Node *root){
    return (root->right != NULL && root->left == NULL);
}//end of funcs

//frees the elements within the tree using post-order (left-right-root)
void freeTree(BST_Node *root){
    if(root != NULL){
        freeTree(root->left);//travels to the left of the tree
        freeTree(root->right);//travels to the right of the tree
        //frees cat memory
        free(root->cat->name);
        free(root->cat->breed);
        free(root->cat);
        //frees current root
        free(root);
    }//end of if
}//end of func

//end of file