#ifndef __UOCHEALTHCENTER_ERROR__H
#define __UOCHEALTHCENTER_ERROR__H

// Define error codes
enum _tApiError
{
    E_SUCCESS = 0, // No error
    E_NOT_IMPLEMENTED = -1, // Called method is not implemented
    E_FILE_NOT_FOUND = -2, // File not found
    E_INVALID_ENTRY_TYPE = -3, // Invalid entry type
    E_INVALID_ENTRY_FORMAT = -4, // Invalid entry format
    E_MEMORY_ERROR = -5, // Memory error
    E_LANDLORD_DUPLICATED = -6, // Landlord duplicated
    E_LANDLORD_NOT_FOUND = -7, // Landlord not found
    E_TENANT_DUPLICATED = -8, // Tenant duplicated
    E_RENTAL_INCOME_NOT_FOUND = -9, // Rental income not found
    E_PROPERTY_DUPLICATED = -10 // Property duplicated
};

// Define an error type
typedef enum _tApiError tApiError;

#endif // __UOCHEALTHCENTER_ERRORS__H