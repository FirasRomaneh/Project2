gcc -w people.c -o people -lpthread
gcc -w worker.c -o worker -lpthread
gcc -w secofficer.c -o secofficer -lpthread
gcc -w teller.c -o teller -lpthread
gcc -w parent.c -o parent -lpthread
./parent

