//
// Created by Abderrahim on 12/16/2023.
//
#include"GameUI.h"
#include<stdio.h>

// #include<windows.h>
// clear screan 
void clrscr()
	{
		system("cls");
		return;
	}

char waitForAnyKey(void)
{
	int pressed;
	while(!kbhit());
	pressed = getch();
	//pressed = tolower(pressed);
	return((char)pressed);
}
// representing the horizontal and vertical coordinates move the cursur
void gotoxy(int x,int y)
	{
		printf("%c[%d;%df",0x1B,y,x);
	}

// return the option in the menu 
int menuSelector(int x, int y, int yStart)
{
	char key;
	int i = 0;
	x = x - 2;
	gotoxy(x,yStart);
	
	printf(">");
	
	gotoxy(1,1);
	do
	{
		key = waitForAnyKey();
		//printf("%c %d", key, (int)key);
		if ( key == (char)UP_ARROW )
		{
			gotoxy(x,yStart+i);
			printf(" ");
			
			if (yStart >= yStart+i ) 
				i = y - yStart - 2;
			else
				i--;
			gotoxy(x,yStart+i);
			printf(">");
		}
		else
			if ( key == (char)DOWN_ARROW )
			{

				gotoxy(x,yStart+i);
				printf(" ");
				
				if (i+2 >= y - yStart ) 
					i = 0;
				else
					i++;
				gotoxy(x,yStart+i);
				printf(">");				
			}	
			//gotoxy(1,1);
			//printf("%d", key);
	} while(key != (char)ENTER_KEY); 
	return(i);
}




void welcomeArt(void)
{
	clrscr(); //clear the console
	printf("\n");	
	printf("           _____ _____ _____ _____ ____ __   __    \n");
    printf("           |  ___|   | |  _  |  _  | __ |\\   //    \n");
    printf("           |___  | | | | |_| | |_| |  __| \\ //     \n");
    printf("      -----|_____|_|___|_____|_____|_|-----|_|------\n");
    printf("      ----------------------------------------------\n");
	printf("\t		Welcome To The Snoopy Game!			\n");
	printf("\t			    Press Any Key To Continue...	\n");			
	printf("\n");
	waitForAnyKey();
	return ;
}


int displayMenu(){

	int x = 10, y = 5;
	int yStart = y;
	int selected;
	
	clrscr(); //clear the console
	gotoxy(x,y++);
	printf("New Game\n");
	gotoxy(x,y++);
	printf("High Scores\n");
	gotoxy(x,y++);

	printf("Rules\n");
	gotoxy(x,y++);
	printf("Exit\n");
	gotoxy(x,y++);

	selected = menuSelector(x, y, yStart);
	return(selected);
}

int main()
{
	welcomeArt();
	int choice=displayMenu();
	return 1;
}
