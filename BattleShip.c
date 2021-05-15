#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BOARDROWS 9
#define BOARDCOLS 11
	
bool dumpComputer = false; //causes dump of computer board before game if true (for debugging)

// boards. 0 means empty, non-zero means ship is there, number is size, -tive means hit on a ship
int playerBoard[BOARDROWS+1][BOARDCOLS+1];
int computerBoard[BOARDROWS+1][BOARDCOLS+1];
int compShotBoard[BOARDROWS+1][BOARDCOLS+1];

int getRand(int lowval,int highval){
    return (rand()%(highval+1-lowval) + lowval);
}


int getShot(bool fromUser, int board[BOARDROWS+1][BOARDCOLS+1]){
    int row,col;
    
    while(1){
        if(fromUser){
            printf("Give a shot (row, col):");
            scanf("%d%d",&row,&col);
            if(col<1 || col > BOARDCOLS || row < 1 || row > BOARDROWS)
                printf("Invalid input\n");
            else  //valid input
                break;
        }
        else { //computer generated
            row=getRand(1,BOARDROWS);
            col=getRand(1,BOARDCOLS);
            if(compShotBoard[row][col]==0) { //make sure haven't shot here before
                compShotBoard[row][col]=1; //valid shot
                break;
            }
        }
    } //will leave this loop with valid input
    if(board[row][col] != 0){
        if(board[row][col]>0)
            board[row][col]= -1*board[row][col]; //make sure is -tive
        return -1*board[row][col]; //a hit!
    }
    return 0; //miss
}
            

bool allShipsNotHit(int board[BOARDROWS+1][BOARDCOLS+1]){
    for(int i=1;i<=BOARDROWS;i++){
        for(int j=1;j<=BOARDCOLS;j++){
            if(board[i][j]>0) //ship and not hit
                return(true);
        }
    }
    return (false); //no ships found not all hit
}

bool noneLeft(int valueToFind,int board[BOARDROWS+1][BOARDCOLS+1]){
    for(int i=1;i<=BOARDROWS;i++){
        for(int j=1;j<=BOARDCOLS;j++){
            if(board[i][j]==valueToFind) //ship and not hit
                return(false);
        }
    }
    return (true); //no ships found, all hit
}


void populateBoard(bool getUserInput, int board[BOARDROWS+1][BOARDCOLS+1]){

    int userInputRow, userInputCol, userInputOrient;
	if (getUserInput) // getUserInput == true
		printf("Rows are 1 - 9, Columns are 1 - 11\nOrientation is 0 for across, 1 for down\n");

	for (int shipSize=5; shipSize>=1; shipSize--) {
		if(getUserInput){ // get input from user if this is true, else its randomized
			printf("Give starting row, starting column and orientation (3 inputs) for ship of size=%d:", shipSize);
			scanf("%d %d %d", &userInputRow, &userInputCol, &userInputOrient);
		}
		else {
			// for randomized board
			userInputRow = getRand(1,9);
			userInputCol = getRand(1,11);
			userInputOrient = getRand(0,1);
		}
		
		if (userInputOrient == 1){
			bool flag = false;
			for (int j = 0; j < shipSize; j++){
				//checks for the invalid inputs
				if ((userInputRow + shipSize - 1 ) > BOARDROWS || userInputCol > BOARDCOLS || userInputOrient < 0 || userInputOrient > 1 || userInputRow < 0 || userInputCol < 0){
					if (getUserInput)
						printf("Invalid Input\n");
					flag = true;
					shipSize++; //should repeat for the same size once again if there is an invalid input
					break;
				}
				else if (board[userInputRow+j][userInputCol] > 0){
					if (getUserInput) // should print only if the user inputs
						printf("Conflicts with ship already placed\n");
					flag = true;
					shipSize++;
					break;
				}
			}
			if (flag == false) { // performs this if and only if there is no invalid input
				for (int j = 0; j < shipSize; j++){
					board[userInputRow+j][userInputCol] = shipSize;
					
				}
			}
		}
		else{// for orientation == 0
			bool flag = false;
			for (int j = 0; j < shipSize; j++){
				
				if ((userInputCol + shipSize - 1) > BOARDCOLS || userInputRow > BOARDROWS || userInputOrient < 0 || userInputOrient > 1 || userInputRow < 0 || userInputCol < 0){
					if (getUserInput)
						printf("Invalid Input\n");
					flag = true;
					shipSize++;
					break;
				}
				else if (board[userInputRow][userInputCol+j] > 0){
					if (getUserInput)
						printf("Conflicts with ship already placed\n");
					flag = true;
					shipSize++;
					break;
				}
			}
			if (flag == false){
				for (int j = 0; j < shipSize; j++){
					board[userInputRow][userInputCol+j] = shipSize;
				}
			}
		}
	}
}

void showBoard(int board[BOARDROWS+1][BOARDCOLS+1]){
    printf("   Cols\n");
    printf("    1  2  3  4  5  6  7  8  9 10 11\n");
    printf("     ________________________________\n");
    for(int i=1;i<=BOARDROWS;i++){
        printf("R%d|",i);
        for(int j=1;j<=BOARDCOLS;j++){
            if(board[i][j]>=0)
                printf(" ");
            printf("%d ",board[i][j]);
        }
        printf("|\n");
    }
    printf("  __________________________________\n");
}

int main(int argc, char **argv)
{
    bool playerBoardOK,computerBoardOK;
    
    // initialize the seed randomly. 

     	srand(time(NULL)); 

    //init the boards
    for(int i=0;i<BOARDROWS;i++){
        for(int j=0;j<BOARDCOLS;j++){
            playerBoard[i][j]=0;
            computerBoard[i][j]=0;
            compShotBoard[i][j]=0;
        }
    }

    populateBoard(true,playerBoard);
    populateBoard(false,computerBoard);
    printf("Your board is\n");
    showBoard(playerBoard);
    if(dumpComputer){
        printf("\n\nComputer board is\n");
        showBoard(computerBoard);
    }

    // play starts here
    playerBoardOK=true;
    computerBoardOK=true;
    while(playerBoardOK && computerBoardOK){
        int hit=getShot(true,computerBoard); //shot from user
        if(hit){
            printf("HIT on size %d\n",hit);
            if(noneLeft(hit,computerBoard))
                printf("Sunk computer's %d\n",hit);
        }
        else
            printf("MISS!!\n");
        hit=getShot(false,playerBoard);
        if(hit){
            printf("Computer hits! Size=%d\n", hit);
            if(noneLeft(hit,playerBoard))
                printf("Sunk your %d\n",hit);
        }
        else
            printf("Computer miss!\n");
        playerBoardOK = allShipsNotHit(playerBoard);
        computerBoardOK = allShipsNotHit(computerBoard);
//    showBoard(playerBoard); //debug
//    showBoard(computerBoard); //debug
   }
   if(playerBoardOK)
       printf("You win!\n");
    else
        printf("You lose.\n");
    showBoard(computerBoard);

	return 0;
}

