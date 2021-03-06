#ifndef FILEOBJECT_H
#define FILEOBJECT_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

class fileObject
{
    /*
        fileObject will store two data types.
        the tagVector will keep all of the tags asscoiated with the object
        that were added by the user.
        The fileName is how we will identify the object and and its corresponding
        file, this will be used to identify it within the memoryVector
        and also looking for it within the directory
    */
    private:
        vector<string> tagList;
        string fileName;

    public:
        fileObject(vector<string>&, string);
        void addTag(string);
        bool deleteTag(string);
        void printTagList();
        bool checkForTag(string tag);
        void stringifyFileObject();
        string returnFileName();
        vector<string>& getTagList();
        string commaChecker(string checkString);
		bool delete_tag_by_index(int index);

};

// constructor if this is a file with no tags yet
fileObject::fileObject(vector<string>& newVectorTag, string newFileName)
{
    tagList = newVectorTag;
    fileName= newFileName;
}

vector<string>& fileObject::getTagList()
{
    return tagList;
}

string fileObject::returnFileName()
{
    return fileName;
}

// this class will add a new tag to the vectorTag
void fileObject::addTag(string tag)
{
    tag = commaChecker(tag);
    tagList.push_back(tag);
}

//simply gets rid of comma from a tag
string fileObject::commaChecker(string tag)
{
    int position;
    int theEnd = tag.length()-1;
    position= tag.find(',', theEnd);
    if (position == theEnd)
    {
        tag= tag.substr(0, theEnd);
    }
    return tag;
}

/*
    this will search through the vectorTag for a string called tag
    and delete it.
    we will also check if the vector tag is empty, if it is we will
    will return true so that the memoryVector class can remove the unneeded
    fileObject
*/
// does this tag actually exist, need a safe guard either as a class here or within
// memoryVector
bool fileObject::deleteTag(string tag)
{
    // search through tag and gets specific index value for the tag
    int enormity = tagList.size();
    int index = 0;
    int counter = 0;

    while (counter < enormity)
    {
        if (tagList[counter] == tag)
        {
            index = counter;
        }
        counter++;
    }
    //cout<<"index: "<<index<<endl;

    // need to do the begin thing, index by itself does not work
    tagList.erase(tagList.begin()+ index);

    // now we will check and return if vectorTag is empty
    bool isEmpty = false;

    if (tagList.size()== 0)
    {
        isEmpty = true;
    }

    return isEmpty;

	}

bool fileObject::delete_tag_by_index(int index)
{
	string goner;
	try{
		goner=tagList[index];
		tagList.erase(tagList.begin()+ index);
		cout<<goner<<" deleted\n";
		if (tagList.size() == 0)
		{
			return false;
		}
		else{
			return true;
		}
	}
	
	catch(exception& e)
	{
		cout<<"Invalid Index\n";
		return true;
	}
	
}

// this is going to print out all the tags- duh
void fileObject::printTagList()
{
    int enormity = tagList.size();

    int counter = 0;
    cout<<fileName<<": ";
    while (counter < enormity)
    {
        // if deals with extra comma at end
        if (counter == enormity -1)
        {
            cout<<tagList[counter];
            break;
        }
        cout<<tagList[counter]<< ", ";
        counter++;

    }
    cout<<endl;

}

// this will search through tagList and return true if the tag exist,
// part of a failsafe to avoid deleting nonexistent tags or adding multiples
// of the same tag
// returns true if tag exist
bool fileObject::checkForTag(string tag)
{
    int enormity = tagList.size();
    int counter = 0;
    bool tagExist = false;

    while (counter < enormity)
    {
        if (tagList[counter]==tag)
        {
            tagExist = true;
        }
        counter++;
    }

    return tagExist;
}

// add file name and vector tag to a file
// this will be how we store tags when program is closed
// on opening, the program will read the file and new fileObjects initialized
void fileObject::stringifyFileObject()
{
    ofstream memoryFile;
    memoryFile.open ("memoryFile.txt", fstream::in | fstream::out | fstream::app);

    int enormity = tagList.size();
    int counter = 0;

    memoryFile<<fileName<<": ";

    while (counter< enormity)
    {
        memoryFile<<tagList[counter]<<" ";
        counter++;
    }
    memoryFile<<endl;
    memoryFile.close();
}


#endif // FILEOBJECT_H
