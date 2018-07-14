#include <iostream>
// #include <omp.h>
using namespace std;
const int SIZE = 9;
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
    bool check(int i, int j) {
        for (int k = 0; k < SIZE; ++k)
            if (board[i][k] == board[i][j] && k != j) return false;
        for (int k = 0; k < SIZE; ++k)
            if (board[k][j] == board[i][j] && k != i) return false;
        for (int pi = 0; pi < 3; ++pi)
            for (int pj = 0; pj < 3; ++pj)
                if (board[pi + i / 3 * 3][pj + j / 3 * 3] == board[i][j]
                    && pi + i / 3 * 3 != i && pj + j / 3 * 3 != j)
                    return false;
        return true;
    }
};

Board* solve_sudoku(Board *board, int i, int j) {
    if (i == SIZE) return board;
    if (j == SIZE) return solve_sudoku(board, i + 1, 0);
    if (board->board[i][j]) return solve_sudoku(board, i, j + 1);
    // #pragma omp parallel for
    for (int n = 1; n <= SIZE; ++n) {
        Board *new_board = new Board(*board);
        new_board->board[i][j] = n;
        if (!new_board->check(i, j)) {
            delete new_board; new_board = nullptr;
            continue;
        }
        if (Board * b = solve_sudoku(new_board, i, j + 1)) return b;
    }
    delete board; board = nullptr;
    return nullptr;
}
bool solver(Board *board) {
    return solve_sudoku(board, 0, 0);
}
int main () {
    std::ios::sync_with_stdio(false);
	#ifdef LOCAL
	freopen("input.txt", "r", stdin);
    #endif
    Board *board = new Board();
    board->print_board();
    if (Board * b = solve_sudoku(board, 0, 0))
        b->print_board();
}