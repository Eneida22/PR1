#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tenant.h"

// Initialize a tenant
void tenant_init(tTenant *tenant) {
    // Check input data (Pre-conditions)
    assert(tenant != NULL);
    
    tenant->start_date.day = -1;
    tenant->start_date.month = -1;
    tenant->start_date.year = -1;
    
    tenant->end_date.day = -1;
    tenant->end_date.month = -1;
    tenant->end_date.year = -1;
    
    tenant->name = NULL;
    tenant->rent = -1.0;
    tenant->age = - 1;
}

// Copy a tenant
void tenant_cpy(tTenant *dst, tTenant src) {
    // Check input data (Pre-conditions)
    assert(dst != NULL);
    
    // Remove old data 
    tenant_free(dst);
    
    // Copy the start date
    date_cpy(&(dst->start_date), src.start_date);
    
    // Copy the end date
    date_cpy(&(dst->end_date), src.end_date);
    
    // Copy the tenant id
    strcpy(dst->tenant_id, src.tenant_id);
    
    // Copy the name
    dst->name = (char*) malloc((strlen(src.name) + 1) * sizeof(char));
    assert(dst->name != NULL);
    strcpy(dst->name, src.name);
    
    // Copy the rent
    dst->rent = src.rent;
    
    // Copy the age
    dst->age = src.age;
    
    // Copy the cadastral_ref
    strcpy(dst->cadastral_ref, src.cadastral_ref);
}

// Parse input from CSVEntry
void tenant_parse(tTenant* data, tCSVEntry entry) {
    char start_date[11];
    char end_date[11];
    
    // Check input data (Pre-conditions)
    assert(data != NULL);    
    assert(csv_numFields(entry) == NUM_FIELDS_TENANT);
    
    // Initialize the tentant
    tenant_init(data);
    
    // Get the date and time
    csv_getAsString(entry, 0, start_date, 11);    
    date_parse(&(data->start_date), start_date);

    csv_getAsString(entry, 1, end_date, 11);    
    date_parse(&(data->end_date), end_date);
    
    // Assign the tenant ID
    csv_getAsString(entry, 2, data->tenant_id, MAX_PERSON_ID + 1);

    // Assign the tenant name
    data->name = (char*) malloc((strlen(entry.fields[3]) + 1) * sizeof(char));
    assert(data->name != NULL);
    memset(data->name, 0, (strlen(entry.fields[3]) + 1) * sizeof(char));
    csv_getAsString(entry, 3, data->name, strlen(entry.fields[3]) + 1);
    
    data->rent = csv_getAsInteger(entry, 4);
    data->age = csv_getAsInteger(entry, 5);
    csv_getAsString(entry, 6, data->cadastral_ref, MAX_CADASTRAL_REF + 1);
}

// Release a tenant
void tenant_free(tTenant *tenant) {
    // Check input data (Pre-conditions)
    assert(tenant != NULL);
    
    // Release the name
    if (tenant->name != NULL) {
        free(tenant->name);
    }
    tenant->name = NULL;
}

// Initialize the tenants data
void tenantData_init(tTenantData *data) {
    // Check input data (Pre-conditions)
    assert(data != NULL);
    
    data->elems = NULL;
    data->count = 0;
}

// Return the number of tenants
int tenantData_len(tTenantData tenants) {
    return tenants.count;
}

// Add a tenant to the data
void tenantData_add(tTenantData *data, tTenant tenant) {
    // Check input data (Pre-conditions)
    assert(data != NULL);
    
    // If tenant does not exist, add it
    if (tenantData_find(*(data), tenant.tenant_id) < 0) {
        // Allocate memory for new element
        if (data->count == 0) {
            // Request new memory space
            data->elems = (tTenant*) malloc(sizeof(tTenant));
        } else {
            // Modify currently allocated memory
            data->elems = (tTenant*) realloc(data->elems, sizeof(tTenant) * (data->count + 1));
        }
        
        assert(data->elems != NULL);
    
        // Initialize the new element
        tenant_init(&(data->elems[data->count]));
        
        // Copy the element to the position
        tenant_cpy(&(data->elems[data->count]), tenant);
        
        // Increase the number of elements
        data->count++;
    }
}

// Return the position of the tenant if it exists, otherwise, it returns -1
int tenantData_find(tTenantData data, const char* tenant_id) {
    int i;
    
    // Check input data (Pre-conditions)
    assert(tenant_id != NULL);
    
    for (i = 0; i < data.count; i++) {
        if (strcmp(data.elems[i].tenant_id, tenant_id) == 0) {
            return i;
        }
    }
    
    return -1;
}

// Release the tenants data
void tenantData_free(tTenantData *data) {
    // Check input data (Pre-conditions)
    assert(data != NULL);
    
    // Release memory
    if (data->elems != NULL) {
        free(data->elems);
    }
    
    // Initialize the structure again
    tenantData_init(data);
}
