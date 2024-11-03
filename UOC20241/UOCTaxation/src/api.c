#include <stdio.h>
#include <assert.h>
#include "csv.h"
#include "api.h"

#include <string.h>
#include <stdlib.h>

#define FILE_READ_BUFFER_SIZE 2048

// Get the API version information
const char* api_version() {
    return "UOC PP 20241";
}

// Load data from a CSV file. If reset is true, remove previous data
tApiError api_loadData(tApiData* data, const char* filename, bool reset) {
    tApiError error;
    FILE *fin;    
    char buffer[FILE_READ_BUFFER_SIZE];
    tCSVEntry entry;
    
    // Check input data
    assert( data != NULL );
    assert(filename != NULL);
    
    // Reset current data    
    if (reset) {
        // Remove previous information
        error = api_freeData(data);
        if (error != E_SUCCESS) {
            return error;
        }
        
        // Initialize the data
        error = api_initData(data);
        if (error != E_SUCCESS) {
            return error;
        }
    }

    // Open the input file
    fin = fopen(filename, "r");
    if (fin == NULL) {
        return E_FILE_NOT_FOUND;
    }
    
    // Read file line by line
    while (fgets(buffer, FILE_READ_BUFFER_SIZE, fin)) {
        // Remove new line character     
        buffer[strcspn(buffer, "\n\r")] = '\0';
        
        csv_initEntry(&entry);
        csv_parseEntry(&entry, buffer, NULL);
        // Add this new entry to the api Data
        error = api_addDataEntry(data, entry);
        if (error != E_SUCCESS) {
            return error;
        }
        csv_freeEntry(&entry);
    }
    
    fclose(fin);
    
    return E_SUCCESS;
}

// Initialize the data structure
tApiError api_initData(tApiData* data) {            
    //////////////////////////////////
    // Ex PR1 2b
    /////////////////////////////////
    
    /////////////////////////////////
    
    tenantData_init(&data -> tenants);
    landlords_init(&data -> landlords);
    rentalIncomes_init(&data -> rentalIncomes);
    
    return E_SUCCESS;
}

// Add a tenant into the data if it does not exist
tApiError api_addTenant(tApiData* data, tCSVEntry entry) {
    //////////////////////////////////
    // Ex PR1 2c
    /////////////////////////////////
    
    /////////////////////////////////
    
 // Validación de entrada
    assert(data != NULL);

    // Comprobar que el tipo es "TENANT"
    if (strcmp(csv_getType(&entry), "TENANT") != 0) {
        return E_INVALID_ENTRY_TYPE;
    }

    // Comprobar que el formato es correcto
    if (csv_numFields(entry) != NUM_FIELDS_TENANT) {
        return E_INVALID_ENTRY_FORMAT;
    }

    // Inicializar el nuevo inquilino y analizar el CSV en tTenant
    tTenant tenant;
    tenant_parse(&tenant, entry);

    // Verificar si el inquilino ya existe
    if (tenantData_find(data->tenants, tenant.tenant_id) >= 0) {
        tenant_free(&tenant);  // Liberar memoria si el inquilino ya existe
        return E_TENANT_DUPLICATED;
    }

    // Añadir el inquilino a data
    tenantData_add(&data->tenants, tenant);

    // Liberar cualquier recurso temporal si es necesario
    tenant_free(&tenant);

    return E_SUCCESS;
}

// Add a landlord if it does not exist
tApiError api_addLandlord(tApiData* data, tCSVEntry entry) {
    //////////////////////////////////
    // Ex PR1 2d
    /////////////////////////////////
    
    /////////////////////////////////
    
    // Comprobar si el tipo de entrada es correcto
    if (strcmp(csv_getType(&entry), "LANDLORD") != 0) {
        return E_INVALID_ENTRY_TYPE;
    }

    // Comprobar si el formato de la entrada es correcto
    if (csv_numFields(entry) != NUM_FIELDS_LANDLORD) {
        return E_INVALID_ENTRY_FORMAT;
    }

    // Parsear el propietario desde la entrada CSV
    tLandlord new_landlord;
    landlord_parse(&new_landlord, entry);

    // Verificar si el propietario ya existe
    if (landlords_find(data->landlords, new_landlord.id) >= 0) {
        landlord_free(&new_landlord); // Liberar memoria en caso de duplicado
        return E_LANDLORD_DUPLICATED;
    }
    
    // Añadir el nuevo propietario a la estructura de datos
    landlords_add(&data->landlords, new_landlord);

    // Operación exitosa
    return E_SUCCESS;
}

// Add a property into the properties of an specific landlord
tApiError api_addProperty(tApiData* data, tCSVEntry entry) {
    //////////////////////////////////
    // Ex PR1 2e
    /////////////////////////////////
    
    /////////////////////////////////
    
    tProperty new_property;
    int landlord_idx;
    int property_idx;
    
    // Verificación de tipo de entrada
    if (strcmp(csv_getType(&entry), "PROPERTY") != 0) {
        return E_INVALID_ENTRY_TYPE;
    }
    
    // Validación del formato (número de campos)
    if (csv_numFields(entry) != 4) {
        return E_INVALID_ENTRY_FORMAT;
    }

    // Parse de los datos de la propiedad
    property_parse(&new_property, entry);

    // Búsqueda del propietario correspondiente
    landlord_idx = landlords_find(data->landlords, new_property.landlord_id);
    if (landlord_idx < 0) {
        return E_LANDLORD_NOT_FOUND;
    }

    // Verificación si la propiedad ya existe
    property_idx = properties_find(data->landlords.elems[landlord_idx].properties, new_property.cadastral_ref);
    if (property_idx >= 0) {
        return E_PROPERTY_DUPLICATED;
    }

    // Agregar la propiedad al propietario
    landlord_add_property(&(data->landlords), new_property);

    return E_SUCCESS;
}

// Add a rental income into a list if the landlord already exists
tApiError api_addRentalIncome(tApiData* data, tCSVEntry entry) {
    //////////////////////////////////
    // Ex PR1 2f
    /////////////////////////////////
    
    /////////////////////////////////
    tRentalIncome new_income;
    int landlord_idx;
    char landlord_id[MAX_PERSON_ID + 1];  // Almacenar temporalmente el ID del propietario

    // Verificar que el tipo de entrada es correcto
    if (strcmp(csv_getType(&entry), "RENTAL_INCOME") != 0) {
        return E_INVALID_ENTRY_TYPE;
    }

    // Verificar si el formato es correcto (esperamos 3 campos)
    if (csv_numFields(entry) != 3) {
        return E_INVALID_ENTRY_FORMAT;
    }

    // Obtener el ID del propietario desde el CSV antes de parsear completamente los ingresos
    csv_getAsString(entry, 2, landlord_id, MAX_PERSON_ID + 1);

    // Buscar al propietario correspondiente al ingreso
    landlord_idx = landlords_find(data->landlords, landlord_id);
    if (landlord_idx < 0) {
        return E_LANDLORD_NOT_FOUND;
    }

    // Asignar el propietario encontrado al ingreso
    new_income.landlord = &(data->landlords.elems[landlord_idx]);

    // Parsear los datos de ingresos fiscales
    rentalIncome_parse(&new_income, entry);

    // Agregar el ingreso a los datos de ingresos fiscales
    tApiError err = rentalIncomes_add(&data->rentalIncomes, new_income);
    if (err != E_SUCCESS) {
        return err;
    }

    return E_SUCCESS;
}

// Find a rental income by year and landlord document id
tRentalIncome* rentalIncomes_find(tRentalIncomeList list, int year, const char* document_id) {
    //////////////////////////////////
    // Ex PR1 2g
    /////////////////////////////////
    
    /////////////////////////////////
    
    tRentalIncome new_income;
    tRentalIncomeListNode* newNode = NULL;
    tRentalIncomeListNode* pNode = NULL;
    tRentalIncomeListNode* pPrev = NULL;
    int landlord_idx;

    // Comprobar si el tipo de entrada es correcto
    if (strcmp(csv_getType(&entry), "RENTAL_INCOME") != 0) {
        return E_INVALID_ENTRY_TYPE;
    }

    // Comprobar si el formato es correcto (esperamos 2 campos)
    if (csv_numFields(entry) != 2) {
        return E_INVALID_ENTRY_FORMAT;
    }

    // Parsear los datos de ingresos fiscales
    rentalIncome_parse(&new_income, entry);

    // Buscar al propietario correspondiente al ingreso
    landlord_idx = landlords_find(data->landlords, new_income.landlord->id);
    if (landlord_idx < 0) {
        return E_LANDLORD_NOT_FOUND;
    }

    // Asignar el propietario encontrado al ingreso
    new_income.landlord = &(data->landlords.elems[landlord_idx]);

    // Crear un nuevo nodo para el ingreso fiscal
    newNode = (tRentalIncomeListNode*)malloc(sizeof(tRentalIncomeListNode));
    assert(newNode != NULL);  // Verificar que la asignación fue exitosa
    rentalIncome_cpy(&(newNode->elem), new_income);
    newNode->next = NULL;

    // Insertar en la lista de ingresos en orden por año
    if (data->rentalIncomes.count == 0) {
        // Si la lista está vacía, añadir como primer elemento
        data->rentalIncomes.first = newNode;
    } else {
        pNode = data->rentalIncomes.first;
        while (pNode != NULL && pNode->elem.year < new_income.year) {
            pPrev = pNode;
            pNode = pNode->next;
        }
        
        if (pPrev == NULL) {
            // Insertar como primer elemento si es el más antiguo
            newNode->next = data->rentalIncomes.first;
            data->rentalIncomes.first = newNode;
        } else {
            // Insertar después de pPrev si es un elemento intermedio o final
            pPrev->next = newNode;
            newNode->next = pNode;
        }
    }
    
    // Incrementar el contador de ingresos en la lista
    data->rentalIncomes.count++;
    return E_SUCCESS;
}

// Get the number of tenants registered on the application
int api_tenantCount(tApiData data) {
    //////////////////////////////////
    // Ex PR1 2h
    /////////////////////////////////
    
    /////////////////////////////////
    return -1;
}


// Get the number of landlords registered on the application
int api_landlordsCount(tApiData data) {
    //////////////////////////////////
    // Ex PR1 2h
    /////////////////////////////////
    
    /////////////////////////////////
    return -1;
}

// Get the number of properties in all landlords registered on the application
int api_propertiesCount(tApiData data) {
    //////////////////////////////////
    // Ex PR1 2h
    /////////////////////////////////
	
    /////////////////////////////////
    return -1;
}

// Get the number of rental incomes registered
int api_rentalIncomesCount(tApiData data) {
    //////////////////////////////////
    // Ex PR1 2h
    /////////////////////////////////
	
    /////////////////////////////////
    return -1;
}


// Free all used memory
tApiError api_freeData(tApiData* data) {
    //////////////////////////////////
    // Ex PR1 2i
    /////////////////////////////////
    
    /////////////////////////////////
    return E_NOT_IMPLEMENTED;
}


// Add a new entry
tApiError api_addDataEntry(tApiData* data, tCSVEntry entry) { 
    //////////////////////////////////
    // Ex PR1 2j
    /////////////////////////////////
    
    /////////////////////////////////
    return E_NOT_IMPLEMENTED;
}

// Get landlord data
tApiError api_getLandlord(tApiData data, const char *id, tCSVEntry *entry) {
    //////////////////////////////////
    // Ex PR1 3a
    /////////////////////////////////
    
    /////////////////////////////////
    return E_NOT_IMPLEMENTED; 
}

// Get the rental income by year of a landlord
tApiError api_getRentalIncome(tApiData data, int year, const char* id, tCSVEntry *entry) {
    //////////////////////////////////
    // Ex PR1 3b
    /////////////////////////////////
    
    /////////////////////////////////
    return E_NOT_IMPLEMENTED; 
}

// Get registered properties
tApiError api_getProperties(tApiData data, tCSVData *properties) {
    //////////////////////////////////
    // Ex PR1 3c
    /////////////////////////////////
    
    /////////////////////////////////
    return E_NOT_IMPLEMENTED;
}

// Get registered rental incomes
tApiError api_getRentalIncomes(tApiData data, tCSVData *rentalIncomes) {
    //////////////////////////////////
    // Ex PR1 3d
    /////////////////////////////////
    
    /////////////////////////////////
    return E_NOT_IMPLEMENTED;
}
