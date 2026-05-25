/*
Jayden Rodriguez Lugo
COP 3223C - intro to C
11/18/25
*/

/*  
1) Add a book to the collection
2) Delete a book from the collection
3) Search for a book from the collection
4) List all of the books by a particular author from the collection
5) List all of the books of a particular subject from the collection 
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_LENGTH 40
#define MAX_BOOKS 1000

FILE *inp1;

//structures
struct book {
    char title[MAX_LENGTH];
    char author[MAX_LENGTH];
    char subject[MAX_LENGTH];
};//end of struc

struct library {
    struct book collection[MAX_BOOKS];
    int num_books;
};//end of struc

//function protoypes
void copybook(struct book* dest, struct book* source);
void addBook(struct library* thislib);
void deleteBook(struct library* source, char* target);
void searchBook(struct library* libBook, char* wantedBook);
void ListByAuthor(struct library* libBook, char* author);
void ListBySubject(struct library* libBook, char* subject);

//main function
int main(){
    //struct varibles
    struct library lLibrary;

    int counter = 0;
    int numOfBooks = 0;
    char looking[MAX_LENGTH];

    //opening the given file
    inp1 = fopen("library.txt", "r");
    //gets the number of books
    fscanf(inp1 , "%d", &numOfBooks);
    //sets the number of books in the library to 0
    lLibrary.num_books = 0;
    //loops through the numbOfBooks
    for(int i = 0; i < numOfBooks; i++){
        //gets input from the file
        fscanf(inp1, "%d", &counter);
        //switch case for the multiple commands given from the file
        switch(counter){
            case 1:
                addBook(&lLibrary);
                break;
            case 2:
                fscanf(inp1,"%s",looking);
                deleteBook(&lLibrary, looking);
                break;
            case 3:
                fscanf(inp1,"%s",looking);
                searchBook(&lLibrary, looking);
                break;
            case 4:
                fscanf(inp1,"%s",looking);
                ListByAuthor(&lLibrary, looking);
                break;
            case 5:
                fscanf(inp1,"%s",looking);
                ListBySubject(&lLibrary, looking);
                break;
            default:
                printf("Funtion not listed\n");
                break;

        }//end of switch
    }//end of for loop

    //closes the file
    fclose(inp1);
}//end of main



//adds books to the collection
void addBook(struct library* thislib){
    struct book lBook;
    //gets the title
    fscanf(inp1,"%s",lBook.title);
    //gets the author
    fscanf(inp1,"%s",lBook.author);
    //gets the subject
    fscanf(inp1,"%s",lBook.subject);
    //inputs the book into the collection
    thislib->collection[thislib->num_books]=lBook;
    //increments the num of books by one
    thislib->num_books += 1;
    printf("The Book %s has been added\n", lBook.title);

}//end of func


//"removes" a book from the collection
void deleteBook(struct library* source, char* target){
    for(int i = 0; i < source->num_books - 1; i++){
        //checks to see if they are both the same length
        if(strcmp(source->collection[i].title, target) == 0){
            //replaces the book with the last book in the collection
            copybook(&source->collection[i], &source->collection[source->num_books]);
            printf("The book %s has been removed from the library.\n", target);
            break;
        }
    }
    //decreases the amount of books by 1
    source->num_books -= 1;
}// end of func

//copies the contents of one book into another book
void copybook(struct book* dest, struct book* source){
    strcpy(dest->title,source->title);
    strcpy(dest->author,source->author);
    strcpy(dest->subject,source->subject);
}//end of func

//looks for books within the collection
void searchBook(struct library* libBook, char* wantedBook){
    //loops through the number of books
    int foundChecker = 1; //checks if book was even found
    for(int i = 0; i < libBook->num_books; i++){
        //if the title matches the wanted book
        if(strcmp(libBook->collection[i].title, wantedBook) == 0){
            printf("The book %s is currently in the library.\n", wantedBook);
            foundChecker = 0; // sets checker to 
            break;
        }//end of if else
    }//end of loop
    //if no matches found
    if(foundChecker != 0){
        printf("The book %s is NOT currently in the library.\n", wantedBook);
    }
    

}//end of func

//lists all the books by the inputed author
void ListByAuthor(struct library* libBook, char* author){
    printf("List of all books by %s\n", author);
    for(int i = 0; i < libBook->num_books; i++){
        //checks to see if they are both the same length
        if(strlen(libBook->collection[i].author) == strlen(author)){
            for(int j = 0; j < strlen(author); j++){
                //checks if they are the same word
                int matchChecker = 0;
                for(int k = 0; k < strlen(author); k++){
                    if(toupper(libBook->collection[i].author[k]) != toupper(author[k])){
                        matchChecker = 1; // if not the same word sets match checker to 1
                    }//end of nested if
                }//end of nested loop
                //checks uppercases the charaters and checks if they match
                if(matchChecker == 0){
                    //if a match then print the books
                    printf("%s\n", libBook->collection[i].title);
                    break;
                }//end of if
            }// end of loop
        }// end of outer if
    }//end of outer loop

}//end of fun


//lists all the books based on the inputed subject
void ListBySubject(struct library* libBook, char* subject){
    printf("List of all books on %s\n", subject);
    //loops through the books
    for(int i = 0; i < libBook->num_books; i++){
        //checks to see if they are both the same length
        if(strlen(libBook->collection[i].subject) == strlen(subject)){
            for(int j = 0; j < strlen(subject); j++){
                //checks if they are the same word
                int matchChecker = 0;
                for(int k = 0; k < strlen(subject); k++){
                    if(toupper(libBook->collection[i].subject[k]) != toupper(subject[k])){
                        matchChecker = 1; // if not the same word sets match checker to 1
                    }//end of nested if
                }//end of nested loop
                //checks uppercases the charaters and checks if they match
                if(matchChecker == 0){
                    //if a match then print the books
                    printf("%s\n", libBook->collection[i].title);
                    break;
                }//end of if
            }// end of loop
        }// end of outer if
    }//end of outer loop

}//end of func


//end of code