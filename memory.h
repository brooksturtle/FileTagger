#ifndef MEMORY_H
#define MEMORY_H
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "fileObject.h"
#include "dirent.h"
#include <math.h>

using namespace std;
class memory
{
    private:
        vector<fileObject> memoryVector;

    public:
        memory(vector<fileObject> &);
        void stringifyMemory();
        // need to improve this to search for multiple tags or
        // has certain tags but does not have others
        vector<string> searchForFilesWithTag(string);
        void remove_from_string( string &str, string chars_to_remove );
        //parameters: tag list, file name
        void createFileObject(vector<string>&, string);
        int checkFileExistence(string);
        void addTagToFile(int, string);
        // this currently only deletes one tag input at a time
        void deleteTagFromFile(string, string);
        //void deleteFileObject(string); not sure if needed
        void deleteFileObject(string file);
        // Loads the previously saved file objects to memory. Only to be used once on creation
        bool initializeMemory();
		int getAlphabeticLocation(string fileName, int);

        void printMemory();
        void switchAdd(string, string);
        string lowerCase(string);
        bool deleteDuplicates(string, int index);
        vector<fileObject> getMemory();
		void printTags();
		void list_delete_tag();

};

vector<fileObject> memory::getMemory()
{
	return memoryVector;
}


// passing by reference to make the run cost less
memory::memory(vector<fileObject> &init)
{
    memoryVector = init;
}

bool memory::initializeMemory(){
    bool worked = true;
    ifstream ifs;
    ifs.open("memoryFile.txt", ios::in);

    vector<string> tags;

    string current_line;
    string temp_filename;
    string temp_tag;
	// Opens text file. It is in the order of file: tag1, tag2
	// takes file as temp_filename and tags as a vector for create 
	// FileObject
    if(ifs.is_open()){

        while(getline(ifs, current_line)){
            stringstream ss(current_line);

            ss >> temp_filename;

            while(ss >> temp_tag)
                tags.push_back(temp_tag);

            ss.str("");
            ss.clear();

            remove_from_string(temp_filename, ":");

            createFileObject(tags, temp_filename);
            tags.clear();
        }
    } else {
        worked = false;
    }

    ifs.close();
    return worked;
} 

void memory::list_delete_tag()
{
	 cout<<"What file would you like to delete the tag from?"<<endl;
	 int enormity = memoryVector.size();
	 for (int i =0; i< enormity; i++)
	 {
         cout<<i<<". ";
		 memoryVector[i].printTagList();
	 }
	 cout<<enormity<<". Go Back\n";
	 int input;
	 cin>>input;
	while (input > enormity || input < 0 || cin.fail())
	{
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout<<"Invalid Input"<<endl;
		cin>>input;
	}
	
	vector<string> tag_list;
	// option to exit deletion menu
	if (input == enormity)
	{
		return;
	}
	// need to get tags to make a selection
	else{
		
		cout<<"What tag would you like to delete?"<<endl;
		tag_list= memoryVector[input].getTagList();
		enormity = tag_list.size();
		if (enormity == 0)
		{
			cout<<"This file has no tags"<<endl;
			return;
		}
		for (int i = 0; i < enormity; i++)
		{
			cout<<i<<". "<<tag_list[i]<<endl;
		}
		int tag_choice;
		 cin>>tag_choice;
		while (tag_choice > enormity || tag_choice < 0 || cin.fail())
		{
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout<<"Invalid Input"<<endl;
			cin>>input;
		}
		bool not_empty =memoryVector[input].delete_tag_by_index(tag_choice);
		if (!not_empty)
		{
			memoryVector.erase(memoryVector.begin()+input);
			return;
		}
		}
	return;
	 
}

void memory::deleteFileObject(string file)
{
    int index = checkFileExistence(file);
    if (index != -1)
        memoryVector.erase(memoryVector.begin()+index);

}
/*
    This method checks if any of the tags to be added in the vector are duplicates,
    it also compares to the tagVector of the file object and searches for duplicates.
    if there are any duplicates the method deletes those tags before returning the vector
*/
bool memory::deleteDuplicates(string tag, int index)
{
    bool match;

    /*
        This will check if duplicates of the tags we are trying to add exist
        already within the fileObject. If they do they are added to the duplicate vector
        to be deleted later.
    */
        match = memoryVector[index].checkForTag(tag);

    return match;
}


/*
    This function will take in the tags you want to add to a
    certain file. If that file does not yet exist in the memoryVector,
    then it will be created with createFileObject. Otherwise
    we will continually add tags from the vector in a loop to the
    designated fileObject.
    Assumes that a nonexistent directory file will not be input
*/
// make this a binary add
void memory::switchAdd(string tag, string file)
{	
	vector<string> tags;
	tags.push_back(tag);
	
	int index;
	int place;
	if (memoryVector.size() == 0)
	{
		index = -1;
		place = 0;
	}
	// error if checked when size is zero
	else{
		index = checkFileExistence(file);
	}
	
	
	 place = getAlphabeticLocation(file, 0);
	
    
    if (index == -1)
    {
        createFileObject(tags, file);
    }
    else
    {
        bool duplicate = deleteDuplicates(tag, index);
        if (duplicate)
            return;
        else
            memoryVector[index].addTag(tag);
    }
	cout << "'" << tag << "' was added succesfully." << endl;
}

void memory::remove_from_string( string &str, string chars_to_remove ) {
   for (unsigned int i = 0; i < chars_to_remove.length(); ++i) {
      str.erase(remove(str.begin(), str.end(), chars_to_remove[i]), str.end());
   }
}

// initializes fileObject and the adds it to the memoryVector
// use binary sort to find location to insert in 
void memory::createFileObject(vector<string>& tags, string file)
{
    fileObject cell(tags, file);
	// caveat if no file objects
	if (memoryVector.size() == 0)
	{
		memoryVector.push_back(cell);
		return;
	}
	else{
		// get proposed file Location, use binary
	int insertPoint = getAlphabeticLocation(file, 0);
	memoryVector.insert(memoryVector.begin()+insertPoint, cell);
	}
}


// will be called knowing that file does not exist
// returns the index of where the file should go alphabetically
// i would like to ignore caps
int memory::getAlphabeticLocation(string fileName, int which)
{
	transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);

	int size = memoryVector.size();
	string returnedFile;
	for (int i =0; i < size; i++)
	{
		returnedFile = memoryVector[i].returnFileName();
		transform(returnedFile.begin(), returnedFile.end(), returnedFile.begin(), ::tolower);
		
		// will return if it finds a place where file does not alphabetically fit
		if (fileName< returnedFile)
		{
			return i;
		}
	}
	return size;
}

// iterates through memory vector and calls print function for file object
void memory::printMemory()
{
     int enormity = memoryVector.size();
     int counter = 0;

     // loops through every object in vector
     while (counter < enormity )
     {
         memoryVector[counter].printTagList();
         counter++;
     }
}


// returns index of fileObject within memoryVector
// if the object is not found then this returns -1
// this speeds process of adding tags and also safeguards
// against adding tag to nonexistent file

// this should be binary
int memory::checkFileExistence(string name)
{
	// lower case the strings
	transform(name.begin(), name.end(), name.begin(), ::tolower);
	if (memoryVector.size() == 0)
	{
		return -1;
	}
    int counter = 0;
    int index = -1;
    string returnedName= "";
	
	int bottom = 0;
	int top = memoryVector.size()-1;
	int middle = (bottom + top )/2;
	
	// this search should be O(log n) vs O(n)
	while (1 ==1)
	{
		
		returnedName = memoryVector[middle].returnFileName();
		transform(returnedName.begin(), returnedName.end(), returnedName.begin(), ::tolower);

		if (name == returnedName)
        {
            index = middle;
			break;
        }
	
		// break condition means we have fully searched and found nothing
		if (top == middle || bottom == middle)
		{
			return -1;
		}
		else if (returnedName > name)
		{
			top = middle;
			middle = (top + bottom)/2;
		}
		else if (returnedName < name)
		{
			bottom = middle;
			middle = (top + bottom + 1)/2;
		}
	}

    return index;
}


/*
    Adds tag to file assuming that checkFileExistence has already been
    called and is returning an index.
*/
void memory::addTagToFile(int index, string tag)
{
    memoryVector[index].addTag(tag);

}

/*
    searches through memoryVector for tags existence. If it exist then
    fileObject deleteTag will be called. Otherwise this method will notify
    user that the file they are trying to delete from does not have any tags.

    If within the file object the tag is nowhere to be found then this method
    (memory::deleteTagFromFile) will notify user.

    If the deleteTag is successful and there are no more tags within that
    fileObjects tagList, this method will call a deletefileObject method to remove
    that fileObject from the memoryVector and free its space.
*/
void memory::deleteTagFromFile(string file, string tag)
{
    int index = checkFileExistence(file);
    if (index == -1)
    {
        cout<<"That file either doesn't exist or does not have any tags in it. Returning to menu.";
        cout<<endl<<endl;
        return;
    }

    bool tagExist;

    tagExist= memoryVector[index].checkForTag(tag);

    if (!tagExist)
    {
        cout<<"That file does not have tag '"<<tag<<"' to delete. Returning to menu.";
        cout<<endl<<endl;
        return;
    }
}


vector<string> memory::searchForFilesWithTag(string tag)
{
    int counter = 0;
    int enormity = memoryVector.size();
    bool tagExist;
    string file;

    vector<string> fileList;

    while (counter < enormity)
    {
        tagExist= memoryVector[counter].checkForTag(tag);

        if (tagExist)
        {
            file = memoryVector[counter].returnFileName();
            fileList.push_back(file);
        }
        counter++;
    }

    return fileList;
}
/*
    goes through the memory vector and calls stringify fileObject for each element
    This prints the fileObjects name and tags to a file called memoryFile.txt.

    The file will be used to initialize the new memoryVector on startup.

    This method should be called only when the program is closing down, and assumes
    the user will terminate the program through an input command and not just x out.
    IF THE PROGRAM IS NOT SHUT DOWN CORRECTLY WE WILL LOSE ANY CHANGES MADE.

    so it may be necessary to perform this function at regular intervals to save changes
    and also notify the user of the importance of exiting the program correctly
*/
void memory::stringifyMemory()
{
    // this wipes all of the previous text in the file
    // avoids rewriting same data to file
    ofstream memoryFile;

    memoryFile.open("memoryFile.txt", std::ofstream::out | std::ofstream::trunc);
    memoryFile.close();

    int counter = 0;
    int enormity = memoryVector.size();

    while (counter < enormity)
    {
        memoryVector[counter].stringifyFileObject();
        counter++;
    }

}

// takes all the tags to be added and changes them to lower case
string memory::lowerCase(string tag)
{
    //takes string from vector, changes it to lower case, then replaces
    // it with the old string
        string myTag = tag;

        transform(myTag.begin(), myTag.end(), myTag.begin(), ::tolower);

        tag = myTag;

    return tag;
}



#endif // MEMORY_H
