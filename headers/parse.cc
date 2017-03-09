#include "parse.h"

/* ------------------------------------  */
/*           input parser                */
/* ------------------------------------  */
void init_dbinputs(DB_INPUT *input){
  //  input->dbname = (char*) calloc(MAX_LENGTH_INPUT, sizeof(char));
  //  input->dbuser = (char*) calloc(MAX_LENGTH_INPUT, sizeof(char));
   // input->dbhost = (char*) calloc(MAX_LENGTH_INPUT, sizeof(char));    
    input->dbpassword = (char*) calloc(MAX_LENGTH_INPUT, sizeof(char));
}

void free_dbinputs(DB_INPUT *input){
    //free(input->dbname);
    //free(input->dbuser);
    //free(input->dbhost);
    free(input->dbpassword);
}

struct option opts_list[] = {
  {"name",1,0,'n'},
  {"user",1,0,'u'},
  {"host",1,0,'H'},
  {"help",0,0,'h'},
  {0,0,0,0}
};

int parse_input(int argc, char **argv, DB_INPUT *inputs)
{
    int c;

    inputs->dbname = "pucp";
    inputs->dbuser = "imuser";
    inputs->dbhost = "localhost";

    while ( (c = getopt_long(argc, argv, "n:u:H:h", opts_list, NULL)) != EOF){
        switch(c){
            case 'n':
                inputs->dbname = optarg;
                break;

            case 'u':
                inputs->dbuser = optarg;
                break;

            case 'H':
                inputs->dbhost = optarg;
                break;

            case 'h':
                puts("-n, --name:\t database name \n"
                     "-u, --user:\t database username\n"
                     "-H, --host:\t database host"
                     );
                exit(0);
                break;

            default:
                 exit(1);
        }
    }

    getpass(&(inputs->dbpassword));
 
    return 1;  
}

/* ------------------------------------  */
/*         hide password input           */
/* ------------------------------------  */
int getch() {
    int ch;
    struct termios t_old, t_new;

    tcgetattr(STDIN_FILENO, &t_old);
    t_new = t_old;
    t_new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t_new);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
    return ch;
}

void getpass(char **pass )
{
  bool show_asterisk = true;
  const char *prompt = "Insert password";
  const char BACKSPACE=127;
  const char RETURN=10;

  string password;
  unsigned char ch=0;

  cout <<prompt<<endl;

  while((ch=getch())!=RETURN)
    {
       if(ch==BACKSPACE)
         {
            if(password.length()!=0)
              {
                 if(show_asterisk)
                 cout <<"\b \b";
                 password.resize(password.length()-1);
              }
         }
       else
         {
             password+=ch;
             if(show_asterisk)
                 cout <<'*';
         }
    }
  cout <<endl;
  strcpy (*pass, password.c_str());
}




