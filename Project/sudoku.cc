#include <iostream>
using namespace std;
const int SIZE = 9;
int board[SIZE][SIZE];
void read_board() {
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            scanf("%d", &board[i][j]);
}
void print_board() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j)
            if (board[i][j]) printf("%d ", board[i][j]);
            else printf("  ");
        printf("\n");
    }
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
bool solve_sudoku(int i, int j) {
    if (i == SIZE) return true;
    if (j == SIZE) return solve_sudoku(i + 1, 0);
    if (board[i][j]) return solve_sudoku(i, j + 1);
    for (int n = 1; n <= SIZE; ++n) {
        board[i][j] = n;
        if (!check(i, j)) continue;
        if (solve_sudoku(i, j + 1)) return true;
    }
    board[i][j] = 0;
    return false;
}
bool solver() {
    return solve_sudoku(0, 0);
}
int main () {
    std::ios::sync_with_stdio(false);
	#ifdef LOCAL
	freopen("input.txt", "r", stdin);
    #endif
    read_board();
    print_board();
    if (solve_sudoku(0, 0))
        print_board();
}