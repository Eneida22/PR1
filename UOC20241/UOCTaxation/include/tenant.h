#ifndef __TENANT_H__
#define __TENANT_H__

#include "date.h"
#include "csv.h"

#define MAX_PERSON_ID 9
#define MAX_CADASTRAL_REF 7

#define NUM_FIELDS_TENANT 7

typedef struct _tTenant {
    tDate start_date;
    tDate end_date;
    char tenant_id[MAX_PERSON_ID + 1];
    char *name;
    float rent;
    int age;
    char cadastral_ref[MAX_CADASTRAL_REF + 1];
} tTenant;

typedef struct _tTenantData {
    tTenant *elems;
    int count;
} tTenantData;

// Initialize a tenant
void tenant_init(tTenant *tenant);

// Copy a tenant
void tenant_cpy(tTenant *dst, tTenant src);

// Parse input from CSVEntry
void tenant_parse(tTenant* data, tCSVEntry entry);

// Release a tenant
void tenant_free(tTenant *tenant);

// Initialize the tenants data
void tenantData_init(tTenantData *data);

// Return the number of tenants
int tenantData_len(tTenantData tenants);

// Add a tenant to the data
void tenantData_add(tTenantData *data, tTenant tenant);

// Find a tenant into the data
int tenantData_find(tTenantData data, const char* tenant_id);

// Release the tenants data
void tenantData_free(tTenantData *data);

#endif
