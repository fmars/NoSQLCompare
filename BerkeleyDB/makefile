export LIBS="-L/usr/local/BerkeleyDB.6.1/lib"
export CXXFLAGS="-I/usr/local/BerkeleyDB.6.1/include"
export LD_LIBRARY_PATH="/usr/local/BerkeleyDB.6.1/lib"
g++ -o insert -L/usr/local/BerkeleyDB.6.1/lib  -I/usr/local/BerkeleyDB.6.1/include TestSimpleAccess_insert.cpp -ldb_cxx

g++ -o query -L/usr/local/BerkeleyDB.6.1/lib  -I/usr/local/BerkeleyDB.6.1/include TestSimpleAccess_query.cpp -ldb_cxx

g++ -o a.out -L/usr/local/BerkeleyDB.6.1/lib  -I/usr/local/BerkeleyDB.6.1/include TestSimpleAccess.cpp -ldb_cxx
