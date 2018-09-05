djhyv2_mcwv35_bsw9tf_project: main.o 
	gcc -o djhyv2_mcwv35_bsw9tf_project main.o -lm

main.o: main.c 
	gcc -g -Wall -Werror main.c -c

