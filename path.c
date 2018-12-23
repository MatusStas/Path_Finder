#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
this function scans file, to get information how much rows and collumns 
current grid has
*/
void size_of_grid(int *row, int *column)
{
	int sign,
		total = 0;

	FILE *fr = fopen("grid.txt","r");			//opening file for reading characters
	while((sign = getc(fr)) != EOF)				//read while not end-of-file
		(sign != '\n') ? total++ : (*row)++;	

	(*column) = total/(*row);				
	
	fclose(fr);									//always need to close file and initialize to NULL
	fr = NULL;
}
/*
Is necessary to free memory, because we work with dynamic arrays
*/
void free_memory(int *arr[],int *row, int *column)
{
	for(int y = 0; y < (*row); y++)
	{
		free(arr[y]);
		arr[y] = NULL;
	}

	free(*arr);
	*row = 0;
	*column = 0;
}
/*
this function fill characters from file to 2 dymensional dynamic array
*/
int load_matrix(int *row, int *column, int *arr[])
{
	int sign,
		y = 0,												//y behaves like row
		x = 0,												//x behaves like collumn
		point_number = 1;

	arr[y] = (int *) malloc((*column) * sizeof(int));		//allocing memory for array of pointers

	FILE *fr = fopen("grid.txt","r");						//opening file for reading characters
	while((sign = getc(fr)) != EOF)							//read while not end-of-file
		{
			if(sign != '\n')								//at this point we need to label characters by norms
			{	
				if(sign == '.')								
					(*(*(arr + y) + x++)) = point_number++;		
				else if(sign == 'S')						//if sign is S like Start label that character as 0
					(*(*(arr + y) + x++)) = 0;
				else if(sign == 'F')						//if sign is F like Finish label that character as -1
					(*(*(arr + y) + x++)) = -1;
				else if(sign == 'X')						//if sign is X like Obstacle label that character as -2
					(*(*(arr + y) + x++)) = -2;
			}
			else
				{
					arr[++y] = (int *) malloc((*column) * sizeof(int));		//allocating memory for next collumn
					x = 0;													//setting x (collumn) to 0, because new collumn starts
				}
		}

	//fclose(fr);												//always need to close file and initialize to NULL
	//fr = NULL;
}
/*
this function works with dynamic array, which holds 7 information about point
first place is point number or name of point represented by number
second and third places are point coordinations x and y
fourth - seventh are points neighbours, neighbours from North, East, South and West
*/
int point_info(int *point,int *point_number, int *row, int *column, int *arr[])	
{
	int y,												//y behaves like row
		x,												//x behaves like collumn
		current_number,
		North,
		East,
		South,
		West;

	*(point + 0) = *point_number;


	for(y = 0; y < (*row); y++)							//scanning entire 2d array to find our point_number
	{
		for(x = 0; x < (*column); x++)
		{
			current_number = *(*(arr + y) + x);
			if(*(point + 0) == current_number)			//if our point_number is found, his other information are filled
				{
					*(point + 1) = y;					//setting y and x coordinates
					*(point + 2) = x;

					//checking the borders or obstacles
					North = y-1;
					*(point + 3) = (North >= 0 && *(*(arr + North) + x) != -2)? *(*(arr + North) + x): -2;	

					East = x+1;
					*(point + 4) = (East < (*column) && *(*(arr + y) + East) != -2)? *(*(arr + y) + East): -2;

					South = y+1;
					*(point + 5) = (South < (*row) && *(*(arr + South) )+ x != -2)? *(*(arr + South) + x): -2;

					West = x-1;
					*(point + 6) = (West >= 0 && *(*(arr + y) + West) != -2)? *(*(arr + y) + West): -2;
					return 0;
				}
		}
	}
	return 0;
}
/*
this function converts characters from 2d array to new array thankfully algorithm called Breadth-first-search (bfs)
*/
int bfs(int *point, int *row, int *column, int *arr[])
{
	int total = (*row)*(*column);

	int list[total],
		index = 0,
		insert = 1;

	list[index] = 0;

	
	int matrix[total][4],
		y, x;

	for(y = 0; y<total; y++)			//initializing all character in matrix to -2 like obstacle
		for(x = 0; x<4; x++)
			matrix[y][x] = -2;

	int current_number,
		borders[total],
		i;

	for(i = 0; i<total; i++)			//initializing all character in array border to 0, like empty
		borders[i] = 0;

	int yesorno = 1;
	while(list[index] != -1)
		{
			int k,y,x,l;
			current_number = list[index];
			
			point_info(&*point, &current_number, &*row, &*column, &*arr);

			for(k = 3; k < 7; k++)
			{
				if(*(point + k) == -2 || *(point + k) == 0)
					continue;

				for(y = 0; y<total; y++)
				{
					for(x = 0; x<4;x++)
					{
						if(*(point + k) == matrix[y][x])
							yesorno = 0;
					}
				}
				if(yesorno)
				{
					matrix[current_number][(borders[current_number])++] = *(point + k);
					list[insert++] = *(point + k);
				}
				yesorno = 1;
			}
			index++;
		}

	int k,constant = 0;

	int *path = (int *) malloc((constant+1) * sizeof(int));
	int searched_number = -1;

	int ending = 0;
	while(searched_number != 0)
	{
		for(y = 0; y<total; y++)
			{
				if(matrix[y][0] == -2)
					continue;
								
				while(matrix[y][x] != -2)
					{	
						if(matrix[y][x++] == searched_number)
							{
								*(path + constant) = y;
								searched_number = y;
								constant++;
								path = (int *) realloc(path, (constant+1)*sizeof(int));
								ending = 0;
							}
						else
							{
								ending++;
								if(ending > 10000)
								{
									printf("Path doesn't exist\n\n");
									return 0;
								}
							}
					}
			x = 0;
			}
	}
	constant--;
	printf("\nShortest path is %d steps to do\n\n", constant);
	for(y = 0; y < constant; y++)
		for(x = 0; x < *row; x++)
			for(k = 0; k < *column; k++)
			{
				if((*(path + y)) == *(*(arr + x) + k))
					*(*(arr + x) + k) = -3;
			}
	return 0;
}


void path_finder(int *row, int *column, int *arr[])
{
	int current_number;
	for(int y = 0; y < *row; y++)
	{
		for(int x = 0; x < *column; x++)
		{
			current_number = *(*(arr + y) + x);
			if(current_number > 0)
				*(*(arr + y) + x) = ' ';
			else if(current_number == -2)
				*(*(arr + y) + x) = '#';
			else if(current_number == -1)
				*(*(arr + y) + x) = 'F';
			else if(current_number == -0)
				*(*(arr + y) + x) = 'S';
			else if(current_number == -3)
				*(*(arr + y) + x) = '.';
		}
	}

	for(int y = 0; y < *row; y++)					//printing grid + path
	{
		for(int x = 0; x < *column; x++)
		{
			printf("%c", *(*(arr + y )+x));
		}
		putchar('\n');
	}
}

int main()									//main program
{
	int row = 0,
		column = 0,
		size,
		point_number = 0;

	int *arr[size];
	int *point = (int *) malloc(7*sizeof(int));	
 	
	size_of_grid(&row, &column);
	load_matrix(&row, &column,&*arr);
	point_info(point, &point_number,&row, &column, &*arr);//**
	bfs(point, &row, &column, &*arr);
	path_finder(&row, &column, &*arr);
	free_memory(*&arr, &row, &column);
	return 0;	
}