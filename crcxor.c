/*
Assignment:
crcxor - CRC + XOR Algorithm Implementation
Author: Jayden Rodriguez Lugo
Language: C
To Compile:
gcc -O2 -std=c11 -o crcxor crcxor.c
To Execute (on Eustis):
./crcxor <message_file> <crc_algorithm>
where:
<message_file> is the path to the input text file
<crc_algorithm> is 3, 4, or 8 (for CRC-3, CRC-4, or CRC-8)
Notes:
- Implements CRC-3, CRC-4, and CRC-8 algorithms
- Implements the XOR (LRC) checksum in addition to CRC
- Processes plain text messages and computes CRC values
- Outputs all intermediate steps and final CRC values
- Tested on Eustis.
Class: CIS3360 - Security in Computing - Spring 2026
Instructor: Jie Lin, Ph.D.
Due Date: march 26 2026
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//file handling
FILE *fText;

//function prototypes
void procressMessage(char* plainTxt, char* CRCalg, int CRC);//converts plain text to decimal -> hex -> binary.
void crcAlgorithm(char* plainTxt, char* CRCx ,int CRC);//computes the CRC algorithm based on given CRC
void toBinary(char* inString, char* outString, int BitCount);//converts given instring to binary
void hextoBinary(char* inString, char* outString, int BitCount);//Converts from hex to binary
void padStr(char* inString, int CRC);//pads the string with the given CRC 
void RemoveSpace(char* plainTxt);//removes the spaces within the string
void AddSpace(char* plainTxt);//adds spaces within the string
void XOR_CheckSum(char* plainTxt, int CRC);//calculates the final message, returns in hex
void LRC(char* inString, char* block, int m);//OR-based integrity algorithm also called LRC (longitudinal redundancy check). m is total num of processed char bits

//main
int main(int argc, char *argv[]){
    // //gets input from the command prompt
    // if(argc != 3){
    //     printf("Error, more than three agrs passed\n");
    //     return 0;
    // }    

    // //gets input from command line
    // char* plainFileName = argv[1];
    // //opens the text files
    // fText = fopen(plainFileName, "r");

    // //if unable to open file
    // if(!fText){
    //     printf("Error when atempting to open file\n");
    //     return 1;
    // }//end of if
    // printf("Running\n");
    // printf("\n");//space
    //try to allocate enough space to fit an unknow amount of char into the string
    char dummy[900] = "vwxyz";//plaintext

    //gets the plaintext from file and prints it out
    // char dummy[900];//plaintext
    // if(!fgets(dummy, sizeof(dummy), fText)){
    //     //if unable to get file data
    //     printf("Error, unable to reading the file!\n");
    //     fclose(fText);
    //     return 1;
    // }//end of if
    //allocates large buffer to prevent memory issues
    char* plainText = (char*)malloc(2000);
    if(!plainText){
        //if unable to allocate memory
        printf("Error, unable to allocating memory!\n");
        //fclose(fText);
        return 1;
    }//end of if

    // int CRC = atoi(argv[2]);//given CRC
    int CRC = 3;
    char* crcVal = (char*)malloc(25 * sizeof(char));//CRC value
    if(!crcVal){
        //if unable to allocate memory
        printf("Error, unable to allocate memory!\n");
        free(plainText);
        fclose(fText);
        return 1;
    }//end of if

    char TEMP_crcValconvertion[25];//temp conversion value
    strcpy(plainText, dummy);//inputs string from dummy into the plainText for use

    printf("The original message:\n%s\n \n", plainText);

    //converts the text to uppercase, prints out processed text
    RemoveSpace(plainText);
    printf("The preprocessed message (invisible characters removed):\n%s\n \n", plainText);

    //processes the message
    procressMessage(plainText, crcVal, CRC);
    
    //processes the crc value
    printf("The crc value for the chosen crc algorithm in binary:\n%s\n\n", crcVal);
    
    //converts the binary into hex
    int j=0;
    char HexBitMain;
    int length = strlen(crcVal);
    //if the CRC is not 3, perform this algroithm for the hex calc
    if(CRC != 3){
        for(int i = 0; i < length; i+= 4){
            int j = 0;//resets j every loop
            for(int b = 0; b < 4; b++){//loops through current block
                j = (j << 1) | (crcVal[i + b] - '0');//shifts j by 1 and ORs (it with crcVal[i + j] - '0')
            }//end of inner loop
            //when value is a number
            if(j < 10){
                HexBitMain = '0' + j;
            }else{//when value is a letter
                HexBitMain = 'A' + (j - 10);
            }//end of else if
            //puts the results into the temp string
            strncat(TEMP_crcValconvertion, &HexBitMain, 1);
        }//end of loop
    }else{//when the CRC is 3
        for(int i = 0; i < length; i++){
            j = j * 2 + (crcVal[i] - '0');
        }//end of loop
        //when value is a number
        if(j < 10){
            HexBitMain = '0' + j;
        }else{//when value is a letter
            HexBitMain = 'A' + (j - 10);
        }//end of else if
        sprintf(TEMP_crcValconvertion, "%c", HexBitMain);//puts the results into the temp string
    }//end of else if

    printf("The crc value for the chosen crc algorithm in hex:\n%s\n\n", TEMP_crcValconvertion);//prints temp str
    free(crcVal);//frees value

    //converting the plaintext to hex and adding the crc value
    j = 0;
    char tempString[900];//temp string
    RemoveSpace(plainText);
    //adds the hexvalues to the plainText
    length = strlen(plainText + 1);
    for(int i = 0; i <= length; i++){
        sprintf(tempString + j, "%02X", plainText[i]);
        j += 2;
    }//end of loop
    
    strcpy(plainText, tempString);
    strcat(plainText, TEMP_crcValconvertion);

    printf("The input in hex for this XOR checksum computation:\n%s\n\n", plainText);
     
    XOR_CheckSum(plainText, CRC);//calcs the XOR

    //prints final plainText
    printf("The final message is going to be transmitted in hex:\n%s\n\n", plainText);

    free(plainText);
    fclose(fText);
    return 0;
}//end of main


//converts plain text to decimal -> hex -> binary, pads message aswell
void procressMessage(char* plainTxt, char* CRCalg, int CRC){
    //prepares the given plaintext
    AddSpace(plainTxt);
    int length = strlen(plainTxt);
    //prints string in decimal
    printf("The decimal representation of the preprocessed message:\n");
    for(int i = 0; i < length; i++){
       if(isalpha(plainTxt[i]) != 0 || isdigit(plainTxt[i]) != 0){//if letter print
           printf("%d", plainTxt[i]);
        }else{
            printf("%c", plainTxt[i]);
        }  
    }
    printf("\n");

    //prints string in hex
    length = strlen(plainTxt);
    printf("\nThe hex representation of the preprocessed message:\n");
    for(int i = 0; i < length; i++){
       if(isalpha(plainTxt[i]) != 0 || isdigit(plainTxt[i]) != 0){//if letter print
           printf("%X", plainTxt[i]);
        }else{
            printf("%c", plainTxt[i]);
        }  
    }
    printf("\n");
    //converts string into binary using a temp convertedString
    char convertedString[900];
    toBinary(plainTxt, convertedString, 8);
    printf("\nThe binary representation of the preprocessed message:\n%s\n\n", convertedString);

    //pads out the string with 0
    padStr(convertedString, CRC);
    printf("The binary representation of the original message prepared for CRC computation\n");
    printf("(padded with %d zeros):\n%s\n\n", CRC, convertedString);
    //gets the CRC value 
    crcAlgorithm(convertedString, CRCalg, CRC);
}//end of func

//calculates the CRC result based on the chosen CRC and the results given from the XOR division
void crcAlgorithm(char* plainTxt, char* CRCx ,int CRC){
    //checks what CRC is being used
    if(CRC == 3){//CRC3
        strcpy(CRCx, "1101");
    }else if(CRC == 4){//CRC4
        strcpy(CRCx, "10110");
    }else if(CRC == 8){//CRC8
        strcpy(CRCx, "100110101");
    }

    RemoveSpace(plainTxt);//removes any spaces from the plainTxt

    //preforms the XOR division 
    int len = strlen(CRCx);
    int txtLen = strlen(plainTxt);
    char crcxResults[20];//temp results string
    strncpy(crcxResults, plainTxt, len);//copies the CRC into crcxresults

    //loop that XORs the rest of the string
    for(int i = len; i <= txtLen; i++){
        //if leading bit is 1
        if(crcxResults[0] == '1'){
            //XOR with the generator
            for(int j = 0; j < len; j++){
                //converts to 1s and 0s then XORS, adds back the ascii 0
                crcxResults[j] = ((crcxResults[j] - '0') ^ (CRCx[j] - '0')) + '0';
            }//end of inner loop
        }//end of if

        //shift everything to the left
        for(int j = 0; j < len-1; j++){
            crcxResults[j] = crcxResults[j+1];
        }//end of inner loop 2

        //if we still have bits left in the message
        if(i < txtLen){
            crcxResults[len-1] = plainTxt[i];
        }else{//if no more bits left
            crcxResults[len-1] = '0';
        }//end of else if
    }//end of loop
    crcxResults[len-1] = '\0';
    strcpy(CRCx,crcxResults);
    //printf("crc after calculations %s\n", CRCx);    
}//end of func

void XOR_CheckSum(char* plainTxt, int CRC){
    char convertedString[900];
    //printf("in string %s\n", plainTxt);
    hextoBinary(plainTxt, convertedString, 4);
    //splits the text
    int j = 0;
    char tempStr[1200];
    int tLength = strlen(convertedString);
    for(int i = 0; i < tLength; i++){
        if(i > 0 && i % 4 == 0){
            tempStr[j++] = ' ';
        }
        tempStr[j++] = convertedString[i]; 
    }
    tempStr[j] = '\0';
    printf("The input in binary for this XOR checksum computation:\n%s\n\n", tempStr);

    //calculating the XOR checksum value
    char blocks[300];
    LRC(tempStr, blocks, CRC+1);
    printf("The xor checksum value for the chosen crc algorithm in binary:\n%s\n\n", blocks);

    //converts blocks to hex, adds hex to plainTxt
    int value = 0;
    char Hexbit;
    for(int i = 0; i < 4; i++){
        //value either shifts to the left by 1 ORs it with the binary in blocks[i]
        value = (value << 1) | (blocks[i] - '0');
    }//end of loop

    //when value is a number
    if(value < 10){
        Hexbit = '0' + value;
    }else{//when value is a letter
        Hexbit = 'A' + (value - 10);
    }//end of else if

    printf("The xor checksum value for the chosen crc algorithm in hex:\n%c\n\n", Hexbit);
    //puts the Hexbit at the end
    int len = strlen(plainTxt);
    plainTxt[len] = Hexbit;
    plainTxt[len + 1] = '\0';
}//end of func

//LRC (longitudinal redundancy check). m is total num of processed char bits (CRC)
//XORs each block of binary (iteration 1 A XOR B = res,iteration 2, res XOR c = res, iteration 3 res XOR D....)
void LRC(char* inString, char* block, int m){
    int j = 0;
    int length = strlen(inString);
    //putting A from the inString into the 
    for(int a = 0; a < 4; a++){
        block[a] = inString[a];
        j++;
    }//end of loop
    block[j] = '\0';
    //printf("%s current block\n", block);
    int b = 0;
    //loop that XORs the rest of the string
    for(int i = 5; i < length; i++){
        //printf("blocks currently %s iteration %d\n", block, i);
        //printf("current inString bit %c\n", inString[i]);
        //printf("b is %d\n", b);
        //XORs each block
        if(inString[i] == '0' || inString[i] == '1'){//checks if digit
            //gets the binary for prev block and the current block
           char bit1 = block[b] - '0';
           char bit2 = inString[i] - '0';
           //XORs results and puts results into the prev block
           char results = (bit1^bit2) + '0';
           //printf("results %c\n", results);
           block[b] = results;
           b++;//go to next position in block
           if(b == 4){b = 0;}
           //printf("blocks currently %s iteration\n", block, i);
        }else{
            //when there is a space, reset b to 0, this means a new binary block to XOR
            b = 0;
        }//end of else if

    }//end of second loop
}//end of func

//removes the spaces from a given string
void RemoveSpace(char* plainTxt){
    int j = 0;
    int length = strlen(plainTxt);
    for(int i = 0; i < length; i++){
        if(isalpha(plainTxt[i]) != 0 ){//if letter make it uppercase
            plainTxt[j] = plainTxt[i];
            j++;
        }else if(isdigit(plainTxt[i]) != 0){//if number then make plainText[j] = plainText[i]
            plainTxt[j] = plainTxt[i];
            j++;
        }
    }//end of loop
    plainTxt[j] = '\0';
}//end of func

//adds a space between characters in a string
void AddSpace(char* plainTxt){
    char sum[900];//dummy string to add spaces with 
    int j = 0;
    int length = strlen(plainTxt);
    for(int i = 0; i < length; i++){
        sum[j++] = plainTxt[i];
        sum[j++] = ' ';
    }//end of loop
    sum[j-1] = '\0';
    strcpy(plainTxt, sum);
}//end of func

//converts from plaintext to binary
void toBinary(char* inString, char* outString, int BitCount){
    int k = 0;
    int bLen = strlen(inString);
    //printf("Entered function\n");
    for(int i = 0; i < bLen; i++){
        int c = 0;//stores the acssi value of the current character
        //printf("Big loop %d\n", i);
        if(isalpha(inString[i]) != 0 || isdigit(inString[i]) != 0){//if letter print
            //printf("Beginning of loop \n");
            //temp bit array
            char *bits = malloc((BitCount + 1) * sizeof(char));
            int c = inString[i];
           //goes from MSB to LSB
           for(int b = 0; b < BitCount; b++){
                //printf("Running inner %d\n", b);
                if(c%2 == 1){
                    bits[(BitCount-1)-b] = '1';
                }else if(c%2 == 0){
                    bits[(BitCount-1)-b] = '0';
                }
                c /= 2;
           }//end of loop
           //printf("Running outter %d\n", i);
           sprintf((char*)(outString + k),"%s", bits);//copis bits to outString
           k += BitCount;
           free(bits);//frees bits to be reused again
           //printf("Running out %d\n", i);
        }else{
            //when space
            //printf("Im called!!\n");
            outString[k++] = inString[i];
        }//end of else if
        outString[k] = '\0';//add null terminator at end
    }//end of loop
}//end of func

void hextoBinary(char* inString, char* outString, int BitCount){
    int k = 0;
    int bLen = strlen(inString);
    //printf("Entered function\n");
    for(int i = 0; i < bLen; i++){
        int c = 0;//stores the hex value of the current character
        //printf("Big loop %d\n", i);
        if(isalpha(inString[i]) != 0 || isdigit(inString[i]) != 0){//if letter print
            char *bits = malloc((BitCount + 1) * sizeof(char));

            //converting hex into decimal
            if(inString[i] >= '0' && inString[i] <= '9'){//when number
                c = inString[i] - '0';
            }else{//when letter
                c = toupper(inString[i]) - 'A' + 10;//toupper to ensure that instring[i] is in uppercase
            }//end of else if

           //goes from MSB to LSB
           for(int b = BitCount - 1 ; b >= 0; b--){
                if(c & 1){//if c and 1 is true then bit b is 1
                    bits[b] = '1';
                }else{//if if c and 1 is false then bit b is 0
                    bits[b] = '0';
                }//end of else if
                c >>= 1;//shifts everything in c by 1
           }//end of loop
           bits[BitCount] = '\0';

           sprintf((char*)(outString + k),"%s", bits);//copis bits to outString
           k += BitCount;
           free(bits);//frees bits to be reused again
        }//if
        outString[k] = '\0';//add null terminator at end
    }//end of loop
}

//pads the string with the given CRC 
void padStr(char* inString, int CRC){
    int j = strlen(inString);
    inString[j++] = ' ';//adds space to the string
    //pads given string with 0. Num of zeros is dependent on the chosen CRC
    for(int i = 0; i < CRC; i++){
        inString[j++] = '0';
    }//end of loop
    inString[j] = '\0';//adds null terminator back
}//end of func

//end of code
