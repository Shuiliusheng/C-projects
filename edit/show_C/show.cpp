#include "show.h"

char buf[40][75]={0};
int buf_x[40]={0};
int buf_y=0;

int x=0,y=0;

int max_w=73;
int max_h=38;

int find_str_flag=0;
char find_str_buf[100]={0};
int find_str_num=0;
int find_x=0,find_y=0;

int open_flag=1;
char open_buf[200]={0};
int open_num=0;

int place_flag[100000]={0};
int num_place=0;

int now_place=0;


HANDLE hout;
COORD coord; //ÆÁÄ»ÉÏµÄ×ø±ê
FILE *p=NULL;


int main()
{
    hout=GetStdHandle(STD_OUTPUT_HANDLE);

    system("mode con cols=80 lines=42");
	open_flag=1;
    open_num=0;

	show_str(0,strlen("------------help infomation-----------"),"------------help infomation-----------");
	show_str(1,strlen("----ctrl+o : open the file to look"),"----ctrl+o : open the file to look");
	show_str(2,strlen("----ctrl+f : find the string "),"----ctrl+f : find the string ");
	show_str(3,strlen("----ctrl+e : exit"),"----ctrl+e : exit");
	show_str(4,strlen("----f1     : find the next dst string "),"----f1     : find the next dst string ");
	show_str(5,strlen("----f2     : help infomation "),"----f2     : help infomation ");

	show_str(max_h+2,11,"open file: ");

	while(1)
    {
        unsigned int input=getch();
		if(open_flag==1)
        {
            if(input==ENTER)
            {
				if(p!=NULL)
					fclose(p);
				p=NULL;
             
                if(!start()||open_num==0)
                {
                    show_str(max_h+2,10,"open file: ");
                    open_num=0;
                }
                else
                {
                    open_flag=0;
                    char temp[100]="file: ";
                    strcat(temp,open_buf);
                    show_str(max_h+2,strlen(temp),temp);
                }
                continue;
            }
			else
            {
                input_str(open_num,open_buf,input);
            }
            char open[240]="open file: ";
            strcat(open,open_buf);
            show_str(max_h+2,strlen(open),open);
		}
		else if(find_str_flag==1)
        {
            if(input==ENTER)
            {
                find_str_flag=2;
                find_x=x;
                find_y=y;
                find_str(find_x,find_y);
                x=find_x;
                y=find_y;
				set_cursor(x,y);
                find_x++;
                continue;
            }
            else
            {
                input_str(find_str_num,find_str_buf,input);
            }
            char temp[100]="input: ";
            strcat(temp,find_str_buf);            
            show_str(max_h+2,strlen(temp),temp);
        }
		while(input==MOVE)
		{
			input=getch();
			if(input==UP)
			{
				if(y>0)
					y--;
				else if(now_place>0)
				{
					now_place--;
					fseek(p,place_flag[now_place],SEEK_SET);
					buf_y=0;
					add_to_buf(p);
					show_buf();
					x=0,y=max_h+1;
					set_cursor(x,y);
				}
				set_cursor(x,y);
			}
			else if(input==DOWN)
			{
				if(y<=max_h)
					y++;
				else if(now_place<num_place-1)
				{
					now_place++;
					fseek(p,place_flag[now_place],SEEK_SET);
					buf_y=0;
					add_to_buf(p);
					show_buf();
					x=0,y=0;
					set_cursor(x,y);
				}
				set_cursor(x,y);
			}
			else if(input==LEFT)
			{
				if(x>0)
					x--;
				set_cursor(x,y);
			}
			else if(input==RIGHT)
			{
				if(x<=max_w)
					x++;
				set_cursor(x,y);
			}
			else if(input==PGUP)
			{
				if(now_place>0)
				{
					now_place--;
					fseek(p,place_flag[now_place],SEEK_SET);
					buf_y=0;
					add_to_buf(p);
					show_buf();
					x=0,y=0;
					set_cursor(x,y);
				}
				
			}
			else if(input==PGDOWN)
			{
				if(now_place<num_place-1)
				{
					now_place++;
					fseek(p,place_flag[now_place],SEEK_SET);
					buf_y=0;
					add_to_buf(p);
					show_buf();
					x=0,y=0;
					set_cursor(x,y);
				}
			}
			else if(input==END)
			{
				if(now_place!=num_place-1)
				{
					now_place=num_place-1;
					fseek(p,place_flag[now_place],SEEK_SET);
					buf_y=0;
					add_to_buf(p);
					show_buf();

					y=buf_y-1;
					x=buf_x[y];
					set_cursor(x,y);
				}
			}
			else if(input==HOME)
			{
				if(now_place!=0)
				{
					now_place=0;
					fseek(p,place_flag[now_place],SEEK_SET);
					buf_y=0;
					add_to_buf(p);
					show_buf();

					x=0,y=0;
					set_cursor(x,y);
				}
			}
			input=getch();
			
		}
		
		if(input==CTRL_O)
        {
            open_flag=1;
            open_num=0;
			x=0,y=0;
			num_place=0;
			find_str_flag=0;
			now_place=0;
			dispear_screen();
			show_str(max_h+2,11,"open file: ");
		}
		else if(input==CTRL_F)
		{
            find_str_flag=1;
            find_str_num=0;
            show_str(max_h+2,6,"input: ");
		}
		else if(input==CTRL_E)
		{
            break;
		}
		else if(input==FUC_F)
		{
			input=getch();
			if(input==F1)
			{
				if(find_str_num>0)
				{
					char temp[100]="search: ";
					strcat(temp,find_str_buf);
					show_str(max_h+2,strlen(temp),temp);
					if(find_x!=x+1)
						find_x=x,find_y=y;
					find_str(find_x,find_y);
					x=find_x;
					y=find_y;
					set_cursor(x,y);
					find_x++;
				}
			}
			else if(input==F2)
			{
				dispear_screen();
				show_str(0,strlen("------------help infomation-----------"),"------------help infomation-----------");
				show_str(1,strlen("----ctrl+o : open the file to look"),"----ctrl+o : open the file to look");
				show_str(2,strlen("----ctrl+f : find the string "),"----ctrl+f : find the string ");
				show_str(3,strlen("----ctrl+e : exit"),"----ctrl+e : exit");
				show_str(4,strlen("----f1     : find the next dst string "),"----f1     : find the next dst string ");
				show_str(5,strlen("----f2     : help infomation "),"----f2     : help infomation ");
				getch();
				dispear_screen();
				show_buf();
			}
		}
		else if(input==ESC)
		{
			break;
		}
    } 	
	return 0;
}


void add_to_buf(FILE *p)
{
	char temp[800];
	char temp1[800];
	int num=0;
	int num1=0;
	while(!feof(p)&&buf_y<=max_h)
	{
		memset(temp1,0,800);
		num=0;
		fgets(temp1,800,p);
		num1=strlen(temp1)-1;
		if(temp1[num1]=='\n')
			temp1[num1]='\0';
		else
			num1++;
		for(int c=0;c<num1;c++)
		{
			if(temp1[c]=='\t')
			{
				for(int j=0;j<4;j++)
					temp[num++]=' ';
				continue;
			}
			temp[num++]=temp1[c];
		}
		for(int i=0;i<num;i++)
		{
			int c=0;
			for(c=i;c<=i+max_w&&c<num;c++)
				buf[buf_y][c-i]=temp[c];
			buf_x[buf_y]=c-i;
			buf_y++;
			i=c-1;
		}
	}
}

bool start()
{
	p=fopen(open_buf,"r+");
	if(p==NULL)
		return false;
	while(!feof(p)&&num_place<100000)
	{
		place_flag[num_place++]=ftell(p);
		buf_y=0;
		add_to_buf(p);	
	}
	fseek(p,0,SEEK_SET);
	buf_y=0;
	add_to_buf(p);
	show_buf();
	return true;
}


void set_cursor(int x,int y)
{
	coord.X=x;
	coord.Y=y;
	SetConsoleCursorPosition(hout,coord);
}


void dispear_char_line(int y)
{
	set_cursor(0,y);
	for(int i=0;i<=79;i++)
		cout <<" ";
}


void show_str(int y,int num,char str[])
{
	dispear_char_line(y);
	set_cursor(0,y);
	for(int i=0;i<num;i++)
		cout <<str[i];
}


void show_buf()
{
	dispear_screen();
	int t=now_place*(max_h+1);
	int n=min(max_h+1,buf_y);
	for(int i=0;i<n;i++)
	{	
		show_str(i,buf_x[i],buf[i]);
		set_cursor(75,i);
		printf("|%04d",i+t);
	}
	char temp[20];
	sprintf(temp,"Page: %d",now_place+1);
	show_str(max_h+2,strlen(temp),temp);
}
	
void input_str(int &num,char buf[],int input)
{
    if(input==BACKSPACE&&num>0)
    {
        buf[num-1]='\0';
        num--;
    }
    else if(input!=BACKSPACE&&input<160&&input>27)
    {
        buf[num++]=input;
        buf[num]='\0';
    }
}

void dispear_screen()
{
	int n=max_h+2;
	for(int i=0;i<=n;i++)
		dispear_char_line(i);
}

int find_str(int &x,int &y)
{
    int l=strlen(find_str_buf);
    if(l==0)
        return 0;

    int i=0,j=0,n=0;

    for(n=y;n<buf_y;n++)
    {
        if(n==y)
            i=x;
        else
            i=0;
        for(;i<buf_x[n];i++)
        {
            for(j=0;j<l;j++)
            {
                if(find_str_buf[j]!=buf[n][i+j])
                    break;
            }
            if(j==l)
            {
                x=i;
                y=n;
                return 1;
            }
        }
    }
    x=0;
    y=0;
	
	now_place=(now_place+1)%num_place;
	fseek(p,place_flag[now_place],SEEK_SET);
	buf_y=0;
	add_to_buf(p);
	show_buf();
	
	find_str(x,y);

    return 0;
}

