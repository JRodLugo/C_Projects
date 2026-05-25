/*
Assignment:
hillplayfair - Hill cipher followed by Playfair cipher
Author: <Jayden Rodriguez Lugo>
Language: C
To Compile:
gcc -O2 -std=c11 -o hillplayfair hillplayfair.c
To Execute (on Eustis):
./hillplayfair encrypt key.txt plain.txt keyword.txt
where:
key.txt = key matrix file
plain.txt = plaintext file
keyword.txt = Playfair keyword file
Notes:
- Input is text; process A-Z only (case-insensitive).
- Tested on Eustis.
Class: CIS3360 - Security in Computing - Spring 2026
Instructor: Dr. Jie Lin
Due Date: February 16th 2026
UCF ID: 5505335
*/


//libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


//file handling
FILE *hKey, *fText, *pKey;

//func prototypes
int charMapper(char mappedText);
char intMapper(int mappedInt);
int** HillCipher(int **key, char *pText, int size);
void genPlayFairTable(char *key, char **table);
void PlayFairCipher(char **table, char *pText);
void printString(char *pText);

//main func
int main(int argc, char *argv[]){
    if(argc != 5){
        printf("Error, more than five agrs passed\n");
        return 1;
    }
    //gets input from the command prompt
    // printf("argc = %d\n", argc);
    // for (int i = 0; i < argc; i++){
    //     printf("argv[%d] = %s\n", i, argv[i]);
    // }
    // printf("\n");
    // if (argc > 1){//left over code from the starter. Not 100% sure on what it does or why its here
    //     int x = atoi(argv[1]); // convert string to int (simple)
    //     printf("Converted argv[1] to int: %d\n", x);
    // }else{//when no command line arguments found
    //     printf("No extra arguments provided.\n");
    //     printf("Try: ./00_args 123\n");
    //     return 0;//return 0
    // }

    //gets input from command line
    char* mode = argv[1];
    char* HillFileName = argv[2];
    char* plainFileName = argv[3];
    char* playFairFileName = argv[4];

    //opens the text files
    fText = fopen(plainFileName, "r");
    pKey = fopen(playFairFileName, "r");
    hKey = fopen(HillFileName, "r");
    //if unable to open file
    if(!fText){
        printf("Failed to open %s\n", plainFileName);
        return 1;
    }
    if(!pKey){
        printf("Failed to open %s\n", playFairFileName);
        fclose(fText);
        return 1;
    }
    if(!hKey){
        printf("Failed to open %s\n", HillFileName);
        fclose(fText);
        fclose(pKey);
        return 1;
    }
    // if(!fText || !pKey || !hKey){
    //     printf("Error when attempting to open file");
    //     return 1;
    // }//end of if

    printf("\n");//space

    //varibles
    char plainText[9000];
    int hillKeyDimensions = 0;
    int **hillKey;//hill cipher key
    char playfKey[26];//play fair key
    char **playFairTable;//5x5 playfair table
    int **hillResults;//the encoded or decoded cipher
    //selects the mode
    printf("Mode:\n");
    printf("Encryption Mode\n\n");

    //gets the plaintext from file and prints it out
    int buffer = 0;
    int ch = 0;
    while((ch = fgetc(fText)) != EOF){
        plainText[buffer++] = ch;
    }
    plainText[buffer] = '\0';

    printf("Original Plaintext:\n");
    printf("%s\n", plainText);
    printf("\n");
    //converts the text to uppercase, prints out processed text
    int j = 0;
    int len = strlen(plainText);
    for(int i = 0; i < len; i++){
        if(isalpha(plainText[i])){
            plainText[j] = toupper(plainText[i]);
            j++;
        }
    }//end of loop
    plainText[j] = '\0';

    printf("Preprocessed Plaintext:\n");
    printString(plainText);
    printf("\n");

    //pads an extra X to the end of the string if string is not an even number
    if((strlen(plainText) % 2) == 1){
        //printf("Im running!\n");
        plainText[j] = 'X';
        plainText[j+1] = '\0';
        //strcat(plainText, 'X');
    }

    /*Hill Cipher Calculations*/

    //reads everything from the hill cipher file into the hillkeyDimensions and hillkey
    fscanf(hKey, "%d", &hillKeyDimensions);
    printf("Hill Cipher Key Dimension:\n");
    printf("%d\n", hillKeyDimensions);
    printf("\n");
    printf("Hill Cipher Key Matrix:\n");
    //dynammic 2d array for the hillkey
    hillKey = (int**)malloc(hillKeyDimensions * sizeof(int*));
    //scans the Hillcypher key into the 2d array
    for(int i = 0; i < hillKeyDimensions; i++){
        hillKey[i] = (int*)malloc(hillKeyDimensions * sizeof(int));
        for(int j = 0; j < hillKeyDimensions; j++){
            fscanf(hKey, "%d", &hillKey[i][j]);//scans data into position (i,j)
            printf("%d ", hillKey[i][j]);//prints number that was just scanned
        }//end of inner loop
        printf("\n");
    }//end of outer loop
    printf("\n");

    //prints out the padded Hill Cipher Plaintext
    printf("Padded Hill Cipher Plaintext:\n");
    printString(plainText);
    printf("\n");
    //prints out Ciphertext
    printf("Ciphertext after Hill Cipher:\n");
    //calculates the hill cipher
    hillResults = HillCipher(hillKey, plainText, hillKeyDimensions);
    //prints out hill cipher
    int charCounter = 0;
    for(int i = 0; i < strlen(plainText)/hillKeyDimensions; i++){
        for(int j = 0; j < hillKeyDimensions; j++){
            printf("%c", intMapper(hillResults[i][j]));
            charCounter++;
        }
        //when character limit is reached
        if((charCounter % 80) == 0){
            //print new line
            printf("\n");
        }
    }//end of print loop
    printf("\n \n");
    

    /*playfair calculations*/

    fgets(playfKey, sizeof(playfKey), pKey);
    printf("Playfair Keyword:\n");
    printf("%s\n\n", playfKey);

    //prepares the plaintext further inscase of any dupe letters
    
    //strcpy(PfText, hillstr)
    j = strlen(plainText);
    for(int i = 0; i < j - 1; i++){
        if(plainText[i] == plainText[i+1]){
            for(int k = j; k >= i+1; k--){
                plainText[k+1] = plainText[k];
            }
            plainText[i+1] = 'X';
            j++;
            plainText[j] = '\0';
            i++;//skips the X
        }
    }//end of loop
    
    if((strlen(plainText) % 2) == 1){
        plainText[j] = 'Z';
        plainText[j+1] = '\0';
    }//end of if

    playFairTable = (char**)malloc(5* sizeof(char*));
    for(int i = 0; i < 5; i++){
        playFairTable[i] = (char*)malloc(5*sizeof(char));
    }
    genPlayFairTable(playfKey,playFairTable);//genertates the table
    printf("Playfair Table:\n");
    //prints the table
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            printf("%c ", playFairTable[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("Ciphertext after Playfair:\n");
    PlayFairCipher(playFairTable, plainText);

    //closes the files
    fclose(fText);
    fclose(hKey);
    fclose(pKey);

    //frees memory for all dynammically allocated items
    for(int i = 0; i < hillKeyDimensions; i++){
        free(hillKey[i]);
        free(hillResults[i]);
    }
    for(int i = 0; i < 5; i++){
        free(playFairTable[i]);
    }
    free(playFairTable);
    free(hillKey);
    free(hillResults);
}//end of main

//prints the plain text how the file wants
void printString(char *pText){
    unsigned int len = strlen(pText);
    //loops over entire string
    for(unsigned int i = 0; i < len; i++){
        printf("%c", pText[i]);
        //when character limit is reached
        if(((i + 1) % 80) == 0){
            //print new line
            printf("\n");
        }
    }//end of loop
    printf("\n");
}//end of func

/*
Char to num mapping for charMapper and intMapper
0 = A, 1 = B, 2 = C, 3 = D, 4 = E , 5 = F, 6 = G, 7 = H, 8 = I, 9 = J, 10 = K, 11 = L, 12 = M, 13 = N, 14 = 0
15 = P, 16 = Q, 17 = R, 18 = S, 19 = T, 20 = U, 21 = V, 22 = W, 23 = X, 24 = Y, 25 = Z
*/

//maps each character to there respective table value
int charMapper(char mappedText){
    //converts the character into the correct int value
    if(mappedText >= 'A' && mappedText <= 'Z'){
        return mappedText - 'A';//returns current character - A to return one of the values from the table
    }
    //error value incase nothing was found
    return -1;
}//end of func

char intMapper(int mappedInt){
    //converts digits back into characters
    if(mappedInt >= 0 && mappedInt <= 25){
        return 'A' + mappedInt;
    }//end of if
    return '?';//if nothing was found
}//end of func

//generates a hill cipher based on inputed conditions
int** HillCipher(int **key, char *pText, int size){
    int block = strlen(pText)/size;//the size of each block
    int **results = (int**)malloc(block * sizeof(int*));//2d int array for the results
    
    int index = 0;
    
    //iterates through the entire string
    for(int i = 0; i < block; i++){
        //allocates memory for the row
        results[i] = (int*)malloc(size * sizeof(int));
        //builds the current vector
        int textMatrix[size];
        for(int j = 0; j < size; j++){
            //converts the character to a number 0-25, inputs result into the array
            textMatrix[j] = charMapper(pText[index + j]);
        }//end of j loop

        //multiplies the key * textMatrix
        for(int r = 0; r < size; r++){
            int sum = 0;
            //inputs the results into sum
            for(int c = 0; c < size; c++){
                sum += key[r][c] * textMatrix[c];
            }//end of column loop
            results[i][r] = sum % 26;//inputs sum mod 26 into the array
        }//end of row loop

        index += size; //moves index to the next block
    }
    //returns the 2d array
    return results;
}//end of func

//generates a 5x5 table
void genPlayFairTable(char *key, char **table){
    //initialises keylength and the hash
    int keylen = strlen(key);
    int hash[26] = {0};

    //removes j from the key if there is
    for(int i = 0; i < keylen; i++){
        char tempCh = toupper(key[i]);
        //skips possible non-letters
        if(tempCh < 'A' || tempCh > 'Z'){
            continue;
        }//end of if
        //when J is found
        if(tempCh == 'J'){
            continue;//skip
        }//end of if
        //marks the letters that can be placed
        if(hash[tempCh - 'A'] == 0){
            hash[tempCh - 'A'] = 2;
        }//end of if
    }//end of loop

    hash['J' - 'A'] = 1;//to get rid of the j, 1 means cannot be placed in table

    int k = 0; 
    int j = 0;
    //inputs the key into the table
    for(int i = 0; i < keylen; i++){
        char tempCh = toupper(key[i]);
        if(hash[tempCh - 'A'] == 2){
            hash[tempCh - 'A'] = 1;//to avoid dupes sets used letters to 1
            //inputs key into index k j++
            table[k][j] = tempCh;
            //printf("j current value is %d\n", j);
            j++;
            if(j == 5){
                k++;
                //printf("k has gone up by 1 k value %d\n", k);
                j = 0;
            }//end of inner if
        }//end of if
    }//end of loop

    //fills in rest of alphabet
    for(int i = 0; i < 26; i++){
        if(hash[i] == 0){
            //inputs the letters of the alphabet into the array
            table[k][j] = (char)(i + 65);
            //printf("j current value is %d\n", j);
            j++;
            if(j == 5){//when j reaches the end of the table
                //printf("k has gone up by 1 k value %d\n", k);
                k++;//set k to the next row
                j = 0;//set j to the first column
            }//end of inner if
        }//end of if
    }//end of loop
}//end of func


//generates the PlayFairCipher
void PlayFairCipher(char **table, char *pText) {
    int len = strlen(pText);

    //double checks for padding, incase a J is inside the string and is next to an I
    char *cipherText = malloc((len * 2 + 2) * sizeof(char));
    int ci = 0;//cipher index

    //double checks the string for any dupes or Js that might have slipped by
    for (int i = 0; i < len; i++) {
        char a = toupper(pText[i]);
        //when a is J, convert it into an I
        if (a == 'J'){
           a = 'I'; 
        }//end of if 
        
        cipherText[ci++] = a;//add a back into the string, increment cipher index

        //checks for dupe pairs
        if (i + 1 < len) {
            char b = toupper(pText[i + 1]);
            //when b is J, set be to I
            if (b == 'J'){
                b = 'I';
            } //end of if

            //insert X between dupes
            if (a == b) {
                cipherText[ci++] = 'X';   
            } else {
                cipherText[ci++] = b;
                i++;                
            }//end of if else
        }//end of if
    }//end of loop

    //append X if string is odd
    if (ci % 2 != 0) {
        cipherText[ci++] = 'X';
    }//end of if

    cipherText[ci] = '\0';//adds null terminator to end of string

    //preforms the playfair encryption 
    for (int i = 0; i < ci; i += 2) {
        char a = cipherText[i];
        char b = cipherText[i + 1];
        //row/column A, row/column B
        int rA, cA, rB, cB;

        //Locates where a and b are in the playfair table
        for (int r = 0; r < 5; r++) {
            for (int c = 0; c < 5; c++) {
                if (table[r][c] == a) {
                    rA = r; cA = c;
                }
                if (table[r][c] == b) {
                    rB = r; cB = c;
                }//end of ifs
            }//end of column loop
        }//end of row loop

        //Apply Playfair calculations
        if (rA == rB) {// Same row → shift right
            cA = (cA + 1) % 5;
            cB = (cB + 1) % 5;
        }
        else if (cA == cB) {// Same column
            rA = (rA + 1) % 5;
            rB = (rB + 1) % 5;
        }
        else {// Rectangle rule
            int temp = cA;
            cA = cB;
            cB = temp;
        }//end of else if

        //inputs results back into the plain text
        cipherText[i] = table[rA][cA];
        cipherText[i + 1] = table[rB][cB];
    }//end of loop

    //prints out the ciphertext
    printString(cipherText);
    free(cipherText);//frees dynamic memory used
}//end of func

//end of code