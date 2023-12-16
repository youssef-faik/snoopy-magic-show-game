#include <stdio.h>
#include "include/GameUI.h"


int main() {
    // Initialize game data
    GameData gameData;
    initializeGame(&gameData);

    // Display the main menu and handle user input
    int choice;
    welcomeArt();
    do {
        
        
        choice = displayMenu();
        
        validateUserChoice(choice);

        switch (choice) {
            case 1:
                displayGameRules();
                break;
            case 2:
                startGame(&gameData);
                break;
            case 3:
                loadSavedGame(&gameData);
                break;
            case 4:
                playLevelByPassword(&gameData);
                break;
            case 5:
                displayScores(&gameData);
                break;
            case 6:
                quitGame();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);

    return 0;
}
