#include "printf.hpp"

char* hexconv(int64_t in){
	char converted[17]= "0000000000000000";
	int64_t temp;
	for(int i = 0; i < 16; i++){
		temp = (in >> (i*4)) & 0xf;
		if(temp >= 10)
			converted[15 - i] = 'a' + (temp - 10);
		else
			converted[15 - i] += temp;
	}
	
	write(1, converted, 16);

	return converted;
}

int printf(const char *fmt, ...){
	int size = 0;
	char cur = fmt[0];

	while(cur != '\0'){
		size++;
		cur = fmt[size];
	}
	hexconv(168);
	for(int i = 0; i < size; i++){
		if(fmt[i] == '%'){
			write(1, "percent found\n", 14);
		}
	}

	return write(1, fmt, size);
}
