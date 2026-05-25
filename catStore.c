/* COP 3502C PA1 This program is written by: Jayden Rodriguez Lugo */
// 2/01/2026


//librarys
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//structs
typedef struct Cat{
    char *name; // dynamically allocated space for the name without wasting space
    int age; // specifies the age of the cat
    float weight; // stores the decimal value in weight for this specific cat
    char *breed; //points to an already allocated breed string(Ragdoll, Siamese, Maincoone, etc..). No malloc/calloc for this property
    int status; //specifies if a cat is adopted, pending, or available, rangesfrom 0 to 2, initially set to available for newly added cats
} Cat;

typedef struct Kennel {
    char *location; // dynamically allocated space for the location without wasting space
    int occupancy; // stores the current number of cats in the kennel
    Cat **cats; // dynamically allocated array of pointers to Cats that reside in the kennel
    int maxCapacity; // specifies the max cap to the kennel
} Kennel;

typedef struct CatStore {
    int **capacities; //dynamically allocated double int array stores the breeds constraints for all kennels
    int numKenels; //specifies the total number of kennels this store owns
    Kennel *kennels; // dynamically allocated array of kennels
} CatStore;

//global varible
const char STATUS_CAT[3][20] = {"ADOPTED", "PENDING", "AVAILABLE"};


//functions
char** readBreeds(int *count);
char* getCharPtrByBreed(char **dictionary, char *breedName, int breedCount);
CatStore *createStore(int kennelCount, int breedCount, char **dictionary);
Kennel* createKennels(int **constraints, int kennelCount, int breedCount, char **dictionary);
Cat **createCats(char **dictionary, int breedCount, int count);
Cat* createSingleCat(char **dictionary, int breedCount);
int canMoveTo(CatStore *s, char *location, char *breed, char **dictionary, int breedCount);
Kennel *getKennelByCat(CatStore *s, Cat *cat);
int getCatPosi(Kennel *home, Cat *cat);
Cat *getCatByName(CatStore *s, char *catName);
void removeCatFromKennel(Kennel *k, Cat *cat);
void runQueries(CatStore *s, char **dictionary, int breedCount,int numQueries);
void freeBreeds(char **dictionary, int breedCount);
void freeStore(int count, CatStore *store);


//main
int main(){
    char **dictionary; //to store array of dynamically allocated strings for breeds types (e.g., {"Ragdoll", "Siamese", "Maincoone"})
    int breedCount; //number of breed types
    int queries = 0;
    scanf("%d", &breedCount);//gets num of breeds from file
    int kennelCount = 0;
    dictionary = readBreeds(&breedCount);
    scanf("%d", &kennelCount);
    CatStore *Store  = createStore(kennelCount, breedCount, dictionary);//creates the store
    //gets the num of queries from user
    scanf("%d", &queries);
    runQueries(Store, dictionary, breedCount, queries);

    //frees memory
    freeBreeds(dictionary, breedCount);
    freeStore(kennelCount, Store);
    //printf("code ran\n");
    return 0;
}//end of main

//reads the breeds into the dictionary
char** readBreeds(int *count){
    //dynamically allocates memory for the string
    char** DicBreeds = (char**)malloc(*count * sizeof(char*));
    //goes through the file and scans the names into the character array
    for(int i = 0; i < *count; i++){
        //allocates space for the names
        DicBreeds[i] = (char*)malloc(26 * sizeof(char));
        //scans the inputs into the array
        scanf("%s", DicBreeds[i]);
    }
    //returns the array
    return DicBreeds;
}//end of func

char* getCharPtrByBreed(char **dictionary, char *breedName, int breedCount){
    //printf("getCharPtrByBreed is running \n");
    //loops through the dictionary
    for(int i = 0; i < breedCount; i++){
        //if dictionary index i is the same as breedname
        if(strcmp(dictionary[i],breedName) == 0){
            //printf("getCharPtrByBreed ran\n");
            return dictionary[i];//returns the index
        }
    }
    //if not found the return null
    //printf("getCharPtrByBreed ran Null \n");
    return NULL;
}//end of func

//creates a single cat
Cat* createSingleCat(char **dictionary, int breedCount){
    //printf("singleCat is running\n");
    //allocates memory for both the struct and the names
    Cat *SingleCat = (Cat*)malloc(sizeof(Cat));
    SingleCat->name = malloc(26 * sizeof(char));
    char breed[26];
    //gets data from the user
    scanf("%s %d %f %s", SingleCat->name, &SingleCat->age, &SingleCat->weight, breed);
    SingleCat->breed = getCharPtrByBreed(dictionary,breed,breedCount);//checks if breed is in dictionary
    SingleCat->status = 2;//sets status is AVAILABLE
    //printf("I ran SingleCat\n");
    return SingleCat;//returns the cat
}//end of func

//creates multiple cats
Cat **createCats(char **dictionary, int breedCount, int count){
    //printf("CreatCats is running\n");
    //allocates memory for the cats
    Cat **MultipleCats = (Cat**)malloc(count * sizeof(Cat*));
    //creates count number of cats 
    for(int i = 0; i < count; i++){
        MultipleCats[i] = createSingleCat(dictionary, breedCount);
    }
    //returns the double Pointer
    //printf("I ran CreateCats\n");
    return MultipleCats;
}//end of func

//creates kennels to store cats in
Kennel* createKennels(int **constraints, int kennelCount, int breedCount, char **dictionary){
    //printf("createKennels is running\n");
    //allocates memory for the kennels
    Kennel *catKennel = (Kennel*)malloc(kennelCount * sizeof(Kennel));
    //creates kennelCount amount of kennels
    for(int i = 0; i < kennelCount; i++){
         int countSum = 0;
        for(int j = 0; j < breedCount; j++){
            countSum += constraints[i][j];
        }
        catKennel[i].occupancy = 0;
        catKennel[i].location = malloc(26 * sizeof(char));
        scanf("%s %d", catKennel[i].location, &catKennel[i].occupancy);//occupancy is num cats in kennel
        catKennel[i].maxCapacity = countSum;
        catKennel[i].cats = createCats(dictionary, breedCount, catKennel[i].occupancy);
    }
    //returns kennels
    //printf("createKennels ran\n");
    return catKennel;
}//end of func

//creates the catStore
CatStore *createStore(int kennelCount, int breedCount, char **dictionary){
    //printf("createStore ran\n");
    //allocates memory for the story itself
    CatStore *store = (CatStore*)malloc(sizeof(CatStore));
    //gets numKenels from kennelCount
    store->numKenels = kennelCount;
    //allocates memory for the capacities
    store->capacities = (int**)malloc(store->numKenels * sizeof(int*));
    //inputs user data for the capacities of each kennel
    for(int i = 0; i < store->numKenels; i++){
        //allocates memory for each element
        store->capacities[i] = (int*)malloc(breedCount * sizeof(int));
        for(int j = 0; j < breedCount; j++){
            scanf("%d", &store->capacities[i][j]);
        }
    }
    store->kennels = createKennels(store->capacities, kennelCount, breedCount, dictionary);
    //returns the store
    //printf("Store Ran\n");
    return store;
}//end of func

//checks if user can move a cat to specified location
int canMoveTo(CatStore *s, char *location, char *breed, char **dictionary, int breedCount){
    //printf("canMoveTo is running\n");
    Kennel *destinationKennel = NULL;
    int count = 0;
    
    //checks for location
    for(int i = 0; i < breedCount; i++){
        if((strcmp(location, s->kennels[i].location) == 0)){
            destinationKennel = &s->kennels[i];
        }//end of if
    }//end of loop
    //printf("Got past the loop!\n");
    //if nothing was found in the first loop
    if(destinationKennel == NULL){
        //printf("canMove exit 0\n");
        return 0;//return not found
    }
    //checks kennel for the breed
    for(int i = 0; i < s->numKenels; i++){
        for(int j = 0; j < 100; j++){
            if(dictionary[j] != NULL && strcmp(dictionary[j], breed) == 0){
                count = j + 1;
                break;
            }//end of if
        }//end of j loop
    }//end of loop
    //printf("Got past loop 2!\n");
    //counts how many cats are in the target kennel already
    int catKennelCount = 0;
    for(int i = 0; i < destinationKennel->occupancy; i++){
        if(strcmp(destinationKennel->cats[i]->name, breed) == 0){
            catKennelCount++;
        }//end of if
    }//end of loop
    //printf("Got past loop 3!\n");
    //if count is greater or equal to capacity
    if(catKennelCount >= destinationKennel->maxCapacity && destinationKennel->occupancy >= destinationKennel->maxCapacity){
        //printf("Exit 2 return 0\n");
        return 0;//cat cannot be moved
    }
    //cat can be moved
    //printf("canMoveTo ran exit 1\n");
    return 1;
}//end of func

//returns the kennel of the specified cat
Kennel *getKennelByCat(CatStore *s, Cat *cat){
    //printf("I ran getKennel by cat\n");
    //loops through each kennel
    for(int i = 0; i < s->numKenels; i++){
        //loops through each cat
        for(int j = 0; j < i; j++){
            //if cat j in kennel i is equal to the inputted cat
            //printf("The current cat is %s\n", s->kennels[i].cats[j]->name);
            if(s->kennels[i].cats[j] == cat){
                //printf("I ran getKennel by cat exit 1\n");
                return &s->kennels[i]; //return kennel i
            }
        }
    }
    //if nothing found return NULL
    //printf("I ran getKennel by catexit 0\n");
    return NULL;
}//end of func

//Returns an int that represents what index the inputed cat is found at in home
int getCatPosi(Kennel *home, Cat *cat){
    //loops through homes cats
    for(int i = 0; i < home->occupancy; i++){
        //if cat[i] is the same at the inputed cat
        if(home->cats[i] == cat){
            return i;//return index
        }//end of if
    }//end of loop
    //if nothing is found
    return -1;
}//end of func

//searches the store, returns cat pointer to the cat with the inputed name
Cat *getCatByName(CatStore *s, char *catName){
    //printf("Getcat is running!\n");
    //loops through both the kennels(i) and the cats within the kennels (j)
    for(int i = 0; i < s->numKenels; i++){
        for(int j = 0; j < s->kennels[i].occupancy; j++){
            //if the names match, return the index
            if(strcmp(s->kennels[i].cats[j]->name, catName) == 0){
                //printf("Get cat ran exit TRUE\n");
                return s->kennels[i].cats[j];
            }//end of func
        }//end of j loop
    }//end of loop
    //incase nothing is found
    //printf("Get cat ran exit FALSE\n");
    return NULL;
}//end of fuc

//removes a specified cat from the given kennel
void removeCatFromKennel(Kennel *k, Cat *cat){
    //if position is invalid return NULL
    if(getCatPosi(k , cat) == -1) printf("Invalid Location\n");
    //printf("I RAN removeCat\n");
    //gets cats position, iterates from that starting position
    for(int i = getCatPosi(k , cat); i < k->occupancy - 1; i++){
        //shifts every cat within the array to the left by one
        k->cats[i] = k->cats[i+1];
    }
    //decreases the occupancy by one, resizes the array
    k->occupancy--;
    k->cats = (Cat*)realloc(k->cats, k->occupancy * sizeof(Cat*));
    //printf("I fully ran removeCat\n");
}//end of func

//handles the asked queries
void runQueries(CatStore *s, char **dictionary, int breedCount,int numQueries){
    int userQuerie = 0;
    //loops through all of the queries
    for(int i = 0; i < numQueries; i++){
        scanf("%d", &userQuerie);
        //prints the cats of the specified breed
        if(userQuerie == 1){
            char breedName[26];
            int exitVairble = 0;
            scanf("%s", breedName);
            for(int j = 0; j < s->numKenels; j++){
                for(int k = 0; k < s->kennels[j].occupancy; k++){
                    if(strcmp(breedName, s->kennels[j].cats[k]->breed) == 0){
                        //prints the cat of specified breed
                        printf("%s %.2lf %d %s %s\n", s->kennels[j].cats[k]->name, 
                            s->kennels[j].cats[k]->weight, s->kennels[j].cats[k]->age, 
                            s->kennels[j].location, STATUS_CAT[s->kennels[j].cats[k]->status]);
                            exitVairble = 1;
                    }//end of k if
                }//end of k loop
            }//end of j loop
            //if no matching breeds
            if(exitVairble == 0){
                printf("No cat with breed %s\n", breedName);
            }//end of if
        } else if(userQuerie == 2){
            //takes in a number and a cat name, changes status of cat. Removes cat if adopted
            char userString[26];
            int statNumb = 0;
            //changes the status of the cat
            scanf("%d %s", &statNumb, userString);
            for(int j = 0; j < s->numKenels; j++){
                //printf("Num of kennels is %d\n", s->numKenels);
                for(int k = 0; k < s->kennels[j].occupancy; k++){
                    //finds the cat and checks if number is less than 2
                    if(getCatByName(s, userString) == s->kennels[j].cats[k] && statNumb <=2){
                        s->kennels[j].cats[k]->status = statNumb;
                        printf("%s is now %s!\n", s->kennels[j].cats[k]->name, STATUS_CAT[statNumb]);
                        //if the cat is adopted
                        if(statNumb == 0){
                            removeCatFromKennel(&s->kennels[j],s->kennels[j].cats[k]);
                        }//end of inner most if
                    }//end of inner if
                    break;
                }//end of k loop
            }//end of j loop
        } else if(userQuerie == 3){ //THIS NEEDS TO BE UPGRADED
            char catName[26];
            char location[26];
            //gets name and location from the user
            scanf("%s %s", catName, location);
            //printf("before cat q3\n");
            //printf("%s is the name of the inputed cat\n", catName);
            Cat *q3cat = getCatByName(s, catName);
            //printf("cat ran q3, my cat name is %s \n", q3cat->name);
            //printf("cat ran q3, my breed is %s \n", q3cat->breed);
            //source and destination kennel
            Kennel *sourceKennel = getKennelByCat(s, q3cat);
            Kennel *destinationKennel = NULL;
            //printf("%s is my breed!\n", q3cat->breed);
            int dummyNum = canMoveTo(s, location, q3cat->breed, dictionary, breedCount);//checks to see if user can move cat
            //printf("%d is dummy\n" , dummyNum);
            //loops through the entire store90

            for(int j = 0; j < s->numKenels; j++){
                //sets the destination kennel to the one the user inputed
                destinationKennel = &s->kennels[j];
                //printf("I am looping it\n");
                //if source matches destination and dummy is 1 meaning it can be moved
                if(sourceKennel == destinationKennel && dummyNum == 1){
                    //reallocs space and removed cat from the source kennel
                    sourceKennel->occupancy--;
                    destinationKennel->occupancy++;
                    destinationKennel->cats = (Cat*)realloc(destinationKennel->cats, sizeof(Cat));
                    removeCatFromKennel(sourceKennel, q3cat);
                    //printf("%s moved successfully to %s\n", catName, location);
                    break;//breaks out of function
                }//end of if
            }//end of j loop
            //if cat cannot be moved
            if(dummyNum == 0){
                printf("%s cannot take a %s cat!\n", location, q3cat->breed);
            }//end if
        }//end of else if
    }// end of loops
}//end of func

//frees memory for the breeds
void freeBreeds(char **dictionary, int breedCount){
    //frees each element within dictionary
    for(int i = 0; i < breedCount; i++){
        free(dictionary[i]);
    }
    free(dictionary);
}//end of func

//frees memory for the entire store
void freeStore(int count, CatStore *store){
    //frees every element for kennels and cats
    for(int i = 0; i < count; i++){
        for(int j = 0; j < store->kennels[i].occupancy; j++){
            free(store->kennels[i].cats[j]->name);
            free(store->kennels[i].cats[j]);
        }
        free(store->kennels[i].location);
        free(store->kennels);
        free(store->capacities[i]);
    }
    //frees store memory
    free(store->capacities);
    free(store);
}//end of func

//end of code