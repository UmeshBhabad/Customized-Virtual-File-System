////////////////////////////////////////////////////////////////////////////
//
//  Header File Inclusion
//
////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
// #include <unistd.h>
#include <stdbool.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////
//
//  User Defined Macros
//
////////////////////////////////////////////////////////////////////////////

// Maximum file size that we allow in the project
#define MAXFILESIZE 50

#define MAXOPENFILES 20

#define MAXINODE 5

#define READ 1
#define WRITE 2
#define EXECUTE 4

#define START 0
#define CURRENT 1
#define END 2

#define EXECUTE_SUCESS 0

#define REGULARFILE 1   // Added new
#define SPECIALFILE 2   // Added new

////////////////////////////////////////////////////////////////////////////
//
//  User Defined Macros for ERROR handling
//
////////////////////////////////////////////////////////////////////////////

#define ERR_INVALID_PARAMETER -1

#define ERR_NO_INODES -2

#define ERR_FILE_ALREADY_EXIST -3
#define ERR_FILE_NOT_EXIST -4

#define ERR_PERMISSION_DENIED -5

#define ERR_INSUFFICIENT_SPACE -6
#define ERR_INSUFFICIENT_DATA -7

#define ERR_MAX_FILES_OPEN -8

////////////////////////////////////////////////////////////////////////////
//
//  User Defined Structures
//
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//
//  Structure Name  :   BootBlock
//  Description     :   Holds the Information to boot the OS
//
////////////////////////////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];
};

////////////////////////////////////////////////////////////////////////////
//
//  Structure Name  :   SuperBlock
//  Description     :   Holds the Information about the file system.
//
////////////////////////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInodes;
    int FreeInodes;
};

////////////////////////////////////////////////////////////////////////////
//
//  Structure Name  :   Inode
//  Description     :   Holds the Information about the file.
//
////////////////////////////////////////////////////////////////////////////

#pragma pack(1)

struct Inode
{
    char FileName[20];
    int InodeNumber;
    int FileSize;
    int ActualFileSize;
    int FileType;   // Added new
    int ReferenceCount;
    int Permission;
    char *Buffer;
    struct Inode *next;
};

typedef struct Inode INODE;
typedef struct Inode *PINODE;
typedef struct Inode **PPINODE;

////////////////////////////////////////////////////////////////////////////
//
//  Structure Name  :   FileTable
//  Description     :   Holds the Information about the opened file.
//
////////////////////////////////////////////////////////////////////////////

struct FileTable
{
    int ReadOffset;
    int WriteOffset;
    int Mode;
    PINODE ptrinode;
};

typedef FileTable FILETABLE;
typedef FileTable *PFILETABLE;

////////////////////////////////////////////////////////////////////////////
//
//  Structure Name  :   UAREA
//  Description     :   Holds the Information about the process.
//
////////////////////////////////////////////////////////////////////////////

struct UAREA
{
    char ProcessName[20];
    PFILETABLE UFDT[MAXOPENFILES];
};

////////////////////////////////////////////////////////////////////////////
//
//  Global Variables or objects used in the project
//
////////////////////////////////////////////////////////////////////////////

BootBlock bootobj;
SuperBlock superobj;
UAREA uareaobj;

PINODE head = NULL;

////////////////////////////////////////////////////////////////////////////
//
//  Function Name   :   InitialiseUAREA
//  Description     :   It is used to initialize UAREA members
//  Author          :   Umesh Shivaji Bhabad
//  Date            :   13/01/2026
//
////////////////////////////////////////////////////////////////////////////

void InitialiseUAREA()
{
    strcpy(uareaobj.ProcessName, "Myexe");

    int i = 0;

    for(i = 0; i < MAXOPENFILES; i++)
    {
        uareaobj.UFDT[i] = NULL;
    }

    printf("Marvellous CVFS : UAREA gets Initialized Sucessfully\n");
}

////////////////////////////////////////////////////////////////////////////
//
//  Function Name   :   InitialiseSuperBlock
//  Description     :   It is used to initialize Super Block members
//  Author          :   Umesh Shivaji Bhabad
//  Date            :   13/01/2026
//
////////////////////////////////////////////////////////////////////////////

void InitialiseSuperBlock()
{
    superobj.TotalInodes = MAXINODE;
    superobj.FreeInodes = MAXINODE;

    printf("Marvellous CVFS : Super Block gets Initialized Sucessfully\n");
}

////////////////////////////////////////////////////////////////////////////
//
//  Function Name   :   CreateDILB
//  Description     :   It is used to create Linked List of Inodes
//  Author          :   Umesh Shivaji Bhabad
//  Date            :   13/01/2026
//
////////////////////////////////////////////////////////////////////////////

void CreateDILB()
{
    int i = 1;

    PINODE newn = NULL;
    PINODE temp = head;

    for(i = 1; i <= MAXINODE; i++)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        strcpy(newn -> FileName, "\0");
        newn -> InodeNumber = i;
        newn -> FileSize = 0;
        newn -> ActualFileSize = 0;
        newn -> FileType = 0;           // Added new
        newn -> ReferenceCount = 0;
        newn -> Permission = 0;
        newn -> Buffer = NULL;
        newn -> next = NULL;

        if(temp == NULL)    // LL is empty;
        {
            head = newn;
            temp = head;
        }
        else    // LL contains atleast one node
        {
            temp -> next = newn;
            temp = temp -> next;
        }

    }

    printf("Marvellous CVFS : DILB created sucessfully\n");
}

////////////////////////////////////////////////////////////////////////////
//
//  Function Name   :   StartAuxillaryDataInitialization
//  Description     :   It is used to Call such functions which are used 
//                      to initialize auxillary data.
//  Author          :   Umesh Shivaji Bhabad
//  Date            :   13/01/2026
//
////////////////////////////////////////////////////////////////////////////

void StartAuxillaryDataInitialization()
{
    strcpy(bootobj.Information, "Booting Process of Marvellous CVFS done.");

    printf("%s\n", bootobj.Information);

    InitialiseSuperBlock();
    CreateDILB();
    InitialiseUAREA();

    printf("Marvellous CVFS : Auxillary Data Initialized Sucessfully.\n");
}

////////////////////////////////////////////////////////////////////////////
//
//  Function Name   :   DisplayHelp
//  Description     :   It is used to display help page.
//  Author          :   Umesh Shivaji Bhabad
//  Date            :   14/01/2026
//
////////////////////////////////////////////////////////////////////////////

void DisplayHelp()
{
    printf("--------------------------------------------------------------------------\n");
    printf("-------------------- Marvellous CVFS Halp Page ----------------------------\n");
    printf("--------------------------------------------------------------------------\n");

    // create help.txt which includes all command info and read the file upon call
    printf("man : It is used to display manual page.\n");
    printf("clear : It is used to clear the terminal.\n");
    printf("creat : It is used to create new file.\n");
    printf("write : It is used to write the data into the file.\n");
    printf("read : It is used to read the data from the file.\n");
    printf("stat : It is used to display statistical information.\n");
    printf("unlink : It is used to delete the file.\n");
    printf("exit : It is used to terminate Marvellous CVFS.\n");

    printf("--------------------------------------------------------------------------\n");
}

////////////////////////////////////////////////////////////////////////////
//
//  Function Name   :   ManPageDisplay
//  Description     :   It is used to display man page.
//  Author          :   Umesh Shivaji Bhabad
//  Date            :   14/01/2026
//
////////////////////////////////////////////////////////////////////////////

void ManPageDisplay(char Name[])
{
    // add man page description availabe on internet
    if(strcmp("ls",Name) == 0)
    {
        printf("About : It is used to list the names of all files.\n");
        printf("Usage : ls\n");
    }
    else if(strcmp("man", Name) == 0)
    {
        printf("About : It is used to display manual page.\n");
        printf("Usage : man command_name\n");
        printf("command_name : It is the name of command.\n");
    }
    else if(strcmp("exit", Name) == 0)
    {
        printf("About : It is used to terminate the shell.\n");
        printf("Usage : exit\n");
    }
    else if(strcmp("clear", Name) == 0)
    {
        printf("About : It is used to clear the shell.\n");
        printf("Usage : clear\n");
    }
    else
    {
        printf("No manual entry for %s\n", Name);
    }
}

////////////////////////////////////////////////////////////////////////////
//
//  Function Name   :   IsFileExist
//  Description     :   It is used to check whether if file exist.
//  Input           :   It accepts filename.
//  Output          :   It returns the true or false.
//  Author          :   Umesh Shivaji Bhabad
//  Date            :   16/01/2026
//
////////////////////////////////////////////////////////////////////////////

bool IsFileExist(
                    char *name              // File name
                )
{
    PINODE temp = head;
    bool bFlag = false;

    while(temp != NULL)
    {
        if((strcmp(name, temp -> FileName) == 0) && (temp -> FileType == REGULARFILE))
        {
            bFlag = true;
            break;
        }
        temp = temp -> next;
    }

    return bFlag;
}

////////////////////////////////////////////////////////////////////////////
//
//  Function Name   :   CreateFile
//  Description     :   It is used to create new regular file.
//  Input           :   It accepts filename and permissions
//  Output          :   It returns the file Descriptor.
//  Author          :   Umesh Shivaji Bhabad
//  Date            :   16/01/2026
//
////////////////////////////////////////////////////////////////////////////

int CreateFile(
                char *name,                 // Name of new file
                int permission              // permission for that file
              )
{
    PINODE temp = head;
    int i = 0;

    printf("Total number of Inode remaining : %d\n", superobj.FreeInodes);

    // Filters

    if(name == NULL)    // if name is missing
    {
        return ERR_INVALID_PARAMETER;
    }

    // if permission value is wrong
    // permission -> 1 -> READ
    // permission -> 2 -> WRITE
    // permission -> 3 -> READ + WRITE

    if((permission < 1) || (permission > 3))
    {
        return ERR_INVALID_PARAMETER;
    }

    // if the inodes are full
    if(superobj.FreeInodes == 0)
    {
        return ERR_NO_INODES;
    }

    // if file is already present
    if(IsFileExist(name) == true)
    {
        return ERR_FILE_ALREADY_EXIST;
    }

    // search empty Inode

    while(temp != NULL)
    {
        if(temp -> FileType == 0)
        {
            break;
        }
        temp = temp -> next;
    }

    // if temp reaches at the end of inode
    if(temp == NULL)
    {
        printf("There is no Inode\n");
        return ERR_NO_INODES;
    }

    // Search for empty UFDT entry
    // Note : 0, 1, 2 are reserved

    for(i = 3; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    // UFDT is full
    if(i == MAXOPENFILES)
    {
        return ERR_MAX_FILES_OPEN;
    }

    // Allocate ememory for file table

    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    // Initialize File table

    uareaobj.UFDT[i] -> ReadOffset = 0;
    uareaobj.UFDT[i] -> WriteOffset = 0;
    uareaobj.UFDT[i] -> Mode = permission;
    
    // Connect File Table with Inode
    uareaobj.UFDT[i] -> ptrinode = temp;

    // Initialize elements of Inode
    strcpy(uareaobj.UFDT[i] -> ptrinode -> FileName, name);
    uareaobj.UFDT[i] -> ptrinode ->FileSize = MAXFILESIZE;
    uareaobj.UFDT[i] -> ptrinode ->ActualFileSize = 0;
    uareaobj.UFDT[i] -> ptrinode ->FileType = REGULARFILE;
    uareaobj.UFDT[i] -> ptrinode -> ReferenceCount = 1;
    uareaobj.UFDT[i] -> ptrinode -> Permission = permission;

    // Allocate memory for file data

    uareaobj.UFDT[i] -> ptrinode -> Buffer = (char *)malloc(MAXFILESIZE);

    superobj.FreeInodes--;

    return i;   // File Descriptor
}

////////////////////////////////////////////////////////////////////////////
//
//  Function Name   :   LsFile
//  Description     :   It is used to list all file.
//  Input           :   Nothing
//  Output          :   Nothing
//  Author          :   Umesh Shivaji Bhabad
//  Date            :   16/01/2026
//
////////////////////////////////////////////////////////////////////////////

void LsFile()
{
    PINODE temp = head;

    printf("--------------------------------------------------------------------------\n");
    printf("--------------- Marvellous CVFS Files Information ------------------------\n");
    printf("--------------------------------------------------------------------------\n");

    while(temp != NULL)
    {
        if(temp -> FileType != 0)
        {
            printf("%d\t%s\t%d\n", temp -> InodeNumber, temp -> FileName, temp -> ActualFileSize);
        }
        
        temp = temp -> next;
    }

    printf("--------------------------------------------------------------------------\n");
}

////////////////////////////////////////////////////////////////////////////
//
//  Function Name   :   UnlinkFile
//  Description     :   It is used to delete the file.
//  Input           :   filename
//  Output          :   Nothing
//  Author          :   Umesh Shivaji Bhabad
//  Date            :   22/01/2026
//
////////////////////////////////////////////////////////////////////////////

int UnlinkFile(
                char *name
                )
{
    int i = 0;

    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(IsFileExist(name) == false)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // Travel the UFDT
    for(i = 3; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            if(strcmp(uareaobj.UFDT[i] -> ptrinode -> FileName, name) == 0)
            {
                // deallocate memory of buffer
                free(uareaobj.UFDT[i] -> ptrinode -> Buffer);
                uareaobj.UFDT[i] -> ptrinode -> Buffer = NULL;

                // reset all the values of inode
                // Don't deallocate memory of inode

                uareaobj.UFDT[i] -> ptrinode -> FileSize = 0;
                uareaobj.UFDT[i] -> ptrinode -> ActualFileSize = 0;
                uareaobj.UFDT[i] -> ptrinode -> FileType = 0;
                uareaobj.UFDT[i] -> ptrinode -> ReferenceCount = 0;
                uareaobj.UFDT[i] -> ptrinode -> Permission = 0;
                memset(uareaobj.UFDT[i] -> ptrinode -> FileName, '\0', sizeof(uareaobj.UFDT[i] -> ptrinode -> FileName));

                // Deallocate the memory of file table

                free(uareaobj.UFDT[i]);

                // set NULL to UFDT

                uareaobj.UFDT[i] = NULL;

                // Increment Free Inodes count

                superobj.FreeInodes++;
                break;
            }   // end of if
        }   // end of if
        
    }   // end of for

    return EXECUTE_SUCESS;
}   // end of function

////////////////////////////////////////////////////////////////////////////
//
//  Function Name   :   WriteFile
//  Description     :   It is used to write the data into the file.
//  Input           :   File Descriptor
//  Input           :   Address of Buffer which contains data
//  Input           :   size of data that we want to write
//  Output          :   number of bytes successfully written
//  Author          :   Umesh Shivaji Bhabad
//  Date            :   22/01/2026
//
////////////////////////////////////////////////////////////////////////////

int WriteFile(
                int fd, 
                char *data, 
                int size
            )
{
    printf("File Descriptor : %d\n", fd);

    printf("Data that we want to write : %s\n", data);

    printf("Number of bytes that we want to write : %d\n", size);

    // Invalid FD
    if(fd < 0 || fd > MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    // FD points to NULL
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // There is No permission to write
    if(uareaobj.UFDT[fd] -> ptrinode -> Permission < WRITE)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Insufficient space
    if((MAXFILESIZE - uareaobj.UFDT[fd] -> WriteOffset) < size)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    // write the data into the file
    strncpy(uareaobj.UFDT[fd] -> ptrinode -> Buffer + uareaobj.UFDT[fd] -> WriteOffset, data, size); // strncpy will  copy exac

    // Update the Write offset
    uareaobj.UFDT[fd] -> WriteOffset = uareaobj.UFDT[fd] -> WriteOffset + size;

    // Update the Actual file size
    uareaobj.UFDT[fd] -> ptrinode -> ActualFileSize = uareaobj.UFDT[fd] -> ptrinode -> ActualFileSize + size;

    return size;
}

////////////////////////////////////////////////////////////////////////////
//
//  Function Name   :   ReadFile
//  Description     :   It is used to read the data from the file.
//  Input           :   File Descriptor
//  Input           :   Address of empty Buffer
//  Input           :   size of data that we want to read
//  Output          :   number of bytes successfully Read
//  Author          :   Umesh Shivaji Bhabad
//  Date            :   22/01/2026
//
////////////////////////////////////////////////////////////////////////////

int ReadFile(
                int fd,
                char *data,
                int size
)
{
    // Invalid FD
    if(fd < 0 || fd > MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(data == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(size <= 0)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // Filter for permission
    if(uareaobj.UFDT[fd] -> ptrinode -> Permission < READ)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Insufficient Data
    if((MAXFILESIZE - uareaobj.UFDT[fd] -> ReadOffset) < size)
    {
        return ERR_INSUFFICIENT_DATA;
    }

    // Read the data
    strncpy(data , uareaobj.UFDT[fd] -> ptrinode -> Buffer + uareaobj.UFDT[fd] -> ReadOffset, size);

    // Update the read offset
    uareaobj.UFDT[fd] -> ReadOffset = uareaobj.UFDT[fd] -> ReadOffset + size;

    return size;
}

////////////////////////////////////////////////////////////////////////////
//
//  Entry point Function of the project
//
////////////////////////////////////////////////////////////////////////////

int main()
{
    char str[80] = {'\0'};
    char Command[5][20] = {{'\0'}};
    char InputBuffer[MAXFILESIZE] = {'\0'};

    char *EmptyBuffer = NULL;

    int iCount = 0;
    int iRet = 0;

    StartAuxillaryDataInitialization();

    printf("--------------------------------------------------------------------------\n");
    printf("--------------- Marvellous CVFS stared sucessfully -----------------------\n");
    printf("--------------------------------------------------------------------------\n");
    
    // Infinite Listening Shell
    while(1)
    {
        fflush(stdin);

        strcpy(str, "");    // memset()

        printf("\nMarvellous CVFS : > ");
        fgets(str, sizeof(str), stdin);     // scanf : formatted, fgets : not formatted

        iCount = sscanf(str, "%s %s %s %s", Command[0], Command[1], Command[2], Command[3]);    // Tokenization

        fflush(stdin);

        if(iCount == 1)
        {
            // Marvellous CVFS : > exit
            if(strcmp("exit", Command[0]) == 0)
            {
                printf("Thank you for using Marvellous CVFS.\n");
                printf("Deallocating all the allocated resources.\n");

                break;
            }
            // Marvellous CVFS : > help
            else if(strcmp("ls", Command[0]) == 0)
            {
                LsFile();
            }
            // Marvellous CVFS : > help
            else if(strcmp("help", Command[0]) == 0)
            {
                DisplayHelp();
            }
            // Marvellous CVFS : > clear
            else if(strcmp("clear", Command[0]))
            {
                // Conditional preprocessing
                #ifdef _WIN32
                    system("cls");  // passing command through program
                #else
                    system("clear");
                #endif
            }
        }   // End of else if 1
        else if (iCount == 2)
        {
            // Marvellous CVFS : > man ls
            if(strcmp("man", Command[0]) == 0)
            {
                ManPageDisplay(Command[1]);
            }

            // Marvellous CVFS : > unlink Demo.txt
            if(strcmp("unlink", Command[0]) == 0)
            {
                iRet = UnlinkFile(Command[1]);

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid Parameter\n");
                }

                if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : unable to delete as there is no such file\n");
                }

                if(iRet == EXECUTE_SUCESS)
                {
                    printf("File gets sucessfully deleted\n");
                }
            }
            // Marvellous CVFS : > write 2
            else if(strcmp("write", Command[0]) == 0)
            {
                printf("Enter the data that you want to write : \n");
                fgets(InputBuffer, MAXFILESIZE, stdin);

                iRet = WriteFile(atoi(Command[1]), InputBuffer, strlen(InputBuffer) - 1);

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid Parameters\n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : There is no such file");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Unable to write as there is no permission\n");
                }
                else if(iRet == ERR_INSUFFICIENT_SPACE)
                {
                    printf("'Error : Unable to write as there is no space\n");
                }
                else
                {
                    printf("%d bytes gets sucessfully written\n", iRet);
                }
            }
            else
            {
                printf("There is no such command\n");
            }
        }   // End of else if 2
        else if(iCount == 3)
        {
            // Marvellous CVFS : > creat Ganesh.txt 3
            if(strcmp("creat", Command[0]) == 0)
            {
                iRet = CreateFile(Command[1], atoi(Command[2]));    // ASCII to Integer

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : unable to create the file as parameters are invalid\n");
                    printf("Please refer man page\n");
                }

                if(iRet == ERR_NO_INODES)
                {
                    printf("Error : unable to create file as there is no inode\n");
                }

                if(iRet == ERR_FILE_ALREADY_EXIST)
                {
                    printf("Error : unable to create file because file already exist\n");
                }

                if(iRet == ERR_MAX_FILES_OPEN)
                {
                    printf("Error : Unable to create file\n");
                    printf("Max opened files limit reached\n");
                }

                printf("File gets Sucessfully created with FD : %d\n", iRet);
            }
            // Marvellous CVFS : > read 3 10
            if(strcmp("read", Command[0]) == 0)
            {
                EmptyBuffer = (char *)malloc(sizeof(atoi(Command[2])));

                iRet = ReadFile(atoi(Command[1]), EmptyBuffer, atoi(Command[2]));

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid Parameter\n");
                }

                if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : File not exist\n");
                }

                if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Permission denied\n");
                }

                if(iRet == ERR_INSUFFICIENT_DATA)
                {
                    printf("Error : Insufficient data\n");
                }

                else
                {
                    printf("Read operation is sucessful\n");
                    printf("Data from file is : %s\n", EmptyBuffer);

                    free(EmptyBuffer);
                }
            }
            else
            {
                printf("There is no such command\n");
            }
        }   // End of else if 3
        else if(iCount == 4)
        {

        }   // End of else if 4
        else
        {
            printf("Command not found\n");
            printf("Please refer help option to get more Information\n");
        }// End of else
    }   // End of while

    
    return 0;
}   // End of main