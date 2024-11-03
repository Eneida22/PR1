#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "landlord.h"

//////////////////////////////////
// Available methods
//////////////////////////////////

// Initialize the properties
void properties_init(tProperties* data) {   
    /////////////
    // Set the initial number of elements to zero.
    data->count = 0;
    /////////////
}

// Get the number of properties
int properties_len(tLandlord data) {
    //////////////
    // Return the number of elements
    return data.properties.count;
    //////////////
}

// Initialize the landlords
void landlords_init(tLandlords* data) {   
    /////////////
    // Set the initial number of elements to zero.
    data->count = 0;
	data->elems = NULL;
    /////////////
}

// Get the number of landlords
int landlords_len(tLandlords data) {   
    //////////////
    // Return the number of elements
    return data.count;
    //////////////
}

// Get the number of properties of all landlords
int landlords_propertiesCount(tLandlords data) {
    int i;
    int count = 0;
    
    // Iterate all landlords
    for (i = 0; i < data.count; i++) {
        count += properties_len(data.elems[i]);
    }
    
    return count;
}


////////////////////////////////////////
void landlords_process_tenant(tLandlords* data, tTenant tenant) {
    int idx;

    // Check input data (Pre-conditions)
    assert(data != NULL);    
    
    // Check if an entry with this data already exists
    idx = landlords_find_by_cadastral_ref(*data, tenant.cadastral_ref);
    
    // If it does not exist, create a new entry
    if (idx >= 0) {
        int months_rented = tenant.end_date.month - tenant.start_date.month + 1;
        float tax_factor;
        float amount_to_add;
        if (tenant.age <= 35)
            tax_factor = 0.1;
        else 
            tax_factor = 0.2;
        amount_to_add = months_rented * tenant.rent * tax_factor - AMOUNT_NO_RENT * months_rented;
        data->elems[idx].tax = data->elems[idx].tax + amount_to_add;
    }
}


// Get a property
void property_get(tLandlord data, int index, char* buffer) { 
    assert(index < data.properties.count);
    sprintf(buffer, "%s;%s,%d;%s", 
        data.properties.elems[index].cadastral_ref,
        data.properties.elems[index].address.street,
        data.properties.elems[index].address.number,
        data.properties.elems[index].landlord_id     
    );
}

// Parse input from CSVEntry
void property_parse(tProperty* data, tCSVEntry entry) {
    // Check input data (Pre-conditions)
    assert(data != NULL);    
    assert(csv_numFields(entry) == NUM_FIELDS_PROPERTY);
    
    // Get the date and time
    csv_getAsString(entry, 0, data->cadastral_ref, MAX_CADASTRAL_REF + 1);    
    csv_getAsString(entry, 1, data->address.street, MAX_STREET + 1);    
    data->address.number = csv_getAsInteger(entry, 2);
    csv_getAsString(entry, 3, data->landlord_id, MAX_PERSON_ID + 1);    
}

////////////////////////////////////////

// Add a new property
void landlord_add_property(tLandlords* data, tProperty property) {
    int idx_landlord;

    // Check input data (Pre-conditions)
    assert(data != NULL);    
    
    idx_landlord = landlords_find(*data, property.landlord_id);
    if (idx_landlord >= 0) {
        int idx_property = -1;

        // Check if an entry with this data already exists
        idx_property = properties_find(data->elems[idx_landlord].properties, property.cadastral_ref);
        
        // If it does not exist, create a new entry
        if (idx_property < 0) {
            int property_count = data->elems[idx_landlord].properties.count;
            assert(property_count < MAX_PROPERTIES);
            property_cpy(&(data->elems[idx_landlord].properties.elems[property_count]), property);
            data->elems[idx_landlord].properties.count++;
            data->elems[idx_landlord].tax = data->elems[idx_landlord].tax + AMOUNT_NO_RENT*12;
        }
    }
}

// Get a landlord
void landlord_get(tLandlords data, int index, char* buffer) {
    assert(index < data.count);
    sprintf(buffer, "%s;%s,%.1f", 
        data.elems[index].name,
        data.elems[index].id,
        data.elems[index].tax
    );
}

// Initialize a landlord
void landlord_init(tLandlord* data) {
    // Check input data (Pre-conditions)
    assert(data != NULL);    
    
    data->name = NULL;
}

// Parse input from CSVEntry
void landlord_parse(tLandlord* data, tCSVEntry entry) {
    // Check input data (Pre-conditions)
    assert(data != NULL);    
    assert(csv_numFields(entry) == NUM_FIELDS_LANDLORD);
    data->properties.count = 0;
    
    data->name = (char*) malloc((strlen(entry.fields[0]) + 1) * sizeof(char));
    assert(data->name != NULL);
    memset(data->name, 0, (strlen(entry.fields[0]) + 1) * sizeof(char));
    csv_getAsString(entry, 0, data->name, strlen(entry.fields[0]) + 1);    
    
    csv_getAsString(entry, 1, data->id, MAX_PERSON_ID + 1);    
    
    data->tax = csv_getAsInteger(entry, 2);
    
    // Initialize the properties
    properties_init(&(data->properties));
}

// Release a landlord
void landlord_free(tLandlord* data) {
    // Check input data (Pre-conditions)
    assert(data != NULL);   
    
    // Release the name
    if (data->name != NULL) {
        free(data->name);
    }
    data->name = NULL;
}

////////////////////////////////////////

// Add a new landlord
void landlords_add(tLandlords* data, tLandlord landlord) {
    int idx;

    // Check input data (Pre-conditions)
    assert(data != NULL);    
    
    // Check if an entry with this data already exists
    idx = landlords_find(*data, landlord.id);
    
    // If it does not exist, create a new entry
    if (idx < 0) {
        if (data->elems == NULL) {
            data->elems = (tLandlord*) malloc(sizeof(tLandlord));
        } else {
            data->elems = (tLandlord*) realloc(data->elems, (data->count + 1) * sizeof(tLandlord));
        }
        assert(data->elems != NULL);
        /////////////////////////////////  
        landlord_cpy(&(data->elems[data->count]), landlord);
        data->count++;        
    }
}

// Remove a landlord
void landlords_del(tLandlords* data, char* id) 
{
    int idx;
    int i;
    
    // Check if an entry with this data already exists
    idx = landlords_find(*data, id);
    
    if (idx >= 0) {
        // Shift elements to remove selected
        for(i = idx; i < data->count-1; i++) {
                // Copy element on position i+1 to position i
                landlord_cpy(&(data->elems[i]), data->elems[i+1]);
        }
        // Update the number of elements
        data->count--;  
        /////////////////////////////////
        if (data->count > 0) {
            data->elems = (tLandlord*) realloc(data->elems, data->count * sizeof(tLandlord));
            assert(data->elems != NULL);
        } else {
			landlords_free(data);
        }
        /////////////////////////////////   
    }
}

// returns true if field tax of expected[index] is greater than the one in declarant[index]
bool mismatch_tax_declaration(tLandlords expected, tLandlords declarant, int index) {
    return (expected.elems[index].tax > declarant.elems[index].tax);
}

// Copy the data from the source to destination
void landlords_cpy(tLandlords* destination, tLandlords source) {
    int i;

    destination->count = source.count;
	destination->elems = (tLandlord*) malloc(source.count * sizeof(tLandlord));
	
    for(i = 0 ; i < landlords_len(source) ; i++) {
        landlord_cpy(&(destination->elems[i]), source.elems[i]);
        // we want to copy all fields from source.elems[i] but want 
        // to set tax to 0 in the expected landlords
        destination->elems[i].tax = 0.0;
    }    
}

// [AUX METHOD] Return the position of a tenant entry with provided information. -1 if it does not exist
int landlords_find(tLandlords data, const char* id) {
    int i;
    int res = -1;

    i = 0;
    while ((i < data.count) && (res < 0)) {
        if((strcmp(data.elems[i].id, id) == 0)) {
            res = i;
        }
        else {
            i++;
        } 
    }
    
    return res;
}


// [AUX METHOD] Return the position of a tenant entry with provided information. -1 if it does not exist
int landlords_find_by_cadastral_ref(tLandlords data, const char* id) {
    int i, j;
    int res = -1;

    i = 0;
    while ((i < data.count) && (res < 0)) {
        j = 0;
        while ((j < data.elems[i].properties.count) && (res < 0)) {
            if((strcmp(data.elems[i].properties.elems[j].cadastral_ref, id) == 0)) {
                res = i;
            }
            j++;
        }
        i++;
    }
    
    return res;
}


// [AUX METHODS] Copy the data from the source to destination
void landlord_cpy(tLandlord* destination, tLandlord source) {
    destination->name = (char*) malloc(sizeof(char) * (strlen(source.name) + 1));
    assert(destination->name != NULL);
    strcpy(destination->name, source.name);
    
    strcpy(destination->id, source.id);
    destination->tax = source.tax;
    
    properties_cpy(&(destination->properties), source.properties);
}

// [AUX METHODS] Copy the properties from sources to destination
void properties_cpy(tProperties *destination, tProperties source) {
    int i;
    
    for (i = 0; i < source.count; i++) {
        property_cpy(&(destination->elems[i]), source.elems[i]);
    }
    
    destination->count = source.count;
}

// [AUX METHOD] Return the position of a property entry with provided information. -1 if it does not exist
int properties_find(tProperties data, const char* cadastral_ref) {
    int i;
    int res = -1;

    i = 0;
    while ((i < data.count) && (res < 0)) {
        if((strcmp(data.elems[i].cadastral_ref, cadastral_ref) == 0)) {
            res = i;
        }
        else {
            i++;
        } 
    }
    return res;
}

// [AUX METHODS] Copy the data from the source to destination
void property_cpy(tProperty* destination, tProperty source) {
    strcpy(destination->cadastral_ref, source.cadastral_ref);
    strcpy(destination->address.street, source.address.street);
    destination->address.number = source.address.number;
    strcpy(destination->landlord_id, source.landlord_id);
}

// Remove all elements 
void landlords_free(tLandlords* data) { 
    /////////////////////////////////
    if (data->elems != NULL) {
        free(data->elems);
    }
    landlords_init(data);
    /////////////////////////////////    
}
