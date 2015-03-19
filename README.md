# rccds-cc
For generating all terminal sets with their branch left vector of size 9,
compile with 
g++ -O2 -DCHORDS=9 lpthread-rccds-test.cc rccds.cc -lpthread
and then run
./a.out > SIZE-9
On my machine it takes about a minute to finish.