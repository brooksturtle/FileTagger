#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <limits>
#include <Windows.h>
#include <vector>
#include <dirent.h> // directory header
#include <stdio.h> // printf()
#include <stdlib.h> // exit()
#include <unistd.h>
#include <sys/stat.h>
#include <iomanip>
#include <cstdio> // for the snippet
#include "fileObject.h"
#include "memory.h"
#include "dirent.h"
#include "FileIO.h"
#include "fileObject.h"



using namespace std;
vector<string> splitString(string);
string traverseDirectories();
int main()
{

    vector<string> stuff;

    vector<fileObject> init;
    FileIO fio;
    memory mem(init);

    if(!mem.initializeMemory()){
        cout << "Error reading memory. Shutting down." << endl;
        return 1;
    }

    fio.sync(mem);

    int choice;
    string tag1;
    string tag;
    string file;
    bool keepGoing = true;
    char y[1000];
    vector<string> fileList;
    int counter;
    int enormity;
    string decision = "x";



    while(keepGoing)
    {
        //1 addtag, 2 deleteTag, 3 print by file, 4 search, 5 stringifyMemory, 6 quit
        cout<<"1. Add a tag\n2. Delete a tag from file\n3. Print by File \n4. Search\n5. Run Files by Tag\n6. Save Changes\n7. Quit"<<endl;
        cin>>choice;
        while (choice > 7 || choice < 1 || cin.fail())
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout<<"Invalid Input"<<endl;
            cin>>choice;
        }

        switch(choice)
        // takes one string at a time.
        {
			int checker;
            case 1: {
                string fileNameAdd;
                cout<<"To what file would you like to add a tag to?"<<endl;
				// This is where we will add the traversing directories!
				fileNameAdd= traverseDirectories();
                cout<<"What tags would you like to add?"<<endl;
                // this takes in whole line cin as a string tag
                // converts char array into vector of individual words.
                cin>>tag1;
                cin.getline(y, 1000);
                tag = y;
                tag = tag1+ tag;
                stuff = splitString(tag);

                /// takes one string at a time
                for (unsigned int i =0; i<stuff.size(); i++)
                {
                        mem.switchAdd(stuff[i], fileNameAdd);
                }
                break;
			}
            case 2: {
                mem.list_delete_tag();
                break;
			}

            case 3: {
                mem.printMemory();
                break;
			}

            case 4: {
                cout<<"What tag would you like to search for?"<<endl;
                string tagSearch;
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                getline(cin, tagSearch);
                fileList = mem.searchForFilesWithTag(tagSearch);
                if(fileList.size() == 0)
                {
					cout << "There are no files with that tag. Returning to menu." << endl;
					break;
				}

                counter = 0;
                enormity = fileList.size();
                cout<<"Files with tag '"<<tagSearch<<"': "<<endl;
                while (counter < enormity)
                {
                    cout<<fileList[counter]<<endl;
                    counter++;
                }
                break;
			}

           
            case 5:
                {
					vector<string> toRun;
                string runTag;
                cout << "What tag would you like to open?" << endl;
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                getline(cin, runTag);
				toRun = mem.searchForFilesWithTag(runTag);
				int toRunSize = toRun.size();
				if (toRunSize == 0)
				{
					cout<<"There are no files with that tag"<<endl;
					break;
				}
				else{
					for (int i = 0; i < toRun.size(); i++)
					{
						const char* why = toRun[i].c_str();
						system(why);
					}
					break;
				}
                }
				
			 case 6: {
                mem.stringifyMemory();
                cout << "Tags and file(s) saved!" << endl;
                break;
			}
			
            case 7: {
                keepGoing=false;
                cout<<"Save Changes? y/n"<<endl;
                while ((decision != "y"  && decision != "n") || cin.fail())
            {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin>>decision;
                transform(decision.begin(), decision.end(), decision.begin(), ::tolower);
                if (decision != "y" && decision!= "n")
                    cout<<"Invalid Input"<<endl;
            }
            if (decision=="y")
            {
                mem.stringifyMemory();
            }
                cout << "Closing program.";
                break;
			}
        }
        cout<<endl;
    }

    return 0;
}
// counter is counter - 2
//traverse Directories is called on add Tag, it shows current directory 
//and allows user to choose a file to add a tag too or move to a different
//directory and choose another file
string traverseDirectories()
{
	DIR *pdir = NULL; 
	struct dirent *pent = NULL;
	
	top:
	cout<<"\nYou are currently in Directory:\n"<<getcwd(NULL, 0)<<":\n\n";
	pdir = opendir ("."); // "." will refer to the current directory
	int counter = 1;
	int input = -1;
	string fileName = "";
	string file_selected= "";
    vector<string> directoryFiles;
	directoryFiles.push_back("0Filler");
	string slash = "/";
	
	vector<pair<int, string>> folders;
	
	if (pdir == NULL) // if pdir wasn't initialised correctly
    { // print an error message and exit the program
        printf ("\nERROR! pdir could not be initialised correctly");
        exit (1); // exit the program, using 1 as the status (most common for a failed execution)
    } // end if
	cout<<"0: Move Back a Directory\n";
	while (pent = readdir (pdir)) // while there is still something in the directory to list
    {
	  if (pent == NULL) // if pent has not been initialised correctly
	  { // print an error message, and exit the program
		  printf ("ERROR! pent could not be initialised correctly");
		  exit (3);
	  }
	  
	  // otherwise, it was initialised correctly. let's print it on the console:
	  if (counter == 1 || counter == 2)
	  {}
	  else
	  {
		//Checks if File is folder Set Up  
		fileName= pent->d_name;  
		
		 string file = pent->d_name;
		  string slash = "/";
		  string pathname = getcwd(NULL, 0) + slash + file;
		  const char *converted_pathname = pathname.c_str();
		  struct stat sb;
		  
		//Check if the file is a folder  
		if (stat(converted_pathname, &sb) == 0 && S_ISDIR(sb.st_mode)){
			cout<<counter-2<<": Folder# "<<fileName<<endl;
			directoryFiles.push_back(": Folder# "+ fileName);
			
		 }  
		else{
			cout<<counter-2<<": "<<fileName<<endl;
			directoryFiles.push_back(fileName);
		}
	  }
	  counter++;
    }
    closedir (pdir);
	cout<<"Selection number?: ";
	cin>> input;
	while (input > counter-2 || input < 0 || cin.fail())
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout<<"Invalid Input: ";
            cin>>input;
        }
	if (input == 0)
	{
		chdir ("..\\");
		goto top;
	}
	else if(directoryFiles.at(input).substr(2,7) == "Folder#")
	{
		string folderChange = "./"+directoryFiles.at(input).substr(10);
		const char* newfolderChange = folderChange.c_str();
		cout<<endl;
		chdir(newfolderChange);
		goto top;
	}
	
	else
	{
		file_selected = getcwd(NULL, 0)+ slash+ directoryFiles.at(input);
	}
	cout<<"\n"<<file_selected<<"\n";
	return file_selected;
}

vector<string> splitString(string tagString)
 {
    string buf; // Have a buffer string
    stringstream ss(tagString); // Insert the string into a stream

    vector<string> tags; // Create vector to hold our words

    while (ss >> buf)
        tags.push_back(buf);

     return tags;
}
