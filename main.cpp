// filesys-new.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "string"
#include "iomanip"
#include "fstream"
using namespace std;


/*Main class for the directory or file*/
class node				
{
public:
	string name;
	node *child;
	node *peer;
	node *parent;
	enum type {file, directory};
	type type1;
	int fno;

	node()    //constructor
	{
		name = "";
		child = NULL;
		peer = NULL;
		parent = NULL;
		type1 = directory;
		fno = 0;
	}

	~node(){};
};



/*Class for the file system which contains functions to operate on file or directory */
class fs
{
	string fsname;
	node *root_dir;
	node *cwd;
	string prompt;
	static int fileno;

public:
	fs()
	{
		fsname = "S:";
		root_dir = new node;
		cwd = new node;
		prompt = fsname;
	}
		
	void mkdir(string);
	void mkfile(string);
	int changedir(string);
	void dir();
	int del(string);
	int counter();
	void showprompt();
	void out();
	void root();

};

int fs::fileno = 0;


/* Create Directory, dir name as argument */
void fs::mkdir(string dname)   
{
	/*Allocate a new node*/
	node *pointer = new node;
	pointer->name = dname;

	/* Insert the new node into the directory list */
	pointer->peer = cwd->child;
	cwd->child = pointer;
	
	pointer->parent = cwd;
	
	/*Change the type as Dir*/
	pointer->type1 = pointer->directory;
	
	/*Assign the fileno */
	pointer->fno = counter();

	/*Save it to disk
	char dname_array[10];
	strcpy(dname_array, dname);
	ofstream outfile(dname_array, ios::binary);
	outfile.write(reinterpret_cast<char*>(pointer), sizeof(*pointer));
	outfile.close();
	ifstream infile(dname, ios::binary);
	infile.read(reinterpret_cast<char*>(pointer), sizeof(*pointer));
	cout<<pointer->name<<endl;*/
	
}

void fs::mkfile(string fname)
{
	/*Allocate a new node*/
	node *pointer = new node;
	pointer->name = fname;

	/*Insert the new node into the directory list */
	pointer->peer = cwd->child;
	cwd->child = pointer;

	pointer->parent = cwd;

	/*Change the type as File */
	pointer->type1 = pointer->file;
	
	/*Assign the fileno*/
	pointer->fno = counter();
	
}

int fs::changedir(string dir_to_change)
{
	/* Create a new node pointer and assign the cwd's child to it*/
	node *pointer = cwd->child;
	
	/*Now, traverse through its peers for matching the dname */
	while (pointer != NULL)
	{
		if (pointer->name == dir_to_change)
		{
			pointer->parent = cwd;
			cwd = pointer;
			/*To display the proper prompt */
			prompt = prompt + "\\" + cwd->name ;
			//cout<<"parent of "<<pointer->name<<" is "<<cwd->name;
			return 0;
			
		}
		pointer = pointer->peer;
	}
	cout<<endl<<"Invalid Directory name";
	return 0;

}

void fs::dir()
{
	/* Create a new node pointer and assign the cwd's child to it*/
	node *pointer = cwd->child;

	/*Now, traverse through its peers and print the names */
	while (pointer != NULL)
	{
		string dir_or_file;
		dir_or_file = ((pointer->type1) == 1)? "<DIR>" : "<File>";
		cout<<dir_or_file<<setw(20)<<pointer->name<<endl;
		pointer = pointer->peer;
	}

}

int fs::del(string name_to_delete)
{
	/* Create a new node pointer and assign the cwd's child to it*/
	node *pointer = cwd->child;
	node *previous = cwd->child;

	/*Now, traverse through its peers for matching the name */
	while (pointer != NULL)
	{
		if (pointer->name == name_to_delete)
		{
			if (previous->name == pointer->name)  //If its the first dir under cwd
				cwd->child = pointer->peer;
			else
				previous->peer = pointer->peer;
			delete pointer;            /* Delete the node using delete()*/
			return 0;
		}
		
		previous = pointer;
		pointer = pointer->peer;
	}

	cout<<endl<<"Invalid name";
	return 0;
}


void fs::out()
{
		node *pointer = cwd->parent;


		if (cwd->parent == NULL)
		{
			return;
		}


	
		int namelength = (cwd->name).length();
		int n = prompt.find(cwd->name);
	
		prompt.erase(n-1, namelength+1);

		cwd = pointer;
}


void fs::root()
{
	cwd = root_dir;
	prompt = fsname;
}


int fs::counter()
{
	fileno++;
	//increment the fileno each time an object is created and return
	return fileno;
}


void fs::showprompt()
{
	cout<<endl<<prompt<<">";

}


/* Command */
#define MKDIR 0
#define MKFILE 1
#define DIR 2
#define CD 3
#define DEL 4
#define EXIT 5
#define OUT 6
#define ROOT 7
#define INVALID -1

int parse_command(string *cmd)
{
	string command[8]= {"mkdir", "mkfile", "dir", "cd", "del", "exit", "cd..", "cd\\"};
	int c = 0;
	
	while (command[c] != *cmd)
	{
		c++;
		if(c>7) return -1; 
	}
	
	return c;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int commandno;  //To parse through the function
	string command;  //The whole command line
	//string prompt;
	string *only_command = new string;  //Separated from name 
	char comd_array[12];  //For storing the command separately
	string input_name;    //Required name from the command line
	int wlen, pos_of_space; //Word length and the position of space
	fs filesys;
	/*Print the opening prompt*/
	filesys.showprompt();

	
	/* Getline commands and parse it through the parse function */

	/* Access commands for operating on the directory and files */
	while (1)
	{
		/* Get a command */
		getline (cin, command, '\n');
		wlen = command.length();  //count the command length
		pos_of_space = command.find(' '); //Find the position of the space
		command.copy(comd_array, pos_of_space, 0); //Copy it to the char array
		if (pos_of_space != string::npos)    
		{
			comd_array[pos_of_space]=0;  //terminate with '\0'
			input_name = command.substr(pos_of_space+1, wlen);
		}
		else							//If it is a single command without name
			comd_array[wlen] = '\0';
		*only_command = comd_array;     //Convert the char array to string object
		
		commandno = parse_command(only_command); //Call the parse function

		/* Print prompt with directory*/
		

		switch (commandno)
		{
			case MKDIR: 
				filesys.mkdir(input_name); 
				break;
			
			case MKFILE: 
				filesys.mkfile(input_name);
				break;

			case DIR: 
				filesys.dir(); 
				break;

			case CD: 
				filesys.changedir(input_name); 
				break;

			case DEL: 
				filesys.del(input_name);
				break;

			case EXIT: 
				return 0; 
				break;
			//case FS: 
			
			case INVALID: 
				cout<<"\nInvalid command\n";
				break;
			
			case OUT: //for cd..
				filesys.out();
				break;

			case ROOT:  /*for cd\   */
				filesys.root();
				break;

			default: 
				cout<<"Invalid command\n";
				break;
		}
		filesys.showprompt();
	}
	return 0;
}