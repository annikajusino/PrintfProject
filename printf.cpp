#include "printf.hpp"

//converts a decimal number into a string representation of its hexadecimal value
char* hexconv(int64_t in){
	char converted[17]; //holds intial conversion of number from dec to hex
	int64_t temp;		
	int leadzero = 0; //used to track the number of leading zeroes on the converted string
	
	//converts the decimal value into a string representation of the hex value
	for(int i = 0; i < 16; i++){
		temp = (in >> (i*4)) & 0xf; //collects the value of the ith byte
		if(temp >= 10)
			converted[15 - i] = 'a' + (temp - 10); // if value >= 10, the hex value is a letter
		else
			converted[15 - i] = '0' + temp; //if value < 10, hex value = decimal value
	}

	//counts leading zeroes
	while (converted[leadzero] == '0')
		leadzero++;

	char *conv_short = new char[17 - leadzero]; //holds the converted string minus leading zeroes
	
	//removes leading zeroes, placing final representation into cov_short
	for(int i = 0; i < 17 - leadzero -1; i++){
		conv_short[i] = converted[i + leadzero];
	}
	conv_short[17 - leadzero - 1] = '\0';

	return conv_short;
}


//writes char array of any length to stdout by counting the size and writing it using typical write() function

void cwrite(const char* wstr) //writes string of variable size
{
	int size = 0;
	char cur = wstr[0];

	while(cur != '\0'){
		size++;
		cur = wstr[size];
	}

	write(1, wstr, size);

}

int scwrite(const char* wstr, char* istr, int index, int rem){
	int size = 0;
	char cur = wstr[0];

	while(cur != '\0'){
		size++;
		cur = wstr[size];
	}
	if(size > rem){
		for(int i = 0; i < size - rem; i++){
			istr[index + i] = wstr[i];
		}
		return size - rem;
	}
	else{
		for(int i = 0; i < size; i++){
			istr[index + i] = wstr[i];
		}
		return size;
	}
}

//converts integer into string

const char* itostr(int tostr)
{
	int whole;
	int digit;
	char* wstr = new char[100]();			//stands for "whole" string
	char* mystr = new char[100]();
	int index = 0;
	int j = 0;

	whole = tostr;
	
	do{
		digit = whole % 10;					//find the digit in the "1's" place

		wstr[index] = (char) (digit + 48);	//add 48 to push it to its corresponding ASCII character, cast to char and put on the string

		whole = whole / 10;					//divides by 10 to bring the next "10's" place down to the "1's" place
		
		index++;							//increment place in the string

	}while(whole != 0);						//checks to see if whole is 0, which only happens when the remaining number is < 10...i.e., there are no more digits. 



	while(j < index)						//steps through the "whole number" string backwards, since it was assigned from smallest to largest
	{
		mystr[j] = wstr[index - j - 1];
		j++;
	}
	
	return mystr;
}

//converts doubles to strings--same as itostr except for commented sections

const char* dtostr(double tostr)	//Needs formatting/precision (default 6) and roundingw
{
	int whole;
	int digit;
	double zeroplus = 0;			//keeps track of the "whole number" part of the number--subtracted later to obtain the decimal part only
	double decimal;
	char* wstr = new char[100]();
	char* mystr = new char[100]();
	int index = 0;
	int i = 0;						//used for counting up the power of 10
	int j = 0;
	int prec = 6;
	int power;						//also used for counting up the power of 10

	whole = tostr / 1;				//integer divide for the sake of whole #'s
	
	do{
		digit = whole % 10;

		wstr[index] = (char) (digit + 48);

		i = 0;
		power = 1;
		while(i < index)
		{
			power = power * 10;		//multiplies 1 by a power of 10 for each time we have scooted up a decimal place
			i++;
		}

		zeroplus = zeroplus + (digit * power);	//adds to keep track of whole number before decimal

		whole = whole / 10;
		
		index++;

	}while(whole != 0);

	while(j < index)
	{
		mystr[j] = wstr[index - j - 1];
		j++;
	}


	decimal = tostr - zeroplus;	//extracts decimal portion

//	if(decimal != 0) //needs to be changed to (if decimal flag is set)
//	{
		//has decimal, append a dot
		mystr[index] = '.';
		index++;
//	}

	i = 0;
	while(i < prec)		//has a rounding error somewhere, works opposite to the idea earlier; multiplies by 10 instead
	{
		decimal = decimal * 10.0;
		digit = (int)decimal % 10;
		mystr[index] = (char) (digit+48);
		index++;
		i++;

	}
	return mystr;
}


int printf(const char *fmt, ...){
	va_list args;				//args list
	int tempi;					//integer buffer for args
	double tempf;				//float/double buffer for args
	int size = 0;				
	char cur = fmt[0];			//current character
	char *buf = new char[1]();	//for writing letter by letter
	const char* converted;		//buffer for numbers converted to strings
	
	va_start(args, fmt);		//init list to arg named "fmt"
	
	while(cur != '\0'){			//count letters
		size++;
		cur = fmt[size];
	}

	for(int i = 0; i < size; i++)	//iterate through string, find percent signs
	{
		if(fmt[i] == '%')			//if found, interate forward to the next character to figure out data type
		{
//			cwrite("Percent found");
			i++;			

									//pull next arg of that data type, convert, and print
			
			if(fmt[i] == 'd')
			{
//				cwrite(" integer ");
				tempi = va_arg(args, int);
				converted = itostr(tempi);
				cwrite(converted);
			}else if(fmt[i] == 'x')
			{
//				write(1,"hex",5);
				converted = hexconv(va_arg(args, int));
				cwrite(converted);
			}else if(fmt[i] == 'f')
				
			//may need additional i++'s to get all of the formatting flags in
			//may need encasing "while != any of these...(d, x, f, s, etc)" to collect flags before %f
			
			
			
			
			{
//				cwrite("float");
				tempf = va_arg(args, double);
				converted = dtostr(tempf);
				cwrite(converted);
			}
		}
		else						//if not % sign, write as usual
		{
			buf[0] = fmt[i];
			write(1, buf, 1);
		}
	}

	va_end(args);

	return 0;
}

int snprintf(char *dest, size_t size, const char *fmt, ...){
	va_list args;				//args list
	int tempi;					//integer buffer for args
	double tempf;				//float/double buffer for args
	int fsize = 0;				
	char cur = fmt[0];			//current character
	char *buf = new char[1]();	//for writing letter by letter
	const char* converted;		//buffer for numbers converted to strings
	int count = 0;
	int to_write;
	va_start(args, fmt);		//init list to arg named "fmt"
	
	while(cur != '\0'){			//count letters
		fsize++;
		cur = fmt[fsize];
	}
	if(fsize > size - 1)
		to_write = size - 1;
	else
		to_write = fsize;

	for(int i = 0; i < to_write; i++)	//iterate through string, find percent signs
	{
		if(fmt[i] == '%')			//if found, interate forward to the next character to figure out data type
		{
			i++;			

									//pull next arg of that data type, convert, and print
			
			if(fmt[i] == 'd')
			{
				tempi = va_arg(args, int);
				converted = itostr(tempi);
				count += scwrite(converted, dest, count, size - count);
				//cwrite(converted);
			}else if(fmt[i] == 'x')
			{
				converted = hexconv(va_arg(args, int));
				count += scwrite(converted, dest, count, size - count);
				//cwrite(converted);
			}else if(fmt[i] == 'f')
			//may need additional i++'s to get all of the formatting flags in
			//may need encasing "while != any of these...(d, x, f, s, etc)" to collect flags before %f
			{
				tempf = va_arg(args, double);
				converted = dtostr(tempf);
				count += scwrite(converted, dest, count, size - count);
				//cwrite(converted);
			}
		}
		else						//if not % sign, write as usual
		{
			buf[0] = fmt[i];
			write(1, buf, 1);
			count++;
		}
	}

	va_end(args);

	return to_write;
}

