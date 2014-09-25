g++ insert.cpp -g -I/home/user/Desktop/dbDriver/include -L/home/user/Desktop/dbDriver/lib -L/usr/local/lib   -pthread -lmongoclient -lboost_thread -lboost_system -o insert 

g++ query.cpp -g -I/home/user/Desktop/dbDriver/include -L/home/user/Desktop/dbDriver/lib -L/usr/local/lib   -pthread -lmongoclient -lboost_thread -lboost_system -o query 
