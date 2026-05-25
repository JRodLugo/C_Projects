/* COP 3502C PA6 This program is written by: Jayden E Rodriguez Lugo */


//libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//structs
/* ---------- Global Modes ---------- */
typedef enum {
    MODE_ADOPTION = 0, /* Max-heap by adoption_key (higher is better) */
    MODE_TRIAGE = 1 /* Min-heap by triage_key (lower is more urgent)*/
} Mode;

/* ---------- Core Cat Record ---------- */
typedef struct Cat {
    char *name; /* dynamically allocated, unique string (<= 25 chars + '\0') */
    char *breed; /* dynamically allocated string */
    int age; /* years */
    int friendliness; /* 0..100 */
    int health; /* 0..100 (higher means healthier) */
    int arrival_id; /* strictly increasing when ADDed */
    int quarantine; /* 0 or 1 (1 => cannot be adopted; S1) */
    double key; /* cached priority value for the **active** mode */
} Cat;

/* ---------- Array-Based Binary Heap of Cat* ---------- */
typedef struct {
    Cat **arr; /* array of Cat* implementing the heap */
    int size; /* current number of elements */
    int capacity; /* allocated capacity */
    Mode mode; /* controls comparator semantics */
} CatHeap;

/* ---------- Global Shelter State ---------- */
typedef struct {
    Mode mode; /* MODE_ADOPTION or MODE_TRIAGE */
    char *featured_breed; /* NULL => none; else dynamically allocated breed string*/
    double alpha; /* multiplier for featured breed (>= 1.0; default 1.0)*/
    int next_arrival_id; /* increments on each ADD */
    CatHeap heap; /* priority structure */
} Shelter;

//func prototypes

/*heapify functions*/
int leftChild(int key);
int rightChild(int key);
void heapify(CatHeap *cHeap, int n, int i);
void swap(CatHeap *cHeap, int index1, int index2);

//returns the larger out of the two given inputs
int max(int input1, int input2);

/* Returns heap index of cat with given name, or -1 if not found */
int find_cat_index(CatHeap *cheap, char *name);
/* Returns the current adoption-mode key for cat c using S->featured_breed and S-
>alpha. */
double compute_adoption_key(Cat *c, Shelter *S);
/* Returns the current triage-mode key for cat c. */
double compute_triage_key(Cat *c);
/* Recomputes all keys for the active mode and rebuilds heap in O(n) using bottom-
up heapify. */
void recompute_all_keys_and_build(Shelter *S);

//prints max and min heaps
void printHeap(CatHeap *cheap, int index, char* curMode);

/* ========== Command Handlers (I/O-Free Logic) ========== */
/* Allocates a new Cat, initializes fields, computes key for active mode,
ensures no duplicate name exists (linear scan), and inserts into the heap. */
void cmd_add (Shelter *S, char *name, char *breed, int age, int friendl, int
health);
/* Locate the cat by name using a linear scan of the heap array.
If found, update the requested field.
For AGE/FRIEND/HEALTH: recompute key for active mode and restore heap order.
For QUARANTINE: update the flag only (numeric key unchanged), then restore heap
order. */
void cmd_update(Shelter *S, char *name, char *field, int new_value);
/* Locate the cat by name using a linear scan of the heap array.
Remove it from the heap, restore heap order, and free the Cat. */
void cmd_remove(Shelter *S, char *name);
/* Prints the current top cat for the active mode (does not modify heap). */
void cmd_peek (Shelter *S);
/* Serves the highest-priority cat based on the active mode.
ADOPTION: adopt highest-priority non-quarantined cat.
TRIAGE: treat most urgent cat. */
void cmd_serve(Shelter *S);
/* Sets S->mode, sets heap mode, then recompute_all_keys_and_build(S). */
void cmd_mode (Shelter *S, char *mode_str);
/* Sets (or clears) featured breed and alpha, then
recompute_all_keys_and_build(S). */
void cmd_featured(Shelter *S, char *breed, double alpha);
/* Non-destructive: print top k according to active mode.
Recommended: copy heap array into a temp heap and extract k from the copy. */
void cmd_print (Shelter *S, int k);

//main func
int main(){
    //varibles
    int numInput = 0;//num of inputs
    Shelter *shelter = (Shelter*)malloc(sizeof(Shelter));//shelter struct
    //default settings for shelter
    shelter->mode = MODE_ADOPTION;
    shelter->next_arrival_id = 0;
    shelter->heap.size = 0;
    shelter->heap.capacity = 25;//change 
    shelter->heap.arr = (Cat**)malloc(shelter->heap.capacity * sizeof(Cat*));
    char uInput[25];//user input
    
    scanf("%d", &numInput);//prompts user for the amount of inputs
    //operation loop
    //scanf("%s", uInput);//gets what action user wanteds to make
    for(int i = 0; i < numInput; i++){
        //printf("entered loop\n");
        scanf("%s", uInput);//gets what action user wanteds to make
        if(strcmp(uInput, "MODE") == 0){//user wants to change current heap mode
            //printf("entered mode\n");
            char mode_str[25];
            scanf("%s", mode_str);//gets mode from user
            cmd_mode(shelter, mode_str);//preforms actions based on mode chance
        }else if(strcmp(uInput, "ADD") == 0){//user wants to add cat to heap
            //gets name, breed, age, friendliness score, and health from user
            //adds cat based off given traits
            //printf("entered add\n");
            char cName[25];
            char cBreed[25];
            int cAge, cFriend, cHealth;
            scanf("%s %s %d %d %d", cName, cBreed, &cAge, &cFriend, &cHealth);
            cmd_add(shelter, cName, cBreed, cAge, cFriend, cHealth);
            //printf("exit add\n");
        }else if(strcmp(uInput, "SERVE") == 0){//
            //does server things
            //printf("entered SERVE\n");
            cmd_serve(shelter);
        }else if(strcmp(uInput, "UPDATE") == 0){//user wants to update a cat within the heap
            //gets name, field to be updated, and the new value from user
            char cName[25];
            char field[25];
            int new_Value;
            scanf("%s %s %d", cName, field, &new_Value);
            //updates the cat
            cmd_update(shelter,cName, field, new_Value);
        }else if(strcmp(uInput, "PRINT") == 0){//
            int k;
            scanf("%d", &k);
            cmd_print(shelter, k);
        }else if(strcmp(uInput, "REMOVE") == 0){//removes specified cat from the heap
            //gets name of the cat from the user
            char cName[25];
            scanf("%s", cName);
            //deletes cat
            cmd_remove(shelter,cName);
        }else if(strcmp(uInput, "PEEK") == 0){
            cmd_peek(shelter);
        }else if(strcmp(uInput, "FEATURED") == 0){
            //gets breed and alpha from the user
            char cBreed[25];
            double aplha;
            scanf("%s %2lf", cBreed, &aplha);
            //does featured
            cmd_featured(shelter, cBreed, aplha);
        }else if(strcmp(uInput, "QUIT") == 0){//breaks out of loop
            break;
        }//end of if else
    }//end of loop
    //printf("Code Ran!\n");
    return 0;
}//end of main

/*heapify functions*/
//gets index for left child in the heap
int leftChild(int key){
    return 2 * key + 1;
}//end of func

//gets index for right child in the heap
int rightChild(int key){
    return 2 * key + 2;
}//end of func

//swaps two elements within the heap
void swap(CatHeap *cHeap, int index1, int index2){
    Cat *tempCat = cHeap->arr[index1];
    cHeap->arr[index1] = cHeap->arr[index2];
    cHeap->arr[index2] = tempCat;
}//end of func

//rearranges the elements in te given subtree rooted at node i in the heap of size n
void heapify(CatHeap *cHeap, int n, int i){
    if(cHeap == NULL){return;}//when heap is empty

    //gets indexs for the left and right child
    int left = leftChild(i);
    int right = rightChild(i);
    int smallest = i;//current smallest

    //checks what mode is currently in use
    if(cHeap->mode == MODE_ADOPTION){
        //if right or left index are within the bounds of the array n
        //and if left or right index cat health is greater than smallest index cat health 
        if(right > n && cHeap->arr[left]->key < cHeap->arr[smallest]->key){
            //set smallest index equal to left index
            smallest = left;
        }
        if(right > n && cHeap->arr[right]->key < cHeap->arr[smallest]->key){
            //set smallest index equal to left index
            smallest = right;
        }//end of ifs
    }else if(cHeap->mode == MODE_TRIAGE){
        //if right or left index are within the bounds of the array n
        //and if left or right index cat health is less than smallest index cat health 
        if(right < n && cHeap->arr[left]->key < cHeap->arr[smallest]->key){
            //set smallest index equal to left index
            smallest = left;
        }
        if(right < n && cHeap->arr[right]->key < cHeap->arr[smallest]->key){
            //set smallest index equal to left index
            smallest = right;
        }//end of ifs
    }//end of if else

    //if smaller value was found
    //swap and heapify with parent
    if(smallest != i){
        swap(cHeap, i, smallest);
        heapify(cHeap, n, smallest);
    }//end of if
}//end of func

/* Returns heap index of cat with given name, or -1 if not found */
int find_cat_index(CatHeap *heap, char *name){
    if(heap->size == 0 || heap == NULL){return -1;}//when heap is empty or NULL
    //loops through the entire array
    for(int i = 0; i < heap->size; i++){
        if(strcmp(name, heap->arr[i]->name) == 0){
            return i;
        }
    }
    //index not found
    return -1;
}//end of func

/* Returns the current adoption-mode key for cat c using S->featured_breed and S->alpha. */
double compute_adoption_key(Cat *c, Shelter *S){
    double base = 1.6 * c->friendliness + 1.1 * c->health - .7 * c->age;
    double mult = (c->breed == S->featured_breed ? S->alpha:1.0);
    double adoption_key = base * mult + (-1e-6 * c->arrival_id);
    return adoption_key;
}//end of func

//returns the larger value between the two inputs
int max(int input1, int input2){
    //if input1 is greater than input2, return input1
    if(input1 > input2){
        return input1;
    }
    return input2;
}//end of func

/* Returns the current triage-mode key for cat c. */
double compute_triage_key(Cat *c){
   
    double traige_key = (100-c->health) * 2.0 + max(0, c->age - 12) * 1.0 - .05 * c->friendliness;
    return traige_key;
    return .0;
}//end of func

/* Recomputes all keys for the active mode and rebuilds heap in O(n) using bottom-up heapify. */
void recompute_all_keys_and_build(Shelter *S){
    //if no cats in the shelter
    if(S == NULL || S->heap.size == 0){return;}//end of if
    for(int i = 0; i < S->heap.size; i++){
        //when in ADOPTION mode
        if(S->mode == MODE_ADOPTION){
            //recomputes the key for the given cat, then heapifys to change cats position witin the heap
            S->heap.arr[i]->key = compute_adoption_key(S->heap.arr[i], S);
            heapify(&S->heap, S->heap.size, 0);
        }else if(S->mode == MODE_TRIAGE){
            //recomputes the key for the given cat, then heapifys to change cats position witin the heap
            S->heap.arr[i]->key = compute_triage_key(S->heap.arr[i]);
            heapify(&S->heap, S->heap.size, 0);
        }//end of else if
    }//end of loop
}//end of func


/* ========== Command Handlers (I/O-Free Logic) ========== */
/* Allocates a new Cat, initializes fields, computes key for active mode,
ensures no duplicate name exists (linear scan), and inserts into the heap. */
void cmd_add (Shelter *S, char *name, char *breed, int age, int friendl, int health){
    //printf("in cmd_add\n");
    if(S == NULL ){return;}//if shelter is empty
    if(S->heap.size == S->heap.capacity){return;}//if heap is at capacity
    if(find_cat_index(&S->heap, name) != -1){return;}//if dupe name is found return
    // printf("passed through base cases\n");
    // printf("making cat\n");
    Cat *cat = malloc(sizeof(Cat));//allocating memory for the cat
    if(!cat){return;}//somthing went wrong with the memory allocation
    //printf("Assigning values");
    //copies age,health, and friendliness scores into the cat
    cat->age = age;
    cat->health = health;
    cat->friendliness = friendl;
    cat->quarantine = 0;
    
    //inputing the cats name and breed into the heap
    unsigned int nameLen = strlen(name) + 1;
    unsigned int breedLen = strlen(breed) + 1;
    cat->name = malloc(nameLen * sizeof(char));
    cat->breed = malloc(breedLen * sizeof(char));
    strcpy(cat->name, name);
    strcpy(cat->breed, breed);
    //printf("computing key\n");
    //computes key based on mode
    if(S->mode == MODE_ADOPTION){
        cat->key = compute_adoption_key(cat, S);
    }else{
        cat->key = compute_triage_key(cat);
    }//end of if else
    //printf("Adding to heap\n");
    //adds cat to the heap, heapifys to reajust the array
    int index = S->heap.size;
    S->heap.arr[index] = cat;
    S->heap.size++;

    heapify(&S->heap, index, 0);
    //printf("Added %s\n", cat->name);
}//end of func

/* Locate the cat by name using a linear scan of the heap array. If found, update the requested field.
For AGE/FRIEND/HEALTH: recompute key for active mode and restore heap order.
For QUARANTINE: update the flag only (numeric key unchanged), then restore heap
order. */
void cmd_update(Shelter *S, char *name, char *field, int new_value){
    if(S == NULL || S->heap.size == 0){return;}//when S is empty or if the heap is empty
    int catIndex = find_cat_index(&S->heap, name);
    if(catIndex == -1){
        //if cat isn't in the array
        printf("Cat %s not found.\n", name);
        return;
    }//end of if

    //when field is AGE/FRIEND/HEALTH
    if(strcmp(field, "QUARANTINE") != 0){
        //set age, fiendliness, or health to new_value
        if(strcmp(field, "AGE") == 0 ){
            S->heap.arr[catIndex]->age = new_value;
        }else if(strcmp(field, "FRIEND") == 0){
            S->heap.arr[catIndex]->friendliness = new_value;
        }else if(strcmp(field, "HEALTH") == 0){
            S->heap.arr[catIndex]->health = new_value;
        }//end of if else

        //recompute key for active mode and restore heap order
        recompute_all_keys_and_build(S);
        printf("Updated %s: %s=%d. Priority adjected\n", name, field, new_value);
    }else{//when field is QUARANTINE
        //update the flag, restore heap
        S->heap.arr[catIndex]->quarantine = new_value;
        heapify(&S->heap,S->heap.size, 0);
        printf("Updated %s: QUARANTINE = %d\n", name, S->heap.arr[catIndex]->quarantine);
    }//end of if else
}//end of func

/* Locate the cat by name using a linear scan of the heap array.
Remove it from the heap, restore heap order, and free the Cat. */
void cmd_remove(Shelter *S, char *name){
    if(S == NULL || S->heap.size == 0){return;}//when S is empty

    //finds the cat
    int catIndex = find_cat_index(&S->heap, name);
    if(catIndex == -1){
        //if cat isn't in the array
        printf("Cat %s not found.\n", name);
        return;
    }

    //stores the cat
    Cat *tempCat = S->heap.arr[catIndex];
    S->heap.arr[catIndex] = S->heap.arr[S->heap.size - 1];//moves last element to catIndex 
    S->heap.size--;//shortens heap by 1
    heapify(&S->heap,S->heap.size, catIndex);//fixes the array,heapifys at catIndex

    //frees cat
    free(tempCat->breed);
    free(tempCat->name);
    free(tempCat);
    printf("Removed %s\n", name);
}//end of func

/* Prints the current top cat for the active mode (does not modify heap). */
void cmd_peek (Shelter *S){
    //when Shelter is NULL or heap is empty
    if(S == NULL ||  S->heap.size == 0){
        printf("No cats available\n");
        return;
    }//end of if

    //gets mode from shelter
    char tempMode[25];
    if(S->mode == MODE_ADOPTION){
        strcpy(tempMode, "ADOPTION");
    }else{
        strcpy(tempMode, "TRAIGE");
    }
    //prints out top cat for current mode
    printf("TOP [%s]: [%s] (key=%.2f, name=%s, breed=%s, age=%d, friend=%d, health=%d)\n", tempMode, tempMode,
    S->heap.arr[0]->key,S->heap.arr[0]->name,S->heap.arr[0]->breed,S->heap.arr[0]->age,S->heap.arr[0]->friendliness,
    S->heap.arr[0]->friendliness);
}//end of func

//prints everything within the given heap
void printHeap(CatHeap *cheap, int index, char* curMode){
    if(cheap == NULL){return;}
    //prints current cat 
    printf("[%d] %s (key=%.2lf, %s)", index, cheap->arr[index]->name,cheap->arr[index]->key, curMode); 
    printHeap(cheap, index + 1, curMode);
}//end of func


/* Serves the highest-priority cat based on the active mode.
ADOPTION: adopt highest-priority non-quarantined cat.
TRIAGE: treat most urgent cat. */
void cmd_serve(Shelter *S){
    //printf("in cmd_serve\n");
    if(S == NULL || S->heap.size == 0){return;}//when S is empty
    int index = 0;
    int quar = 0;
    //printf("going to first if\n");
    if(S->mode == MODE_ADOPTION){
        for(int i = 0; i < S->heap.size; i++){
            if(S->heap.arr[index]->quarantine == 0){
                //printf("cat found\n");
                quar = 0;
                printf("serve now: %s (key=%.2lf, name=%s, breed=%s, age=%d friend=%d, health=%d)\n", S->heap.arr[index]->name,
                S->heap.arr[index]->key,S->heap.arr[index]->name, S->heap.arr[index]->breed, S->heap.arr[index]->age, 
                S->heap.arr[index]->friendliness, S->heap.arr[index]->health);
                
                //stores the cat
                Cat *tempCat = S->heap.arr[index];
                S->heap.arr[index] = S->heap.arr[S->heap.size - 1];//moves last element to catIndex 
                S->heap.size--;//shortens heap by 1
                heapify(&S->heap,S->heap.size, index);//fixes the array,heapifys at catIndex

                //frees cat
                free(tempCat->breed);
                free(tempCat->name);
                free(tempCat);
                break;
            }else{
                //printf("cat not found\n");
                index++;
                quar = 1;
            }//end of look if else
        }//end of for loop
        if(quar == 1){
            //when all cats are in quarantine
            printf("No adoptable cats available\n");
        }
    }else{
        //print out cat highest priotity non-quarantined cat or most urgent cat
        //printf("mode triage printing\n");
        printf("serve now: %s (key=%.2lf, name=%s, breed=%s, age=%d friend=%d, health=%d)\n", S->heap.arr[index]->name,
        S->heap.arr[index]->key,S->heap.arr[index]->name, S->heap.arr[index]->breed, S->heap.arr[index]->age, 
        S->heap.arr[index]->friendliness, S->heap.arr[index]->health);

        //stores the cat
        Cat *tempCat = S->heap.arr[index];
        S->heap.arr[index] = S->heap.arr[S->heap.size - 1];//moves last element to catIndex 
        S->heap.size--;//shortens heap by 1
        heapify(&S->heap,S->heap.size, index);//fixes the array,heapifys at catIndex

        //frees cat
        free(tempCat->breed);
        free(tempCat->name);
        free(tempCat);
    }//end of else if
    //printf("exiting serve\n");
}//end of func

/* Sets S->mode, sets heap mode, then recompute_all_keys_and_build(S). */
void cmd_mode (Shelter *S, char *mode_str){
    if(S == NULL){return;}//when Shelter is NULL

    //changes the mode for the heap
    if(strcmp(mode_str, "ADOPTION") == 0){
        S->mode = MODE_ADOPTION;//max heap
    }else if(strcmp(mode_str, "TRIAGE") == 0){
        S->mode = MODE_TRIAGE;//min heap
    }else{
        //when user inputs an invalid mode
        printf("Unknown mode %s\n", mode_str);
        return;
    }//end of else if

    //rebuilds the heap
    printf("Mode set to %s. Rebuilding priotities...\n", mode_str);
    recompute_all_keys_and_build(S);
}//end of func

/* Sets (or clears) featured breed and alpha, then recompute_all_keys_and_build(S). */
void cmd_featured(Shelter *S, char *breed, double alpha){
    if(S == NULL || S->heap.size == 0){return;}//when S is empty
    
    if(strcmp(breed, "NONE") == 0){//when the breed is NONE, set featured_breed to NULL, set aplha to 0
        S->featured_breed = NULL;
        S->alpha = 0.0;
        printf("Featured breed cleared. Rebuilding priorities...\n");
        recompute_all_keys_and_build(S);
    }else{//sets featured breed and aplha to inputed breed and aplha
        strcpy(S->featured_breed, breed);
        S->alpha = alpha;
        //rebuilds the heap
        printf("Featured breed set to %s with alpha=%.2lf\n", breed, alpha);
        recompute_all_keys_and_build(S);
    }//end of if else
}//end of func

/* Non-destructive: print top k according to active mode.
Recommended: copy heap array into a temp heap and extract k from the copy. */
void cmd_print (Shelter *S, int k){
    //if S is NULL or heap size is zero
    if(S == NULL || S->heap.size == 0){
        //when S is empty
        printf("No cats available\n");
        return;
    }//end of if

    if(k <= 0){return;}//if k is less than or equal to zero
    char currMode[25];
    if(S->mode == MODE_ADOPTION){
        strcpy(currMode, "ADOPTION");
    }else{
        strcpy(currMode, "TRIAGE");
    }

    CatHeap *tempHeap = &S->heap;//create a temp heap to extract k from

    // //print in asending or decending order if its a mini heap of max heap
    // //prints k lines from the heap
    // for(int i = 0; i < k; i++){
    //     printf("[%d] %s (key=%.2lf, %s)\n", i + 1, tempHeap->arr[i]->name, tempHeap->arr[i]->key, currMode);
    // }//end of func
    printf("[%d] %s (key=%.2lf, %s)\n", 1, tempHeap->arr[0]->name, tempHeap->arr[0]->key, currMode);
}//end of func

//end of code