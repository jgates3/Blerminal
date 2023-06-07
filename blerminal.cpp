#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std; 


const int maxVal = 21;
const int maxCard = 12;
const string moneyLog = "pmoney_log.txt";

int getRandomNumber(int max) {
    return rand() % max + 1;
}

int getPmoney() {
    ifstream file(moneyLog);
    if (file.is_open()) {
        int playerMoney;
        file >> playerMoney;
        file.close();
        return playerMoney;
    }
    return 100;
}

void updatePMoney(int playerMoney) {
    ofstream file(moneyLog);
    if (file.is_open()) {
        file << playerMoney;
        file.close();
    } 
    else {
        cout << "Error has occurred! Player money hasn't been saved." << endl;
    }
}

void zaHando(int playerSum, int dealerSum, int cpuSum, int playerCards[], int dealerCards[], int cpuCards[], int numPlayerCards, int numDealerCards, int numCPUCards) {
    cout << "Your hand: ";
    int i = 0;
    while(i < numPlayerCards) {
        cout << playerCards[i] << " ";
        i++;
    }
    cout << "(" << playerSum << ")" << endl;

    cout << "Dealer's hand: ";
    int j = 0;
    while (j < numDealerCards) {
        cout << dealerCards[j] << " ";
        j++;
    }
    cout << "(" << dealerSum << ")" << endl;

    cout << "CPU's hand: ";
    int k = 0;
    while (k < numCPUCards) {
        cout << cpuCards[k] << " ";
        k++;
    }
    cout << "(" << cpuSum << ")" << endl;
}


char cpuDM(int cpuSum, int difficultyPref) {
     if (difficultyPref == 1) { //easy mode
        if (cpuSum < 12) {  //aim for 12 sum 
            return 'H'; 
        }
        else {
            return 'S';
        }
    } 
    else if (difficultyPref == 2) { //normal mode
        if (cpuSum < 17){
            return 'H'; //aim for 17 sum
        }
        else {
            return 'S';
        }
    }
    else if (difficultyPref == 3) {
        if (cpuSum < 18){ //aim for 18 sum 
            return 'H'; 
        }
        else { 
            return 'S'; 
        }
    }
    return 'S';
}

void Blackjack() {
    int v1 = getRandomNumber(maxCard);
    int v2 = getRandomNumber(maxCard);
    int d1 = getRandomNumber(maxCard);
    int d2 = getRandomNumber(maxCard);
    int c1 = getRandomNumber(maxCard);
    int c2 = getRandomNumber(maxCard);
    string x;

    int playerCards[10] = {v1, v2};
    int dealerCards[10] = {d1, d2};
    int cpuCards[10] = {c1, c2};

    int startingVal = 2;
    int numPlayerCards = startingVal;
    int numDealerCards = startingVal;
    int numCPUCards = startingVal;

    int playerSum = v1 + v2;
    int dealerSum = d1 + d2;
    int cpuSum = c1 + c2;

    int playerMoney = getPmoney(); //read the player money from created file
    int betAmount = 0;
    int difficultyPref;

    cout << "Set the difficulty of the game. Type 1 for Easy, 2 for Normal, 3 for Hard" << endl;
    cin >> difficultyPref;

    cout << "You have $" << playerMoney << ". Place your bet: $";
    cin >> betAmount;

    if (betAmount <= 0) {
        cout << "You must bet a positive integer!" << endl;
        return;
    } else if (betAmount > playerMoney) {
        cout << "Insufficient funds. You cannot bet money that you don't have!" << endl;
        return;
    }

    cout << "Your hand: " << v1 << " and " << v2 << endl;
    cout << "Dealer's hand: " << d1 << ". Will you hit, stand, double down, or forfeit?" << endl;

    bool playerTurn = true; //allows for infinite player hits until they reach 21 or stand
    while (playerTurn) {
        cin >> x;

        if (x == "H" || x == "h") { //hit!
            int v3 = getRandomNumber(maxCard); //gets a random card 
            playerSum = playerSum + v3; //updates the total of the player's cards 
            playerCards[numPlayerCards++] = v3; //updates the player card list that has the player's cards
            cout<<" "<<endl;
            cout << "Your hand: ";
            int l = 0;
            while (l < numPlayerCards) {
                cout << playerCards[l] << " "; //loops to show all cards in the player card list
                l++;
            }
            cout << "(" << playerSum << ")" << endl;

            if (playerSum > maxVal) { //if the sum of the player's cards is above 21, they lose!
                cout << "You went over 21! You lose!" << endl;
                cout<<" "<<endl;
                playerMoney = playerMoney - betAmount; //update playerMoney
                zaHando(playerSum, dealerSum, cpuSum, playerCards, dealerCards, cpuCards, numPlayerCards, numDealerCards, numCPUCards);
                updatePMoney(playerMoney);
                return;
            }
        } 
        else if (x == "S" || x == "s") { //stand
            playerTurn = false; //end player turn 
        } 
        else if (x == "D" || x == "d") { //double down 
            if (numPlayerCards == 2) { //can only double down when you have two cards
                betAmount = betAmount * 2; //double set betAmount
                int v3 = getRandomNumber(maxCard);
                playerSum = playerSum + v3;
                playerCards[numPlayerCards++] = v3;
                cout<<" "<<endl;
                cout << "Your hand: ";
                int m = 0;
                while (m < numPlayerCards) {
                    cout << playerCards[m] << " ";
                    m++;
                }
                cout << "(" << playerSum << ")" << endl;

                if (playerSum > maxVal) {
                    cout<<" "<<endl;
                    cout <<" You went over 21! You lose!" << endl;
                    playerMoney = playerMoney - betAmount;
                    zaHando(playerSum, dealerSum, cpuSum, playerCards, dealerCards, cpuCards, numPlayerCards, numDealerCards, numCPUCards);
                    updatePMoney(playerMoney);
                    return;
                }
                playerTurn = false;
            } 
            else { //double down after first move
                cout << "Invalid input. You can only double down on your first move." << endl;
            }
        } 
        else if (x == "F" || x == "f") { //forfeit button
            cout << "You forfeit. You lose!" << endl;
            playerMoney = playerMoney - betAmount;
            zaHando(playerSum, dealerSum, cpuSum, playerCards, dealerCards, cpuCards, numPlayerCards, numDealerCards, numCPUCards);
            updatePMoney(playerMoney);
            return;
        } 
        else {
            cout << "Invalid input. Please enter 'H' for Hit, 'S' for Stand, 'D' for Double Down, or 'F' for Forfeit."<< endl;
        }
    }

    cout << "Dealer's hand: " << d1 << " and " << d2 << endl;

    switch (difficultyPref) { 
        case 1: //easy
            while (dealerSum < 12) { //dealer tries to get atleast 12
                int dealerCard = getRandomNumber(maxCard);
                dealerSum = dealerSum + dealerCard;
                dealerCards[numDealerCards++] = dealerCard;
                cout << "Dealer hits and draws a " << dealerCard << endl;
            }
            break;
        case 2: //normal
            while (dealerSum < 17) { //dealer tries to get atleast 17
                int dealerCard = getRandomNumber(maxCard);
                dealerSum = dealerSum + dealerCard;
                dealerCards[numDealerCards++] = dealerCard;
                cout << "Dealer hits and draws a " << dealerCard << endl;
            }
            break;
        case 3: //hard 
            while (dealerSum < 17 || (dealerSum == 17 && (rand() % 100) < 40)) {  //If the sum of the cards is 17, it has a 40% chance of drawing a card.
                int dealerCard = getRandomNumber(maxCard);
                dealerSum = dealerSum + dealerCard;
                dealerCards[numDealerCards++] = dealerCard;
                cout << "Dealer hits and draws a " << dealerCard << endl;
            }
            break;
        default:
            cout << "Dealer difficulty set to Normal." << endl;
            break;
    }
    while (cpuSum < 17) { //CPU logic
        char cpuMove = cpuDM(cpuSum, difficultyPref); //CPU gets decision making 
        if (cpuMove == 'H') { //if move is hit
            int c3 = getRandomNumber(maxCard);
            cpuSum = cpuSum + c3;
            cpuCards[numCPUCards++] = c3;
            cout << "CPU hits and draws a " << c3 << endl;
        } 
        else {
            break;
        }
    }
    zaHando(playerSum, dealerSum, cpuSum, playerCards, dealerCards, cpuCards, numPlayerCards, numDealerCards, numCPUCards);

    if (dealerSum > maxVal) {
        cout << "Dealer went over 21! You win!" << endl;
        playerMoney = playerMoney + betAmount;
    } 
    else if ((playerSum > maxVal && cpuSum > maxVal) || (playerSum == dealerSum && cpuSum == dealerSum)) {
        cout << "Wow! A three-way tie! What are the odds!" << endl;
    } 
    else if (playerSum <= maxVal && cpuSum > maxVal) {
        cout << "The CPU went over 21! You win and the CPU loses!" << endl;
        playerMoney = playerMoney + betAmount;
    } 
    else if (playerSum > maxVal && cpuSum <= maxVal) {
        cout << "You went over 21! You lose." << endl;
        playerMoney = playerMoney - betAmount;
    } 
    else if (playerSum > dealerSum && playerSum > cpuSum) {
        cout << "You win!" << endl;
        playerMoney = playerMoney + betAmount;
    } 
    else if (playerSum < dealerSum && cpuSum < dealerSum) {
        cout << "You and the CPU lose!" << endl;
        playerMoney = playerMoney - betAmount;
    } 
    else if (playerSum == dealerSum && cpuSum < dealerSum) {
        cout << "You tie with the dealer and the CPU loses!" << endl;
    } 
    else {
        cout << "You lose, but the CPU wins!" << endl;
        playerMoney = playerMoney - betAmount;
    }

        updatePMoney(playerMoney);
        cout << "You now have $" << playerMoney << endl;
    }

int main() {
    srand(static_cast<unsigned int>(time(0))); // helps generates random numbers

    cout << "Welcome to Blackjack!" << endl;

    char playAgain = 'y';
    while (playAgain == 'y' || playAgain == 'Y') {
        Blackjack();

        cout << "Would you like to play again? (y/n): ";
        cin >> playAgain;
        cout << endl;
    }
    if (playAgain == 'n' || playAgain == 'N'){
        exit(0);
    }

    cout << "Thank you for playing Blackjack!" << endl;

    return 0;
}

