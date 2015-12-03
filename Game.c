#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>

#define TITLE " Conway's Game of Life"
#define COMMANDS " Q - Quit | R - Read File | S - Step"
#define LIVE_CELL '*'
#define DEAD_CELL '_'
#define BOX_BOTTOM_LEFT 200
#define BOX_BOTTOM_RIGHT 188
#define BOX_TOP_LEFT 201
#define BOX_TOP_RIGHT 187
#define BOX_HORIZONTAL 205
#define BOX_VERTICAL 186
#define BOX_T_LEFT 204
#define BOX_T_RIGHT 185

#define WIDTH 75
#define HEIGHT 20
#define STEP_DELAY_MILLISECONDS 500
#define BUFFER 80

void print_top_line(void)
{
	int i;

	printf("%c", BOX_TOP_LEFT);

	for (i = 0; i < WIDTH; i++)
	{
		printf("%c", BOX_HORIZONTAL);
	}

	printf("%c\n", BOX_TOP_RIGHT);
}

void print_bottom_line(void)
{
	int i;

	printf("%c", BOX_BOTTOM_LEFT);

	for (i = 0; i < WIDTH; i++)
	{
		printf("%c", BOX_HORIZONTAL);
	}

	printf("%c\n", BOX_BOTTOM_RIGHT);
}

void print_middle_line(void)
{
	int i;

	printf("%c", BOX_T_LEFT);
	
	for (int i = 0; i < WIDTH; i++)
	{
		printf("%c", BOX_HORIZONTAL);
	}

	printf("%c\n", BOX_T_RIGHT);
}

void print_title_bar(void)
{
	printf("%c%-*s%c\n", BOX_VERTICAL, WIDTH, TITLE, BOX_VERTICAL);
}

void print_grid(bool grid[][HEIGHT])
{
	int x;
	int y;
	bool current_organism;
	char organism_character;

	for (y = 0; y < HEIGHT; y++)
	{
		printf("%c", BOX_VERTICAL);
		for (x = 0; x < WIDTH; x++)
		{
			current_organism = grid[x][y];
			organism_character = current_organism ? LIVE_CELL : DEAD_CELL;
			printf("%c", organism_character);
		}
		printf("%c\n", BOX_VERTICAL);
	}
}

void print_command_bar(void)
{
	printf("%c%-*s%c\n", BOX_VERTICAL, WIDTH, COMMANDS, BOX_VERTICAL);
}

void print_screen(bool grid[][HEIGHT])
{
	print_top_line();
	print_title_bar();
	print_middle_line();
	print_grid(grid);
	print_middle_line();
	print_command_bar();
	print_bottom_line();
}

bool is_in_bounds(int x, int y)
{
	bool inBoundsX = x >= 0 && x < WIDTH;
	bool inBoundsY = y >= 0 && y < HEIGHT;

	return inBoundsX && inBoundsY;
}

int get_neighbor_count(bool grid[][HEIGHT], int x, int y)
{
	int neighbor_count = 0;
	int dx;
	int dy;
	int grid_x;
	int grid_y;

	for (dx = -1; dx <= 1; dx++)
	{
		for (dy = -1; dy <= 1; dy++)
		{
			if (dx == 0 && dy == 0)
				continue;

			grid_x = dx + x;
			grid_y = dy + y;

			if (!is_in_bounds(grid_x, grid_y))
				continue;

			if(grid[grid_x][grid_y])
				neighbor_count += 1;
		}
	}

	return neighbor_count;
}

void simulate_step(bool grid[][HEIGHT])
{
	bool new_generation[WIDTH][HEIGHT];
	int x;
	int y;
	int neighbor_count;
	bool current_existing_organism;
	bool current_new_organism;

	for (x = 0; x < WIDTH; x++)
	{
		for (y = 0; y < HEIGHT; y++)
		{
			current_existing_organism = grid[x][y];
			neighbor_count = get_neighbor_count(grid, x, y);

			switch (neighbor_count)
			{
			case 0:
			case 1:
				current_new_organism = false;
				break;
			case 2:
				current_new_organism = current_existing_organism;
				break;
			case 3:
				current_new_organism = true;
				break;
			default:
				current_new_organism = false;
				break;
			}

			new_generation[x][y] = current_new_organism;
		}
	}

	memcpy(grid, new_generation, sizeof(new_generation));
}

void clear_screen(void)
{
	system("cls");
}

void read_grid_from_file(bool grid[][HEIGHT])
{
	bool current_cell = false;
	FILE* input_file;
	char current_line[WIDTH + 3]; // 3: null-terminating + newline characters
	char file_name[BUFFER];
	char current_char;
	int scans;
	int x;
	int y;

	// Prompt for user input
	printf("Enter input file name: ");

	do {
		scans = scanf("%s", file_name);
	} while (scans != 1);

	input_file = fopen(file_name, "r");

	if (input_file == NULL)
	{
		printf("Could not open '%s', aborting.\n", file_name);
		return;
	}

	for (y = 0; y < HEIGHT; y++)
	{
		fscanf(input_file, "%s", current_line);

		for (x = 0; x < WIDTH; x++)
		{
			current_char = current_line[x];

			switch (current_char)
			{
			case LIVE_CELL:
				current_cell = true;
				break;
			default:
			case DEAD_CELL:
				current_cell = false;
				break;
			}

			grid[x][y] = current_cell;
		}
	}

	fclose(input_file);
}

int main(void)
{
	bool grid[WIDTH][HEIGHT];
	int x;
	int y;
	char command;
	
	for (x = 0; x < WIDTH; x++)
	{
		for (y = 0; y < HEIGHT; y++)
		{
			grid[x][y] = false;
		}
	}

	while (true)
	{
		clear_screen();
		print_screen(grid);
		simulate_step(grid);

		// Decipher next command
		do {
			command = getch();
			tolower(command);

			switch (command)
			{
			case 'q':
				return EXIT_SUCCESS;
			case 'r':
				clear_screen();
				read_grid_from_file(grid);
				print_screen(grid);
				break;
			}
		} while (command != 's');
	}
}