#include <iostream>
#include <cassert>



struct character{
    char symbol;
    int xCoordinate;
    int yCoordinate;
    int health;
    int relictsFound;
};

struct environment{
    char board[5][5];
    int gameStatus;
    int roundsPlayed;
    int relictsTotal;
};

int createRandomNumberBetween(int lowerCase, int upperCase){
    if(lowerCase > upperCase){
        return -1;
    }
    return rand() % upperCase + lowerCase;
}

void placeRelictRandomly(char board[5][5]){
    int xCoordinate = createRandomNumberBetween(0,5);
    int yCoordinate = createRandomNumberBetween(0,5);
    if(xCoordinate == 0 && yCoordinate == 0){
        placeRelictRandomly(board);
    }
    else{
        board[xCoordinate][yCoordinate] = 'R';
    }
    std::cout << "relict randomly placed" << std::endl;
}

int createStartingBoard(char board[5][5]) {
    int relictCounter = 0;
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            int randomNo = createRandomNumberBetween(0,10);
            //empty field
            if(randomNo < 4){
                board[x][y] = 'E';
            }
            //danger field
            else if(randomNo < 8){
                board[x][y] = 'D';
            }
            //well field
            else if(randomNo < 9){
                board[x][y] = 'W';
            }
            //relict field
            else{
                board[x][y] = 'R';
                relictCounter++;
            }
        }
    }
    if(board[0][0] == 'R'){
        relictCounter--;
    }
    board[0][0] = 'E';
    if(relictCounter == 0){
        placeRelictRandomly(board);
        relictCounter++;
    }
    return relictCounter;
}

bool isMovePossible(int xCoordinate, int yCoordinate, char input){
    // 0 - 4
    if(input == 'w' || input == 'W'){
        if(yCoordinate < 1){
            return false;
        }
        else return true;
    }
    if(input == 'a' || input == 'A'){
        if(xCoordinate < 1){
            return false;
        }
        else return true;
    }
    if(input == 's' || input == 'S'){
        if(yCoordinate > 3){
            return false;
        }
        else return true;
    }
    if(input == 'd' || input == 'D'){
        if(xCoordinate > 3){
            return false;
        }
        else return true;
    }
}

char receiveInput(int xCoordinate, int yCoordinate){
    char input;
    bool validInput = false;

    while(!validInput){
        std::cout << "Control your player using W - A - S - D | Where would you like to go?" << std::endl;
        std::cout << ": ";
        std::cin >> input;

        if(input == 'w' || input == 'W' || input == 'a' || input == 'A' || input == 's' || input == 'S' || input == 'd' || input == 'D'){
            if(isMovePossible(xCoordinate, yCoordinate, input)){
                validInput = true;
            }
            else{
                std::cout << "Invalid move || ";
            }
        }
        else{
            std::cout << "Invalid input || ";
        }
    }
    if(input == 'W'){
        input = 'w';
    }
    if(input == 'A'){
        input = 'a';
    }
    if(input == 'S'){
        input = 's';
    }
    if(input == 'D'){
        input = 'd';
    }

    return input;
}

char movePlayer(char board[5][5], char input, int* xCoordinate, int* yCoordinate){
    char event;
    //move up
    if(input == 'w'){
        board[*yCoordinate][*xCoordinate] = 'E';
        *yCoordinate -= 1;
        event = board[*yCoordinate][*xCoordinate];
        board[*yCoordinate][*xCoordinate] = '@';
    }

    //move left
    if(input == 'a'){
        board[*yCoordinate][*xCoordinate] = 'E';
        *xCoordinate -= 1;
        event = board[*yCoordinate][*xCoordinate];
        board[*yCoordinate][*xCoordinate] = '@';
    }

    //move down
    if(input == 's'){
        board[*yCoordinate][*xCoordinate] = 'E';
        *yCoordinate += 1;
        event = board[*yCoordinate][*xCoordinate];
        board[*yCoordinate][*xCoordinate] = '@';
    }

    //move right
    if(input == 'd'){
        board[*yCoordinate][*xCoordinate] = 'E';
        *xCoordinate += 1;
        event = board[*yCoordinate][*xCoordinate];
        board[*yCoordinate][*xCoordinate] = '@';
    }
    return event;
}

void handleEvent(char event, int* health, int relictsTotal, int* relictsFound, int* gameStatus){
    if(event == 'D'){
        if(createRandomNumberBetween(0,5) == 0){
            *health -=1;
        }
        if(*health == 0){
            *gameStatus = -1;
        }
    }
    if(event == 'W'){
        *health +=1;
    }
    if(event == 'R'){
        *relictsFound += 1;
        if(relictsTotal - *relictsFound == 0){
            *gameStatus = 1;
        }
    }
}

void printBoard(char board[5][5]){
    for(int x = 0; x < 5; x++){
        for(int y = 0; y < 5; y++){
            if(board[x][y] == 'R'){
                std::cout << "\033[33mR\033[0m" << " ";
            }
            else if(board[x][y] == '@'){
                std::cout << "\033[32m@\033[0m" << " ";
            }
            else {
                std::cout << board[x][y] << " ";
            }
        }
        std::cout << std::endl;
    }
}

void printStats(int roundNo, int health, int relictsFound, int relictsLeft){
    std::cout << "Health: "  << health << " || relicts left: " << relictsLeft << " || relicts found: " << relictsFound;
    if(roundNo){
        std::cout << " || move number " << roundNo;
    }
    std::cout << std::endl << std::endl;
}


int main() {

    //initialize random seed
    srand(time(NULL));

    //initialize variables
    struct character player = {
            .symbol = '@',
            .xCoordinate = 0,
            .yCoordinate = 0,
            .health = 5,
            .relictsFound = 0
    };

    struct environment game = {
            .gameStatus = 0,
            .roundsPlayed = 0,
            .relictsTotal = -1
    };

    //Tests
    assert("Board generation - Is relict included?" && createStartingBoard(game.board) > 0);
    assert("Testing impossible moves - UP" && isMovePossible(1, 0, 'w') == false);
    assert("Testing impossible moves - DOWN" && isMovePossible(1, 4, 's') == false);
    assert("Testing impossible moves - LEFT" && isMovePossible(0, 1, 'a') == false);
    assert("Testing impossible moves - RIGHT" && isMovePossible(4, 1, 'd') == false);
    //assert("Receiving event after move" && movePlayer(game.board, 's', &player.xCoordinate, &player.yCoordinate) == ('E' || 'D' || 'W' || 'R'));


    char input;
    char event;

    //setup game
    game.relictsTotal = createStartingBoard(game.board);
    game.board[player.yCoordinate][player.xCoordinate] = '@';
    printBoard(game.board);
    printStats(game.roundsPlayed, player.health, player.relictsFound, (game.relictsTotal - player.relictsFound));

    while (game.gameStatus == 0){
        input = receiveInput(player.xCoordinate, player.yCoordinate);
        event = movePlayer(game.board, input, &player.xCoordinate, &player.yCoordinate);
        handleEvent(event, &player.health, game.relictsTotal, &player.relictsFound, &game.gameStatus);
        game.roundsPlayed++;

        printBoard(game.board);
        printStats(game.roundsPlayed, player.health, player.relictsFound, (game.relictsTotal - player.relictsFound));
    }

    if(game.gameStatus == 1){
        std::cout << "\033[33mCongratulations, you have won!\033[0m" << std::endl;
    }

    if(game.gameStatus == -1){
        std::cout << "\033[31mYou have lost || You are a disgrace!\033[0m" << std::endl;
    }

    return 0;
}
