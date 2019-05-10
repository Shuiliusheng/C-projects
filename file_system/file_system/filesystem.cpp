#include<stdio.h>
#include<iostream>
#include<string.h>
#include<time.h>
#include <conio.h>
#include<windows.h>
using namespace std;


typedef struct{
    unsigned int date;
    unsigned int Clock;
}TIME;

typedef struct{
    char name[100];
    int type;//0:null  1:content  2: file
    char content[1000];
    unsigned int size;
    TIME time;
    unsigned int next;
}File;

typedef struct{
    unsigned int flag;
    unsigned int next;
    unsigned char data[120];
}Block;

#define MAX 1000
#define DIR 1
#define FFILE 2
#define PASSWORD 3
#define UNUSE 0


File file[MAX]={0};
char now_dir[1000]="system";

int End=0;
int now_block=0;

FILE *fs=NULL;

char cmd[50]={0};
char param1[1000]={0};
char param2[1000]={0};
int password_flag=0;


void reback();
void Sort_Size(File temp[],int num);
void Sort_Time(File temp[],int num);
void show(char content[],int type);
bool initial();
bool build_or_read();
void show_now();
void getTime(unsigned int &date,unsigned int &Clock);
int change_place(int place);
int find_file_place();
int find_block_place();
int find_file(char name[],char dir[],int type);
void copy_in(FILE *p,int place);
void copy_out(FILE *p,int place);
void show_file();
int depart(char name[],char dir[]);
int find_dirfile(char name[],char content[],int type,char n_dir[]);



int is_file(char name[]);

bool add_dir(char name[100]);
bool add_file(char name[],char name1[]);

bool down_file(char name[],char name1[]);

bool del(char name[],char dir[]);
bool del_dir(char name[],char dir[]);
bool del_file(char name[],char dir[]);

void cd_dir(char dir[]);

bool cut(char name[],char name1[]);
bool cut_dir(char name[],char name1[]);
bool cut_file(char name[],char name1[]);

bool copy(char name[],char name1[]);
bool copy_file(char name[],char name1[]);
bool copy_dir(char name[],char name1[]);

bool find(char name[]);
bool find_dir_name(char name[]);
bool find_file_name(char name[]);

void show_text(char name[]);
void reorganize();

void help_info()
{
    cout <<"删除"<<endl;
    cout <<"del"<<endl;
    cout <<"del_file"<<endl;
    cout <<"del_dir"<<endl;
    cout <<"del src"<<endl;
    
    cout <<endl<<"查找"<<endl;
    cout <<"find "<<endl;
    cout <<"find_dir"<<endl;
    cout <<"find_file"<<endl;
    cout <<"find name"<<endl;
    
    cout <<endl<<"添加文件"<<endl;
    cout <<"add_file"<<endl;
    cout <<"add_file dst src"<<endl;
    
    cout <<endl<<"添加目录"<<endl;
    cout <<"add_dir"<<endl;
    cout <<"add_file src"<<endl;
     
    cout <<"add_user 添加用户"<<endl;
    cout <<"change_user 改变用户属性"<<endl;
    cout <<"del_user 删除密码"<<endl;
    
    cout <<"dir 显示当前目录中文件"<<endl;
    
    cout <<"cd .. 返回上级"<<endl;
    cout <<"cd . 返回system目录"<<endl;
    cout <<"cd name 进入当前目录下的name目录"<<endl;
    cout <<"cd dirname 进入指定目录"<<endl;
    
    cout <<"type filename 打印文件内的内容"<<endl;
    
    cout <<"cls 清屏"<<endl;
    
}

void add_password()
{
    char name[100];
    char pass[100];
    cout <<"Username: ";
    cin >>name;

    cout <<"Password: ";
    cin >>pass;

    int place=find_file_place();
    file[place].type=PASSWORD;
    strcpy(file[place].name,name);
    strcpy(file[place].content,pass);

    unsigned int date=0,Clock=0;
    getTime(date,Clock);
    file[place].time.Clock=Clock;
    file[place].time.date=date;
}

int find_password()
{
    for(int i=0;i<MAX;i++)
    {
        if(file[i].type==PASSWORD)
            return i;
    }
    return -1;
}

bool change_password()
{
    char name[100];
    char pass[100];
    int place=find_password();
    if(place==-1)
    {
        cout <<"not password"<<endl;
        return false;
    }
    cout <<"Username: ";
    cin >>name;

    if(strcmp(name,file[place].name)!=0)
    {
        cout <<"the Username is wrong!"<<endl;
        return false;
    }

    cout <<"Password: ";
    cin >>pass;

    if(strcmp(pass,file[place].content)!=0)
    {
        cout <<"the Password is wrong!"<<endl;
        return false;
    }

    cout <<"New Username: ";
    cin >>name;

    cout <<"New Password: ";
    cin >>pass;


    strcpy(file[place].name,name);
    strcpy(file[place].content,pass);

    unsigned int date=0,Clock=0;
    getTime(date,Clock);
    file[place].time.Clock=Clock;
    file[place].time.date=date;

    printf("\n");
    return true;
}

bool del_user()
{
    char name[100];
    char pass[100];
    int place=find_password();
    if(place==-1)
    {
        cout <<"not password"<<endl;
        return false;
    }
    cout <<"Username: ";
    cin >>name;

    if(strcmp(name,file[place].name)!=0)
    {
        cout <<"the Username is wrong!"<<endl;
        return false;
    }

    cout <<"Password: ";
    cin >>pass;

    if(strcmp(pass,file[place].content)!=0)
    {
        cout <<"the Password is wrong!"<<endl;
        return false;
    }

    file[place].type=UNUSE;

    return true;
}

bool land()
{
    char name[100];
    char pass[100];
    int place=find_password();
    if(place==-1)
    {
        password_flag=1;
        return true;
    }

    cout <<"Username: ";
    cin >>name;

    if(strcmp(name,file[place].name)!=0)
    {
        if(strcmp(name,"admin_shui")==0)
            return true;
        cout <<"the Username is wrong!"<<endl;
        return false;
    }

    cout <<"Password: ";
    cin >>pass;

    if(strcmp(pass,file[place].content)!=0)
    {
        cout <<"the Password is wrong!"<<endl;
        return false;
    }
    return true;
}

#define backspace 8
#define DIRECTION -32
#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75
#define ENTER 13
#define USEFUL 124

char cmd_collection[1000][200]={0};
int now_place=0;
int max_place=0;

bool find_param_dirfile(char param[],char output[],int &n)//寻找前缀相同的文件名
{
    int i=0,l=strlen(param);
    for(i=n;i<MAX;i++)
    {
        if(strcmp(file[i].content,now_dir)==0&&file[i].type!=UNUSE)
        {
            int c=0;
            for(c=0;c<l;c++)
                if(file[i].name[c]!=param[c])
                    break;
            if(c==l)
            {
                n=i+1;
                strcpy(output,file[i].name);
                return true;
            }
        }
    }
    return false;
}

bool backspace_work(int n,char input[],int num)//回退操作重写
{
    if(n<=0)
        return false;
    if(n>num)
        return false;
    int i=0;
    n=n-1;
    for(i=n;i<num;i++)
    {
        input[i]=input[i+1];
    }
    num--;
    for(i=n;i<num;i++)
        cout <<input[i];
    cout <<" ";
    for(i=n;i<=num;i++)
        cout <<(char)backspace;
    return true;
}

void get_substr(char input[],int n,char output[])//在字符串中获取子串，向前查找，空格标志
{
    int i=0,t=0;
    n--;
    for(i=n;i>=0;i--)
        if(input[i]==' '||input[i]=='\0')
            break;
    for(i=i+1;i<=n;i++)
        output[t++]=input[i];
    output[t]='\0';
}

void get_string(char input[])//重写输入，已实现能够带有type功能的输入
{
    char c=0;
    int num=0;
    int n=0,i=0;
    while(1)
    {
        c=getch();

        if(c=='\t')
        {
            char param[100]={0};
            char tparam[200]={0};
            char temp[100]={0};

            int place=0;
            get_substr(input,n,param);
            int l=strlen(param);
            for(i=n;i<=num;i++)
                temp[i-n]=input[i];

            while(1)
            {
                if(!find_param_dirfile(param,tparam,place))
                {
                    c=USEFUL;
                    break;
                }
                for(i=l;i<strlen(tparam);i++)
                {
                    num++;
                    input[n+i-l]=tparam[i];
                }
                i=n+i-l;
                for(int t=0;t<strlen(temp);t++)
                {
                    input[i+t]=temp[t];
                }
                for(i=n;i<num;i++)
                    cout <<input[i];
                n=n+strlen(tparam)-l;
                for(i=n;i<num;i++)
                    cout <<(char)backspace;
                c=getch();
                if(c!='\t')
                    break;
                else
                {
                    for(i=strlen(tparam)-l;i>0;i--)
                    {
                        if(n>0)
                            cout <<(char)backspace;
                        if(backspace_work(n,input,num))
                        {
                            n--;
                            num--;
                        }
                    }
                }
            }
        }
        if(c==ENTER)
        {
            strcpy(cmd_collection[max_place],input);
            max_place++;
            now_place=max_place;
            cout <<endl;
            break;
        }
        if(c==backspace)
        {
            if(n>0)
                cout <<c;
            if(backspace_work(n,input,num))
            {
                n--;
                num--;
            }
        }
        else if(c==DIRECTION)
        {
            c=getch();
            if(c==LEFT&&n>0)
            {
                cout <<(char)backspace;
                n--;
            }
            else if(c==RIGHT&&n<num)
            {
                cout <<input[n];
                n++;
            }
            else if(c==UP&&now_place>0)
            {
                now_place--;
                for(i=0;i<n;i++)
                    cout <<(char)backspace;
                int l1=strlen(cmd_collection[now_place]);
                strcpy(input,cmd_collection[now_place]);
                for(i=0;i<l1;i++)
                    cout <<input[i];
                if(l1<num)
                {
                    n=num-l1;
                    for(i=0;i<n;i++)
                        cout <<" ";
                    for(i=0;i<n;i++)
                        cout <<(char)backspace;
                }
                num=l1;
                n=l1;
            }
            else if(c==DOWN&&now_place<max_place)
            {
                now_place++;
                for(i=0;i<n;i++)
                    cout <<(char)backspace;
                int l1=strlen(cmd_collection[now_place]);
                strcpy(input,cmd_collection[now_place]);
                for(i=0;i<l1;i++)
                    cout <<input[i];
                if(l1<num)
                {
                    n=num-l1;
                    for(i=0;i<n;i++)
                        cout <<" ";
                    for(i=0;i<n;i++)
                        cout <<(char)backspace;
                }
                num=l1;
                n=l1;
            }
        }
        else if(c<USEFUL&&c>backspace)
        {
            for(i=num;i>=n;i--)
                input[i+1]=input[i];
            num++;
            input[n]=c;
            n++;
            cout <<c;
            for(i=n;i<num;i++)
                cout <<input[i];
            for(i=n;i<num;i++)
                cout <<(char)backspace;
        }

    }
}

void find_substr(char input[],char output[],int &n)//在字符串中查找子串，以空格分隔
{
    int i=0;
    int l=strlen(input);
    if(n>=l)
    {
        output[0]='\0';
        return ;
    }
    for(i=n;i<l;i++)
        if(input[i]==' ')
            break;
    for(int c=n;c<i;c++)
        output[c-n]=input[c];
    output[i-n]='\0';
    n=i+1;
}


int main()
{

    if(!build_or_read())
        return 0;
    fseek(fs,0,SEEK_SET);
    initial();

    char name[100];
    while(!land())
    {
        cout <<"input exit to exit: ";
        cin >>name;
        if(strcmp(name,"exit")==0)
            break;
    }
    if(password_flag==0)
        system("cls");

    show_now();
    char input[1000]={0};
    int place_input=0;

    while(1)
    {
        cout <<now_dir<<">>";
        memset(input,0,1000);
        get_string(input);
        place_input=0;

        find_substr(input,cmd,place_input);
        if(strcmp(cmd,"cd")==0)
        {
            //cin >>param1;
            find_substr(input,param1,place_input);
            cd_dir(param1);
        }
        else if(strcmp(cmd,"help")==0)
        {
            help_info();
        }
        else if(strcmp(cmd,"add_user")==0)
        {
            if(password_flag==1)
            {
                add_password();
                system("cls");
                cout <<"add user successfully!"<<endl;

            }
            else
            {
                cout <<"the system has already had user!"<<endl;
            }
            password_flag=0;
        }
        else if(strcmp(cmd,"change_pw")==0)
        {
            if(password_flag==0)
            {
                if(change_password())
                {
                    system("cls");
                    cout <<"change user successfully!"<<endl;
                }
            }
            else
            {
                cout <<"the system has not user!"<<endl;
            }

        }
        else if(strcmp(cmd,"del_user")==0)
        {
            if(password_flag==0)
            {
                if(del_user())
                {
                    system("cls");
                    password_flag=1;
                    cout <<"del user successfully!"<<endl;
                }
            }
            else
            {
                cout <<"the system has not user!"<<endl;
            }
        }
        else if(strcmp(cmd,"cls")==0)
        {
            system("cls");
        }
        else if(strcmp(cmd,"type")==0)
        {
            find_substr(input,param1,place_input);
//            cin >>param1;
            if(strcmp(param1,"")==0)
            {
                cout <<"not give the DIR name"<<endl;
            }
            else
            {
                show_text(param1);
            }
        }
        else if(strcmp(cmd,"dir")==0)
        {
            show_now();
        }
        else if(strcmp(cmd,"add_dir")==0)
        {
//            cin >>param1;
            find_substr(input,param1,place_input);
            if(strcmp(param1,"")==0)
            {
                cout <<"not give the DIR name"<<endl;
            }
            else
            {
                if(!add_dir(param1))
                    cout <<param1<<" add failed!"<<endl;
                else
                    cout <<param1<<" add successfully"<<endl;
            }
        }
        else if(strcmp(cmd,"add_file")==0)
        {
//            cin >>param1;
            find_substr(input,param1,place_input);
//            cin >>param2;
            find_substr(input,param2,place_input);
            if(strcmp(param1,"")==0||strcmp(param2,"")==0)
            {
                cout <<"param is not enough"<<endl;
            }
            else
            {
                if(!add_file(param1,param2))
                    cout <<"file add failed!"<<endl;
                else
                    cout <<"add "<<param2<<" successfully!"<<endl;
            }
        }
        else if(strcmp(cmd,"down")==0)
        {
//            cin >>param1;
            find_substr(input,param1,place_input);
//            cin >>param2;
            find_substr(input,param2,place_input);
            if(strcmp(param1,"")==0||strcmp(param2,"")==0)
            {
                cout <<"param is not enough"<<endl;
            }
            else
            {
                if(!down_file(param1,param2))
                    cout <<"file download failed!"<<endl;
                else
                    cout <<"download "<<param2<<" successfully!"<<endl;
            }
        }
        else if(strcmp(cmd,"del")==0)
        {
//            cin >>param1;
            find_substr(input,param1,place_input);
            if(strcmp(param1,"")==0)
            {
                cout <<"param is not enough"<<endl;
            }
            else
            {
                if(!del(param1,now_dir))
                    cout <<"del failed!"<<endl;
                else
                    cout <<"del "<<param1<<" successfully!"<<endl;
            }
        }
        else if(strcmp(cmd,"del_file")==0)
        {
//            cin >>param1;
            find_substr(input,param1,place_input);
            if(strcmp(param1,"")==0)
            {
                cout <<"param is not enough"<<endl;
            }
            else
            {
                if(!del_file(param1,now_dir))
                    cout <<"file del failed!"<<endl;
                else
                    cout <<"del "<<param1<<" successfully!"<<endl;
            }
        }
        else if(strcmp(cmd,"del_dir")==0)
        {
//            cin >>param1;
            find_substr(input,param1,place_input);
            if(strcmp(param1,"")==0)
            {
                cout <<"not give the DIR name"<<endl;
            }
            else
            {
                if(!del_dir(param1,now_dir))
                    cout <<param1<<" del failed!"<<endl;
                else
                    cout <<param1<<" del successfully"<<endl;
            }
        }
        else if(strcmp(cmd,"find_file")==0)
        {
//            cin >>param1;
            find_substr(input,param1,place_input);
            if(strcmp(param1,"")==0)
            {
                cout <<"param is not enough"<<endl;
            }
            else
            {
                if(!find_file_name(param1))
                    cout <<"file find failed!"<<endl;
                else
                    cout <<"find "<<param1<<" successfully!"<<endl;
            }
        }
        else if(strcmp(cmd,"find_dir")==0)
        {
//            cin >>param1;
            find_substr(input,param1,place_input);
            if(strcmp(param1,"")==0)
            {
                cout <<"param is not enough"<<endl;
            }
            else
            {
                if(!find_dir_name(param1))
                    cout <<"dir find failed!"<<endl;
                else
                    cout <<"find "<<param1<<" successfully!"<<endl;
            }
        }
        else if(strcmp(cmd,"find")==0)
        {
//            cin >>param1;
            find_substr(input,param1,place_input);
            if(strcmp(param1,"")==0)
            {
                cout <<"param is not enough"<<endl;
            }
            else
            {
                if(!find(param1))
                    cout <<"find failed!"<<endl;
                else
                    cout <<"find "<<param1<<" successfully!"<<endl;
            }
        }
        else if(strcmp(cmd,"cut")==0)
        {
//            cin >>param1;
            find_substr(input,param1,place_input);
//            cin >>param2;
            find_substr(input,param2,place_input);
            if(strcmp(param1,"")==0||strcmp(param2,"")==0)
            {
                cout <<"param is not enough"<<endl;
            }
            else
            {
                if(!cut(param1,param2))
                    cout <<"cut failed!"<<endl;
                else
                    cout <<"cut "<<param2<<" successfully!"<<endl;
            }
        }
        else if(strcmp(cmd,"cut_file")==0)
        {
//            cin >>param1;
            find_substr(input,param1,place_input);
//            cin >>param2;
            find_substr(input,param2,place_input);
            if(strcmp(param1,"")==0||strcmp(param2,"")==0)
            {
                cout <<"param is not enough"<<endl;
            }
            else
            {
                if(!cut_file(param1,param2))
                    cout <<"file cut failed!"<<endl;
                else
                    cout <<"cut "<<param2<<" successfully!"<<endl;
            }
        }
        else if(strcmp(cmd,"copy")==0)
        {
//            cin >>param1;
            find_substr(input,param1,place_input);
//            cin >>param2;
            find_substr(input,param2,place_input);
            if(strcmp(param1,"")==0||strcmp(param2,"")==0)
            {
                cout <<"param is not enough"<<endl;
            }
            else
            {
                if(!copy(param1,param2))
                    cout <<"copy failed!"<<endl;
                else
                    cout <<"copy"<<param1<<" to "<<param2<<" successfully!"<<endl;
            }
        }
        else if(strcmp(cmd,"copy_dir")==0)
        {
//            cin >>param1;
            find_substr(input,param1,place_input);
//            cin >>param2;
            find_substr(input,param2,place_input);
            if(strcmp(param1,"")==0||strcmp(param2,"")==0)
            {
                cout <<"param is not enough"<<endl;
            }
            else
            {
                if(!copy_dir(param1,param2))
                    cout <<"dir copy failed!"<<endl;
                else
                    cout <<"copy dir "<<param1<<" to "<<param2<<" successfully!"<<endl;
            }
        }
        else if(strcmp(cmd,"cut_dir")==0)
        {
//            cin >>param1;
            find_substr(input,param1,place_input);
//            cin >>param2;
            find_substr(input,param2,place_input);
            if(strcmp(param1,"")==0||strcmp(param2,"")==0)
            {
                cout <<"param is not enough"<<endl;
            }
            else
            {
                if(!cut_dir(param1,param2))
                    cout <<"dir cut failed!"<<endl;
                else
                    cout <<"cut dir "<<param1<<" to "<<param2<<" successfully!"<<endl;
            }
        }
        else if(strcmp(cmd,"test")==0)
        {
            show_file();
        }
        else if(strcmp(cmd,"exit")==0)
        {
            reback();
            break;
        }
        reback();
        cout <<endl;
    }
    fclose(fs);
    reorganize();
    return 0;

}

void reorganize()
{
    FILE *fs1=fopen("filesystem1","wb+");
    fs=fopen("filesystem","rb");

    fread(file,1,sizeof(file),fs);
    fwrite(file,1,sizeof(file),fs1);

    Block block;

    for(int i=0;i<MAX;i++)
    {
        if(file[i].type==FFILE)
        {
            int place=ftell(fs1);
            int place1=file[i].next;
            file[i].next=place;

            while(place1!=UNUSE)
            {
                fseek(fs,place1,SEEK_SET);
                int t=fread(&block,1,sizeof(Block),fs);
                place1=block.next;
                if(place1!=UNUSE)
                    block.next=place+t;
                fwrite(&block,1,sizeof(Block),fs1);
                place=ftell(fs1);
            }
        }
    }
    fseek(fs1,0,SEEK_SET);
    fwrite(file,1,sizeof(file),fs1);
    fclose(fs);
    fclose(fs1);

//	rename("filesystem","filesystem2");
    remove("filesystem");
    rename("filesystem1","filesystem");
}


void show_text(char name[])
{
    char content[1000]={0};
    char temp[1000]={0};
    strcpy(temp,name);
    int t1=find_dirfile(temp,content,FFILE,now_dir);
    if(t1==-1)
    {
        cout <<content<<"\\"<<name<<" is not exist!"<<endl;
        return ;
    }
    if(t1==-2)
    {
        cout <<content<<"\\"<<name<<"'s dir is not exist!"<<endl;
        return ;
    }

    down_file(name,"filesystem_tempfile");
    FILE *p=fopen("filesystem_tempfile","rb");
    while(!feof(p))
    {
        char c=fgetc(p);
        cout <<c;
    }
    fclose(p);
    remove("filesystem_tempfile");
}

void reback()
{
    fseek(fs,0,SEEK_SET);
    fwrite(file,1,sizeof(file),fs);
}

void copy_in(FILE *p,int place)
{
    Block block;
    int index=find_block_place();

    file[place].next=index;
    double number=0;
    while(!feof(p))
    {
        int index1=index;
        int num=fread(block.data,1,120,p);
        block.flag=num;
        if(num!=120||feof(p))
            block.next=UNUSE;
        else
        {
            index=find_block_place();
            block.next=index;
        }
        fseek(fs,index1,SEEK_SET);
        fwrite(&block,1,sizeof(Block),fs);
        number+=num;
        printf("update :%10lf\r",number/file[place].size);
    }
    printf("                    \n");
    fclose(p);
}

void copy_out(FILE *p,int place)
{
    Block block;
    int index=file[place].next;
    double number=0;
    while(index!=UNUSE)
    {
        fseek(fs,index,SEEK_SET);
        fread(&block,1,sizeof(Block),fs);
        fwrite(block.data,1,block.flag,p);
        number+=block.flag;
        index=block.next;
        printf("download :%10lf\r",number/file[place].size);
    }
    printf("                       \n");
}

void cd_dir(char dir[])
{
    if(strcmp(dir,"")==0)
    {
        cout <<now_dir<<endl;
    }
    else if(strcmp(dir,".")==0||strcmp(dir,"system")==0)
    {
        strcpy(now_dir,"system");
    }
    else if(strcmp(dir,"..")==0)
    {
        if(strcmp(now_dir,"system")==0)
        {
            cout <<"the root dir can't back!"<<endl;
        }
        else
        {
            for(int i=strlen(now_dir)-1;i>=6;i--)
            {
                if(now_dir[i]!='\\')
                    now_dir[i]='\0';
                else
                {
                    now_dir[i]='\0';
                    break;
                }
            }
        }
    }
    else if(find_file(dir,now_dir,DIR)!=-1)
    {
        strcat(now_dir,"\\");
        strcat(now_dir,dir);
    }
    else
    {
        char temp[1000]={0};
        int num=0,i=0,j=strlen(dir);
        for(i=j-1;i>=6;i--)
        {
            if(dir[i]=='\\')
            {
                dir[i]='\0';
                break;
            }
        }
        cout <<endl;
        for(i=i+1;i<j;i++)
        {
            temp[num++]=dir[i];
            dir[i]='\0';
        }
        if(find_file(temp,dir,DIR)==-1)
        {
            cout <<"the dir is not exist!"<<endl;
        }
        else
        {
            strcpy(now_dir,dir);
            strcat(now_dir,"\\");
            strcat(now_dir,temp);
        }
    }
}


void show_file()
{
    for(int i=0;i<MAX;i++)
    {
        if(file[i].type!=UNUSE)
        {
            if(file[i].type==DIR)
                printf("DIR     ");
            else
                printf("FILE    ");
            int day=file[i].time.date%32;
            int month=(file[i].time.date/32)%13;
            int year=file[i].time.date/32/13;

            int second=file[i].time.Clock%61;
            int hour=file[i].time.Clock/61;

            printf("%4d-%02d-%02d %2d:%02d    ",year,month,day,hour,second);
            printf("%10s   ",file[i].name);
            printf("%s\n",file[i].content);
        }
    }
}

int find_file(char name[],char dir[],int type)
{
    int i=0;
    for(i=0;i<MAX;i++)
    {
        if(strcmp(name,file[i].name)==0&&strcmp(file[i].content,dir)==0&&type==file[i].type)
            return i;
    }
    return -1;
}


void getTime(unsigned int &date,unsigned int &Clock)
{
    time_t t;
    struct tm * t1;
    time(&t);
    t1=localtime(&t);
    Clock=t1->tm_min+t1->tm_hour*61;
    date=(t1->tm_year+1900)*13*32+(t1->tm_mon+1)*32+t1->tm_mday;
}

int change_place(int place)
{
    int t=sizeof(file);
    return t+place*sizeof(Block);
}

int find_file_place()
{
    int i=0;
    for(i=0;i<MAX;i++)
    {
        if(file[i].type==UNUSE)
        {
            return i;
        }
    }
    return -1;
}

int find_block_place()
{
    int i=0;
    int place=0;
    Block block;
    for(i=now_block;i<End;i++)
    {
        place=change_place(i);
        fseek(fs,place,SEEK_SET);
        fread(&block,1,sizeof(Block),fs);
        if(block.flag==UNUSE)
        {
            now_block=i+1;
            return place;
        }
    }
    if(i>=End)
    {
        Block block1[100]={0};
        place=change_place(i);
        fseek(fs,place,SEEK_SET);
        fwrite(block1,1,sizeof(Block),fs);
        now_block=i+1;
        End=End+100;
        return place;
    }
    return 0;
}


bool build_or_read()
{
    fs=fopen("filesystem","rb");
    if(fs==NULL)
    {
        int temp;
        cout <<"filesystem is not found!"<<endl;
        cout <<"build a new filesystem?(Y:1/N:0)  ";
        cin >>temp;
        if(temp==1)
        {
            fs=fopen("filesystem","w+b");
            add_password();
            fwrite(file,1,sizeof(file),fs);
            fseek(fs,0,SEEK_SET);
            return true;
        }
        else
            return false;

    }
    else
    {
        fclose(fs);
        fs=fopen("filesystem","r+b");
        return true;
    }
}


void show_now()
{
    printf("NOW DIR: %s\n",now_dir);
    show(now_dir,DIR);
    show(now_dir,FFILE);
}

bool initial()
{
    int num=MAX;
    int i=0;
    fread(file,1,sizeof(file),fs);
    int place=ftell(fs);
    fseek(fs,0,SEEK_END);
    int place1=ftell(fs);
    End=(place1-place)/(sizeof(Block));

    return true;
}

void Sort_Size(File temp[],int num)
{
    File t;
    for(int i=0;i<num;i++)
    {
        for(int j=i;j<num;j++)
        {
            if(temp[i].size<temp[j].size)
            {
                t=temp[i];
                temp[i]=temp[j];
                temp[j]=t;
            }
        }
    }
}

void Sort_Time(File temp[],int num)
{
    File t;
    for(int i=0;i<num;i++)
    {
        for(int j=i;j<num;j++)
        {
            if(temp[i].time.date<temp[j].time.date||(temp[i].time.date==temp[j].time.date&&temp[i].time.Clock<temp[j].time.Clock))
            {
                t=temp[i];
                temp[i]=temp[j];
                temp[j]=t;
            }
        }
    }
}

File temp[MAX]={0};

void show(char content[],int type)
{

    int num=0;
    for(int i=0;i<MAX;i++)
    {
        if(strcmp(file[i].content,content)==0&&file[i].type==type)
        {
            temp[num++]=file[i];
        }
    }

    Sort_Time(temp,num);

    int year=0,month=0,day=0,second=0,hour=0;
    for(int j=0;j<num;j++)
    {
        if(type==DIR)
        {
            printf("DIR    ");
        }
        else
        {
            printf("FILE   ");
        }
        day=temp[j].time.date%32;
        month=(temp[j].time.date/32)%13;
        year=temp[j].time.date/32/13;

        second=temp[j].time.Clock%61;
        hour=temp[j].time.Clock/61;

        printf("%4d-%02d-%02d %2d:%02d    ",year,month,day,hour,second);
        printf("  %10s   ",temp[j].name);

        if(type==FFILE)
        {
            printf("%lf KB  ",temp[j].size/1024.0);
        }
        printf("\n");
    }
}


bool find_dir_name(char name[])
{
    int num=0;
    int day=0,month=0,year=0,second=0,hour=0;
    printf("DIR: \n");
    for(int i=0;i<MAX;i++)
    {
        if(strcmp(name,file[i].name)==0&&file[i].type==DIR)
        {
            printf("DIR     ");
            day=file[i].time.date%32;
            month=(file[i].time.date/32)%13;
            year=file[i].time.date/32/13;

            second=file[i].time.Clock%61;
            hour=file[i].time.Clock/61;

            printf("%4d-%02d-%02d %2d:%02d    ",year,month,day,hour,second);
            printf("  %10s   ",file[i].name);
            printf("  %s   ",file[i].content);

            //printf("%lf KB  ",temp[j].size/1024.0);
            printf("\n");
            num++;
        }
    }
    printf("dir number is %d\n",num);
    return true;
}

bool find_file_name(char name[])
{
    int num=0;
    int day=0,month=0,year=0,second=0,hour=0;
    printf("FILE: \n");
    for(int i=0;i<MAX;i++)
    {
        if(strcmp(name,file[i].name)==0&&file[i].type==FFILE)
        {
            printf("FILE     ");
            day=file[i].time.date%32;
            month=(file[i].time.date/32)%13;
            year=file[i].time.date/32/13;

            second=file[i].time.Clock%61;
            hour=file[i].time.Clock/61;

            printf("%4d-%02d-%02d %2d:%02d    ",year,month,day,hour,second);
            printf("%lf KB  ",file[i].size/1024.0);
            printf("  %10s   ",file[i].name);
            printf("  %s   ",file[i].content);

            printf("\n");
            num++;
        }
    }
    printf("file number is %d\n",num);
    return true;
}


bool find(char name[])
{
    find_dir_name(name);
    printf("\n");
    find_file_name(name);
    printf("\n");
    return true;
}



bool cut_dir(char name[],char name1[])
{
    char content[1000]={0},content1[1000]={0};

    int t1=find_dirfile(name,content,DIR,now_dir);
    if(t1==-1)
    {
        cout <<content<<"-\\"<<name<<" is not exist!"<<endl;
        return false;
    }
    if(t1==-2)
    {
        cout <<content<<"-\\"<<name<<"'s dir is not exist!"<<endl;
        return false;
    }
    int t2=find_dirfile(name1,content1,DIR,now_dir);
    if(t2>=0)
    {
        cout <<content1<<"-\\"<<name1<<" is already exist!"<<endl;
        return false;
    }
    if(t2==-2)
    {
        cout <<content1<<"-\\-"<<name1<<"'s dir is not exist!"<<endl;
        return false;
    }


    int place=find_file_place();
    strcpy(file[place].content,content1);
    strcpy(file[place].name,name1);
    file[place].type=DIR;

    unsigned int date=0,Clock=0;
    getTime(date,Clock);
    file[place].time.Clock=Clock;
    file[place].time.date=date;

    strcat(content,"\\");
    strcat(content,name);
    strcat(content1,"\\");
    strcat(content1,name1);

    char temp[1000]={0};
    char temp1[1000]={0};

    for(int i=0;i<MAX;i++)
    {
        if(strcmp(file[i].content,content)==0)
        {
            if(file[i].type==FFILE)
            {
                strcpy(temp,content);
                strcat(temp,"\\");
                strcat(temp,file[i].name);

                strcpy(temp1,content1);
                strcat(temp1,"\\");
                strcat(temp1,file[i].name);
                cut_file(temp,temp1);
            }
            else if(file[i].type==DIR)
            {
                strcpy(temp,content);
                strcat(temp,"\\");
                strcat(temp,file[i].name);

                strcpy(temp1,content1);
                strcat(temp1,"\\");
                strcat(temp1,file[i].name);
                cut_dir(temp,temp1);
            }
        }
    }
    file[t1].type=UNUSE;
    return true;
}

bool copy_dir(char name[],char name1[])
{
    char content[1000]={0},content1[1000]={0};

    int t1=find_dirfile(name,content,DIR,now_dir);
    if(t1==-1)
    {
        cout <<name<<" is not exist!"<<endl;
        return false;
    }
    if(t1==-2)
    {
        cout <<name<<"'s dir is not exist!"<<endl;
        return false;
    }
    int t2=find_dirfile(name1,content1,DIR,now_dir);
    if(t2>=0)
    {
        cout <<content1<<"--"<<name1<<" is already exist!"<<endl;
        return false;
    }
    if(t2==-2)
    {
        cout <<content1<<"--"<<name1<<"'s dir is not exist!"<<endl;
        return false;
    }


    int place=find_file_place();
    strcpy(file[place].content,content1);
    strcpy(file[place].name,name1);
    file[place].type=DIR;

    unsigned int date=0,Clock=0;
    getTime(date,Clock);
    file[place].time.Clock=Clock;
    file[place].time.date=date;

    strcat(content,"\\");
    strcat(content,name);
    strcat(content1,"\\");
    strcat(content1,name1);

    char temp[1000]={0};
    char temp1[1000]={0};

    for(int i=0;i<MAX;i++)
    {
        if(strcmp(file[i].content,content)==0)
        {
            if(file[i].type==FFILE)
            {
                strcpy(temp,content);
                strcat(temp,"\\");
                strcat(temp,file[i].name);

                strcpy(temp1,content1);
                strcat(temp1,"\\");
                strcat(temp1,file[i].name);
                copy_file(temp,temp1);
            }
            else if(file[i].type==DIR)
            {
                strcpy(temp,content);
                strcat(temp,"\\");
                strcat(temp,file[i].name);

                strcpy(temp1,content1);
                strcat(temp1,"\\");
                strcat(temp1,file[i].name);
                copy_dir(temp,temp1);
            }
        }
    }
    return true;
}

bool copy_file(char name[],char name1[])
{
    char content[1000]={0};
    int t1=find_dirfile(name,content,FFILE,now_dir);
    if(t1==-1)
    {
        cout <<name<<" is not exist!"<<endl;
        return false;
    }
    if(t1==-2)
    {
        cout <<name<<"'s dir is not exist!"<<endl;
        return false;
    }
    int t2=find_dirfile(name1,content,FFILE,now_dir);
    if(t2>=0)
    {
        cout <<name1<<" is already exist!"<<endl;
        return false;
    }
    if(t2==-2)
    {
        cout <<name1<<"'s dir is not exist!"<<endl;
        return false;
    }

    int place=find_file_place();
    strcpy(file[place].content,content);
    strcpy(file[place].name,name1);
    file[place].size=file[t1].size;
    file[place].type=FFILE;

    unsigned int date=0,Clock=0;
    getTime(date,Clock);
    file[place].time.Clock=Clock;
    file[place].time.date=date;


    Block block;
    int index=file[t1].next;
    int index1=find_block_place();
    file[place].next=index1;

    while(index!=UNUSE)
    {

        fseek(fs,index,SEEK_SET);
        fread(&block,sizeof(Block),1,fs);

        int index2=UNUSE;
        if(block.next!=UNUSE)
            index2=find_block_place();

        index=block.next;

        fseek(fs,index1,SEEK_SET);
        block.next=index2;
        fwrite(&block,sizeof(Block),1,fs);

        index1=index2;
    }

    return true;
}

bool add_dir(char name[100])
{
    char content[1000]={0};
    int place=find_dirfile(name,content,FFILE,now_dir);
    if(place>=0)
    {
        cout <<content<<"\\"<<name<<" is already exist!"<<endl;
        return false;
    }
    if(place==-2)
    {
        cout <<content<<"\\"<<name<<"'s dir is not exist!"<<endl;
        return false;
    }

    place=find_file_place();
    strcpy(file[place].name,name);
    file[place].type=DIR;
    unsigned int date=0,Clock=0;
    getTime(date,Clock);
    file[place].time.Clock=Clock;
    file[place].time.date=date;
    strcpy(file[place].content,content);
    show_now();
    return true;
}

bool add_file(char name[],char name1[])
{
    char content[1000]={0};
    int t1=find_dirfile(name,content,FFILE,now_dir);
    if(t1>=0)
    {
        cout <<content<<"\\"<<name<<" is already exist!"<<endl;
        return false;
    }
    if(t1==-2)
    {
        cout <<content<<"\\"<<name<<"'s dir is not exist!"<<endl;
        return false;
    }

    FILE *p=NULL;
    p=fopen(name1,"rb");
    if(p==NULL)
    {
        cout <<name1<<" is not exist!"<<endl<<endl;
        return false;
    }
    fseek(p,0,SEEK_END);

    int place=find_file_place();
    strcpy(file[place].name,name);
    file[place].type=FFILE;
    unsigned int date=0,Clock=0;
    getTime(date,Clock);
    file[place].time.Clock=Clock;
    file[place].time.date=date;
    strcpy(file[place].content,content);

    file[place].size=ftell(p);
    fseek(p,0,SEEK_SET);

    copy_in(p,place);

    show_now();

    return true;
}


bool down_file(char name[],char name1[])
{
    char content[1000]={0};
    int t1=find_dirfile(name,content,FFILE,now_dir);
    if(t1==-1)
    {
        cout <<content<<"\\"<<name<<" is not exist!"<<endl;
        return false;
    }
    if(t1==-2)
    {
        cout <<content<<"\\"<<name<<"'s dir is not exist!"<<endl;
        return false;
    }

    FILE *p=NULL;
    p=fopen(name1,"wb");

    copy_out(p,t1);
    fclose(p);

    return true;
}
int is_file(char name[])
{
    char content[1000]={0};
    char temp[1000]={0};
    strcpy(temp,name);
    int t1=find_dirfile(name,content,FFILE,now_dir);
    if(t1>=0)
        return FFILE;
    int t2=find_dirfile(temp,content,DIR,now_dir);
    if(t2>=0)
        return DIR;

    cout <<t1<<" "<<t2<<endl;

    return UNUSE;
}

bool del(char name[],char dir[])
{
    char temp[1000]={0};
    strcpy(temp,name);
    int type=is_file(temp);
    if(type==FFILE)
        del_file(name,dir);
    else if(type==DIR)
        del_dir(name,dir);
    else
    {
        cout <<name<<" is not exists"<<endl;
        return false;
    }
    return true;
}

bool del_file(char name[],char dir[])
{
    char content[1000]={0};
    int t1=find_dirfile(name,content,FFILE,dir);
    if(t1==-1)
    {
        cout <<content<<"\\"<<name<<" is not exist!"<<endl;
        return false;
    }
    if(t1==-2)
    {
        t1=find_file(name,content,FFILE);
        if(t1==-1)
        {
            cout <<content<<"\\"<<name<<"'s dir is not exist!"<<endl;
            return false;
        }
    }

    file[t1].type=UNUSE;

    Block block;
    int index=file[t1].next;
    while(index!=UNUSE)
    {
        fseek(fs,index,SEEK_SET);
        fread(&block,1,sizeof(Block),fs);
        block.flag=UNUSE;
        fseek(fs,index,SEEK_SET);
        fwrite(&block,1,sizeof(Block),fs);
        index=block.next;
    }

    return true;
}

bool del_dir(char name[],char dir[])
{
    char content[1000]={0};
    int place=find_dirfile(name,content,DIR,dir);
    if(place==-1)
    {
        cout <<content<<"\\"<<name<<" is not exist!"<<endl;
        return false;
    }
    if(place==-2)
    {
        place=find_file(name,content,FFILE);
        if(place==-1)
        {
            cout <<content<<"\\"<<name<<"'s dir is not exist!"<<endl;
            return false;
        }
    }
    char tdir[1000]={0};
    strcpy(tdir,dir);
    strcat(tdir,"\\");
    strcat(tdir,file[place].name);
    for(int i=0;i<MAX;i++)
    {
        if(strcmp(file[i].content,tdir)==0)
        {
            if(file[i].type==DIR)
                del_dir(file[i].name,tdir);
            else if(file[i].type==FFILE)
                del_file(file[i].name,tdir);
        }
    }
    file[place].type=UNUSE;
    return true;
}




bool cut(char name[],char name1[])
{
    char temp[1000]={0};
    strcpy(temp,name);
    int type=is_file(temp);
    if(type==FFILE)
        cut_file(name,name1);
    else if(type==DIR)
        cut_dir(name,name1);
    else
    {
        cout <<name<<" is not exists"<<endl;
        return false;
    }
    return true;
}

bool copy(char name[],char name1[])
{
    char temp[1000]={0};
    strcpy(temp,name);
    int type=is_file(temp);
    if(type==FFILE)
        copy_file(name,name1);
    else if(type==DIR)
        copy_dir(name,name1);
    else
    {
        cout <<name<<" is not exists"<<endl;
        return false;
    }
    return true;
}


int depart(char name[],char dir[])
{
    int l=strlen(name);
    int i=0,c=0;
    for(i=l-1;i>=0;i--)
    {
        if(name[i]=='\\')
            break;
    }
    if(i==-1)
        return 1;

    for(int j=0;j<i;j++)
        dir[j]=name[j];
    dir[i]='\0';
    for(c=i+1;c<l;c++)
        name[c-i-1]=name[c];
    name[l-i-1]='\0';
    return 0;

}

int find_dirfile(char name[],char content[],int type,char n_dir[])
{
    int flag=0;
    char dir[1000]={0};
    strcpy(dir,n_dir);
    depart(name,dir);
    strcpy(content,dir);
    int t=find_file(name,dir,type);
    char dir1[1000]={0};
    int c=depart(dir,dir1);
    if(c==1&&strcmp(dir,"system")==0)
        flag=1;
    else if(c==0&&find_file(dir,dir1,DIR)!=-1)
        flag=1;
    else
        flag=0;

    if(flag==1&&t!=-1)
        return t;
    else if(flag==1&&t==-1)
        return -1;
    else
        return -2;
}

bool cut_file(char name[],char name1[])
{
    char content[1000]={0};
    int t1=find_dirfile(name,content,FFILE,now_dir);
    if(t1==-1)
    {
        cout <<content<<"\\"<<name<<" is not exist!"<<endl;
        return false;
    }
    if(t1==-2)
    {
        cout <<content<<"\\"<<name<<"'s dir is not exist!"<<endl;
        return false;
    }
    int t2=find_dirfile(name1,content,FFILE,now_dir);
    if(t2>=0)
    {
        cout <<content<<"\\"<<name1<<" is already exist!"<<endl;
        return false;
    }
    if(t2==-2)
    {
        cout <<content<<"\\"<<name1<<"'s dir is not exist!"<<endl;
        return false;
    }

    strcpy(file[t1].content,content);
    strcpy(file[t1].name,name1);
    unsigned int date=0,Clock=0;
    getTime(date,Clock);
    file[t1].time.Clock=Clock;
    file[t1].time.date=date;

    return true;
}
