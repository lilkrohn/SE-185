#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <ncurses.h>
#define MAXWORDS 100
#define WORDLEN 11
#define DEBUG 0   // set to 0 to disable debug output
#define ARROW '<'
#define SPACE ' '
//gcc lab10 -o lab10.c -lncurses
//./ds4rd.exe -d 054c:09cc -D DS4_BT -t -b  -j -bt | ./lab10 wordslist.txt

// reads words from the file
// into wl and trims the whitespace off of the end
// DO NOT MODIFY THIS FUNCTION
int readWords(char* wl[MAXWORDS], char* filename);

//modifies s to trim white space off the right side
// DO NOT MODIFY THIS FUNCTION
void trimws(char* s);

void draw_character(int x, int y, char use);

void generate_list(char* wordlist[], int wordCount);

int main(int argc, char* argv[]) {
	char* wordlist[MAXWORDS];
	int wordCount;
	int sentence[1000];
	int i = 0;
	int row = 0;
	int column = 1;
	int index = 0;
	int length = 0;
	int line = 20;
	int time, j1, j2, j3, j4;
	int rightclick,leftclick,option,share,rTwo, lTwo,rOne, lOne; 
	int Button_T, Button_X, Button_S, Button_C;
	int moveTime = 0;
	wordCount = readWords(wordlist, argv[1]);
	
	if (DEBUG) {
		printf("Read %d words from %s \n",wordCount, argv[1]);
		for (i = 0; i< wordCount; i++) {
			printf("%s,", wordlist[i]);

			}
		printf("\n");
		}
	
	int rowCount = wordCount / 5;
	int timeOld = 0;
	
	if(wordCount%rowCount != 0 ){
		rowCount ++;
	}

	initscr();
	refresh();
	noecho();
	
//build word table
	generate_list(wordlist, wordCount);
	

	
	do {
		scanf("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d,", &time, &Button_T, &Button_C, &Button_X, &Button_S, &rightclick, &leftclick, &option, &share, &rTwo, &lTwo, &rOne, &lOne, &j1, &j2, &j3, &j4);

		//this increments
		if (time > moveTime + 200) {
			
		
			//pointer wont go past X rows and joystick is down
			if (row > 0 && j2 < -100){ // -+++
				draw_character(column * 15, row, SPACE);
				row--;
				draw_character(column * 15, row, ARROW);
			
			}
			
			//point wont go less then 0 rows and joystick is up
			if (row < rowCount  && j2 > 100){ 
				draw_character(column * 15, row, SPACE);
				row++;
				draw_character(column * 15, row, ARROW);
				
			}
			
			//no more then 5 colums and joystick is right
			if (column < 5 && j1 > 100) { 
				draw_character(column * 15, row, SPACE);
				column++;
				draw_character(column * 15, row, ARROW);
				
			}
			
			//not less then 1 column and joystick is left
			if (column > 1 && j1 < -100){ 
				draw_character(column * 15, row, SPACE);
				column--;
				draw_character(column * 15, row, ARROW);
				
			}
		
		

		
			//adds word with space
			if (Button_T == 1){
				index = ((column - 1) * 16) + row;
				sentence[i] = (strlen(wordlist[index]) + 1);
				mvprintw(line, length, " %s", wordlist[index]);
				length += sentence[i];
				i++;

			}

			//removes the last word/punctuation
			if (Button_X == 1) {
				int y, z, sentence_sum;
				sentence_sum = length - sentence[i-1];
				for (z = length; z >= sentence_sum; z--) {
					mvprintw(line, z, " ");
				
				}
				length = sentence_sum;
				i--;
			}

			//adds word without a space
			if (Button_S == 1) {
				index = ((column - 1) * 16) + row;
				sentence[i] = strlen(wordlist[index]);
				mvprintw(line, length, "%s", wordlist[index]);
				length += sentence[i];
				i++;
			}

			//capitalizes the first letter of the next word appended
			if (Button_C == 1) {
				index = ((column - 1) * 16) + row;
				int f;
				int word_length = strlen(wordlist[index]) + 1;
				sentence[i] = strlen(wordlist[index]) + 1;
				for (f = 0; f < word_length; f++) {
					if (islower(wordlist[index][f]) && f == 0) {
						wordlist[index][f] = toupper(wordlist[index][f]);
						mvprintw(line, length, " %c", wordlist[index][f]);
						//mvprintw(rowCount+1, 0,"    ");
						length += 2;
						refresh();
						
					}
					else {
						mvprintw(line, length, "%c", wordlist[index][f]);
						//mvprintw(rowCount+1, 0,"CAP");
						length += 1;
						i++;
						refresh();
					}
				}
			}
			
			//pressing right joystick goes to next line
			if (leftclick == 1) {
				line = line +1; 
			}
			// pressing left clears 
			if (rightclick == 1) {
				int x;
				for (x = 0; x < MAXWORDS; x++) {
					mvprintw(line, x, " ");
				}
				length = 0;
			}
			
			timeOld = time;
			refresh();
			moveTime = time;
		}
		
	}
	while(1);
	endwin();


    return 0;
	}

void draw_character(int x, int y, char use) {
	mvaddch(y,x,use);
	refresh();
}

void generate_list(char* wordlist[], int wordCount) {
	  int row, column, i = 0;

	  for (column = 0; column < 5; column++) {
		  for (row = 0; row < 16; row++) {
			  mvprintw(row, column * 15, "%15s", wordlist[i]);
		  refresh();
			  i++;
		  if (i > wordCount - 1) {
				  break;
			  }
		 }
	 }

}

//The use of the defined arrow character at the top of this program is reponsible for indicting the selected word

// DO NOT MODIFY THIS FUNCTION
void trimws(char* s) {
        int len = strlen(s) ;
        int x;
        if (len ==0) return;
        x = len-1;
        while (isspace(s[x]) && (x>=0)) {
                s[x] = '\0';
                x--;
                }
        }


// DO NOT MODIFY THIS FUNCTION
int readWords(char* wl[MAXWORDS], char* filename) {
	int numread =0;
	char line[WORDLEN];
	char *p;
	FILE* fp = fopen(filename,"r");
	while (!feof(fp)) {
		p  =fgets(line, WORDLEN, fp);
		if (!feof(fp) && p !=NULL) {
			trimws(line);
			wl[numread] = (char *)  malloc(strlen(line)+1);
			strcpy(wl[numread], line);
			numread++;
			}
		}
	fclose(fp);
	return numread;
	}