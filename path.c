#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void size_of_grid(int *row, int *column)
{
	int sign,
		total = 0;

	FILE *fr = fopen("grid.txt","r");		//opening file for reading characters
	while((sign = getc(fr)) != EOF)			//read while not end-of-file
		(sign != '\n') ? total++ : (*row)++;

	(*column) = total/(*row);
	close(fr);								//always need to close file and initialize to NULL
	fr = NULL;
}


int main()									//main program
{
	char sign;
	int row = 0, column = 0;
	char *arr[row];

	while(scanf("%c",&sign) == 1)			//while we will type signs to run functions
		{
			if(sign == 'q')	
				return 0;					//quit the program
			else if(sign == 's')
			{
				size_of_grid(&row,&column);
			}
		}

}