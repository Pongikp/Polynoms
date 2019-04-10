#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#pragma warning(disable:4996)
#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

typedef struct monom {
	int coefficient;
	int power;
}Monom;


Monom* getPolynom(int *polynomSize);
void llocCheck(Monom* polynom);
void zeroBucketCoeff(Monom* polyBucket, int start, int end);
void no_power(Monom* polyBucket, int start, int end);
void printPolySum(Monom* polynom1, int size1, Monom* polynom2, int size2);
void printPolyMul(Monom *polynom1, int size1, Monom* polynom2, int size2);
void printCurrSum(int currPower1, int currPower2, int currCoeff1, int currCoeff2, int i, int size, int flag_firstOrLast);

//delete this functinn when i finish
void printPolynom(Monom *polynom, int size);



void main()
{
	Monom *polynom1, *polynom2;             // The input polynoms
	unsigned int polynom1Size; int polynom2Size; // The size of each polynom

	printf("Please enter the first polynom:\n");
	polynom1 = getPolynom(&polynom1Size);   // get polynom 1

	printf("Please enter the second polynom:\n");
	polynom2 = getPolynom(&polynom2Size);   // get polynom 2

	
	printf("The multiplication of the polynoms is:\n"); // print the multiplication
	printPolyMul(polynom1, polynom1Size, polynom2, polynom2Size);
	printf("\n");

	printf("The sum of the polynoms is:\n"); // print the sum
	printPolySum(polynom1, polynom1Size, polynom2, polynom2Size);
	printf("\n");

	printPolynom(polynom1, polynom1Size);
	printPolynom(polynom2, polynom2Size);

	// releasing all memory allocations
	free(polynom1);
	free(polynom2);
}

Monom* getPolynom(int *polynomSize) {

	unsigned int power;  // power of monom >=0
	int coeff; //coefficnet of monom , any int value
	unsigned int maxPower = 0; // stores the maximum power in the bucket
	char ch; // space and newline input
	Monom* polyBucket; // bucket of monoms which represents a polynom
	int bucketSize = 0; // size of polyBucket


	//first input
	scanf("%d", &power);
	ch = getchar();
	scanf("%d", &coeff);
	ch = getchar();

	//save max power
	maxPower = max(maxPower, power);

	//create a polynom "bucket" array depending on the size of "power"
	//llocCheck makes sure there was no error with malloc
	if (power == 0) {
		llocCheck(polyBucket = (Monom*)malloc(sizeof(Monom)));
		bucketSize = 1;
	}
	else {
		llocCheck(polyBucket = (Monom*)malloc((power + 1) * sizeof(Monom)));
		bucketSize = power + 1;
	}

	//zero the coeffs created in the array
	zeroBucketCoeff(polyBucket, 0, bucketSize);

	//set all starting power to -99
	no_power(polyBucket, 0, bucketSize);

	//insert into the polyBucket
	polyBucket[power].power = power;
	polyBucket[power].coefficient = coeff;


	//get input untill new line
	while (ch != '\n') {  

		scanf("%d", &power);
		ch = getchar();
		scanf("%d", &coeff);
		ch = getchar();

		//save max power
		maxPower = max(maxPower, power);

		//check if a bigger bucket is needed depending on the power
		if (power >= bucketSize) {

			
			//reallocate memory for the bucket if needed
			llocCheck(polyBucket=(Monom*)realloc(polyBucket, ((power * 2) * sizeof(Monom))));


			// set all new coeffs to 0 and all new powers created to -99 (unsed)
			zeroBucketCoeff(polyBucket, bucketSize, power * 2); 
			no_power(polyBucket, bucketSize, power * 2); 

			//save new bucket size
			bucketSize = (power * 2) + 1; 
		}

		//insert into the polyBucket
		polyBucket[power].power = power;
		polyBucket[power].coefficient = polyBucket[power].coefficient + coeff;

	} // end of while

	
	//check if allocated too much memory for the bucket and realloc if needed
	if (maxPower <= bucketSize - 1) {

		llocCheck(polyBucket = (Monom*)realloc(polyBucket, ((maxPower + 1) * sizeof(Monom))));
	}


	//function outputs, the polynom and its size
	*polynomSize = maxPower+1;
	return polyBucket;
}

void llocCheck(Monom* polynom) {

	if (polynom == NULL) printf("llock failed");
}

void zeroBucketCoeff(Monom* polyBucket, int start, int end) { // zeroes coeffs value from "start" to "end"
	int i;

	for (i = start; i < end; i++) {
		polyBucket[i].coefficient = 0;
	}

}

void no_power(Monom* polyBucket, int start, int end) {

	int i;

	for (i = start; i < end; i++) {
		polyBucket[i].power = -99;
	}
}

void printPolySum(Monom* polynom1, int size1, Monom* polynom2, int size2) {

int i, smallSize, sizeDelta;
int currCoeff1, currPower1, currCoeff2, currPower2;// current coeff and currPower of both polynoms

int flag_firstOrLast = 0; // count the valid monoms printed

sizeDelta = fabs(size1 - size2); // the diffrence in size between the polynoms
smallSize = min(size1, size2); //value of the smaller size

// print sum of poly1+poly2
for (i = 0; i < smallSize; i++) {

	currPower1 = polynom1[i].power;
	currPower2 = polynom2[i].power;
	currCoeff1 = polynom1[i].coefficient;
	currCoeff2 = polynom2[i].coefficient;


	//count valid monoms printed
	if (currPower1 >= 0 || currPower2 >= 0 && (currCoeff1 != 0 || currCoeff2 != 0))
		flag_firstOrLast++;


	//print sum of 2 monoms (only if its valid)
	printCurrSum(currPower1, currPower2, currCoeff1, currCoeff2, i, smallSize, flag_firstOrLast);

}

// print the rest of the bigger polynom (if a bigger one exists)
if (sizeDelta > 0) {

	if (size1 > size2) {
		
		for (i = size2; i < size1; i++) {

			currPower1 = polynom1[i].power;
			currCoeff1 = polynom1[i].coefficient;

			//count valid monoms
			if (currPower1 >= 0 || currPower2 >= 0 && (currCoeff1 != 0 || currCoeff2 != 0))
				flag_firstOrLast ++;

			//printing only first monom
			currPower2 = -99;
			currCoeff2 = 0;


			//print  monom1 (only if its valid)
			printCurrSum(currPower1, currPower2, currCoeff1, currCoeff2, i, size1, flag_firstOrLast);
		}
	}
	else { // size1 < size2 because delta>0
		
		for (i = size1; i < size2; i++) {

			currPower2 = polynom2[i].power;
			currCoeff2 = polynom2[i].coefficient;

			//count valid monoms
			if (currPower1 >= 0 || currPower2 >= 0 && (currCoeff1 != 0 || currCoeff2 != 0))
				flag_firstOrLast ++;


			//printing only second monom
			currPower1 = -99;
			currCoeff1 = 0;

	
			//print monom2
			printCurrSum(currPower1, currPower2, currCoeff1, currCoeff2, i, size2, flag_firstOrLast);
		}
	}
}
}

void printCurrSum(int currPower1, int currPower2, int currCoeff1, int currCoeff2, int i, int size, int flag_firstOrLast) {

	int coeffSum;
	int validPower;

	//only print valid polynoms 
	if ((currPower1 >= 0 || currPower2 >= 0) && (currCoeff1 != 0 || currCoeff2 != 0)) {

		
		//check if powers are the same or diffrent 
		if (currPower1 == currPower2) {
			validPower = currPower1;
			coeffSum = currCoeff1 + currCoeff2;
		}
		else {
			if (currPower1 < 0) {
				//power1 isnt valid print power2 and coeff2

				coeffSum = currCoeff2;
				validPower = currPower2;
			}
			else {
				//power2 isnt valid, print power1 and coeff1

				coeffSum = currCoeff1;
				validPower = currPower1;
			}

			//print the sign 

			if (flag_firstOrLast > 1) {

				if (coeffSum < 0) {
					coeffSum = fabs(coeffSum);
					printf(" - ");
				}
				else printf(" + ");
			}
		}


		//print the coeff and power if coeff isnt 0,1 or -1
		if (coeffSum < 1 || coeffSum> 1) {
			if (validPower == 0) printf("%d^%d", coeffSum, validPower); // dont print x
			if (validPower > 1) printf("%dx^%d", coeffSum, validPower); // print x
		}

		//dont print the coeff if its 1 or -1
		else {
			if (coeffSum == 1) printf("x^%d", currPower1);
			if (coeffSum == -1) printf("x^%d", currPower1); 
		}
	}
}

void printPolyMul(Monom *polynom1, int size1, Monom* polynom2, int size2) {
	
	int i, j; //general indexs for buckets
	Monom* mulBucket;
	int bucketSize; //size of mulBucket
	int flag_firstOrLast=0; // to know what sign to print
	int currPower, currResult; // the current power and result of the multipication

	bucketSize = size1 + size2-1; // the max bucketSize needed for multipication

	//create empty multi bucket
	llocCheck(mulBucket = (Monom*)malloc(bucketSize * sizeof(Monom)));
	zeroBucketCoeff(mulBucket, 0, bucketSize); // set all new coeffs to 0
	no_power(mulBucket, 0, bucketSize); // set all new powers created to -99

	//multiply polynoms and insert into bucket
	for (i = 0; i < size1; i++) {

		for (j = 0; j < size2; j++) {

			if (polynom1[i].power != -99 && polynom2[j].power != -99) {
				currPower = polynom1[i].power + polynom2[j].power;
				currResult = polynom1[i].coefficient * polynom2[j].coefficient;
				flag_firstOrLast++;

				printCurrSum(currPower, -99, currResult, 0, i, bucketSize, flag_firstOrLast);
			}

		}
	}

	free(mulBucket);


}




//delete this when i finish
void printPolynom(Monom *polynom, int size){

	int i;
	int currCoeff, currPower;

	for (i = 0; i < size; i++) {

		currPower = polynom[i].power;
		currCoeff = polynom[i].coefficient;

		 
		printf("power is : %d, index is : %d", currPower, i);
		printf("\n");
		
		printf("coeff is : %d, index is : %d", currCoeff, i);
		printf("\n");
		
	}

	printf("END OF POLYNOM");
	printf("\n\n");
}