# memory-concentration
Memory concentration is a game built in C based on the popular card game "Concentration". 

## The Game

The game is based on the popular card game "Concentration". The game constists of two human players and a deck of cards. The deck of cards is based on a standard playing deck of cards. To begin the game, all the cards are face down represented as (X) on the terminal screen. Each player wil be prompted to choose two cards. If they are a pair, the player scores a point.

## The Deckfile

Each game uses a deckfile that contains all the cards found in a standard deck of cards. For example.
Each deckfile contains 13 characters on 4 lines

Example
```
 234567890JQKA
 AKQJ098765432
 23456789AKQJ0
 42356AKJQ0987
```
The characters in the deckfile are :
A - Ace card
K - King card
Q - Queen card
J - Jack card
2 to 10 cards

Note: the 10 card is represented as '0' in the deckfile

## Player turns

Each turn Player 0 and Player 1 will be prompted for their choice of two cards:
```
Player O: 
```
```
Player 1:
```

Players then choose the row/column of each card as follows:

```
rXcX rXcX"
```

where X represents the respective row, column on the board.

For Example: 

```
Player 0: r0c0 r2c0
```

Would refer to Player 0 has selected row 0 column 0 and row 0 column 1 to see if they are a pair. If an invalid card is chosen, the player will be reprompted.

#### Pair chosen

If a successful pair has been chosen on the board, the game board will be updated with the correctly chosen cards replaced with "-". The scores will also be showed onscreen.

Example

```
Match!
- X X X X X X X X X X X X 
X X X X X X X X X X X X X 
- X X X X X X X X X X X X 
X X X X X X X X X X X X X 
Scores:
Player 0: 1
Player 1: 0
```

#### Not a pair

If the two cards chosen are not a pair, then the chosen cards will be turned "face up" for the player to see. 
This "face up" board will appear for 3 seconds before the terminal screen is cleared and the face down board is shown.

Example
```
- X X X X X X X X X X X X 
X X X X X X X X X X X X X 
- X X X 6 X X X X X X X X 
X X X X X A X X X X X X X 
```

3 seconds later
```
- X X X X X X X X X X X X 
X X X X X X X X X X X X X 
- X X X X X X X X X X X X 
X X X X X X X X X X X X X 
Player 1 turn: 
```

## End of Game

The game finishes when all pairs of cards have been correctly found. The player with the most points is the winner.

Example
```
Winner: Player 1
```

## Invocation

The parameters to start the game are (in order)

1. The deck file
2. h for human player for player 0
3. h for human player for player 1

## Exit Codes

0 - A successful exit
1 - Incorrect number of arguments provided
2 - Deckfile cannot be found
3 - Invalid deckfile contents
4 - Incorrect player types

## Compilation

A makefile is provided. Code can be compiled according to C99 standards using the 'make' command on the terminal.
