#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#define SIZE 200

char selection;

struct gridGame {
	int **slots;
	int gridSize;
	int score;
};

struct highscoreData {
	int number;
    char id[6];
    char name[101];
    int score;
//    highscoreData *prev, *next;
}; 
//*head[SIZE], *tail[SIZE];

//highscoreData *createNode(char name[], int score) {
//	
//	highscoreData *newNode = (highscoreData *) malloc(sizeof(highscoreData));
//	
//	strcpy(newNode->name, name);
//	newNode->score;
//	
//	newNode->next = newNode->prev = NULL;
//	return newNode;
//}

//int hashFunction(char name[]) {
//	
//	return name[0] % SIZE;
//}

//void insert(char name[], int score) {
//	
//	highscoreData *newNode = createNode(name, score);
//	int hash = hashFunction(name);
//	
//	if (head[hash] == NULL) {
//		head[hash] = tail[hash] = newNode;
//	} else {
//		tail[hash]->next = newNode;
//		newNode->prev = tail[hash];
//		tail[hash] = newNode;
//	}
//}

void displayHighscore(struct highscoreData *highscore) {

    printf("|%4d | %9s | %20s | %5d |\n", highscore->number, highscore->id, highscore->name, highscore->score);
}

int compareScore(const void *low, const void *high) {
    return ((struct highscoreData*)high)->score - ((struct highscoreData*)low)->score;
}

void exit() {
    
    HWND myWindow = GetConsoleWindow();
    ShowWindow(myWindow, SW_HIDE);
}

void highScore();

void addRandomSlot(struct gridGame *grid) {
	
    int emptySlot[grid->gridSize * grid->gridSize][2];
    int empty = 0;

    for (int i = 0; i < grid->gridSize; i++) {
        for (int j = 0; j < grid->gridSize; j++) {
            if (grid->slots[i][j] == 0) {
                emptySlot[empty][0] = i;
                emptySlot[empty][1] = j;
                empty++;
            }
        }
    } if (empty > 0) {
    	
    	int k = rand() % empty;
       	int value = (rand() % 2 + 1) * 2;
       	int l = emptySlot[k][0];
       	int m = emptySlot[k][1];
       	grid->slots[l][m] = value;
    }
}

void Movement(struct gridGame *grid, char arrow) {
	
	int i, j;
    int size = grid->gridSize;
    int **slots = grid->slots;
    int totalScore = 0;

    if (arrow == 'W' || arrow == 'w') {
        for (j = 0; j < size; j++) {
            i = 1;
            while (true) {
                while (i < size && slots[i][j] == 0) { i++; }
                if (i == size) { break; }
                while (i > 0 && (slots[i - 1][j] == 0 || slots[i][j] == slots[i - 1][j])) {
                    slots[i - 1][j] += slots[i][j];
                    totalScore += slots[i - 1][j];
                    slots[i][j] = 0; i--;
                } i++;
            }
        }
    } 
	else if (arrow == 'A' || arrow == 'a') {
        for (i = 0; i < size; i++) {
            j = 1;
            while (1) {
                while (j < size && slots[i][j] == 0) { j++;}
                if (j == size) { break;}
                while (j > 0 && ( slots[i][j - 1] == 0 || slots[i][j] == slots[i][j - 1]) ) {
                    slots[i][j - 1] += slots[i][j];
                    totalScore += slots[i][j - 1];
                    slots[i][j] = 0; j--;
                } j++;
            }
        }
    } 
	else if (arrow == 'S' || arrow == 's') {
        for (j = 0; j < size; j++) {
            i = size - 2;
            while (true) {
                while (i >= 0 && slots[i][j] == 0) { i--; }
                if (i < 0) { break; }
                while (i < size - 1 && ( slots[i + 1][j] == 0 || slots[i][j] == slots[i + 1][j]) ) {
                    slots[i + 1][j] += slots[i][j];
                    totalScore += slots[i + 1][j];
                    slots[i][j] = 0; i++;
                } i--;
            }
        }
    } 
	else if (arrow == 'D' || arrow == 'd') {
        for (i = 0; i < size; i++) {
            j = size - 2;
            while (true) {
                while (j >= 0 && slots[i][j] == 0) { j--; }
                if (j < 0) { break; }
                while (j < size - 1 && ( slots[i][j + 1] == 0 || slots[i][j] == slots[i][j + 1]) ) {
                    slots[i][j + 1] += slots[i][j];
                    totalScore += slots[i][j + 1];
                    slots[i][j] = 0; j++;
                } j--;
            }
        }
    }
    grid->score += totalScore;
}

int over(const struct gridGame *grid) {
	
	int size = grid->gridSize;
	int **slots = grid->slots;
	
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			if(slots[i][j] == 0) {
				return 0;
			}
		}
	}	
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			if(j < size - 1 && slots[i][j] == slots[i][j + 1]){
				return 0;
			}
		}
		for(int j = 0; j < size; j++){
			if(i < size - 1 && slots[i][j] == slots[i + 1][j]){
				return 0;
			}
		}
    return 1;
	}
}

void startGrid(struct gridGame *grid, int gridSize) {
	
	grid->gridSize = gridSize;
	grid->score = 0;
	grid->slots = (int **)malloc(gridSize * sizeof(int *));
	
	for (int i = 0; i < gridSize; i++) {
		grid->slots[i] = (int *)malloc(gridSize * sizeof(int));
		for (int j  =  0; j < gridSize; j++) {
			grid->slots[i][j] = 0;
		}
	}
}

void insertUser(int score, const struct gridGame *grid);

void printGrid(const struct gridGame *grid) {
	
    system("cls");
    for (int i = 0; i < grid->gridSize; i++) {
        for (int j = 0; j < grid->gridSize; j++) {
            if (grid->slots[i][j] != 0) {
                printf(" %d ", grid->slots[i][j]);
            } else {
                printf("    ");
            }
        }
        printf("\n");
    }
    puts("");
    printf("Press X to quit the game.\nScore: %d\n", grid->score);
}

int startGame() {

	srand(time(NULL));
    int input;
    printf("Input Grid Size [4 or 6]: ");
    scanf("%d", &input);
    if (input != 4 && input != 6) {
        startGame();
    }
	
    struct gridGame grid;
    startGrid(&grid, input);

    addRandomSlot(&grid);

    while (true) {
        printGrid(&grid);
        if (over(&grid)) {
            printf("Press enter to continue...");
    		getchar();
			char enterKey;
   			do {
        		enterKey = getchar();
    		} while (enterKey != '\n');
    		system("cls");
    		insertUser(grid.score, &grid);
        }
    move:
        char character;
    	character = getch();
    	switch(character) {
	    	case 'w':
				Movement(&grid, character);
				addRandomSlot(&grid);
	    		break;
	    	case 'W':
				Movement(&grid, character);
				addRandomSlot(&grid);
    			break;
    		case 'a':
				Movement(&grid, character);
				addRandomSlot(&grid);
    			break;
    		case 'A':
				Movement(&grid, character);
				addRandomSlot(&grid);
	    		break;
	    	case 's':
				Movement(&grid, character);
				addRandomSlot(&grid);
	    		break;
	    	case 'S':
				Movement(&grid, character);
				addRandomSlot(&grid);
	    		break;
	    	case 'd':
				Movement(&grid, character);
				addRandomSlot(&grid);
		    	break;
    		case 'D':
				Movement(&grid, character);
				addRandomSlot(&grid);
    			break;
    		case 'x':
    			printf("Press Enter To Continue...");
    			getchar();
				char enterKey;
   				do {
        			enterKey = getchar();
    			} while (enterKey != '\n');
    			system("cls");
    			insertUser(grid.score, &grid);
    			break;
    		case 'X':
    			printf("Press Enter To Continue...");
    			getchar();
   				do {
        			enterKey = getchar();
    			} while (enterKey != '\n');
    			system("cls");
    			insertUser(grid.score, &grid);
    			break;
    		default:
    			goto move;
    			break;
    	}
	}
}

int homeMenu() {

    system("cls");
    puts("");
    printf("1. New Game\n");
    printf("2. Highscore\n");
    printf("3. Exit\n");
    printf(">> ");
    scanf("%c", &selection);

    switch(selection) {
	    case '1':
    	    startGame();
	        break;
	    case '2':
    	    highScore();
    	    break;
    	case '3':
    	    exit();
    	    break;
    	default:
        	homeMenu();
        	break;
    }
}

int main() {
    
    homeMenu();

    return 0;
}

void highScore() {
    
    system("cls");
    FILE *fptr = fopen("highscore.txt", "r");
    if (fptr == NULL) {
        printf("Error opening file.\n");
        return;
    }

    struct highscoreData highscore[100];
    int i = 0;

    char displayLine[100];
    while (fgets(displayLine, sizeof(displayLine), fptr)) {
        sscanf(displayLine, "%[^#]#%[^#]#%d", highscore[i].id, highscore[i].name, &highscore[i].score);
        highscore[i].number = i + 1; i++;
    }
    
    qsort(highscore, i, sizeof(struct highscoreData), compareScore);
    printf("| No. |    ID     |         Name         | Score |\n");
    for (int j = 0; j < i; j++) {
    	highscore[j].number = j + 1;
        displayHighscore(&highscore[j]);
    }
    puts("");
    fclose(fptr);
    system("pause");
    homeMenu();
}

void insertUser(int score, const struct gridGame *grid) {
	
	char username[16];
	char id[6];
	int randomNumbers[3];
	FILE *fptr = fopen("highscore.txt", "a+");
	
	printf("Save Data....\n");
	do {
		printf("Input Name [name length must be between 3 - 15]: ");
		scanf("%s", username);
	} while(strlen(username) <= 3 || strlen(username) >= 15);
	char capitalID[3];
	for(int i = 0; i < 2; i++){
		if(username[i] >= 'a' && username[i] <= 'z'){
			capitalID[i] = toupper(username[i]);
		}
		else{
			capitalID[i] = username[i];
		}
	}
	for (int j = 0; j < 3; j++) {
        randomNumbers[j] = rand() % (10);
    }
	
	capitalID[2] = '\0';
	sprintf(id, "%c%c%d%d%d", capitalID[0], capitalID[1], randomNumbers[0], randomNumbers[1], randomNumbers[2]);
	fprintf(fptr, "%c%c%d%d%d#%s#%d\n", capitalID[0], capitalID[1], randomNumbers[0], randomNumbers[1], randomNumbers[2], username, score);
	fclose(fptr);
	
	homeMenu();
}
