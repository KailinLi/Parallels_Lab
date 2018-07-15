#include <iostream>
#include <omp.h>
#include <ctime>
using namespace std;
const int SIZE = 9;
clock_t begin_time;
struct Board {
    int board[SIZE][SIZE];
    Board() {
        for (int i = 0; i < SIZE; ++i)
            for (int j = 0; j < SIZE; ++j)
                scanf("%d", &board[i][j]);
    }
    Board(const Board &b) {
        for (int i = 0; i < SIZE; ++i)
            for (int j = 0; j < SIZE; ++j)
                board[i][j] = b.board[i][j];
    }
    void print_board() {
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j)
                if (board[i][j]) printf("%d ", board[i][j]);
                else printf("  ");
            printf("\n");
        }
        printf("\n");
    }
    bool check(int i, int j, int n) {
        for (int k = 0; k < SIZE; ++k)
            if (board[i][k] == n && k != j) return false;
        for (int k = 0; k < SIZE; ++k)
            if (board[k][j] == n && k != i) return false;
        for (int pi = 0; pi < 3; ++pi)
            for (int pj = 0; pj < 3; ++pj)
                if (board[pi + i / 3 * 3][pj + j / 3 * 3] == n
                    && pi + i / 3 * 3 != i && pj + j / 3 * 3 != j)
                    return false;
        return true;
    }
};

void solve_sudoku(Board *board, int i, int j) {
    if (i == SIZE) {
        board->print_board();
        clock_t end = clock();
        double timeCost = (double)(end - begin_time)/CLOCKS_PER_SEC;
        printf("time: %lf\n", timeCost);
        return;
    }
    if (j == SIZE) {
        solve_sudoku(board, i + 1, 0);
        return;
    }
    if (board->board[i][j]) {
        solve_sudoku(board, i, j + 1);
        return;
    }
    for (int n = 1; n <= SIZE; ++n) {
        if (!board->check(i, j, n)) continue;
        #pragma omp task firstprivate (n, i, j, board)
        {
            Board new_board(*board);
            new_board.board[i][j] = n;
            solve_sudoku(&new_board, i, j + 1);
        }
    }
    #pragma omp taskwait
    return;
}
void solver(Board *board) {
    #pragma omp parallel sections
    {
        solve_sudoku(board, 0, 0);
    }
    return;
}
int main () {
    std::ios::sync_with_stdio(false);
	#ifdef LOCAL
	freopen("input.txt", "r", stdin);
    #endif
    Board board;
    board.print_board();
    begin_time = clock();
    solver(&board);
    // clock_t end = clock();
    // double timeCost = (double)(end - begin)/CLOCKS_PER_SEC;
    // printf("time: %lf\n", timeCost);
}