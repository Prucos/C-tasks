/**
 The code below utilizes hash table + Linked lists functonality to store almost 200 000 words dictionary in RAM.
 After that, code analyzes the given text and returns misspelled words.
 **/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

/*
    Node type definition
*/ 
typedef struct nodes {
    char word[46];
    struct nodes *next;
}
node;

/*
    Returnes a hashcode for a given value 
*/  
node *hashArr[200] = { NULL };
int wordsCounter = 0;
    
int hashAd(char* word) {
    int length = strlen(word);
    int hashSum = 0;
    for (int i = 0; i < length; i++) {
        if (word[i] == '\'') continue;
        hashSum += (tolower(word[i]) - 'a');
    }
    return hashSum % 200;
}


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // preparing word for checking
    
    int wordLen = strlen(word);
    char lowerWord[46];
    memset( lowerWord, 0, 46);
    //printf("\nsearching for a raw word %s", word);          // DEBUG
    
    for (int i = 0; i < wordLen; i++) {
        char c = word[i];
        lowerWord[i] = tolower(c);
    };
    
   int wordIndex = hashAd(lowerWord);
   
   //check function
   
   if (hashArr[wordIndex] == NULL) {
        //printf("NULL AT SEARCG!");                                     //DEBUG
        return false;
    }
    node *crawler = hashArr[wordIndex];
    while (crawler) {
        if (!strcmp(crawler->word, lowerWord)) {
            //printf ("\nEQUAL words %s and %s", word, lowerWord);                  //DEBUG
            return true;
        } else {
            crawler = crawler->next;
        }
    };
    //printf ("\n%s and %s words not found or mispelled", word, lowerWord);                  //DEBUG
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    node *new;
    
    // lengh of hash table
    //int lenHash = sizeof(hashArr) / sizeof(hashArr[0]);
    
    FILE* dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        fclose(dict);
        return 2;
    }
    
    char wordBuffer[46];
    
    
    while(fscanf(dict, "%s", wordBuffer ) != EOF) {
        int hashCode = hashAd(wordBuffer);
        new = malloc(sizeof(node));
        // check for successfull malloc
        if (new == NULL) {
            printf("Error, couldn't accolate memmory");
            return 1;
        };
        
        strcpy(new->word, wordBuffer);
        new->next = hashArr[hashCode];
        hashArr[hashCode] = new;
        wordsCounter++;
        
    }

    /* printing all words for debug or comprehension purposes    
    for (int i = 0; i < lenHash; i++){
        if (hashArr[i] != NULL) {
            printf("\nFOUND!");
            node *crawler = hashArr[i];
            while (crawler) {
                printf("\nHash value = %i, Word = %s",i, crawler->word);
                crawler = crawler->next;
            }
        } else {
            printf("\n%i is empty", i);
        };
    }
    */
    fclose(dict);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if ( wordsCounter > 0) return wordsCounter;
    return 0;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    int length = sizeof(hashArr) / sizeof(hashArr[0]);
    node *listItem;
    for (int i = 0; i < length; i++) {
        listItem = hashArr[i];
        
        while(listItem != NULL) {
            hashArr[i] = listItem->next;
            free(listItem);
            listItem = hashArr[i];
        }
    }
    return true;
}
