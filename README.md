NoSQLCompare
============

//Sept. 2th
Test several popular NoSQL database features and performance.

Use C++ driver to connect and manipulate database server.

//current test method for mongodb
{
	Insert undefined data into database. Each record has 7 field which is a key value pair in which value is a random integer or 
  a random string with a random length in a predefine range. In total insert 1000000 records. Do query on the database within
  each field.
}


See the storage size, memory used and query performance and charset support.

//Sept.24th
For three types of NoSQL databases and each actual databases software for each type, now have understood the basic ideal behind each type of databases and have a prelimilary understanding of each database software's feature and usage. Also done the C++ driver test for each databases software in which performs insertion, query operations.

Having understood the basic features of each type of NoSQL databases, now focus on Wide-Column Oriented Database and use HyperTable to do the actual data performance test.
First step. Parse the original data used in actual world from XML file.
Next. Reach the details of HyperTable HQL query language.
Next. Analysis the data and required operations.
Next. Design the database data model especially about how to store and query one-to-many relationship.
Next. Code and test.


//Sept.24th
Find it hard to maintain one-to-many relationship by Hypertable because of the column value search limitation and doesn't support RegEx.
Thus the solution may only be that use multiple version of the cell or use multiple table.

When Ruchi said that try to use mongodb and find if there is any solution, I really didn't want to move to MongoDB because I just started with HyperTable. And in my mind that MongoDB doesn't support that feature either.

However the really embarrassing thing is that, when I tried to see MongoDB, both of these two features are supported.
MongoDB supports embedded structure search and RegEx search.
