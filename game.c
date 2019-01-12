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

int is_move_valid(Game* game, char* moveOne, char* moveTwo){
    if(moveOne[0]!= 'r' || moveTwo[0] != 'r'){
        return 0;
    }
    
    if(moveOne[2]!= 'c' || moveTwo[2] != 'c'){
        return 0;
    }

    if(moveOne[1]>3 || moveTwo[1]>3){
        return 0;
    }   
    
 
    return 0;
}


/*
*   Function for displaying the board and revealing the selected cards
*
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
    sleep(3);
    system("clear");
} 

void print_current_scores(Game* game){

    printf("Scores:\n");
    
    printf("Player 0: %d\n", game->playerZeroPoints);
    printf("Player 1: %d\n", game->playerOnePoints);

}

/**
*
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

    if(argc != 4){
        fprintf(stderr, "Usage: memory deckfile p1type p2type\n");
        fflush(stderr);
        exit(1);
    }
    
    if(access(argv[1], F_OK)== -1){
        fprintf(stderr, "Can't access deck file\n");
        fflush(stderr);
        exit(2);
    }
    
    if(argv[2][0]!= 'h' && argv[3][0]!='p'){
        fprintf(stderr, "Invalid player type\n");
        fflush(stderr);
        exit(4);
    }
    
    if(argv[2][1]!= '\0' && argv[2][1]!='\0'){
        fprintf(stderr, "Invalid player type\n");
        fflush(stderr);
        exit(4);  
    }
}

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
*
*
*
*/
void play_game(Game* game){
    
    while(game_not_over(game)){
        printf("Player %d turn: ", game->currentPlayer);
        char buffer[80];
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);
        char firstWord[20], secondWord[20];
        sscanf(buffer, "%s %s", firstWord, secondWord);    
        choose_card(game, firstWord, secondWord);
        game->currentPlayer++;
        game->currentPlayer%=2;

        
    }

}

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
    check_arguments(argc, argv);   
    Game* game = malloc(sizeof(Game));
    validate_deck_file(game, argc, argv);
    store_deck_file(game, argc, argv);
    make_face_up(game, argc, argv);
    initialize_game(game);

    print_face_up(game);
    
    play_game(game);   

    return 0;
}
