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


int point_info(int *point,int *point_number, int *row, int *column, int *arr[])	
{
	int i,
		j,
		character,
		up,
		right,
		down,
		left;

	*(point + 0) = *point_number;


	for(i = 0; i < (*row); i++)
	{
		for(j = 0; j < (*column); j++)
		{
			character = *(*(arr + i) + j);
			if(*(point + 0)== character)
				{
					*(point + 1) = i;	
					*(point + 2) = j;

					up = i-1;
					*(point + 3) = (up >= 0 && *(*(arr + up) + j) != -2)? *(*(arr + up) + j): -2;	

					right = j+1;
					*(point + 4) = (right < (*column) && *(*(arr + i) + right) != -2)? *(*(arr + i) + right): -2;

					down = i+1;
					*(point + 5) = (down < (*row) && *(*(arr + down) )+ j != -2)? *(*(arr + down) + j): -2;

					left = j-1;
					*(point + 6) = (left >= 0 && *(*(arr + i) + left) != -2)? *(*(arr + i) + left): -2;
					return 0;
				}
		}
	}
	return 0;
}


int skelet(int *point, int *row, int *column, int *arr[])
{
	int list[100000], index = 0;
	list[index] = 0;
	int insert = 1;

	int total = (*row)*(*column);
	int matrix[total][4];

	for(int i =0; i<total; i++)
		for(int j = 0; j<4;j++)
			matrix[i][j] = -2;

	int walker;
	int borders[100000] = {0};

	int yesorno = 1;
	while(list[index] != -1)
		{
			int k,i,j,l;
			walker = list[index];
			//printf("%d |", walker);
			point_info(&*point, &walker, &*row, &*column, &*arr);

			for(k = 3; k < 7; k++)
			{
				
				if(*(point + k) == -2 || *(point + k) == 0)
					continue;
				//printf("%d ",*(point + k));
				for(i = 0; i<total; i++)
				{
					for(j = 0; j<4;j++)
					{
						if(*(point + k) == matrix[i][j])
						{
							yesorno = 0;
						}
					}
				}
				if(yesorno)
				{
					matrix[walker][(borders[walker])++] = *(point + k);
					list[insert++] = *(point + k);
				}
				yesorno = 1;

			}
			//putchar('\n');
			index++;
			//*return 0;
		}

	int i,j,k,constant = 0;

	int *path = (int *) malloc((constant+1) * sizeof(int));
	int searched_number = -1;

	int ending = 0;
	while(searched_number != 0)
	{
		for(i = 0; i<total; i++)
			{
				if(matrix[i][0] == -2)
					continue;
								
				while(matrix[i][j] != -2)
					{	
						if(matrix[i][j++] == searched_number)
							{
								*(path + constant) = i;
								searched_number = i;
								constant++;
								path = (int *) realloc(path, (constant+1)*sizeof(int));
								ending = 0;
							}
						else
							{
								ending++;
								if(ending > 50000)
								{
									printf("Path doesn't exist\n\n");
									return 0;
								}
							}
					}
			j = 0;
			}
	}
	constant--;
	printf("Shortest path is to do %d steps\n\n", constant);
	for(i = 0; i < constant; i++)
		for(j = 0; j < *row; j++)
			for(k = 0; k < *column; k++)
			{
				if((*(path + i)) == *(*(arr + j) + k))
					*(*(arr + j) + k) = -8;
			}
	return 0;
}


void path_finder(int *row, int *column, int *arr[])
{
	int number;
	for(int i = 0; i < *row; i++)
	{
		for(int j = 0; j < *column; j++)
		{
			number = *(*(arr + i) + j);
			if(number > 0)
				*(*(arr + i) + j) = '.';
			else if(number == -2)
				*(*(arr + i) + j) = 'X';
			else if(number == -1)
				*(*(arr + i) + j) = 'F';
			else if(number == -0)
				*(*(arr + i) + j) = 'S';
			else if(number == -8)
				*(*(arr + i) + j) = '*';
		}
	}

	for(int i = 0; i < *row; i++)
	{
		for(int j = 0; j < *column; j++)
		{
			printf("%c", *(*(arr + i )+j));
		}
		putchar('\n');
	}
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
 	int point_number = 0;
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
			else if(sign == 'i')
				point_info(point, &point_number,&row, &column, &*arr);
			else if(sign == 'f')
			{
				skelet(point, &row, &column, &*arr);
				path_finder(&row, &column, &*arr);
				return 0;
			}
		}
}