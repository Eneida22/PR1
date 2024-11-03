#include "test_pr1.h"
#include "api.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>


// Run all tests for PR1
bool run_pr1(tTestSuite *test_suite, const char *input) {
    bool ok = true;
    tTestSection* section = NULL;

    assert(test_suite != NULL);

    testSuite_addSection(test_suite, "PR1", "Tests for PR1 exercices");

    section = testSuite_getSection(test_suite, "PR1");
    assert(section != NULL);

    ok = run_pr1_ex1(section, input);
    ok = run_pr1_ex2(section, input) && ok;
    ok = run_pr1_ex3(section, input) && ok;

    return ok;
}

// Run all tests for Exercice 1 of PR1
bool run_pr1_ex1(tTestSection *test_section, const char *input) {
  bool passed = true, failed = false;
  const char *version;

  /////////////////////////////
  /////  PR1 EX1 TEST 1  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX1_1", "Read version information.");
  // Get the version
  version = api_version();
  if (strcmp(version, "UOC PP 20241") != 0) {
    failed = true;
    passed = false;
  }
  end_test(test_section, "PR1_EX1_1", !failed);

  return passed;
}

// Run all tests for Exercice 2 of PR1
bool run_pr1_ex2(tTestSection *test_section, const char *input) {
  tApiData data;
  tApiError error;
  tCSVEntry entry;
  
  tRentalIncome *pRentalIncome = NULL;
  int nTenants = 0;
  int nLandlords = 0;
  int nProperties = 0;
  int nRentalIncomes = 0;
  bool passed = true;
  bool failed = false;
  bool fail_all = false;

  /////////////////////////////
  /////  PR1 EX2 TEST 1  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX2_1", "Initialize the API data structure");
  // Initialize the data
  error = api_initData(&data);
  if (error != E_SUCCESS) {
    failed = true;
    passed = false;
    fail_all = true;
  }
  end_test(test_section, "PR1_EX2_1", !failed);

  /////////////////////////////
  /////  PR1 EX2 TEST 2  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX2_2", "Add an invalid tenant");
  if (fail_all) {
    failed = true;
  } else {
    csv_initEntry(&entry);
    csv_parseEntry(&entry, "01/01/2023;31/12/2023;12345678A;Lucas;600.0;25;ABC1234;EXTRA", "TENANTT");
	
    error = api_addTenant(&data, entry);
    if (error != E_INVALID_ENTRY_TYPE) {
      failed = true;
      passed = false;
      fail_all = true;
    }
    csv_freeEntry(&entry);
	
	csv_initEntry(&entry);
	csv_parseEntry(&entry, "01/01/2023;31/12/2023;12345678A;Lucas;600.0;25;ABC1234;EXTRA", "TENANT");
	
    error = api_addTenant(&data, entry);
    if (error != E_INVALID_ENTRY_FORMAT) {
      failed = true;
      passed = false;
      fail_all = true;
    }
    csv_freeEntry(&entry);
	
	csv_initEntry(&entry);
	csv_parseEntry(&entry, "01/01/2023;31/12/2023;12345678A;Lucas;600.0;25", "TENANT");
	
    error = api_addTenant(&data, entry);
    if (error != E_INVALID_ENTRY_FORMAT) {
      failed = true;
      passed = false;
      fail_all = true;
    }
    csv_freeEntry(&entry);
  }
  end_test(test_section, "PR1_EX2_2", !failed);

  /////////////////////////////
  /////  PR1 EX2 TEST 3  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX2_3", "Add a valid tenant");
  if (fail_all) {
    failed = true;
  } else {
    csv_initEntry(&entry);
	csv_parseEntry(&entry, "01/01/2023;31/12/2023;12345678A;Lucas;600.0;25;ABC1234", "TENANT");
    
    error = api_addTenant(&data, entry);
    if (error != E_SUCCESS) {
      failed = true;
      passed = false;
      fail_all = true;
    }
    csv_freeEntry(&entry);
  }
  end_test(test_section, "PR1_EX2_3", !failed);
  
  /////////////////////////////
  /////  PR1 EX2 TEST 4  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX2_4", "Add a duplicated tenant");
  if (fail_all) {
    failed = true;
  } else {
    csv_initEntry(&entry);
	csv_parseEntry(&entry, "01/01/2023;31/12/2023;12345678A;Lucas;600.0;25;ABC1234", "TENANT");
              
    error = api_addTenant(&data, entry);
    if (error != E_TENANT_DUPLICATED) {
      failed = true;
      passed = false;
      fail_all = true;
    }
    csv_freeEntry(&entry);
  }
  end_test(test_section, "PR1_EX2_4", !failed);
  
  /////////////////////////////
  /////  PR1 EX2 TEST 5  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX2_5", "Add an invalid landlord");
  if (fail_all) {
    failed = true;
  } else {
    csv_initEntry(&entry);
    csv_parseEntry(&entry, "John;87654321K;0;EXTRA", "LANDLORDD");
	
    error = api_addLandlord(&data, entry);
    if (error != E_INVALID_ENTRY_TYPE) {
      failed = true;
      passed = false;
      fail_all = true;
    }
    csv_freeEntry(&entry);
	
	csv_initEntry(&entry);
	csv_parseEntry(&entry, "John;87654321K;0;EXTRA", "LANDLORD");
	
    error = api_addLandlord(&data, entry);
    if (error != E_INVALID_ENTRY_FORMAT) {
      failed = true;
      passed = false;
      fail_all = true;
    }
    csv_freeEntry(&entry);
	
	csv_initEntry(&entry);
	csv_parseEntry(&entry, "John;87654321K", "LANDLORD");
	
    error = api_addLandlord(&data, entry);
    if (error != E_INVALID_ENTRY_FORMAT) {
      failed = true;
      passed = false;
      fail_all = true;
    }
    csv_freeEntry(&entry);
  }
  end_test(test_section, "PR1_EX2_5", !failed);
  
  /////////////////////////////
  /////  PR1 EX2 TEST 6  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX2_6", "Add a valid landlord");
  if (fail_all) {
    failed = true;
  } else {
    csv_initEntry(&entry);
	csv_parseEntry(&entry, "John;87654321K;0", "LANDLORD");
              
    error = api_addLandlord(&data, entry);
    if (error != E_SUCCESS) {
      failed = true;
      passed = false;
      fail_all = true;
    }
    csv_freeEntry(&entry);
  }
  end_test(test_section, "PR1_EX2_6", !failed);
  
  /////////////////////////////
  /////  PR1 EX2 TEST 7  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX2_7", "Add a duplicated landlord");
  if (fail_all) {
    failed = true;
  } else {
    csv_initEntry(&entry);
	csv_parseEntry(&entry, "John;87654321K;0", "LANDLORD");
              
    error = api_addLandlord(&data, entry);
    if (error != E_LANDLORD_DUPLICATED) {
      failed = true;
      passed = false;
      fail_all = true;
    }
    csv_freeEntry(&entry);
  }
  end_test(test_section, "PR1_EX2_7", !failed);
  
  /////////////////////////////
  /////  PR1 EX2 TEST 8  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX2_8", "Add an invalid property");
  if (fail_all) {
    failed = true;
  } else {
    csv_initEntry(&entry);
    csv_parseEntry(&entry, "ABC1234;Balmes;25;87654321K;EXTRA", "PROPERTYY");
	
    error = api_addProperty(&data, entry);
    if (error != E_INVALID_ENTRY_TYPE) {
      failed = true;
      passed = false;
      fail_all = true;
    }
    csv_freeEntry(&entry);
	
	csv_initEntry(&entry);
	csv_parseEntry(&entry, "ABC1234;Balmes;25;87654321K;EXTRA", "PROPERTY");
	
    error = api_addProperty(&data, entry);
    if (error != E_INVALID_ENTRY_FORMAT) {
      failed = true;
      passed = false;
      fail_all = true;
    }
    csv_freeEntry(&entry);
	
	csv_initEntry(&entry);
	csv_parseEntry(&entry, "ABC1234;Balmes;25", "PROPERTY");
	
    error = api_addProperty(&data, entry);
    if (error != E_INVALID_ENTRY_FORMAT) {
      failed = true;
      passed = false;
      fail_all = true;
    }
    csv_freeEntry(&entry);
    
    csv_initEntry(&entry);
	csv_parseEntry(&entry, "ABC1234;Balmes;25;00000000T", "PROPERTY");
	
    error = api_addProperty(&data, entry);
    if (error != E_LANDLORD_NOT_FOUND) {
      failed = true;
      passed = false;
      fail_all = true;
    }
    csv_freeEntry(&entry);
  }
  end_test(test_section, "PR1_EX2_8", !failed);
  
  /////////////////////////////
  /////  PR1 EX2 TEST 9  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX2_9", "Add a valid property");
  if (fail_all) {
    failed = true;
  } else {
    csv_initEntry(&entry);
	csv_parseEntry(&entry, "ABC1234;Balmes;25;87654321K", "PROPERTY");
              
    error = api_addProperty(&data, entry);
    if (error != E_SUCCESS) {
      failed = true;
      passed = false;
      fail_all = true;
    }
    csv_freeEntry(&entry);
  }
  end_test(test_section, "PR1_EX2_9", !failed);
  
  /////////////////////////////
  /////  PR1 EX2 TEST 10  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX2_10", "Add a duplicated property");
  if (fail_all) {
    failed = true;
  } else {
    csv_initEntry(&entry);
	csv_parseEntry(&entry, "ABC1234;Balmes;25;87654321K", "PROPERTY");
              
    error = api_addProperty(&data, entry);
    if (error != E_PROPERTY_DUPLICATED) {
      failed = true;
      passed = false;
      fail_all = true;
    }
    csv_freeEntry(&entry);
  }
  end_test(test_section, "PR1_EX2_10", !failed);
  
  /////////////////////////////
  /////  PR1 EX2 TEST 11  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX2_11", "Add an invalid rental income");
  if (fail_all) {
    failed = true;
  } else {
    csv_initEntry(&entry);
    csv_parseEntry(&entry, "2024;3500.0;87654321K;EXTRA", "RENTAL_INCOMEE");
	
    error = api_addRentalIncome(&data, entry);
    if (error != E_INVALID_ENTRY_TYPE) {
      failed = true;
      passed = false;
      fail_all = true;
    }
    csv_freeEntry(&entry);
	
	csv_initEntry(&entry);
	csv_parseEntry(&entry, "2024;3500.0;87654321K;EXTRA", "RENTAL_INCOME");
	
    error = api_addRentalIncome(&data, entry);
    if (error != E_INVALID_ENTRY_FORMAT) {
      failed = true;
      passed = false;
      fail_all = true;
    }
    csv_freeEntry(&entry);
	
	csv_initEntry(&entry);
	csv_parseEntry(&entry, "2024;3500.0", "RENTAL_INCOME");
	
    error = api_addRentalIncome(&data, entry);
    if (error != E_INVALID_ENTRY_FORMAT) {
      failed = true;
      passed = false;
      fail_all = true;
    }
    csv_freeEntry(&entry);
    
    csv_initEntry(&entry);
	csv_parseEntry(&entry, "2024;3500.0;00000000T", "RENTAL_INCOME");
	
    error = api_addRentalIncome(&data, entry);
    if (error != E_LANDLORD_NOT_FOUND) {
      failed = true;
      passed = false;
      fail_all = true;
    }
    csv_freeEntry(&entry);
  }
  end_test(test_section, "PR1_EX2_11", !failed);
  
  /////////////////////////////
  /////  PR1 EX2 TEST 12  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX2_12", "Add a valid rental income");
  if (fail_all) {
    failed = true;
  } else {
    csv_initEntry(&entry);
	csv_parseEntry(&entry, "2024;3500.0;87654321K", "RENTAL_INCOME");
              
    error = api_addRentalIncome(&data, entry);
    if (error != E_SUCCESS) {
      failed = true;
      passed = false;
      fail_all = true;
    }
    csv_freeEntry(&entry);
  }
  end_test(test_section, "PR1_EX2_12", !failed);

  /////////////////////////////
  /////  PR1 EX2 TEST 13  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX2_13", "Find a rental income in the data");
  if (fail_all) {
    failed = true;
  } else {

	pRentalIncome = rentalIncomes_find(data.rentalIncomes, 2024, "00000000T");
    
    if (pRentalIncome != NULL) {
        failed = true;
        passed = false;
        fail_all = true;
    } else {
        pRentalIncome = rentalIncomes_find(data.rentalIncomes, 2024, "87654321K");
        
        if (pRentalIncome == NULL) {
          failed = true;
          passed = false;
          fail_all = true;
        } else if ((strcmp(pRentalIncome->landlord->id, "87654321K") != 0)) {
            failed = true;
            passed = false;
            fail_all = true;
        }
    }
  }
  end_test(test_section, "PR1_EX2_13", !failed);

  /////////////////////////////
  /////  PR1 EX2 TEST 14  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX2_14", "Check the number of tenants");
  if (fail_all) {
    failed = true;
  } else {
    nTenants = api_tenantCount(data);
    if (nTenants != 1) {
      failed = true;
      passed = false;
      fail_all = true;
    }
  }
  end_test(test_section, "PR1_EX2_14", !failed);
  
  /////////////////////////////
  /////  PR1 EX2 TEST 15  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX2_15", "Check the number of landlords");
  if (fail_all) {
    failed = true;
  } else {
    nLandlords = api_landlordsCount(data);
    if (nLandlords != 1) {
      failed = true;
      passed = false;
      fail_all = true;
    }
  }
  end_test(test_section, "PR1_EX2_15", !failed);
  
  /////////////////////////////
  /////  PR1 EX2 TEST 16  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX2_16", "Check the number of properties");
  if (fail_all) {
    failed = true;
  } else {
    nProperties = api_propertiesCount(data);
    if (nProperties != 1) {
      failed = true;
      passed = false;
      fail_all = true;
    }
  }
  end_test(test_section, "PR1_EX2_16", !failed);
  
  /////////////////////////////
  /////  PR1 EX2 TEST 17  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX2_17", "Check the number of rental incomes");
  if (fail_all) {
    failed = true;
  } else {
    nRentalIncomes = api_rentalIncomesCount(data);
    if (nRentalIncomes != 1) {
      failed = true;
      passed = false;
      fail_all = true;
    }
  }
  end_test(test_section, "PR1_EX2_17", !failed);
  
  /////////////////////////////
  /////  PR1 EX2 TEST 18  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX2_18", "Free API data");
  if (fail_all) {
    failed = true;
  } else {
    error = api_freeData(&data);
    nTenants = api_tenantCount(data);
    nLandlords = api_landlordsCount(data);
	nProperties = api_propertiesCount(data);
    nRentalIncomes = api_rentalIncomesCount(data);
    if (error != E_SUCCESS || nTenants != 0 || nLandlords != 0 || nProperties != 0 || nRentalIncomes != 0) {
      failed = true;
      passed = false;
      fail_all = true;
    }
  }
  end_test(test_section, "PR1_EX2_18", !failed);

  /////////////////////////////
  /////  PR1 EX2 TEST 19  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX2_19", "Load data from file");
  // Load basic data to the API
  if (fail_all) {
    failed = true;
  } else {
    error = api_loadData(&data, input, true);
    nTenants = api_tenantCount(data);
    nLandlords = api_landlordsCount(data);
	nProperties = api_propertiesCount(data);
    nRentalIncomes = api_rentalIncomesCount(data);
    if (error != E_SUCCESS || nTenants != 3 || nLandlords != 2 || nProperties != 3 || nRentalIncomes != 4) {
      failed = true;
      passed = false;
    }
  }
  end_test(test_section, "PR1_EX2_19", !failed);

  // Release all data
  api_freeData(&data);

  return passed;
}

// Run all tests for Exercice 3 of PR1
bool run_pr1_ex3(tTestSection *test_section, const char *input) {
  tApiData data;
  tApiError error;
  tCSVEntry entry;
  tCSVEntry refEntry;
  tCSVData report;
  tCSVData refReport;
  int nTenants;
  int nLandlords;
  int nProperties;
  int nRentalIncomes;
  bool passed = true;
  bool failed = false;
  bool fail_all = false;

  // Initialize the data
  error = api_initData(&data);
  if (error != E_SUCCESS) {
    passed = false;
    fail_all = true;
  }

  if (!fail_all) {
    error = api_loadData(&data, input, true);
    nTenants = api_tenantCount(data);
    nLandlords = api_landlordsCount(data);
	nProperties = api_propertiesCount(data);
    nRentalIncomes = api_rentalIncomesCount(data);
    if (error != E_SUCCESS || nTenants != 3 || nLandlords != 2 || nProperties != 3 || nRentalIncomes != 4) {
      passed = false;
      fail_all = true;
    }
  }

  /////////////////////////////
  /////  PR1 EX3 TEST 1  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX3_1", "Request a valid landlord");
  if (fail_all) {
    failed = true;
  } else {
    csv_initEntry(&entry);
    csv_initEntry(&refEntry);
    csv_parseEntry(&refEntry, "John;87654321K;4800.0", "LANDLORD");
    error = api_getLandlord(data, "87654321K", &entry);
    if (error != E_SUCCESS || !csv_equalsEntry(entry, refEntry)) {
      failed = true;
      passed = false;
    }
    csv_freeEntry(&entry);
    csv_freeEntry(&refEntry);
  }
  end_test(test_section, "PR1_EX3_1", !failed);

  /////////////////////////////
  /////  PR1 EX3 TEST 2  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX3_2", "Request a missing landlord");
  if (fail_all) {
    failed = true;
  } else {
    csv_initEntry(&entry);
	error = api_getLandlord(data, "NO_LANDLORD", &entry);
	if (error != E_LANDLORD_NOT_FOUND) {
      failed = true;
      passed = false;
    }
    csv_freeEntry(&entry);
  }
  end_test(test_section, "PR1_EX3_2", !failed);
  
  /////////////////////////////
  /////  PR1 EX3 TEST 3  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX3_3", "Request a valid rental income");
  if (fail_all) {
    failed = true;
  } else {
    csv_initEntry(&entry);
    csv_initEntry(&refEntry);
    csv_parseEntry(&refEntry, "2023;3500.0;87654321K", "RENTAL_INCOME");
    error = api_getRentalIncome(data, 2023, "87654321K", &entry);
    if (error != E_SUCCESS || !csv_equalsEntry(entry, refEntry)) {
      failed = true;
      passed = false;
    }
    csv_freeEntry(&entry);
    csv_freeEntry(&refEntry);
  }
  end_test(test_section, "PR1_EX3_3", !failed);

  /////////////////////////////
  /////  PR1 EX3 TEST 4  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX3_4", "Request a missing rental income");
  if (fail_all) {
    failed = true;
  } else {
    csv_initEntry(&entry);
	error = api_getRentalIncome(data, 2024, "NO_RENTAL_INCOME", &entry);
	if (error != E_RENTAL_INCOME_NOT_FOUND) {
      failed = true;
      passed = false;
    }
    csv_freeEntry(&entry);
	
	csv_initEntry(&entry);
	error = api_getRentalIncome(data, 2026, "87654321K", &entry);
	if (error != E_RENTAL_INCOME_NOT_FOUND) {
      failed = true;
      passed = false;
    }
    csv_freeEntry(&entry);
  }
  end_test(test_section, "PR1_EX3_4", !failed);

  /////////////////////////////
  /////  PR1 EX3 TEST 5  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX3_5", "Get registered properties");
  if (fail_all) {
    failed = true;
  } else {
    csv_init(&report);
    csv_init(&refReport);

	csv_addStrEntry(&refReport, "ABC1234;Balmes;25;87654321K", "PROPERTY");	
	csv_addStrEntry(&refReport, "ZYX1234;Balmes;26;87654321K", "PROPERTY");
	csv_addStrEntry(&refReport, "QWE1234;Turing;99;54927077H", "PROPERTY");

    error = api_getProperties(data, &report);
    if (error != E_SUCCESS || !csv_equals(report, refReport)) {
      failed = true;
      passed = false;
    }
    csv_free(&report);
    csv_free(&refReport);
  }
  end_test(test_section, "PR1_EX3_5", !failed);

  /////////////////////////////
  /////  PR1 EX3 TEST 6  //////
  /////////////////////////////
  failed = false;
  start_test(test_section, "PR1_EX3_6", "Get registered rental incomes");
  if (fail_all) {
    failed = true;
  } else {
    csv_init(&report);
    csv_init(&refReport);		
    		
	csv_addStrEntry(&refReport, "2023;3500.00;87654321K" ,"RENTAL_INCOME");
	csv_addStrEntry(&refReport, "2024;7500.10;54927077H" ,"RENTAL_INCOME");
	csv_addStrEntry(&refReport, "2024;8800.55;87654321K" ,"RENTAL_INCOME");
	csv_addStrEntry(&refReport, "2025;3500.99;54927077H" ,"RENTAL_INCOME");		
		
    error = api_getRentalIncomes(data, &report);
    if (error != E_SUCCESS || !csv_equals(report, refReport)) {
      failed = true;
      passed = false;
    }
    csv_free(&report);
    csv_free(&refReport);
  }
  end_test(test_section, "PR1_EX3_6", !failed);
	
  // Release all data
  api_freeData(&data);

  return passed;
}
