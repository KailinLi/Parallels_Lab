#include <iostream>
#include <pthread.h>
#include <ctime>
#include <vector>
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
struct Data {
    Board *board;
    int i;
    int j;
    Data(Board *b, int i, int j): board(b), i(i), j(j) {}
};
void* solve_sudoku(void *data) {
    Board *board = ((Data*)data)->board;
    int i = ((Data*)data)->i;
    int j = ((Data*)data)->j;
    // cout << i << "*" << j << endl;
    if (i == SIZE) {
        board->print_board();
        clock_t end = clock();
        double timeCost = (double)(end - begin_time)/CLOCKS_PER_SEC;
        printf("time: %lf\n", timeCost);
        return (void *)0;
    }
    if (j == SIZE) {
        Data d(board, i + 1, 0);
        solve_sudoku(&d);
        return (void *)0;
    }
    if (board->board[i][j]) {
        Data d(board, i, j + 1);
        solve_sudoku(&d);
        return (void *)0;
    }
    vector<pthread_t> tid;
    for (int n = 1; n <= SIZE; ++n) {
        if (!board->check(i, j, n)) continue;
        {
            tid.push_back(0);
            Board new_board(*board);
            new_board.board[i][j] = n;
            Data d(&new_board, i, j + 1);
            pthread_create(&tid[tid.size() - 1], NULL, solve_sudoku, (void *)&d);   
        }
    }
    void *tret;
    for (int i = 0; i < tid.size(); ++i)
        pthread_join(tid[i], &tret);
    return (void *)0;
}
void solver(Board *board) {
    Data d(board, 0, 0);
    solve_sudoku(&d);
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