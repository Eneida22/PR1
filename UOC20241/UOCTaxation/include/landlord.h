#ifndef __TAXATION_H__
#define __TAXATION_H__
#include "csv.h"
#include "tenant.h"

///////////////////////////
#define MAX_PROPERTIES 80
#define MAX_NAME 15
#define MAX_STREET 25
#define AMOUNT_NO_RENT 150.0

#define NUM_FIELDS_LANDLORD 3
#define NUM_FIELDS_PROPERTY 4

typedef struct _tAddress {
    char street[MAX_STREET];
    int number;
} tAddress;

typedef struct _tProperty {
    char cadastral_ref[MAX_CADASTRAL_REF + 1];
    tAddress address;
    char landlord_id[MAX_PERSON_ID + 1];
} tProperty;

typedef struct _tProperties {
    tProperty elems[MAX_PROPERTIES];
    int count;
} tProperties;

typedef struct _tLandlord {
    char *name;
    char id[MAX_PERSON_ID + 1];
    float tax;
    tProperties properties;
} tLandlord;

typedef struct _tLandlords {
    tLandlord *elems;
    int count;
} tLandlords;

//////////////////////////////////
// Available methods
//////////////////////////////////
// Initialize the properties data
void properties_init(tProperties* data);

// Get the number of properties
int properties_len(tLandlord data);

// Initialize the landlords data
void landlords_init(tLandlords* data);

// Get the number of landlords
int landlords_len(tLandlords data);

// Get the number of properties of all landlords
int landlords_propertiesCount(tLandlords data);

// Initialize a landlord
void landlord_init(tLandlord* data);

// Parse input from CSVEntry
void landlord_parse(tLandlord* data, tCSVEntry entry);

// Release a landlord
void landlord_free(tLandlord* data);

// Add a new tenant
void landlords_add(tLandlords* data, tLandlord tenant);

//Remove a landlord
void landlords_del(tLandlords* data, char* id);

// Remove all elements
void landlords_free(tLandlords* data);

// Copy the data from the source to destination
void landlords_cpy(tLandlords* destination, tLandlords source);

// Parse input from CSVEntry
void property_parse(tProperty* data, tCSVEntry entry);

// Add a new property
void landlord_add_property(tLandlords* data, tProperty property);

// Calculate and update taxation given tenant
void landlords_process_tenant(tLandlords* data, tTenant tenant);

// Get landlord data in a string
void landlord_get(tLandlords data, int index, char* buffer);

// Get a property data in a string
void property_get(tLandlord data, int index, char* buffer);

// returns true if field tax of expected[index] is greater than the one in declarant[index]
bool mismatch_tax_declaration(tLandlords expected, tLandlords declarant, int index);

/////////////////////////////////////
// Aux methods
/////////////////////////////////////

// [AUX METHOD] Return the position of a property entry with provided information. -1 if it does not exist
int properties_find(tProperties data, const char* cadastral_ref);

// [AUX METHODS] Copy the data from the source to destination
void property_cpy(tProperty* destination, tProperty source);

// [AUX METHODS] returns the position of a landlord that has a property with that cadastral ref
int landlords_find_by_cadastral_ref(tLandlords data, const char* cadastral_ref);

// [AUX METHOD] Return the position of a landl entry with that landlord id. -1 if it does not exist
int landlords_find(tLandlords data, const char* landlord_id);

// [AUX METHODS] Copy the data from the source to destination
void landlord_cpy(tLandlord* destination, tLandlord source);

// [AUX METHODS] Copy the properties from sources to destination
void properties_cpy(tProperties *destination, tProperties source);

////////////////////////////////////////////
#endif
