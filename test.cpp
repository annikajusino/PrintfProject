#include "printf.hpp"

int main(){
	char test[12];
	snprintf(test, 12, "test%dtest\n", 12345);
	
	int size = 0;
	char cur = test[0];
	while(cur != '\0'){
		size++;
		cur = test[size];
	}
	write(1, test, size);
}
