#ifndef __RENTAL_INCOMES_H__
#define __RENTAL_INCOMES_H__

#include <stdbool.h>
#include "error.h"
#include "tenant.h"
#include "landlord.h"

#define NUM_FIELDS_RENTAL_INCOME 3

typedef struct _tRentalIncome {
    tLandlord *landlord;
    int year;
    float totalIncome;
} tRentalIncome;

typedef struct _tRentalIncomeListNode {
    tRentalIncome elem;
    struct _tRentalIncomeListNode *next;
} tRentalIncomeListNode;

typedef struct _tRentalIncomeList {
    tRentalIncomeListNode *first;
    int count;
} tRentalIncomeList;

// Parse a rental income
void rentalIncome_parse(tRentalIncome* data, tCSVEntry entry);

// Initialize a rental incomes list
void rentalIncomes_init(tRentalIncomeList *list);

// Check if the linked list is empty
bool rentalIncomes_isEmpty(tRentalIncomeList list);

// Return the number of elements of the list
int rentalIncomes_len(tRentalIncomeList list);

// Create and initialize a node of tRentalIncomeListNode
tRentalIncomeListNode* rentalIncomes_createNode(tRentalIncome rentalIncome);

// Add an element into the linked list
tApiError rentalIncomes_add(tRentalIncomeList *list, tRentalIncome rentalIncome);

// Release the list
void rentalIncomes_free(tRentalIncomeList *list);

#endif
