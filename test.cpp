#include "printf.hpp"

int main(){
	char buffer[100];
	buffer[0] = 'a';
//	buffer[1] = 's';
//	buffer[2] = 'd';
//	buffer[3] = 'f';
//	buffer[4] = 'g';
//	buffer[6] = '\0';
//	buffer[5] = 'o';

	int check = printf("test\n");
	printf("check = %d\n", check);
	printf("check = %x\n", 9223372036854775807);
	printf("check = %d\n", 9223372036854775807);
	printf("check = %f %f %f\n", 1.04, 12341.92093457, 0.000006);
	snprintf(buffer, 100, "The half of %d is %d %s", 60, 60/2, "This is a practice sentence");

	printf("%s\n", buffer);
}
