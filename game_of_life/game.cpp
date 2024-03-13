#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>

#include <thread>
#include <chrono>

void pretty_print(std::vector<std::vector<int>> &grid, int rows, int cols) {
    std::cout << "|";
    for (int j=0; j<cols; j++) {
        std::cout << "__";
    }
    std::cout << "_|" << std::endl;

    for (int i=0; i<rows; i++) {
        std::cout << "|";
        for (int j=0; j<cols; j++) {
            if (grid[i][j]==1) {
                std::cout << " X";
            } else {
                std::cout << "  ";
            }
        }
        std::cout << " |" << std::endl;
    }

    std::cout << "|";
    for (int j=0; j<cols; j++) {
        std::cout << "__";
    }
    std::cout << "_|" << std::endl;
}

int get_neighbors(const std::vector<std::vector<int>> &grid, int rows, int cols, int row, int col) {
    int n = 0;

    int neighbors[8][2] = {{row-1, col-1}, {row-1,col}, {row-1, col+1},
                           {row, col-1},                {row, col+1},
                           {row+1, col-1}, {row+1, col}, {row+1, col+1}};

    for (int i=0; i<8; i++) {
        int r = neighbors[i][0];
        if (r==-1) r = rows-1;
        if (r==rows) r = 0;

        int c = neighbors[i][1];
        if (c==-1) c = cols-1;
        if (c==cols) c = 0;

        if (grid[r][c]==1) {
            n++;
        }
    }

    return n;
}

bool will_be_alive(const std::vector<std::vector<int>> &grid, int rows, int cols, int row, int col) {
    int n_neighbors = get_neighbors(grid, rows, cols, row, col);

    if (grid[row][col]==1) {
        if (n_neighbors<2) return false;
        if (n_neighbors>3) return false;

        return true;
    }

    if (n_neighbors==3) {
        return true;
    }

    return false;
}

void init_generation(std::vector<std::vector<int>> &grid, int rows, int cols, float prob) {
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            if (std::rand() > prob*RAND_MAX) {
                grid[i][j] = 1;
            };
        }
    } 
}

int next_generation(std::vector<std::vector<int>> &grid, int rows, int cols) {
    std::vector<std::vector<int>> old_grid(rows, std::vector<int>(cols));
    old_grid = grid;

    int population = 0;
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            if (will_be_alive(old_grid, rows, cols, i, j)) {
                grid[i][j] = 1;
                population++;
            } else {
                grid[i][j] = 0;
            }
        }
    } 
    
    return population;
}

int main() {
    int rows = 100;
    int cols = 100;

    std::vector<std::vector<int>> grid(rows, std::vector<int>(cols));

    init_generation(grid, rows, cols, 0.84);

    pretty_print(grid, rows, cols);

    int count = 0;

    using namespace std::chrono_literals;
    while (count<10000) {
        int population = next_generation(grid, rows, cols);
        pretty_print(grid, rows, cols);
        count++;

        std::cout << "Generation : " << count;
        std::cout << " Population : " << population << std::endl;
        std::this_thread::sleep_for(50ms);
    }

    return 0;
}
