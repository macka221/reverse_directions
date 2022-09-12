#include <iostream>
#include <fstream>
#include <limits.h>

#define HEADER "Directions "
#define ERROR INT_MIN
#define STACK_SIZE 10
#define INFILE_NAME "forward_direction.txt"
#define OUTFILE_NAME "return_direction.txt"

using namespace std;

enum COMPASS {EAST, NORTH, WEST, SOUTH};

template <class XType>
class Stack
{
    public:
        Stack();
        ~Stack();
        bool Is_Empty() const;
        bool Is_Full() const;
        void Push( XType );
        void Pop();
        XType Top() const;
        int Size() const;

    private:
        struct Stack_Node
        {
            XType Element;
            Stack_Node* next;
        };
        typedef Stack_Node *Stack_Ptr;

        XType element_Array[STACK_SIZE];
        Stack_Ptr tos;
        int s_size;
};

struct DIR{
    string instructions;
    COMPASS returnCompass;
};

string subString(const string[]);//args: direction
string swapCompass(string[], COMPASS);//args: direction, oldCompass

string determineCompass(const string[]);//args: instruction
string compassToStr(COMPASS);//args: currentCompass
string swapStreets(string, string);// args: instruction1, instruction2
string changeArrive(string[], COMPASS);// args: startInstruction, direction
string return_substring(const string, int=0, int=0);// args: str, sIndex, fIndex
int countSpaces(const string);// args: instruction
void GrabDirection(string, Stack<DIR>&);//args: fileDirections, directionStack
void reverseDirections(ofstream&, int, Stack<DIR>&);//args: ReturnFile, directionsStack
void splitString(string[], const string);//args: instruction
void setInstruction(string, DIR&);// instruction, dirValue
void strToCompass(string, COMPASS&);// arg: direction, currentCompass
void changeCompass(const string, COMPASS&);// args: LR, currCompass
void swapLR(string[], string);//args: direction, LR

int main()
{
    Stack<DIR> directions;

    GrabDirection(INFILE_NAME, directions);

    cout << "Process Completed." << endl;

    return 0;
}

template<class XType>
Stack<XType>::Stack()
{
    s_size = 0;
    tos = nullptr;
}

template <class XType>
Stack<XType>::~Stack()
{
    s_size = 0;
    tos = nullptr;
}

template <class XType>
bool Stack<XType>::Is_Empty() const
{
    return (s_size == 0) && (tos == nullptr);
}

template<class XType>
bool Stack<XType>::Is_Full() const
{
    return false;
}

template<class XType>
void Stack<XType>::Push(XType value)
{
    if (!tos && s_size < STACK_SIZE)
    {
        element_Array[s_size] = value;
        //s_size++;
    }
    else
    {
        Stack_Ptr SPtr = new Stack_Node;

        SPtr->Element = value;
        if (!tos)
        {
            tos = SPtr;
        }
        else
        {
            SPtr->next = tos;
            tos = SPtr;
        }
        //s_size++;
    }

    s_size++;
}

template<class XType>
void Stack<XType>::Pop()
{

    if (!this->Is_Empty())
    {
        if (tos)
        {
            (tos->next) ? tos = tos->next : tos = nullptr;
        }
        s_size--;
    }
    else
    {
        cout << "Stack is empty. Pop Operation ignored." << endl;
    }
}

template<class XType>
XType Stack<XType>::Top() const
{
    if (!this->Is_Empty())
    {
        if (!tos)
        {
            return element_Array[s_size - 1];
        }
        else
        {
            return tos->Element;
        }
    }
    else
    {
        cout << "Stack is empty!"
             << "Top operation aborted.\n";
    }
}

template<class XType>
int Stack<XType>::Size() const
{
    return s_size;
}


int countSpaces(const string instruction)
{
    int counter = 1;

    for (int i = 0; i < instruction.size(); i++)
    {
        if (instruction[i] == ' ')
        {
            counter++;
        }
    }

    return counter + 1;
}


string subString(const string direction[])
{
    string currentDir;

    if (direction[0] == "Head" || direction[0] == "Turn")
    {
        currentDir = direction[1];
    }
    else if (direction[0] == "Continue" || direction[0] == "Arrive")
    {
        currentDir = direction[0];
    }
    else
    {
        cout << "Edge Case: direction[0] = " << direction[0] << endl;
        currentDir = direction[0];
    }

    return currentDir;
}

string swapCompass(string directions[], COMPASS oldCompass)
{
    string newDirection;
    int len = sizeof(directions) + 1;

    if (directions[0] == "Arrive")
    {
        newDirection = "Head ";
    }
    else if (directions[0] == "Head")
    {
        newDirection = "Arrive at ";
    }

    if (newDirection == "Head ")
    {
        switch(oldCompass)
        {
            case EAST:
            {
                newDirection = newDirection + "west from ";
                break;
            }
            case NORTH:
            {
                newDirection = newDirection + "south from ";
                break;
            }
            case WEST:
            {
                newDirection = newDirection + "east from ";
                break;
            }
            case SOUTH:
            {
                newDirection = newDirection + "north from ";
                break;
            }
        }

        for (int i = 2; i <= len; i++)
        {
            newDirection = newDirection + directions[i] + " ";
        }

    }
    else if (newDirection == "Arrive at ")
    {
        for (int i = 3; i <= len; i++)
        {
            newDirection = newDirection + directions[i] + " ";
        }
    }

    return newDirection;
}

void splitString(string instruction[], const string direction)
{
    string temp;
    int index = 0;

    for(int i = 0; i < direction.size(); i++)
    {
        if (direction[i] == ' ')
        {
            instruction[index] = temp;
            index++;
            temp = "";
        }
        else if (i == direction.size() - 1)
        {
            temp = temp + direction[i];
            instruction[index] = temp;
        }
        else
        {
            temp = temp + direction[i];
        }
    }
}

void GrabDirection(string inFileName, Stack<DIR> &directions)
{
    string currentLine;
    int totalInstructions = 1;
    int wordCount;
    ifstream inFile;
    ofstream outFile;

    outFile.open(OUTFILE_NAME, ios::out);
    inFile.open(inFileName);
    int counter = 1;

    while (totalInstructions != 0)
    {
        int i = 0;
        string temp;
        DIR newValue, topVal;

        getline(inFile, temp);
        totalInstructions = stoi(temp);
        COMPASS newDir;


        while (i < totalInstructions)
        {
            getline(inFile, currentLine);
            wordCount = countSpaces(currentLine);
            string listDirection[wordCount];
            splitString(listDirection, currentLine);
            string subStr = subString(listDirection);

            if (listDirection[0] == "Head")
            {
                //creates new starting compass.
                newValue.instructions = currentLine;
                strToCompass(subStr, newDir);
                newValue.returnCompass = newDir;
            }
            else if (subStr == "left" || subStr == "right")
            {
                //changes current compass based on LR
                changeCompass(subStr, newDir);
                newValue.instructions = currentLine;
                newValue.returnCompass = newDir;
            }
            else
            {
                //maintains current compass.
                newValue.instructions = currentLine;
                newValue.returnCompass = newDir;
            }
            directions.Push(newValue);
            i++;
        }

        if (totalInstructions != 0)
        {
            reverseDirections(outFile, counter, directions);
            counter++;
        }
    }

    inFile.close();
    outFile.close();
}

void reverseDirections(ofstream &outFile, int iter, Stack<DIR> &directions)
{
    string Arrival;
    DIR currentInstruction, nextInstruction;
    int currentRoute = 1;

    outFile << HEADER << iter << ':' << endl;

    while (!directions.Is_Empty())
    {
        currentInstruction = directions.Top();
        directions.Pop();

        int words = countSpaces(currentInstruction.instructions);
        string direct1[words + 1];
        splitString(direct1, currentInstruction.instructions);
        string LR_Compass = subString(direct1);

        if (direct1[0] == "Arrive" || direct1[0] == "Head")
        {
            currentInstruction.instructions = swapCompass(direct1, currentInstruction.returnCompass);
        }
        else if (!directions.Is_Empty())
        {
            nextInstruction = directions.Top();
            currentInstruction.instructions = swapStreets(currentInstruction.instructions, nextInstruction.instructions);
        }

        outFile << currentInstruction.instructions << endl;
    }

    outFile << endl;

}


string compassToStr(COMPASS currentCompass)
{
    string strCompass;

    switch (currentCompass)
    {
    case EAST:
        {
            strCompass = "east";
            break;
        }
    case NORTH:
        {
            strCompass = "north";
            break;
        }
    case  WEST:
        {
            strCompass = "west";
            break;
        }
    case SOUTH:
        {
            strCompass = "south";
            break;
        }
    }

    return strCompass;
}

void strToCompass(string direction, COMPASS &currentCompass)
{
    if (direction == "north")
    {
        currentCompass = NORTH;
    }
    else if (direction == "south")
    {
        currentCompass = SOUTH;
    }
    else if (direction == "west")
    {
        currentCompass = WEST;
    }
    else
    {
        currentCompass = EAST;
    }
}


void changeCompass(const string LR, COMPASS &currCompass)
{
    switch (currCompass)
    {
    case NORTH:
        {
            (LR == "left") ? currCompass = EAST: currCompass = WEST;
            break;
        }
    case EAST:
        {
            (LR == "left") ? currCompass = SOUTH: currCompass = NORTH;
            break;
        }
    case WEST:
        {
            (LR == "left") ? currCompass = NORTH: currCompass = SOUTH;
            break;
        }
    case SOUTH:
        {
            (LR == "left") ? currCompass = WEST: currCompass = EAST;
            break;
        }
    }
}

void swapLR(string directions[], string LR)
{
    (LR == "left") ? directions[2] = "right" : directions[2] = "left";
}

string swapStreets(string instruction1, string instruction2)
{
    string newDirection, subStr;
    int sIndex;
    char caseDirection = instruction2[0];

    switch (caseDirection)
    {
        case 'T':
            {
                subStr = return_substring(instruction2, 0, 6);
                if (subStr == "Turn l")
                {
                    sIndex = 10;
                }
                else
                {
                    sIndex = 11;
                }

                break;
            }
        case 'H':
            {
                int len = instruction2.size();
                string instructionArray[len], temp;
                splitString(instructionArray, instruction2);
                (instruction2[5] == 'n' || instruction2[5] == 's') ? sIndex = 15 : sIndex = 14;
                sIndex = instructionArray[3].size() + sIndex + 2;

                break;
            }
        case 'C':
            {
                sIndex = 12;

                break;
            }
    }

    subStr = return_substring(instruction1, 0, 6);

    if (subStr == "Turn le" || subStr == "Turn ri")
    {
        (subStr == "Turn le") ? newDirection = "Turn right " : newDirection = "Turn left ";
        newDirection = newDirection + return_substring(instruction2, sIndex);
    }
    else if (subStr == "Continu")
    {
        newDirection = return_substring(instruction1, 0, 10) + return_substring(instruction2, sIndex + 1);
    }

    return newDirection;
}


string return_substring(const string str, int sIndex, int fIndex)
{
    string subStr;

    if (sIndex < 0)
    {
        sIndex = str.size() - sIndex;
    }

    if (fIndex < 0)
    {
        fIndex = str.size() - fIndex;
    }
    else if (fIndex == 0)
    {
        fIndex = str.size() - 1;
    }

    if (sIndex > fIndex)
    {
        for (int iter = sIndex; iter >= fIndex; iter--)
        {
            subStr = str[iter] + subStr;
        }
    }
    else
    {
        for (int iter = sIndex; iter <= fIndex; iter++)
        {
            subStr = subStr + str[iter];
        }
    }

    return subStr;
}
