#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define ARR_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/**
 * prints out usage info
 * 
 * @param exec_name name of the executable, usually tictactoe
 */
void help(char *exec_name) {
    printf("Usage: %s\n", exec_name);
    puts("Players take turns trying to get three in a row. Player 1, who goes first");
    puts("is X, and Player 2 is O. Players first enter coordinates separated by commas,");
    puts("with the row first and the column second (e.g. \"1, 1\")");

    exit(EXIT_SUCCESS);
}

/**
 * \brief prints the current state of the board
 */
void print_board(char board[3][3]) {
    printf("%c|%c|%c\n", board[0][0], board[0][1], board[0][2]);
    for (int i = 1; i < 3; i++) {
        puts("-+-+-");
        printf("%c|%c|%c\n", board[i][0], board[i][1], board[i][2]);
    }
}

/**
 * @param s string to read player turn data into
 * @param n size of string s
 * @param was_invalid specifies whether the last time read_player_input was called,
 * s was an invalid player input
 */
void read_player_input(char *s, size_t n, bool was_invalid) {
    if (was_invalid) {
        puts("The coordinates you entered were invalid. Please try again.");
    } else {
        puts("Please enter the coordinates for your turn: ");
    }

    fgets(s, n, stdin);
    s[strcspn(s, "\r\n")] = '\0';
}


/**
   @param s input string of player
   @out x x-coordinate of position that player entered
   @out y y-coordinate of position that player entered
   @ret whether the position entered was valid or not
*/
bool parse_player_input(char *s, int *y, int *x) {
    char delims[] = ", ";

    char *endptr = NULL;
    char *pch = strtok(s, delims);
    if (tolower(*pch) == 'q') {
        goto QUIT;
    }

    *y = strtol(pch, &endptr, 10);

    if (*endptr != '\0' || *y < 0 || *y >= 3) {
        goto INVALID;
    }

    pch = strtok(NULL, delims);
    if (tolower(*pch) == 'q') {
        goto QUIT;
    }

    *x = strtol(pch, &endptr, 10);

    if (*endptr != '\0' || *x < 0 || *x >= 3) {
        goto INVALID;
    }

    return true;

 QUIT:
    puts("Player has decided to quit.");
    exit(EXIT_FAILURE);

 INVALID:
    *y = -1;
    *x = -1;

    return false;
}

/**
 * @param board current board state
 * @param y y-coordinate of player input
 * @param x x-coordinate of player input
 * @ret whether the square specified by y and x is taken or not.
*/
bool square_taken(char board[3][3], int y, int x) {
    return board[y][x] != ' ';
}

/**
 * @param board current board state
 * @ret whether the current board has a winner
 */
bool has_won(char board[3][3]) {
    static const int winStates[][3][2] = {
        // wins across rows
        { { 0, 0 }, { 0, 1 }, { 0, 2 } },
        { { 1, 0 }, { 1, 1 }, { 1, 2 } },
        { { 2, 0 }, { 2, 1 }, { 2, 2 } },
        // wins across columns
        { { 0, 0 }, { 1, 0 }, { 2, 0 } },
        { { 0, 1 }, { 1, 1 }, { 2, 1 } },
        { { 0, 2 }, { 1, 2 }, { 2, 2 } },
        // wins across diagonals
        { { 0, 0 }, { 1, 1 }, { 2, 2 } },
        { { 0, 2 }, { 1, 1 }, { 2, 0 } }
    };

    for (int i = 0; i < ARR_SIZE(winStates); i++) {
        char player = board[winStates[i][0][0]][winStates[i][0][1]];

        if (player == ' ') {
            continue;
        }

        if (
        player == board[winStates[i][1][0]][winStates[i][1][1]] &&
        player == board[winStates[i][2][0]][winStates[i][2][1]]
        ) {
            return true;
        }
    }

    return false;
}


int main(int argc, char *argv[]) {
    if (argc >= 2) {
        help(argv[0]);
    }

    char board[3][3];
    char buf[8];

    int i;
    for (i = 0; i < 3; i++) {
        memset(board[i], ' ', 3);
    }

    char playerTurn = 'X';
    int y = -1, x = -1;
    i = 1;
    bool hw = false;

    do {
        printf("Turn %d\n", i);
        puts("======");
        print_board(board);

        printf("It is now player %c's turn\n", playerTurn);

        read_player_input(buf, ARR_SIZE(buf), false);

        while (!parse_player_input(buf, &y, &x) || square_taken(board, y, x)) {
            read_player_input(buf, ARR_SIZE(buf), true);
        }

        putchar('\n');

        board[y][x] = playerTurn;

        hw = has_won(board);
        i++;
        playerTurn ^= 'X' ^ 'O';

    } while (!hw && i <= 9);

    if (hw) {
        printf("%c has won!\n", playerTurn);
    } else {
        puts("It was a draw.");
    }

    return EXIT_SUCCESS;
}

