#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "rental_incomes.h"

// Parse a rental income
void rentalIncome_parse(tRentalIncome* data, tCSVEntry entry) {
    assert(data != NULL);
    assert(csv_numFields(entry) == NUM_FIELDS_RENTAL_INCOME);
    
    data->year = csv_getAsInteger(entry, 0);
    data->totalIncome = csv_getAsReal(entry, 1);
}

// Initialize a rental incomes list
void rentalIncomes_init(tRentalIncomeList *list) {
    // Check input data (Pre-conditions)
    assert(list != NULL);
    
    list->first = NULL;
    list->count = 0;
}

// Check if the linked list is empty
bool rentalIncomes_isEmpty(tRentalIncomeList list) {
    return list.count == 0;
}

// Return the number of elements of the list
int rentalIncomes_len(tRentalIncomeList list) {
    return list.count;
}

// Create and initialize a node of tRentalIncomeListNode
tRentalIncomeListNode* rentalIncomes_createNode(tRentalIncome rentalIncome) {
    tRentalIncomeListNode *pNode = NULL;
    
    pNode = (tRentalIncomeListNode*) malloc(sizeof(tRentalIncomeListNode));
    assert(pNode != NULL);
    
    pNode->elem.landlord = rentalIncome.landlord;
    pNode->elem.year = rentalIncome.year;
    pNode->elem.totalIncome = rentalIncome.totalIncome;
    
    pNode->next = NULL;
    
    return pNode;
}

// Add an element into the linked list
tApiError rentalIncomes_add(tRentalIncomeList *list, tRentalIncome rentalIncome) {
    tRentalIncomeListNode *pNode = NULL, *pCurrent = NULL, *pPrev = NULL;
    
    // Check input data (Pre-conditions)
    assert(list != NULL);
    
    pNode = rentalIncomes_createNode(rentalIncome);
    
    if (pNode == NULL) {
        return E_MEMORY_ERROR;
    }
    
    pCurrent = list->first;
    pPrev = NULL;
    
    while (pCurrent != NULL && (pCurrent->elem.year < rentalIncome.year ||
           (pCurrent->elem.year == rentalIncome.year && strcmp(pCurrent->elem.landlord->id, rentalIncome.landlord->id) < 0))) {
        pPrev = pCurrent;
        pCurrent = pCurrent->next;
    }

    if (pPrev == NULL) {
        pNode->next = list->first;
        list->first = pNode;
    } else {
        pPrev->next = pNode;
        pNode->next = pCurrent;
    }
    
    list->count++;
    
    return E_SUCCESS;
}

// Release the list
void rentalIncomes_free(tRentalIncomeList *list) {
    tRentalIncomeListNode *pNode = NULL, *pAux = NULL;
    
    // Check input data (Pre-conditions)
    assert(list != NULL);
    
    pNode = list->first;
    
    while (pNode != NULL) {
        pAux = pNode->next;
        free(pNode);
        pNode = pAux;
    }
    
    rentalIncomes_init(list);
}
