#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <string.h>

char sodoku[9][9],temp_sodoku[9][9],color_state[9][9];
char selected_i,selected_j,op_select,flag_wrong_input,flag_start,flag_collision,max_hint,current_hint,flag_hint,flag_answer,num_players=3;
int temp_score;

clock_t start;

typedef struct player
{
    char name[30];
    int score;

} cr_player;

cr_player players[10]= {{"Omar",48360},{"Ibrahim",36150},{"Mohamed",203145}};

enum difficulity {beginner=18,easy=24,medium=36,hard=48,expert=56} level;

void play();
void print_logo();
void main_scn();
void print_sodoku();
void setColor(int Color_index);
void cr_color_state();
void cr_sodoku();
void create_sodoku();
void cr_temp_sodoku();
char ch_collision(char row,char column,char value);
void swap_in_row(char row,char column_1,char column_2);
char ch_column_collision(char row,char column,char value);
void game_init();
char check_solution();
void get_first_place();
void go_right();
void go_left();
void go_up();
void go_down();
char ch_repeated(char r,char c,char value);
void diff_level_scn();
void clear();
void hint();
void cong();
void calc_score(int);
void get_name();
void score();
char calc_digit(int n);
void sort_name(char order);
void sort_score(char order);

int main()
{
    cr_player * players=0;
    while(1)
    {
        Sleep(100);
        main_scn();
    }


    return 0;
}


char ch_collision(char r,char c,char temp)
{
    for(char row =0; row <=r; row++)
        if(sodoku[row][c]==temp)
        {
            return 1;
        }
    for(char col =0; col <=c; col++)
        if(sodoku[r][col]==temp)
        {
            return 1;
        }
    return 0;
}

void cr_temp_sodoku()
{
    char temp_r,temp_c;
    for (char r=0; r<9; r++)
        for(char c=0; c<9; c++)
            temp_sodoku[r][c]=sodoku[r][c];


    for (char count=0; count<level; count++)
    {
        do
        {
            temp_r=rand()%10;
            temp_c=rand()%10;

        }
        while(temp_sodoku[temp_r][temp_c]==0);
        temp_sodoku[temp_r][temp_c]=0;
    }

}
void cr_color_state()
{
    for (char r=0; r<9; r++)
        for(char c=0; c<9; c++)
            color_state[r][c]=temp_sodoku[r][c];

}


void setColor(int ForgC)
{
    WORD wColor;// We will need this handle to get the current background attribute
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        // Mask out all but the background attribute,and add in the foreground color
        wColor=(csbi.wAttributes&0xF0)+(ForgC&0x0F);
        SetConsoleTextAttribute(hStdOut,wColor);
    }
}


void print_sodoku()
{

    int temp;
    setColor(1);

    printf("\t\t ");
    for(char i=0; i<29; i++)
        printf("_");

    for (char i=0; i<9; i++)
    {
        printf("\n\t\t\t\t\t\t|");
        for(char j=0; j<9; j++)
        {
            if(flag_answer && (((temp_sodoku[i][j])!=sodoku[i][j])) && !color_state[i][j])
                setColor(2);

            else if (color_state[i][j] && color_state[i][j]<10)
                setColor(1);
            else if(color_state[i][j]==10)//for hint
                setColor(2);
            else
                setColor(6);
            if(i==selected_i && j==selected_j && flag_start &&!flag_answer)
            {
                setColor(13);
                printf(" # ");
                flag_start =0;
            }
            else if(i==selected_i && j==selected_j && !flag_answer)
            {
                setColor(13);
                if(temp_sodoku[i][j])
                    printf(" %d ",temp_sodoku[i][j]);
                else
                    printf(" # ");

            }
            else if(!temp_sodoku[i][j] && !flag_answer)
                printf("   ");
            else
            {
                if(!flag_answer)
                    printf(" %d ",temp_sodoku[i][j]);
                else
                    printf(" %d ",sodoku[i][j]);
            }
            setColor(1);
            if((j+1)%3==0)
                printf("|");
        }

        if((i+1)%3==0)
        {
            printf("\n\t\t\t\t\t\t|");
            for(char i=0; i<30; i++)
                if((i+1)%10==0)
                    printf("|");
                else
                    printf("_");
        }
    }
    printf("\n\t\t");
    for(char i=0; i<91; i++)
        printf("_");
    printf("\n\t\t\t\t\t\t");
    if(op_select==1)
        setColor(13);
    else
        setColor(1);
    printf("Hint    ");
    if(op_select==2)
        setColor(13);
    else
        setColor(1);
    printf("Answer    ");
    if(op_select==3)
        setColor(13);
    else
        setColor(1);
    printf("Clear    ");
    if(op_select==4)
        setColor(13);
    else
        setColor(1);
    printf("Exit    ");
    if(flag_wrong_input)
    {
        setColor(4);
        printf("\n\n Use arrow keys and enter to select item.");
        printf("\n Use Backspace to delete item.");
        printf("\n Enter numbers between 1 and 9 only.\n");
        flag_wrong_input=0;

    }
    if(flag_collision && !op_select )
    {
        setColor(4);
        printf("\n\n This number is repeated in the row or column.");
        printf("\n Think in another number...");
        flag_collision=0;
    }
    if(flag_hint)
    {
        setColor(1);
        if(current_hint<max_hint)
            printf("\n\n You have %d available hints till now.",max_hint-current_hint);
        else if(current_hint>=max_hint)
        {
            setColor(4);
            printf("\n\n You have already used all available hints in this level");
        }
        flag_hint=0;

    }
}


void create_sodoku()
{
    char i=0,flag=0,temp=0;
    char over_flow_flag=0;

    char temp_arr[10]= {1};
    char temp_arr_2[10]= {1};

    do
    {
        for (char r=0; r<9; r++)
            for(char c=0; c<9; c++)
                sodoku[r][c]=0;
        for(char r=0; r<9; r++)
        {
            for(char c=0; c<9; c++)
            {
                over_flow_flag=0;
                do
                {
                    temp=(rand()%9)+1;
                    over_flow_flag++;
                    if (over_flow_flag==20)
                        break;
                }
                while(ch_collision(r,c,temp));
                if (over_flow_flag ==20)
                {
                    for(char k=1;k<10;k++)
                    {
                        temp_arr[k]= 0;
                        temp_arr_2[k]= 0;
                    }
                    for(char i=0; i<9; i++)
                        temp_arr[sodoku[r][i]]++;

                    i=1;
                    do
                    {
                        for(; i<10; i++)
                            if((temp_arr[i]==0)&&(temp_arr_2[i]==0))
                            {
                                sodoku[r][c]=i;
                                break;
                            }


                        flag =0;
                        for(char m=0; m<c; m++)
                        {
                            flag =0;
                            if(!ch_column_collision(r,c,sodoku[r][m]) && !ch_column_collision(r,m,sodoku[r][c]))
                            {
                                swap_in_row(r,m,c);
                                flag=1;
                                break;
                            }

                        }

                    }
                    while(!flag && (i++<10));
                }
                else
                    sodoku[r][c]=temp;
            }
        }
    }
    while(check_solution());

}

void swap_in_row(char r,char c1,char c2)
{
    char temp=sodoku[r][c1];
    sodoku[r][c1]=sodoku[r][c2];
    sodoku[r][c2]=temp;
}


char ch_column_collision(char r,char c,char temp)
{
    for(char row =0; row <=r; row++)
        if(sodoku[row][c]==temp)
            return 1;
    return 0;
}

void main_scn()
{
    char current_choice=0;
    while(1)
    {

        char temp;
        print_logo();
        printf("\n\n\t\t\t\t ");

        if(current_choice==0)
            setColor( 5 );
        else
            setColor( 1 );
        printf("\n\n\t\t\t\t\t\t\t  --- Play ---");
        if(current_choice==1)
            setColor( 5 );
        else
            setColor( 1 );
        printf("\n\n\t\t\t\t\t\t\t  --- Rules ---");
        if(current_choice==2)
            setColor( 5 );
        else
            setColor( 1 );
        printf("\n\n\t\t\t\t\t\t\t  --- score ---");
        if(current_choice==3)
            setColor( 5 );
        else
            setColor( 1 );
        printf("\n\n\t\t\t\t\t\t\t  --- Exit ---\n");

        int chr1, chr2;
        setColor( 1 );
        printf("Press any arrow key...\n");
        chr1 = getch();

        if (GetKeyState(VK_RETURN) < 0)
            switch (current_choice)
            {
            case 0:
                diff_level_scn();
                get_name();
                play();
                break;
            case 1:
                rules();
                break;
            case 2:
                score();
                break;
            case 3:
                system("cls");
                exit(0);
            }

        if (chr1 == 0xE0) //to check scroll key interrupt
        {
            chr2 = getch();                      //to read arrow key
            switch(chr2)
            {
            case 72:                             //UP ARROW KEY
                if(current_choice>0)
                    current_choice--;
                break;
            case 80:                             //DOWN ARROW KEY
                if(current_choice<3)
                    current_choice++;
                break;
            case 75:                             //LEFT ARROW KEY
                break;
            case 77:                             // RIGHT ARROW KEY
                break;
            default:

                break;
            };
        }
    }

}

void rules()
{

    print_logo();

    setColor(1);
    printf("\n\n\t\t\t Rule 1 -");
    setColor(3);
    printf("Each row must contain the numbers from 1 to 9, without repetitions");
    setColor(1);
    printf("\n\n\t\t\t Rule 2 -");
    setColor(3);
    printf("Each column must contain the numbers from 1 to 9, without repetitions");
    setColor(1);
    printf("\n\n\t\t\t Rule 3 -");
    setColor(3);
    printf("Each 3*3 box can only contain each number from 1 to 9 once");
    setColor(1);
    printf("\n\n\t\t\t Rule 4 -");
    setColor(3);
    printf("The sum of every single row, column and nonet must equal 45");
    setColor(3);
    printf("\n\n\n\t\t\tOnce the puzzle is solved,");
    printf("\n\t\t\tthis means that every row, column, and 3 * 3 box");
    printf("\n\t\t\twill contain every number from 1 to 9 exactly once.");
    printf("\n\t\t\tIn other words, no number can be repeated in any 3 * 3 box, row, or column. ");

    setColor(5);
    printf("\n\n\n\t\t\tplease press Enter to go back...");
    while(1)
    {
        if(GetKeyState(VK_RETURN) > 0)
            return;
    }
}


void print_logo()
{
    system("cls");
    setColor(1);
    printf("\n\n\t");
    for(char i=0; i<49; i++)
        printf("_");
    printf(" Sodoku Game ");
    for(char i=0; i<45; i++)
        printf("_");
    printf("\n\t\t");
    for(char i=0; i<91; i++)
        printf("_");
    printf("\n\n\t\t\t\t ");
    setColor(15);

}

void play()
{
    int chr1, chr2;
    game_init();
    do
    {
        print_logo();
        print_sodoku();
        chr1 = getch();
        flag_wrong_input=1;
        if (chr1 == 0xE0) //to check scroll key interrupt
        {
            chr2 = getch();  //to read arrow key
            flag_wrong_input=0;

            switch(chr2)
            {
            case 72:                 //UP ARROW KEY
                go_up();
                break;
            case 80:                 //DOWN ARROW KEY
                go_down();
                break;
            case 75:                 //LEFT ARROW KEY
                go_left();
                break;
            case 77:                 // RIGHT ARROW KEY
                go_right();
                break;
            default:
                break;
            };
        }
        else
        {
            if(chr1>48 && chr1<58)
            {
                flag_wrong_input=0;
                if(!ch_repeated(selected_i,selected_j,chr1-48))
                    temp_sodoku[selected_i][selected_j]=chr1-48;
                else
                    flag_collision =1;
            }

            if(chr1==127 || chr1==8)
            {
                flag_wrong_input=0;
                temp_sodoku[selected_i][selected_j]=0;
            }


            if (GetKeyState(VK_RETURN) < 0)
            {
                flag_wrong_input=0;
                switch (op_select)
                {
                case 1:
                    flag_hint=1;
                    if(current_hint<max_hint)
                    {
                        hint();
                        current_hint++;
                    }
                    break;
                case 2:
                    flag_answer=1;
                    break;
                case 3:
                    clear();
                    break;
                case 4:
                    players[num_players++].score=0;
                    return;
                    break;
                default:
                    break;
                }
            }



        }

    }
    while(!check_solution());
    start =clock() - start;
    calc_score((int)(((int)start)/CLOCKS_PER_SEC));
    players[num_players].score=temp_score;
    cong();
    num_players++;
}
void game_init()
{
    srand(time(0));
    create_sodoku();
    cr_temp_sodoku();
    cr_color_state();
    get_first_place();
    op_select=0;
    flag_answer=0;
    current_hint=0;
    flag_start=1;
    temp_score=0;
    start = clock();
}


char check_solution()
{
    char sum;
    for(char i=0; i<9; i++)
    {
        for(char j=0; j<9; j++)
            if(temp_sodoku[i][j]==0)
                return 0;
    }
    for(char i=0; i<9; i++)
    {
        sum=0;
        for(char j=0; j<9; j++)
            sum+=temp_sodoku[i][j];
        if(sum!=45)
            return 0;
    }
    for(char i=0; i<9; i++)
    {
        sum=0;
        for(char j=0; j<9; j++)
            sum+=temp_sodoku[j][i];
        if(sum!=45)
            return 0;
    }
    return 1;
}

void get_first_place()
{
    for(char i=0; i<9; i++)
    {
        for(char j=0; j<9; j++)
            if(color_state[i][j]==0)
            {
                selected_i=i;
                selected_j=j;
                return;
            }
    }

}

void go_up()
{
    if(!flag_answer)
        if(selected_i>0)
        {
            for(int i=selected_i-1; i>=0; i--)
                for(int j=0; j<9; j++)
                    if(color_state[i][j]==0)
                    {
                        selected_i=i;
                        selected_j=j;
                        op_select=0;
                        return;
                    }
        }
}

void go_down()
{
    if(selected_i<8)
    {
        for(int i=selected_i+1; i<9; i++)
            for(int j=0; j<9; j++)
                if(color_state[i][j]==0)
                {
                    selected_i=i;
                    selected_j=j;
                    return;
                }
        op_select=1;
        selected_i=9;

    }
    else if(!op_select)
    {
        selected_i=9;
        op_select=1;
    }
}
void go_left()
{
    if(!op_select)
    {
        if(selected_j>0)
            for(int j=selected_j-1; j>=0; j--)
                if(!color_state[selected_i][j])
                {
                    selected_j=j;
                    return;
                }
    }
    else
    {
        if(op_select>1)
            op_select--;
    }

}

void go_right()
{
    if(!op_select)
    {
        if(selected_j<8)
            for(int j=selected_j+1; j<9; j++)
                if(!color_state[selected_i][j])
                {
                    selected_j=j;
                    return;
                }
    }
    else
    {
        if(op_select<4)
            op_select++;
    }
}

char ch_repeated(char r,char c,char value)
{
    for(char row =0; row <=9; row++)
        if(temp_sodoku[row][c]==value)
            return 1;

    for(char col =0; col <=9; col++)
        if(temp_sodoku[r][col]==value)
            return 1;

    return 0;

}

void diff_level_scn()
{
    char current_choice=0;
    while(1)
    {
        print_logo();

        printf("\n\n\t\t\t\t ");

        if(current_choice==0)
            setColor( 5 );
        else
            setColor( 1 );
        printf("\n\n\t\t\t\t\t\t\t  --- Beginner  ---");
        if(current_choice==1)
            setColor( 5 );
        else
            setColor( 1 );
        printf("\n\n\t\t\t\t\t\t\t  --- Easy      ---");
        if(current_choice==2)
            setColor( 5 );
        else
            setColor( 1 );
        printf("\n\n\t\t\t\t\t\t\t  --- Medium    ---");
        if(current_choice==3)
            setColor( 5 );
        else
            setColor( 1 );
        printf("\n\n\t\t\t\t\t\t\t  --- Hard      ---");

        if(current_choice==4)
            setColor( 5 );
        else
            setColor( 1 );
        printf("\n\n\t\t\t\t\t\t\t  --- Expert    ---");

        int chr1, chr2;
        setColor( 1 );
        chr1 = getch();

        if (GetKeyState(VK_RETURN) < 0)
            switch (current_choice)
            {
            case 0:
                level =beginner;
                max_hint =2;
                return;
            case 1:
                level =easy;
                max_hint =3;
                return;
            case 2:
                level =medium;
                max_hint =3;
                return;
            case 3:
                level =hard;
                max_hint =4;
                return;
            case 4:
                level =expert;
                max_hint =4;
                return;
            default:
                break;
            }

        if (chr1 == 0xE0) //to check scroll key interrupt
        {
            chr2 = getch();                      //to read arrow key
            switch(chr2)
            {
            case 72:                             //UP ARROW KEY
                if(current_choice>0)
                    current_choice--;
                break;
            case 80:                             //DOWN ARROW KEY
                if(current_choice<4)
                    current_choice++;
                break;
            default:
                break;
            };
        }
    }
}

void clear()
{
    for (char r=0; r<9; r++)
        for(char c=0; c<9; c++)
            if(!color_state[r][c])
                temp_sodoku[r][c]=0;

}

void hint()
{
    char counter =0,r,c,flag=0;

    for(char i=0; i<3; i++)
    {
        flag=0;
        do
        {
            flag++;
            r=(rand()%9);
            c=(rand()%9);
            if(flag >40)
                break;
        }
        while(color_state[r][c] ||temp_sodoku[r][c]);
        if(flag<40)
        {
            counter++;
            temp_sodoku[r][c]=sodoku[r][c];
            color_state[r][c]=10;
        }
    }
    if(counter<3)
    {
        for (char r=0; r<9; r++)
            for(char c=0; c<9; c++)
                if(!color_state[r][c] && !temp_sodoku[r][c])
                {
                    temp_sodoku[r][c]=sodoku[r][c];
                    color_state[r][c]=10;
                    ++counter;
                    if(counter>=3)
                        return;
                }


    }
}
void cong()
{
    system("scn");
    int chr1;

    for(char i=0; i<30; i++)
    {
        print_logo();
        setColor(1);
        printf("\n\n\n\t");

        for(char j=0; j<i; j++)
            printf("   ");
        printf("<<<< CONGRATULATIONS >>>>");
        printf("\n\n\t\t\t\t\t\t\t Your score is");
        setColor(11);
        printf("  %d ",temp_score);
        setColor(5);
        printf("\n\n\n\nstill press Enter to continue...");
        Sleep(100);
        chr1 = getch();
        if (GetKeyState(VK_RETURN) < 0)
            return;
    }
    setColor(1);
}

void calc_score(int t)
{
    switch(level)
    {
    case beginner:
        temp_score=(50000-((int)current_hint*1000+100*t));
        break;
    case easy:
        temp_score=100000-(current_hint*2000)-150*t;
        break;
    case medium:
        temp_score=200000-(current_hint*3000)-200*t;
        break;
    case hard:
        temp_score=300000-(current_hint*4000)-300*t;
        break;
    case expert:
        temp_score=400000-(current_hint*5000)-400*t;
        break;

    }
}

void get_name()
{
    system("cls");
    print_logo();
    setColor(1);
    printf("\n\n\t\t\t\t Enter your name: ");
    setColor(5);
    gets(players[num_players].name);
}

void score()
{
    char choice=0,Ascending_flag=0,temp;
    int chr1, chr2;
    while(1)
    {
        system("cls");
        print_logo();
        setColor(1);
        printf("\n\n\t\t\t    ");
        if(choice==0)
            setColor(5);
        else
            setColor(1);
        printf("Name");
        setColor(1);
        printf("    |   ");

        if(choice==1)
            setColor(5);
        else
            setColor(1);
        printf("Score");
        if(choice==2)
            setColor(5);
        else
            setColor(1);
        if(!Ascending_flag)
            printf("           Sort in Ascending order");
        else
            printf("           Sort in Descending order");

        if(choice==3)
            setColor(5);
        else
            setColor(1);
        printf("         Go back");

        setColor(1);
        printf("\n\t\t\t____________|____________");

        for(char i=0; i<10; i++)
        {
            setColor(1);
            printf("\n\t\t\t|");
            setColor(3);
            if(strlen(players[i].name))
            {
                printf("%s",players[i].name);
                temp=strlen(players[i].name);
                for(char j=temp-1; j<10; j++)
                    printf(" ");
            }
            else
                for(char j=0; j<11; j++)
                    printf(" ");
            setColor(1);
            printf("|");
            setColor(3);

            if(players[i].score)
            {
                printf("%d",players[i].score);
                temp=calc_digit(players[i].score);
                for(char j=temp-1; j<10; j++)
                    printf(" ");
            }
            else
            {
                char j=0;
                if(strlen(players[i].name) && !(players[i].score))
                    printf("%d",j++);
                for(; j<11; j++)
                    printf(" ");
            }
            setColor(1);
            printf("|");

        }

        setColor( 1 );
        chr1 = getch();

        if (GetKeyState(VK_RETURN) < 0)
            switch (choice)
            {
            case 0:
                sort_name(Ascending_flag);
                break;
            case 1:
                sort_score(Ascending_flag);
                break;
            case 2:
                if(Ascending_flag)
                    Ascending_flag=0;
                else
                    Ascending_flag=1;
                break;
            case 3:
                return;
            default:
                break;
            }

        if (chr1 == 0xE0)            //to check scroll key interrupt
        {
            chr2 = getch();          //to read arrow key
            switch(chr2)
            {
            case 75:                 //LEFT ARROW KEY
                if(choice>0)
                    choice--;
                break;
            case 77:                 // RIGHT ARROW KEY
                if(choice<3)
                    choice++;
                break;
            default:
                break;
            };
        }








    }
}


char calc_digit(int n)
{
    char counter=0;
    while(n!=0)
    {
        n=n/10;
        counter++;
    }
    return counter;
}

void sort_name(char order)
{
    cr_player temp;

    for(char i=0; i<num_players; i++)
        for(char j=i+1; j<num_players; j++)
        {
            if(order)
            {
                if(strcmp(players[i].name,players[j].name)>0)
                {
                    temp=players[i];
                    players[i]=players[j];
                    players[j]=temp;
                }
            }
            else if(strcmp(players[i].name,players[j].name)<0)
            {
                temp=players[i];
                players[i]=players[j];
                players[j]=temp;
            }

        }

}

void sort_score(char order)
{
    cr_player temp;

    for(char i=0; i<num_players; i++)
        for(char j=i+1; j<num_players; j++)
        {
            if(order)
            {
                if(players[i].score>players[j].score)
                {
                    temp=players[i];
                    players[i]=players[j];
                    players[j]=temp;
                }
            }
            else if(players[i].score<players[j].score)
            {
                temp=players[i];
                players[i]=players[j];
                players[j]=temp;
            }
        }

}
