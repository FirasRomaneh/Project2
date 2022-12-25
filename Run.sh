gcc -w people.c -o people -lpthread
gcc -w worker.c -o worker -lpthread
gcc -w parent.c -o parent -lpthread
./parent

