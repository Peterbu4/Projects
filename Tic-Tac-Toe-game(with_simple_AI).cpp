#include <iostream>
#include <vector>
#include <limits>
#include <chrono>
#include <numeric>

using namespace std;

// Representing players and free cells (each cell is asigned to one of the players or to no one)
enum class Player { None, Human, AI };

// Class representing the board
class Board {
public:
    int size;
    int win_length;
    vector<vector<Player>> grid;

    Board(int s, int wl) : size(s), win_length(wl) {
        grid.resize(size, vector<Player>(size, Player::None));
    }

    // Displaying the board
    void print() const {
        cout << "  ";
        for (int i = 0; i < size; ++i) {
            cout << i << " ";
        }
        cout << endl;
        for (int r = 0; r < size; ++r) {
            cout << r << " ";
            for (int c = 0; c < size; ++c) {
                if (grid[r][c] == Player::Human) {
                    cout << "O ";
                }
                else if (grid[r][c] == Player::AI) {
                    cout << "X ";
                }
                else {
                    cout << ". ";
                }
            }
            cout << endl;
        }
    }

    // Checking if the move is correct and executes it
    bool make_move(int row, int col, Player p) {
        if (row >= 0 && row < size && col >= 0 && col < size && grid[row][col] == Player::None) {
            grid[row][col] = p;
            return true;
        }
        return false;
    }

    // Checking if the game is finished (win/lose)
    Player check_win() const {
        // Checking rows, columns and diagonals
        for (int r = 0; r < size; ++r) {
            for (int c = 0; c < size; ++c) {
                if (grid[r][c] != Player::None) {
                    // Checking a row
                    if (c + win_length <= size) {
                        bool win = true;
                        for (int i = 0; i < win_length; ++i) {
                            if (grid[r][c + i] != grid[r][c]) {
                                win = false;
                                break;
                            }
                        }
                        if (win) return grid[r][c];
                    }
                    // Checking a column
                    if (r + win_length <= size) {
                        bool win = true;
                        for (int i = 0; i < win_length; ++i) {
                            if (grid[r + i][c] != grid[r][c]) {
                                win = false;
                                break;
                            }
                        }
                        if (win) return grid[r][c];
                    }
                    // Checking a diagonal (up-left to down-right)
                    if (r + win_length <= size && c + win_length <= size) {
                        bool win = true;
                        for (int i = 0; i < win_length; ++i) {
                            if (grid[r + i][c + i] != grid[r][c]) {
                                win = false;
                                break;
                            }
                        }
                        if (win) return grid[r][c];
                    }
                    // Checking a diagonal (up-right to down-left)
                    if (r + win_length <= size && c - win_length + 1 >= 0) {
                        bool win = true;
                        for (int i = 0; i < win_length; ++i) {
                            if (grid[r + i][c - i] != grid[r][c]) {
                                win = false;
                                break;
                            }
                        }
                        if (win) return grid[r][c];
                    }
                }
            }
        }

        // Checking for draw
        for (int r = 0; r < size; ++r) {
            for (int c = 0; c < size; ++c) {
                if (grid[r][c] == Player::None) {
                    return Player::None; // Gra trwa
                }
            }
        }
        return Player::None; // Draw if there is no free cell and no one has won
    }

    // Generating the list of possible moves
    vector<pair<int, int>> get_available_moves() const {
        vector<pair<int, int>> moves;
        for (int r = 0; r < size; ++r) {
            for (int c = 0; c < size; ++c) {
                if (grid[r][c] == Player::None) {
                    moves.push_back({ r, c });
                }
            }
        }
        return moves;
    }
};

// Min-Max algorith with alpha-beta pruning 
int minimax(Board board, int depth, int alpha, int beta, Player current_player, Player maximizing_player, int max_depth) {
    Player winner = board.check_win();
    if (winner == maximizing_player) {
        return 100 - depth; // AI wins
    }
    else if (winner != Player::None) { // One of the players wins or draw
        return depth - 100; // Player wins
    }
    else if (board.get_available_moves().empty() || depth >= max_depth) {
        return 0; // Draw or reached maximum depth
    }

    if (current_player == maximizing_player) {
        int max_eval = numeric_limits<int>::min();
        for (const auto& move : board.get_available_moves()) {
            Board new_board = board;
            new_board.make_move(move.first, move.second, current_player);
            int eval = minimax(new_board, depth + 1, alpha, beta, (current_player == Player::Human ? Player::AI : Player::Human), maximizing_player, max_depth);
            max_eval = max(max_eval, eval);
            alpha = max(alpha, eval);
            if (eval >= beta) {
                break; // beta prunning
            }
        }
        return max_eval;
    }
    else {
        int min_eval = numeric_limits<int>::max();
        for (const auto& move : board.get_available_moves()) {
            Board new_board = board;
            new_board.make_move(move.first, move.second, current_player);
            int eval = minimax(new_board, depth + 1, alpha, beta, (current_player == Player::Human ? Player::AI : Player::Human), maximizing_player, max_depth);
            min_eval = min(min_eval, eval);
            beta = min(beta, eval);
            if (eval <= alpha) {
                break; // alpha prunning
            }
        }
        return min_eval;
    }
}

// Function used to find the best move for AI
pair<int, int> find_best_move(Board board, Player ai_player) {
    int best_eval = numeric_limits<int>::min();
    pair<int, int> best_move = { -1, -1 };
    int max_depth = 4; // Setting the maximum search depth, needs to be adjusted!

    for (const auto& move : board.get_available_moves()) {
        Board new_board = board;
        new_board.make_move(move.first, move.second, ai_player);
        int eval = minimax(new_board, 0, numeric_limits<int>::min(), numeric_limits<int>::max(), (ai_player == Player::Human ? Player::AI : Player::Human), ai_player, max_depth);

        if (eval > best_eval) {
            best_eval = eval;
            best_move = move;
        }
    }
    return best_move;
}

int main() {
    cout << "Let's start the game!" << std::endl;

    int board_size;
    do {
        cout << "Enter board size (min = 3, max = 10): ";
        cin >> board_size;
        if (board_size <= 0 || board_size > 10) {
            cout << "Incorrect size. Try again." << std::endl;
        }
    } while (board_size <= 0 || board_size > 10);

    int win_length;
    do {
        cout << "Enter the nimber of signs in a line to win (min = 3, max = 5): ";
        cin >> win_length;
        if (win_length < 3 || win_length > 5 || win_length > board_size) {
            cout << "Incorrect value. Try again." << std::endl;
        }
    } while (win_length < 3 || win_length > 5 || win_length > board_size);


    char player_choice_char;
    Player human_player_symbol;
    Player ai_player_symbol;

    do {
        cout << "Choose symbol you want to play: 'O' or 'X'.";
        cin >> player_choice_char;
        if (player_choice_char == 'O' || player_choice_char == 'o') {
            human_player_symbol = Player::Human; // Player::Human always represents 'O' on the board
            ai_player_symbol = Player::AI; // Player::AI always represents 'X' on the board
            cout << "Gracz (O) vs. AI (X)" << std::endl;
        }
        else if (player_choice_char == 'X' || player_choice_char == 'x') {
            human_player_symbol = Player::AI; // Player::AI always represents 'X' on the board although it is Human who will use that symbol
            ai_player_symbol = Player::Human; // Player::Human always represents 'O' on the board although it is AI who will use that symbol
            cout << "Gracz (X) vs. AI (O)" << std::endl;
        }
        else {
            cout << "Incorrect choice. Choose 'O' or 'X'." << std::endl;
        }
    } while (player_choice_char != 'O' && player_choice_char != 'o' && player_choice_char != 'X' && player_choice_char != 'x');

    // Setting symbols representing players dependently on user's decision
    if (player_choice_char == 'O' || player_choice_char == 'o') {// Human chooses 'O'
        human_player_symbol = Player::Human; // Human plays 'O'
        ai_player_symbol = Player::AI; // AI plays 'X'
    }
    else { // Human chooses 'X'
        human_player_symbol = Player::AI; // Human plays 'X'
        ai_player_symbol = Player::Human; // AI plays 'O'
    }


    Board game_board(board_size, win_length);
    Player current_turn = Player::Human;
    vector<double> times;

    // Main loop of the game
    while (true) {
        game_board.print();

        if (current_turn == human_player_symbol) {
            cout << "Yout turn (" << (human_player_symbol == Player::Human ? "O" : "X") << "):" << endl;
            int row, col;
            do {
                cout << "Enter the row: ";
                cin >> row;
                cout << "Enter the column: ";
                cin >> col;
                if (!game_board.make_move(row, col, human_player_symbol)) {
                    cout << "Incorrect move. Try again." << endl;
                }
                else {
                    break;
                }
            } while (true);
        }
        else {
            cout << "AI move (" << (ai_player_symbol == Player::Human ? "O" : "X") << ")..." << endl;
            auto start = chrono::high_resolution_clock::now();
            pair<int, int> ai_move = find_best_move(game_board, ai_player_symbol);
            auto stop = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> time = stop - start;
            times.push_back(time.count());
            cout << "Thinking time: " << time.count() << " ms"<<endl;
            game_board.make_move(ai_move.first, ai_move.second, ai_player_symbol);
        }

        Player winner = game_board.check_win();
        if (winner == human_player_symbol) {
            game_board.print();
            cout << "Congratulations! You won!" << endl;
            break;
        }
        else if (winner == ai_player_symbol) {
            game_board.print();
            cout << "AI won!" << endl;
            break;
        }
        else if (game_board.get_available_moves().empty()) {
            game_board.print();
            cout << "Draw!" << endl;
            break;
        }

        // Change of turn
        if (current_turn == human_player_symbol) {
            current_turn = ai_player_symbol;
        }
        else {
            current_turn = human_player_symbol;
        }
    }
    if (!times.empty())
    {
        double sum = accumulate(times.begin(), times.end(), 0.0);
        cout << "Average time of move analysis by AI: " << sum / times.size() << " ms";
    }

    return 0;
}
