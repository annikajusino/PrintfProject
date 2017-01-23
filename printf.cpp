#include "printf.hpp"

int printf(const char *fmt, ...){
	int size = 0;
	char cur = fmt[0];

	while(cur != '\0'){
		size++;
		cur = fmt[size];
	}
	
	for(int i = 0; i < size; i++){
		if(fmt[i] == '%'){
			write(1, "percent found\n", 14);
		}
	}

	return write(1, fmt, size);
}
