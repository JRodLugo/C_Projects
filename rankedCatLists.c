/*COP 3502C PA4 This program is written by: Jayden Rodriguez Lugo*/

/*
Sorts cats according to a selected trait, produces an official cat Ranked List
orded from the highest to lowest score

This is done by implimenting two different soritng algorithms, merge sort and quicksort
the code submitted, one for merge sort and one using quick sort
*/

//libraries
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//defined things
#define MAXSIZE 12 //max size of all strings in the problem, including both cat names and trait names
#define NUMTRAITS 7 //total num of possible keys for sorting (six traits plus one for the total score)
#define TOTAL 6 //the index into the scores array indicating the total score for a cat
#define BASECASESIZE 30 //size at which recursive sorting algorithm must switch to insertion sort
#define MAX_STR 25 // maximum name length of a cat

const char TRAITS[NUMTRAITS][MAXSIZE+1] = {"Cuteness", "Fluffiness",
"Agility", "Friendliness", "Intelligence", "Laziness", "Total"};

//cat struct
typedef struct Cat {
    char *name;
    int scores[NUMTRAITS];
}Cat;

//func prototypes
int compareTo(Cat* ptrC1, Cat* ptrC2, int key);
Cat **createCats(int count);//creates multiple cats
Cat* createSingleCat();//creates one cat
void insertionSort(Cat** list, int n, int key);//insertionSort if list is to short 
void freeMemory(Cat** list, int numCats);

//merge sort funcs
void mergeSortRec(Cat** list, int low, int high, int key);
void merge(Cat** list, int low, int mid, int high, int key);
void mergeSort(Cat** list, int n, int key);

//quick sort funcs
void quickSortRec(Cat** list, int low, int high, int key);
void quickSort(Cat** list, int n, int key);
int partition(Cat** list, int low, int high, int key);
void swap(Cat *a, Cat *b);

//main function
int main(){
    //printf("FIRST");
    int numCats = 0;
    int SelectedTrait = 0;// given traits in TRAITS(0 would rank cats based on their Cutness score)
    scanf("%d",&numCats); //user input for number of cats
    Cat** cList = createCats(numCats);//creates the cats
    scanf("%d", &SelectedTrait); //user input for ranked trait 
    //printf("Test");
    //mergeSort(cList, numCats, SelectedTrait);
    //printf("Test 2");
    quickSort(cList, numCats, SelectedTrait);
    //when list is smaller use insertionSort to sort the list
    if(numCats <= BASECASESIZE){
        //insertionSort(cList,numCats,SelectedTrait);
    }else{//if the list is to large then preform the more complicated algorithms to work the list
        //mergeSort(cList, numCats, SelectedTrait);
        //quickSort(cList, numCats, SelectedTrait);
    }
    
    //prints out the sorted list
    printf("\n%s Ranklist\n", TRAITS[SelectedTrait]);
    for(int i = 0; i < numCats; i++){
        printf("%d. %-15s %d\n", i+1, cList[i]->name, cList[i]->scores[SelectedTrait]);
    }//end of loop

    freeMemory(cList, numCats);
    //printf("Code Ran!\n");
    return 0;
}//end of main

//creates a single cat
Cat* createSingleCat(){
    //allocates memory for a single cat
    Cat *SingleCat = (Cat*)malloc(sizeof(Cat));
    char Temp_Name[MAX_STR];
    scanf("%s", Temp_Name);//user input for the cat name
    int len = strlen(Temp_Name) + 1;//gets length of the string plus the buffer
    SingleCat->name = (char*)malloc(len * sizeof(char));
    strcpy(SingleCat->name, Temp_Name);
    //inputs traits into cat
    int total = 0;//total score for all traits
    for(int i = 0; i < NUMTRAITS-1; i++){
        scanf("%d", &SingleCat->scores[i]);
        total += SingleCat->scores[i];
    }//end of loop
    SingleCat->scores[NUMTRAITS-1] = total; //inputs cats total score
    return SingleCat;//returns the cat
}//end of func

//creates multiple cats
Cat **createCats(int count){
    //allocates memory for the cats
    Cat **MultipleCats = (Cat**)malloc(count * sizeof(Cat*));
    //creates count number of cats 
    for(int i = 0; i < count; i++){
        MultipleCats[i] = createSingleCat();
    }
    //returns the double Pointer
    return MultipleCats;
}//end of func

//insertion sort if the cat list is to small to use the other sorting algorithms
void insertionSort(Cat** list, int n, int key){
    for(int i = 0; i < n; i++){
        Cat* block = list[i];//saves current cat in block
        int j = i - 1;
        //inserts into the given sequence, while j is greater or equal 0 and while compareTo is greater than 0
        while(j >= 0 && compareTo(list[j], block, key) > 0){
            list[j+1] = list[j];
            j--;
        }
        list[j+1] = block; //inserts element
    }//end of loop

}//end of func

// wrapper function Merge Sorts the array list of size n according to the trait indicated by the integer key.
void mergeSort(Cat** list, int n, int key){
    //key is what trait is being compared
    if(n < 0){return;}//if no cats to compare, return
    //printf("Im running mergeSort\n");
    mergeSortRec(list, 0, n-1, key);//does the merg sort
    //printf("I ran! mergSort\n");
}//end of func

// Performs a Merge Sort on list[low..high] according to the trait indicated by the integer key.
void mergeSortRec(Cat** list, int low, int high, int key){
    //printf("entered mergeSortRec\n");
    if (low == high){
        return;
    }//if low equals high return
        
    int mid = (low + high) / 2;
    mergeSortRec(list, low, mid, key);
    mergeSortRec(list, mid+1, high, key);
    merge(list, low,mid,high, key);
    //printf("exited mergeSortRec");
}//end of func

//merges the array
void merge(Cat** list, int low, int mid, int high, int key){
    //printf("entered merge\n");
    int low_sz = mid - low + 1;
    int high_sz = high - (mid+1) + 1;
    Cat **L = (Cat**)malloc(low_sz * sizeof(Cat*));
    Cat **H = (Cat**)malloc(high_sz * sizeof(Cat*));
    //printf("1\n");
    for(int i = 0; i < low_sz; i++){
        L[i] = list[low + i];
        //printf("%s L current %d i value\n", L[i]->name, i);
    }
    for(int j = 0; j < high_sz; j++){
        H[j] = list[mid + 1 + j];
        //printf("%s H current %d j value\n", H[j]->name, j);
    }
    //printf("2\n");
    int i = 0;  //index for left array
    int j = 0;  //index for right array
    int k = low;  //index for original array
    //printf("3\n");
    while(i < low_sz && j < high_sz){
        if(compareTo(L[i], H[j], key) <= 0){
            list[k] = L[i];
            i++;
        }
        else{
            list[k] = H[j];
            j++;
        }
        k++;//moves to next element in list
    }//end of loop
    //printf("4\n");
    while(i < low_sz){
        list[k] = L[i];
        k++;
        i++;
    }
    while(j < high_sz){
        list[k] = H[j];
        k++;
        j++;
    }
    //printf("5\n");
    free(L);
    free(H);
    //printf("Exited merge\n");
}//end of func

// wrapper func for Quick Sorts the array list of size n according to the trait indicated by the integer key.
void quickSort(Cat** list, int n, int key){
    //key is what trait is being compared
    if(n < 0){return;}//if no cats to compare, return
    quickSortRec(list, 0, n-1, key);//does the quick sort
}//end of func

// Performs a Quick Sort on list[low..high] according to the trait indicated by the integer key.
void quickSortRec(Cat** list, int low, int high, int key){
    if (low < high){
        int split = partition(list, low, high, key);
        quickSortRec(list, low, split-1, key);
        quickSortRec(list, split+1, high, key);
    }
}//end of func

//preforms the quick sort calculation on a given section of the array
int partition(Cat** list, int low, int high, int key){
    int i = (low + rand() % (high-low + 1));
    swap(list[low], list[i]);
    int lowpos = low;
    low++;

    while(low <= high){
        while(low <= high && compareTo(list[low], list[lowpos], key) < 0){low++;}
        while(low <= high && compareTo(list[high], list[lowpos], key) >= 0){high--;}
        //end of inner loops
        if(low<high){swap(list[low], list[high]);}
    }//end of loop

    swap(list[lowpos], list[high]);
    //printf("after swap 3 %s %s\n", list[lowpos]->name, list[high]->name);
    return high;
}//end of func

//swaps cats
void swap(Cat *a, Cat *b){
    Cat t = *a;
    *a = *b;
    *b = t;
}//end of func

/* Returns a negative integer(-1) if the cat pointed to by ptrC1
"comes before" the cat pointed to by ptrC2 on the ranklist
sorted by the trait indicated by the integer key, breaking ties
by the cat's name in alphabetical order. 
Returns 0 if the two cats are identical 
returns a positive integer(1) if the cat pointed to by ptrC2 
comes before the cat pointed to by ptrC1. */
int compareTo(Cat* ptrC1, Cat* ptrC2, int key){
    //if the ptrC1 comes first
    if(ptrC1->scores[key] > ptrC2->scores[key]){
        return -1;
    } else if(ptrC1->scores[key] < ptrC2->scores[key]){//if the ptrC2 comes first
        return 1;
    }//end of else if

    //comparing cats alphabetically
    if(strcmp(ptrC1,ptrC2) < 0){//when ptrC1 name is first(alphabetical order)
        return -1;
    }else if(strcmp(ptrC1,ptrC2) > 0){//when ptrC2 name is first(alphabetical order)
        return 1;
    }//end of else if

    //when the two cats are identical
    return 0;
}//end of func

//frees all dynamically allocated memory
void freeMemory(Cat** list, int numCats){
    //printf("Entered func\n");
    for(int i = 0; i < numCats; i++){
        //printf("%s cat\n", list[i]->name);
        free(list[i]->name);//frees cats name
        free(list[i]);//frees cat
        //printf("Looping\n");
    }//end of func

    //frees list
    free(list);
}//end of func


//end of code


