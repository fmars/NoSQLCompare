
#ifndef _PAXUS_STORAGE_H
#define _PAXUS_STORAGE_H

#include <tcutil.h>
#include <tcbdb.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "logger.h"


class paxus_storage{
private:
   std::string storageFilePath;
   TCBDB *bdb;
   BDBCUR *cur;
   
public:
   paxus_storage(const std::string &outFile);
   bool create_database();
   bool open_database();
   bool insert_record(const void *key, int key_size, const void *value, int value_size);
   void *lookup_record(const void *key, int key_size, int *sp);
   bool insert_string_record(const char *key_str, const char *value_str);
   char *lookup_string_record(const char *key_str);
   bool remove_record(const void *key, int key_size);
   void close_database();
   void traverse_string_records();
   void traverse_records();
   bool transaction_start();
   bool transaction_end();
   bool delete_records();
   bool seek_first_record();
   bool seek_next_record();
   bool remove_curr_record();
   void *curr_key(int *sp);
   void *curr_val(int *sp);
   void del_cursor();

   uint64_t database_size();
};

#endif
