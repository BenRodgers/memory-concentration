#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<strings.h>
#include<ctype.h>

/**
*   Struct for holding the Game info
*
*
*/
struct Game {
    char playerOne;
    char playerTwo;    
    char** board;
    char** faceUp;
    int playerZeroPoints;
    int playerOnePoints;    
    int currentPlayer;
};

typedef struct Game Game;

/**
*   Function for checking that a deck file is valid
*
*
*
*/
void validate_deck_file(Game* game, int argc, char** argv){
    char buffer[80];        
    FILE* fp = fopen(argv[1], "r");
    memset(buffer, 0, sizeof(buffer)); 
    
    while(fgets(buffer, sizeof(buffer), fp) != NULL){
        for(int i = 0; i < strlen(buffer); i++) {
            if(!isdigit(buffer[i]) && buffer[i] != 'K' && buffer[i] != 'A'
            && buffer[i] != 'Q' && buffer[i] != 'J' && buffer[i]!='\n'){
                fprintf(stderr, "Invalid Deck File\n");
                fflush(stderr); 
                exit(3);
            }                
        }   
    }
    fclose(fp);
}

/**
*   Make the face-up board that the player can see 
*   
*
*/
void make_face_up(Game* game, int argc, char** argv){
    char buffer[80];
    FILE* fp = fopen(argv[1], "r");
    game->faceUp = (char**)malloc(4 * sizeof(char*));
    for(int i=0; i<4; i++){
        game->faceUp[i] = (char*) malloc(13 * sizeof(char));
    }
    
    int i = 0;
    while(fgets(buffer, sizeof(buffer), fp) != NULL){
        for(int j = 0; j < 13; j++){
            game->faceUp[i][j] = 'X';
        }
        i++;                   
    } 
    fclose(fp);
}

/*
*   Function for printing the board face up
*
*
*
*/
void print_face_up(Game* game) {    
    for(int i=0; i<4; i++){
        for(int j=0; j<13; j++){
            printf("%c ", game->faceUp[i][j]);
        }
        printf("\n");
    }
}

/**
*   Function for validating the deck file
*   
*
*/
void store_deck_file(Game* game, int argc, char** argv){
    char buffer[80];
    FILE* fp = fopen(argv[1], "r");

    game->board = (char**)malloc(4 * sizeof(char*));
    
    for(int i=0; i<4; i++){
        game->board[i] = (char*) malloc(13 * sizeof(char));
    }
    
    int i = 0;
    while(fgets(buffer, sizeof(buffer), fp) != NULL){
        for(int j = 0; j < 13; j++){
            game->board[i][j] = buffer[j];
        }
        i++;                   
    } 
    fclose(fp);
}


/**
*   Function for printing the board
*
*
*/
void print_board(Game* game){
    for(int i=0; i<4; i++){
        for(int j=0; j<13; j++){
            printf("%c", game->board[i][j]);
        }
        printf("\n");
    }
}


/**
*   Function for checking if a move is valid
*
*
*/
int is_move_valid(Game* game, char* moveOne, char* moveTwo){
    int moveOneX, moveOneY, moveTwoX, moveTwoY;
    char moveOneR, moveTwoR, moveOneC, moveTwoC;

    if(sscanf(moveOne, "%c%d%c%d", &moveOneR, &moveOneY, &moveOneC, &moveOneX) != 4){
        return 0;
    }

    if(sscanf(moveTwo, "%c%d%c%d", &moveTwoR, &moveTwoY, &moveTwoC, &moveTwoX) != 4){
        return 0;
    }

    if(moveOneR != 'r' || moveOneC != 'c' || moveTwoR != 'r' || moveOneC != 'c'){
        return 0;
    }

    if(moveOneX < 0 || moveOneX > 12 || moveTwoX < 0 || moveTwoX > 12){
        return 0;
    }

    if(moveOneY < 0 || moveOneY > 3 || moveTwoY < 0 || moveTwoY > 12) {
        return 0;
    }

    return 1;
}


/*
*   Function for displaying the board and revealing the selected cards
*
*
*/
void print_temporary_board(Game* game, int moveOneX, int moveOneY, 
    int moveTwoX, int moveTwoY) {
    
    for(int i=0; i<4; i++){
        for(int j=0; j<13; j++) {
            if(i == moveOneY && j == moveOneX){
                printf("%c ", game->board[moveOneY][moveOneX]);
                continue;
            }
            if(i == moveTwoY && j == moveTwoX){
                printf("%c ", game->board[moveTwoY][moveTwoX]);
                continue;
            }
            else {
                printf("%c ", game->faceUp[i][j]);
            }
        }
        printf("\n");
    }
    //Show the board for three seconds
    sleep(3);
    //Clear the terminal screen
    system("clear");
} 


/**
*   Function for printing the current scores
*
*
*
*/
void print_current_scores(Game* game){
    printf("Scores:\n");    
    printf("Player 0: %d\n", game->playerZeroPoints);
    printf("Player 1: %d\n", game->playerOnePoints);
}

/** 
*   Function for updating the score
*
*
*/
void update_score(Game* game){
    if(game->currentPlayer == 0) {
        game->playerZeroPoints++;
    }
    else {
        game->playerOnePoints++;
    }
    
}

/**
*   Function for a player choosing a card. Reveals the cards the player has chosen
*
*
*
*/
void choose_card(Game* game, char* moveOne, char* moveTwo) {
   int moveOneY, moveOneX;
   //Get coordinates of card
   sscanf(moveOne, "r%dc%d", &moveOneY, &moveOneX);
   
   int moveTwoY, moveTwoX;
   sscanf(moveTwo, "r%dc%d", &moveTwoY, &moveTwoX);

   if(game->board[moveOneY][moveOneX] == game->board[moveTwoY][moveTwoX]){
        
        printf("MATCH!\n");
        
        game->faceUp[moveOneY][moveOneX] = '-';
        game->faceUp[moveTwoY][moveTwoX] = '-';
        print_face_up(game);        
        update_score(game);
        print_current_scores(game);
            
        
    }
    else {
        printf("NO MATCH!\n");
        print_temporary_board(game, moveOneX, moveOneY, moveTwoX, moveTwoY);
        print_face_up(game);
    }
}


/**
*   Function for checking arguments are correct for memory game
*
*
*
*/
void check_arguments(int argc, char** argv){
    //Check correct arg count
    if(argc != 4){
        fprintf(stderr, "Usage: memory deckfile p1type p2type\n");
        fflush(stderr);
        exit(1);
    }
    //Check deck file is available
    if(access(argv[1], F_OK)== -1){
        fprintf(stderr, "Can't access deck file\n");
        fflush(stderr);
        exit(2);
    }
    //Check valid player type
    if(argv[2][0]!= 'h' && argv[3][0]!='p'){
        fprintf(stderr, "Invalid player type\n");
        fflush(stderr);
        exit(4);
    }
    //Check valid player type - no extra chars
    if(argv[2][1]!= '\0' && argv[2][1]!='\0'){
        fprintf(stderr, "Invalid player type\n");
        fflush(stderr);
        exit(4);  
    }
}

/**
*   Function for checking if the game is over
*
*
*
*/
int game_not_over(Game* game){
    for(int i=0; i<4; i++){
        for(int j=0; j<12; j++){
            if(game->faceUp[i][j] == 'X'){
                return 1;
            }
        }
    }
    
    return 0;
}

/**
*   Function for the main game loop
*
*
*/
void play_game(Game* game){
    
    while(game_not_over(game)){
        //Prompt player
        printf("Player %d turn: ", game->currentPlayer);
        char buffer[80];
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);
        char firstMove[20], secondMove[20];
        memset(firstMove, 0, sizeof(firstMove));
        memset(secondMove, 0, sizeof(secondMove));
        

        if(sscanf(buffer, "%s %s", firstMove, secondMove) != 2){
            continue;
        }    
        
        if(!is_move_valid(game, firstMove, secondMove)){
            continue;
        }
        else {
            choose_card(game, firstMove, secondMove);
            game->currentPlayer++;
            game->currentPlayer%=2;
        }
    }

}

/**
*   Function for initializing the game variables to zero
*
*
*
*/
void initialize_game(Game* game){
    game->currentPlayer = 0;
    game->playerZeroPoints = 0;
    game->playerOnePoints = 0;
}

/**
*   Main function for memory game
*
*
*/
int main(int argc, char** argv){
    //Validate game arguments
    check_arguments(argc, argv);
    //Make game    
    Game* game = malloc(sizeof(Game));
    //Check deck file
    validate_deck_file(game, argc, argv);
    //Save deck file into memory
    store_deck_file(game, argc, argv);
    //Make cards visible to players
    make_face_up(game, argc, argv);
    //Setup game variables 
    initialize_game(game);
    //Print cards for displaying to player
    print_face_up(game);
    //Play game
    play_game(game);   

    return 0;
}
