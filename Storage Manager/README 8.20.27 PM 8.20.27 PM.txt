-Storage Manager-

Project Modules
C source files: storage_mgr.c, dberror.c, test_assign1_1.c, Header files: storage_mgr.h, dberror.h, test_helper.h

Team Members: (Group 4)

1. Gaurav Singhania(A20445477)

2. Chaitanya Nagaram(A20450288)

3. Sachin Devatar(A20443522)


Abstract:

The objective of this assignment is to implement a simple storage manager - a module that is capable of reading blocks from a file on disk into memory and writing blocks from memory to a file on disk. The storage manager deals with pages (blocks) of fixed size (PAGE_SIZE). In addition to reading and writing pages from a file, it provides methods for creating, opening, and closing files. The storage manager has to maintain several types of information for an open file: The number of total pages in the file, the current page position for reading, writing and appending the file, the file name, and a POSIX file descriptor or FILE pointer.

To Run:

With given default test cases:
Command: make test_assign1_1.c
Run: ./test_assign1_1.c
To remove object files use this command: make clean


The storage manager has the following functionalities:

Manipulating page files:

- initStorageManager() : 

initializes the storage manager program.

- createPageFile() : 

We will try to open the pagefile, if the file exist and then it will ask for overwrite, based on user permission if it's yes than the file will be open in write mode.If file does not exist, then new file will be created with the page size of 4096 bytes. After creating it will return RC_OK;

- openPageFile() : 

it will open the file in read mode if the file is already exists. If the file does not exist then it will return error code.Here it will initialize the file handler variable like current page position, number of page, file name and store the pointer info into file handle.

- ClosePageFile() :

It will close the file, if the file does not exist and then return error code based on conditions.

- destroyPageFile() :

It will remove the existing file and returns the error code based on condition.

Reading blocks from disc:

- readBlock() :
It will set the file descriptor to particular block from the starting of file and reads the page size that is 4096 bytes then it will loads it to the memory which is defined as mempage. It will return error code if,
  1.file handle not initialized
  2.file not found.
  3.unable to read file.
  4.Current page position does not exist.

- getBlockPos() :

It will find the current page position by using file descriptor. It will return the error code if the file does not initialize or if the file failed to open.

- readFirstBlock() :

It will set the file descriptor to beginning of the file and reads the first page content to 'mempage'. It will return error code if,
  1.file handle not initialized
  2.file not found.
  3.unable to read file.

- readPreviousBlock() :

It will set the file descriptor to previous page and reads the page content to 'mempage'. It will return error code if,
  1.file handle not initialized
  2.file not found.
  3.if the current page is less than 1.
 
- readCurrentBlock() :

It will set the file descriptor to current page position and reads the page content to 'mempage'. It will return error code if,
  1.file handle not initialized
  2.file not found.
  
- readNextBlock() :

It will set the file descriptor to next page and reads the page content to 'mempage'. It will return error code if,
  1.file handle not initialized
  2.file not found.
  3.if the current block is greater than totalpages-1.

- readLastBlock() :
It will set the file descriptor to last page and reads the page content to 'mempage'. It will return error code if,
  1.file handle not initialized
  2.file not found.
  3.unable to read file.


Writing blocks to a page file:

- writeBlock() : 
    It will examine if the file is present and move the file pointer to the particular page from the starting of the file and writes 1 page to it, then the file handle variable are upgrade.
It will return error code if,

1. file handle not initialized
2. file not found.
3. file handler points to non-existing page

- writeCurrentBlock() :

It will examine if the file is present and move the file pointer to the particular page from the starting of the file and writes 1 page to it, then the file handle variable are upgrade.
It will return error code if,

1. file handle not initialized
2. file not found.

- appendEmptyBlock() :
when we call append it will going to add new empty block to the file.
It will return error code if,

1. file handle not initialized
2. Unable to write the file.

- ensureCapacity() :

If the file does not have enough pages, then it will add more pages using appendEmptyBlock function.













 







