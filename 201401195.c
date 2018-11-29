#include <sys/utsname.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <wait.h>

int len1,bg=0;
char *home;
char *readl();
char **splitcommandbyspace(char *commandline);
char **splitcommandbypipe(char *commandline);
char **splitcommandbysemicolon(char *commandline);
int execute(char arguments[]);
void prompt();
int run(char *arguments[]);
int input = 0,last=0;
int first = 1,np=0;


int main()
{
	char cwd1[1024];
	//The getcwd() function places an absolute pathname of the current working directory in the array pointed to by cwd1, 
	//and return cwd1. The second argument is the size in bytes of the character array pointed to by the cwd1 argument. 
	//If cwd1 is a null pointer, the behavior of getcwd() is unspecified.
	home=getcwd(cwd1,sizeof(cwd1));
	len1=strlen(home);	
	prompt();
	return 0;
}


void prompt()
{
	/* "utsname" is a structure[struct] containing following members:
	char  sysname[]  Name of this implementation of the operating system. 
	char  nodename[] Name of this node within the communications network to which this node is attached, if any. 
	char  release[]  Current release level of this implementation. 
	char  version[]  Current version level of this release. 
	char  machine[]  Name of the hardware type on which the system is running. */
	struct utsname name;
	// "uname" gets the name of current system
	uname(&name);
	int status=1;
	char **arguments,**ar2; 
	char *commandline;	
	while(status==1)
	{	
		char cwd[1024],j;
		char *getp;	
		getp=getcwd(cwd,sizeof(cwd));
		int len2=strlen(getp);
		/* getlogin() returns a pointer to a string containing the name of the user
		logged in on the controlling terminal of the process,
		or a NULL pointer if this information cannot be determined. */
		
		printf("%s@%s:",getlogin(),name.sysname);    // prints command prompt Eg: "ramya@lenovo"
		printf("~");                                 
		int v;                                       
                                                             
		for(v=len1;v<len2;v++)                       //
		{                                            //
			printf("%c",getp[v]);	             //    All this is about appending command prompt with current location
		}                                            //
		printf("$");                                 //
		/*if(strcmp(home,getp)))   
		  {
		//printf("dfs");
		printf("%s@%s:%s$",getlogin(),name.sysname,getp);
		}
		else
		{
		//	printf("sc");
		printf("%s@%s:~$",getlogin(),name.sysname);
		}*/



		commandline = readl(); // Takes input as a String. Input is the command in terminal Eg: "cd Documents; mkdir newDocument"
		arguments = splitcommandbysemicolon(commandline); // returns array of strings(commands) seperated by ";"
		j=0;
		int o,count=0;;
		
		for(j=0;arguments[j]!='\0';j++) // for each argument in the array of arguments split by ";"
		{
			ar2=splitcommandbypipe(arguments[j]); // argument is further split by pipe ( | ) Eg: "ls -l | grep "\.txt$""
			for(o=0;ar2[o]!=NULL;o++) // for each part of the argument Eg: here part1 is "ls -l"
			{
				if(np==1)
				{
				//	printf("first");
					first=1;
					last=1;
				}
				else if(o==np-1)
				{
				//	printf("second");
					first=0;
					last=1;
				}
				else if(o==0)
				{
				//	printf("third");
					first=1;
					last=0;
				}
				else
				{
					printf("forth");
					first=0;
					last=0;
				}
				//printf("firt=%d,last=%d,input=%d",first,last,input);
				status = execute(ar2[o]);
				;
			bg=0;
			}
			
		}
		input=0;
		free(arguments);
		free(commandline);
	}	
}
// Takes input from user as a String and returns that String. (the command that we write on command prompt)
char *readl()
{
	int i=0,c,bfsize=1024;
	char *strarray = malloc(sizeof(char) * bfsize);
	while (1)
	{
		c = getchar();
		if (c == EOF || c == '\n') 
		{
			strarray[i] = '\0';
			return strarray;
		} 
		else 
			strarray[i] = c;
		i++;
		if (i >= bfsize) 
		{
			bfsize = bfsize+1024;
			strarray = realloc(strarray, bfsize);
			if (strarray==NULL) 
			{
				perror("Error message: allocation error\n");
				exit(-1);
			}
		}
	}
}

// splits command with semicolon as delimiter. Returns an array of Strings. 
// Eg: if commandline="cd Documents; mkdir newDocument; cd newDocument" ,
//  This function returns an of array of strings {"cd Documents", "mkdir newDocument", "cd newDocument"}
char **splitcommandbysemicolon(char *commandline)
{
	int bfsize =256,i = 0;
	char **tokens = malloc(sizeof(char*) * bfsize), *token, **temp;
	token = strtok(commandline,";");
	while(token != NULL) 
	{
		tokens[i] = token;
		//printf("dsg");
		i++;
		if (i>= bfsize)
		{
			bfsize = bfsize+256;
			temp = tokens;
			tokens = realloc(tokens, bfsize * sizeof(char*));
			if (tokens!=NULL) 
			{
				free(temp);
				perror("Error message: allocation error\n");
				exit(-1);
			}
		}

		token = strtok(NULL, ";");

	}
	tokens[i] = NULL;
	return tokens;
}
// Similar to splitcommandbysemicolon  at line 158. This function splits by pipe ( | ) as delimiter.
char **splitcommandbypipe(char *commandline)
{
	int bfsize =256,i = 0;
	char **tokens = malloc(sizeof(char*) * bfsize), *token, **temp;
	token = strtok(commandline,"|");
	while(token != NULL) 
	{
		tokens[i] = token;
		//printf("dsg");
		i++;
		if (i>= bfsize)
		{
			bfsize = bfsize+256;
			temp = tokens;
			tokens = realloc(tokens, bfsize * sizeof(char*));
			if (tokens!=NULL) 
			{
				free(temp);
				perror("Error message: allocation error\n");
				exit(-1);
			}
		}

		token = strtok(NULL, "|");

	}
	tokens[i] = NULL;
	np=i;
	return tokens;
}

// Similar to splitcommandbysemicolon  at line 158. This function splits by space as delimiter.
char **splitcommandbyspace(char *commandline)
{
	int bfsize =256,i = 0;
	char **tokens = malloc(sizeof(char*) * bfsize), *token, **temp;
	token = strtok(commandline," ");
	while(token != NULL) 
	{
		tokens[i] = token;
		//printf("dsg");
		i++;
		if (i>= bfsize)
		{
			bfsize = bfsize+256;
			temp = tokens;
			tokens = realloc(tokens, bfsize * sizeof(char*));
			if (tokens!=NULL) 
			{
				free(temp);
				perror("Error message: allocation error\n");
				exit(-1);
			}
		}

		token = strtok(NULL, " ");

	}
	if(strcmp(tokens[i-1], "&") == 0)
	{
		bg=1;
		tokens[i-1] = NULL;
	}
	else
		tokens[i] = NULL;

	return tokens;
}
int execute(char argument[])
{
	char **arguments;
	arguments=splitcommandbyspace(argument);
	if (arguments[0] == NULL) 
		return 1;
	else
	{
		if(strcmp(arguments[0],"exit")==0)
			return 0;
		else if(strcmp(arguments[0],"cd")==0)
			return runcd(arguments);
	}
	return run(arguments);
}


int run(char *arguments[])
{

	pid_t pid;
	int lt=0,gt=0,input_file_index=0,output_file_index=0,aoutput_file_index=0,at=0;
	int status=0,i,j,f,f2,f3;
	char *input_file;
	char *output_file;
	int pi[2];
	pipe(pi);
	//To create a simple pipe with C, we make use of the pipe() system call.
	//It takes a single argument, which is an array of two integers, and if successful, the array will contain two new file descriptors to be used for the pipeline.
	//After creating a pipe, the process typically spawns a new process (remember the child inherits open file descriptors).
	pid = fork();

	if (pid == 0) 
	{
		
		 if (first == 1 && last == 0 && input == 0) 
		 {
			dup2( pi[1], 1 );
		 }
		 else if (first == 0 && last == 0 && input != 0) 
		 {
			dup2(input, 0);
			dup2(pi[1], 1);
		}
		else
		{
			dup2( input,0);
		}
		
		for(i=0;arguments[i]!=NULL;i++)
		{
			if(strcmp(arguments[i],"<")==0)
			{
				lt=1;
				input_file_index=i+1;
				
			}
			if(strcmp(arguments[i],">")==0)
			{
				gt=1;
				output_file_index=i+1;
			}
			if(strcmp(arguments[i],">>")==0)
			{
				at=1;
				aoutput_file_index=i+1;
			}
		}
		if(lt==1)
		{
			
			
			//dup(0);
			if( access( arguments[input_file_index], F_OK ) != -1 )
			{ 
			f = open(arguments[input_file_index],O_RDONLY);
			dup2(f,0);
			close(f);
			arguments[input_file_index-1]=NULL;
			}
			
			
			else
			{
			strcpy(arguments[input_file_index-1],arguments[input_file_index]);
			arguments[input_file_index]=NULL;
			}
			
		}

		if(at==1)
		{
			
			arguments[aoutput_file_index-1]=NULL;
			//dup(1);
			f3 = open(arguments[aoutput_file_index],O_RDONLY | O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
			dup2(f3,1);
			close(f3);
			
		}
		if(gt==1)
		{
			arguments[output_file_index-1]=NULL;
			//dup(1);
			f2 = open(arguments[output_file_index],O_RDONLY | O_WRONLY | O_CREAT, S_IRWXU);
			dup2(f2,1);
			close(f2);
		}
		
		if (execvp(arguments[0], arguments) == -1)
		{
			//	printf("DSFG");
			perror("Error message");
		}	
		exit(-1);
	} 
	else if (pid < 0)

		perror("Error message");

	else 
	{
		if(bg==0)
		{
			do {
				waitpid(pid, &status, WUNTRACED);
			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		}
		if (input != 0) 
	close(input);
 
		close(pi[1]);
 
		if (last == 1)
			close(pi[0]);
	input=pi[0];
		
	}
	
		
	
	return 1;
}
int runcd(char *arguments[])
{
	if (arguments[1] == NULL)

		fprintf(stderr, "Error: expected argument to \"cd\" \n");

	else if (chdir(arguments[1]) != 0)  // Changes directory to  arguments[1] "cd Desktop", arguments[1] is Desktop
		perror(arguments[1]);


	return 1;
}
