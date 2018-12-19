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
	
	fclose(fr);								//always need to close file and initialize to NULL
	fr = NULL;
}


void free_memory(int *arr[],int *row, int *column)
{
	for(int i = 0; i < (*row); i++)
	{
		free(arr[i]);
		arr[i] = NULL;
	}

	free(*arr);
	
	*row = 0;
	*column = 0;
}


int load_matrix(int *row, int *column, int *arr[])
{
	if(*arr != NULL)
	{
		free_memory(&*arr, row, column);
		return 0;
	}

	int sign,
		i = 0,
		j = 0,
		counting = 1;

	arr[i] = (int *) malloc((*column) * sizeof(int));		//allocing memory for array of pointers

	FILE *fr = fopen("grid.txt","r");
	while((sign = getc(fr)) != EOF)
		{
			if(sign != '\n')
			{	
				if(sign == '.')
					(*(*(arr + i) + j++)) = counting++;
				else if(sign == 'S')
					(*(*(arr + i) + j++)) = 0;
				else if(sign == 'F')
					(*(*(arr + i) + j++)) = -1;
				else if(sign == 'X')
					(*(*(arr + i) + j++)) = -2;
			}
			else
				{
					arr[++i] = (int *) malloc((*column) * sizeof(int));
					j = 0;
				}
		}
}


void print_matrix(int *row, int *column, int *arr[])
{
	int character;
	for(int i = 0; i < (*row); i++)
	{
		for(int j = 0; j < (*column); j++)
		{
			character = *(*(arr + i) + j);
			if(character < 10 && character > -1)
				printf("% d ", character);
			else{printf("%d ", character);}
		}
		putchar('\n');
	}
}


int find_point(int *point, int *row, int *column, int *arr[])	
{
	int i,
		j,
		character;

	scanf("%d", &point[0]);


	for(i = 0; i < (*row); i++)
	{
		for(j = 0; j < (*column); j++)
		{
			character = *(*(arr + i) + j);
			if(*(point + 0)== character)
				{
					*(point + 1) = i;	
					*(point + 2) = j;
					return 0;
				}
		}
	}
	return 0;
}

int main()									//main program
{
	char sign;
	int row = 0, column = 0;
	int size;
	int *arr[size];
	int *point = (int *) malloc(7*sizeof(int));	// first position what character
												// second position x location
												// third postion y location
												// 4 - 7 neighbours if obstacle or doesn't exist set to -2
 
	while(scanf("%c",&sign) == 1)			//while we will type signs to run functions
		{
			if(sign == 'q')	
				return 0;					//quit the program
			else if(sign == 'l')
			{
				size_of_grid(&row, &column);
				load_matrix(&row, &column,&*arr);
			}
			else if(sign == 'p')
				print_matrix(&row, &column, &*arr);
			else if(sign == 'f')
			{
				find_point(point, &row, &column, &*arr);
				//printf("%d %d %d\n", *(point + 0),*(point + 1),*(point + 2));
			}

		}
}