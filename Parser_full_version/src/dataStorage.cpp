#include "dataStorage.h"


paxus_storage::paxus_storage(const std::string &outFile):storageFilePath(outFile),bdb(NULL)
{

}

bool paxus_storage::create_database()
{
   if(NULL != bdb)
   {
      logWarning() << "database is already open for reading";
      return false;
   }
   /* create the object */
   bdb = tcbdbnew();
   if(!tcbdbsetcache(bdb, 1000, 1000))
   {
      logError() << "Error in setting db cache for " << storageFilePath;
      return false;
   }

   /* open the database */
   if(!tcbdbopen(bdb, storageFilePath.c_str(), BDBOWRITER | BDBOCREAT)){
      int ecode = tcbdbecode(bdb);
      logError() <<  "open error: " << tcbdberrmsg(ecode);
      return false;
   }
   tcbdboptimize(bdb, 512, 1024, 10000, 2, 8, BDBTBZIP);
   return true;
}

bool paxus_storage::open_database()
{
   if(NULL != bdb)
   {
      logWarning() << "database is already open for writing";
      return false;
   }

   bdb = tcbdbnew();

   if(!tcbdbsetcache(bdb, 1000, 1000))
   {
      logError() << "Error in setting db cache for " << storageFilePath;
      return false;
   }

   if(!tcbdbopen(bdb, storageFilePath.c_str(), BDBOREADER )){
      int ecode = tcbdbecode(bdb);
      logError() <<  "open error: " << tcbdberrmsg(ecode);
      return false;
   }
   else 
   return true;
}

bool paxus_storage::insert_record(const void *key, int key_size, const void *value, int value_size)
{
   
   if(NULL != bdb)
   {
       if (!tcbdbput(bdb, key, key_size, value, value_size)) {
         logError() << "insert error: " << tcbdberrmsg(tcbdbecode(bdb));
         return false;
      }
      return true;
   }
   
   logError() << "no open database object ";
   return false;
  
}

void *paxus_storage::lookup_record(const void *key, int key_size, int *sp)
{
   // If key is not found it will return NULL
   if(NULL != bdb)
   {
     return(tcbdbget(bdb, key, key_size, sp));
   }
   logError() << "no open database ";
   return NULL;
}

bool paxus_storage::insert_string_record(const char *key_str, const char *value_str)
{
   if (!tcbdbput2(bdb, key_str, value_str)){
      logError() << "insert error: " << tcbdberrmsg(tcbdbecode(bdb));
      return false;
   }
   return true;
}

char *paxus_storage::lookup_string_record(const char *key_str)
{
   // If key is not found it will return NULL
    if(NULL != bdb)
   {
      return(tcbdbget2(bdb, key_str));
   }
   logError() << "no open database ";
   return NULL;
}

void paxus_storage::close_database()
{
   if(!tcbdbclose(bdb)){
      int ecode = tcbdbecode(bdb);
      logError() << "close error: " << tcbdberrmsg(ecode);
   }
  /* delete the object */
  tcbdbdel(bdb);
  bdb = NULL;
}

void paxus_storage::traverse_string_records()
{
   char *key, *value;
   cur = tcbdbcurnew(bdb);
   tcbdbcurfirst(cur);
   while((key = tcbdbcurkey2(cur)) != NULL){

      value = tcbdbcurval2(cur);
      if(value){
         logInfo() << (char *)key;
         free(value);
      }
      free(key);
      tcbdbcurnext(cur);
   }
   tcbdbcurdel(cur);
}

void paxus_storage::traverse_records()
{
   void *key, *value;
   int sp;
   cur = tcbdbcurnew(bdb);
   tcbdbcurfirst(cur);
   while((key = tcbdbcurkey(cur,&sp)) != NULL){

      //value = tcbdbcurval(cur);
      //if(value){
      logInfo() << (char *)key;
      //free(value);
      //}
      free(key);
      tcbdbcurnext(cur);
   }
   //else
   //logError() << tcbdberrmsg(tcbdbecode(bdb));
   tcbdbcurdel(cur);
}

bool paxus_storage::remove_record(const void *key, int key_size)
{
   if (!tcbdbout(bdb, key, key_size)){
      logError() << "remove error: " << tcbdberrmsg(tcbdbecode(bdb));
      return false;
   }
   return true;
}

bool paxus_storage::delete_records()
{
   if (!tcbdbvanish(bdb)){
      logError() << "delete error: " << tcbdberrmsg(tcbdbecode(bdb));
      return false;
   }
   return true;
}

bool paxus_storage::seek_first_record()
{
   cur = tcbdbcurnew(bdb);

    if (!tcbdbcurfirst(cur)){
      logError() << "No records in databse: " << tcbdberrmsg(tcbdbecode(bdb));
      return false;
   }
   return true;
}
bool paxus_storage::seek_next_record()
{
   if (!tcbdbcurnext(cur)){
      logError() << "No records in databse: " << tcbdberrmsg(tcbdbecode(bdb));
      return false;
   }
   return true;
}
bool paxus_storage::remove_curr_record()
{
   if (!tcbdbcurout(cur)){
      logError() << "Cannot find record to remove: " << tcbdberrmsg(tcbdbecode(bdb));
      return false;
   }
   return true;
}

void *paxus_storage::curr_key(int *sp)
{
    return(tcbdbcurkey(cur, sp));
}

void  *paxus_storage::curr_val(int *sp)
{
    return(tcbdbcurval(cur, sp));
}

void paxus_storage::del_cursor()
{
   tcbdbcurdel(cur);
}


// The database is locked by the thread while the transaction so that only one transaction can be activated with a database object at the same time
bool paxus_storage::transaction_start()
{
   if (!tcbdbtranbegin(bdb)){
      logError() << "transaction  error: " << tcbdberrmsg(tcbdbecode(bdb));
      return false;
   }
   return true;
}

//to commit the transaction of a B+ tree database object
bool paxus_storage::transaction_end()
{
   if (!tcbdbtrancommit(bdb)){
      logError() << "transaction  error: " << tcbdberrmsg(tcbdbecode(bdb));
      return false;
   }
   return true;
}

uint64_t paxus_storage::database_size()
{
  return(tcbdbfsiz(bdb));
}
