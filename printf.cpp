#include "printf.hpp"

//converts a decimal number into a string representation of its hexadecimal value
char* hexconv(long long in){
	char converted[17]; //holds intial conversion of number from dec to hex
	long long temp;		
	long long leadzero = 0; //used to track the number of leading zeroes on the converted string
	
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

	char conv_short[17 - leadzero]; //holds the converted string minus leading zeroes
	
	//removes leading zeroes, placing final representation into cov_short
	for(int i = 0; i < 17 - leadzero -1; i++){
		conv_short[i] = converted[i + leadzero];
	}
	conv_short[17 - leadzero - 1] = '\0';

	return conv_short;
}


//writes char array of any length to stdout by counting the size and writing it using typical write() function

int cwrite(const char* wstr) //writes string of variable size
{
	int size = 0;
	char cur = wstr[0];

	while(cur != '\0'){
		cur = wstr[size];
		size++;
	}

	return write(1, wstr, size);

}

//copies one char array (wstr) to another char array (dest), starting from char at index, up to index == max. Returns the number of characters copied
int scwrite(const char* wstr, char* dest, int index, int max){
	int size = 0;			//size of copied string
	char cur = wstr[0];		//current character
	
	while(cur != '\0'){ //measures size of copied string
		size++;
		cur = wstr[size];
	}

	if((size + index) > max){ //if the copied string length + index exceeds the max, writes up to the max
		for(int i = 0; i < (max-index) ; i++){
			dest[index + i] = wstr[i];
		}
		return max-index ;
	}
	else{ //if copied string length + index won't exceed max, writes whole string to dest
		for(int i = 0; i < size; i++){
			dest[index + i] = wstr[i];
		}
		return size;
	}
}

//converts integer into string

const char* itostr(long long tostr)
{
	long long whole;
	long long digit;
	char wstr[100];			//stands for "whole" string
	char mystr[100];
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

	mystr[j] = '\0';


	return mystr;
}

//converts doubles to strings--same as itostr except for commented sections

const char* dtostr(double tostr)//, int b4, int after)	//Needs formatting/precision (default 6) and roundingw
{
	int whole;
	int digit;
	double zeroplus = 0;			//keeps track of the "whole number" part of the number--subtracted later to obtain the decimal part only
	double decimal;
	char wstr[100];
	char mystr[100];
	int index = 0;
	int i = 0;						//used for counting up the power of 10
	int j = 0;
	int prec = 6;
	int power;						//also used for counting up the power of 10
/*
	if(after != -1)
	{
		prec = after;
	}
*/

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
		mystr[index] = (char) (decimal+48);				//.whatever * 10 = w.hatever, take w + 48 (corresponding char)
		decimal = decimal - int(mystr[index] - 48);		//take w-48 to get original integer, subtract w to get .hatever
		index++;										//scoot forward and continue
		i++;

	}

	mystr[index] = '\0';
	return mystr;
}


int printf(const char *fmt, ...){
	va_list args;				//args list
	long long tempi;					//integer buffer for args
	double tempf;				//float/double buffer for args
	int size = 0;				
	char cur = fmt[0];			//current character
	char buf[1];	//for writing letter by letter
	const char* converted;		//buffer for numbers converted to strings
	int count = 0;				//tracks number of characters written to output for returning from printf
	char *pstring;

//	int b4 = 0;
//	int after = 0;

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
			/*						
			if(fmt[i] != ('d' || 'f' || 'x'))
			{
				while(fmt[i] != '.')
				{
					b4 = b4 * 10;
					b4 = b4 + (char)(fmt[i] + 48);
					i++;
				}

				i++;

				while(fmt[i] != 'f')
				{
					after = after * 10;
					after = (char)(fmt[i] + 48);
					i++
				}

			}
			*/
			
			if(fmt[i] == 'd')
			{
//				cwrite(" integer ");
				tempi = va_arg(args, long long);
				converted = itostr(tempi);
				count += cwrite(converted);
			}else if(fmt[i] == 'x')
			{
//				write(1,"hex",5);
				converted = hexconv(va_arg(args, long long));
				count += cwrite(converted);
			}else if(fmt[i] == 'f')
					
			//may need additional i++'s to get all of the formatting flags in
			//may need encasing "while != any of these...(d, x, f, s, etc)" to collect flags before %f
			{
//				cwrite("float");
				tempf = va_arg(args, double);
				converted = dtostr(tempf);
				count += cwrite(converted);
			}else if(fmt[i] == 's')
			{
				pstring = va_arg(args, char*);
				cwrite(pstring);
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

	return count;
}

int snprintf(char *dest, size_t size, const char *fmt, ...){
	va_list args;				//args list
	long long tempi;					//integer buffer for args
	double tempf;				//float/double buffer for args
	long long asize = size-1;			//number of non-termination characters that can be written
	const char* converted;		//buffer for numbers converted to strings
	long long count = 0;				//tracks number of characters written to dest
	va_start(args, fmt);		//init list to arg named "fmt"
	long long i = 0;					//iteration variable
	char *pstring;

	while(fmt[i] != '\0' && count < asize)
	{
		if(fmt[i] == '%')			//if found, interate forward to the next character to figure out data type
		{
			i++;											
			
			//pull next arg of that data type, convert, and print
			if(fmt[i] == 'd')
			{
				tempi = va_arg(args, long long);
				converted = itostr(tempi);
				count += scwrite(converted, dest, count, asize );
			}else if(fmt[i] == 'x')
			{
				converted = hexconv(va_arg(args, long long));
				count += scwrite(converted, dest, count, asize);
			}else if(fmt[i] == 'f')
			//may need additional i++'s to get all of the formatting flags in
			//may need encasing "while != any of these...(d, x, f, s, etc)" to collect flags before %f
			{
				tempf = va_arg(args, double);
				converted = dtostr(tempf);
				count += scwrite(converted, dest, count, asize);
			}else if(fmt[i] == 's')
			{
				pstring = va_arg(args, char*);
				count += scwrite(pstring, dest, count, asize);
			}
		}
		else						//if not % sign, write as usual
		{
			dest[count] = fmt[i];	
			count++;
		}
		i++;
	}
	dest[size-1] = '\0';
	va_end(args);

	return count;
}
