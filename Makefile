target: test
test: main.o mem_test.o event_test.o host_to_dev.o dev_to_dev.o dev_to_host.o
	gcc -o test  main.o mem_test.o event_test.o host_to_dev.o dev_to_dev.o dev_to_host.o
main.o: main.c 
	gcc -o main.o -c  main.c 
mem_test.o: mem_test.c
	gcc -o mem_test.o -c mem_test.c
event_test.o: event_test.c
	gcc -o event_test.o -c event_test.c
host_to_dev.o: host_to_dev.c
	gcc -o host_to_dev.o -c host_to_dev.c
dev_to_dev.o: dev_to_dev.c
	gcc -o dev_to_dev.o -c dev_to_dev.c
dev_to_host.o: dev_to_host.c
	gcc -o dev_to_host.o -c dev_to_host.c
clean:
	rm test *.o 
