#include <stdio.h>

int main(void) {

	int X[] = {1,2,3};
	int Y[] = {1,2,3};
	int Z[] = {1,2,3};

	int c = 2;

	__asm("#zanka");
	for(int i = 0; i < 3; i++) {
		Y[i] = X[i] / c;
		X[i] = X[i] + c;
		Z[i] = Y[i] + c;
		Y[i] = c - Y[i];
	}
	__asm("#konec zanka");


	printf("%d %d %d", Y[2], X[2], Z[2]);	
	return 0;
}
