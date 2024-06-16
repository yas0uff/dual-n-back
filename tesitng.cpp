#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

class GameBoard {
public:
    GameBoard();
    void updateBoard(int position);
    void draw(int currentScore);

private:
    vector<string> board;
};

GameBoard::GameBoard() {
    board = vector<string>(9, " ");
}

void GameBoard::updateBoard(int position) {
    for (int i = 0; i < 9; ++i) {
        board[i] = (i == position) ? "X" : " ";
    }
}

void GameBoard::draw(int currentScore) {
    // Clear the console before drawing
    cout << "\033[2J\033[1;1H";

    cout << "     |     |      \n";
    cout << "  " << board[0] << "  |  " << board[1] << "  |  " << board[2] << "\n";
    cout << "_____|_____|_____ \n";
    cout << "     |     |      \n";
    cout << "  " << board[3] << "  |  " << board[4] << "  |  " << board[5] << "\n";
    cout << "_____|_____|_____ \n";
    cout << "     |     |      \n";
    cout << "  " << board[6] << "  |  " << board[7] << "  |  " << board[8] << "\n";
    cout << "     |     |      \n";
    cout << "\n";
    cout << "Current score: " << currentScore << endl;
}

class StimulusManager {
public:
    StimulusManager(int nBackLevel);
    int getRandomPosition();
    bool checkMatch(int currentStimulus);

    vector<int> positions;

private:
    int nBackLevel;
};

StimulusManager::StimulusManager(int nBackLevel) : nBackLevel(nBackLevel) {
    srand(static_cast<unsigned>(time(0))); // Seed for random number generation
}

int StimulusManager::getRandomPosition() {
    return rand() % 9;
}

bool StimulusManager::checkMatch(int currentStimulus) {
    if (positions.size() >= nBackLevel) {
        return positions[positions.size() - nBackLevel] == currentStimulus;
    }
    return false;
}

class Player {
public:
    Player();
    void updateScore();
    void showFinalScore(int totalRounds);
    int getScore();

private:
    int score;
};

Player::Player() : score(0) {}

void Player::updateScore() {
    score++;
}

void Player::showFinalScore(int totalRounds) {
    cout << "Game Over." << endl;
    cout << "Your overall score is: " << score << " out of " << totalRounds << endl;
}

int Player::getScore() {
    return score;
}

int getNBackLevelFromUser() {
    int n;
    cout << "Enter the level of back (1 or 2): ";
    cin >> n;
    return n;
}

bool isTimeUp(clock_t startTime, int timeLimitSeconds) {
    clock_t currentTime = clock();
    double elapsedSeconds = static_cast<double>(currentTime - startTime) / CLOCKS_PER_SEC;
    return (elapsedSeconds >= timeLimitSeconds);
}

int main() {
    int nBackLevel = getNBackLevelFromUser();
    if (nBackLevel != 1 && nBackLevel != 2) {
        cout << "Invalid input! Please enter either 1 or 2." << endl;
        return getNBackLevelFromUser();
    }

    const int TOTAL_ROUNDS = 10;
    const int ROUND_TIME_LIMIT_SECONDS = 5;

    GameBoard board;
    StimulusManager stimulusManager(nBackLevel);
    Player player;

    for (int round = 0; round < TOTAL_ROUNDS; ++round) {
        int randomPosition = stimulusManager.getRandomPosition();
        board.updateBoard(randomPosition);
        board.draw(player.getScore()); // Pass current score to draw function

        // Prompt for user input with time limit
        cout << "Does the position of X match the previous position? (y/n): ";

        clock_t startTime = clock();
        char userInput;
        cin >> userInput;

        // Check if time limit exceeded
        if (isTimeUp(startTime, ROUND_TIME_LIMIT_SECONDS)) {
            cout << "You have exceeded the given time span." << endl;
            break;
        }

        bool isMatch = stimulusManager.checkMatch(randomPosition);
        if ((userInput == 'y' && isMatch) || (userInput == 'n' && !isMatch)) {
            player.updateScore();
        }

        if (stimulusManager.positions.size() >= nBackLevel) {
           stimulusManager.positions.erase(stimulusManager.positions.begin());
        }
        stimulusManager.positions.push_back(randomPosition);
    }

    player.showFinalScore(TOTAL_ROUNDS);
    return 0;
}
