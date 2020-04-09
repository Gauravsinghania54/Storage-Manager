
#include <stdio.h>
#include <stdlib.h>
#include "dberror.h"
#include "storage_mgr.h"


void initStorageManager (void)
{
    printf("\nStorage manager initialized");
}

RC createPageFile(char *fileName)
{
    FILE *fp;
    char con;
    
    fp = fopen(fileName,"r");
    if(fp!=NULL)
    {
        printf("file is already there\n");
        printf("\n\n If You want to Overwrite ?.(y/n)");
        scanf("%c",&con);
        if(con=='y')
        {
            fp = fopen(fileName,"w");
            printf("\n Edit Content");
            fseek(fp,0,SEEK_SET);//SEEK_SET seeks for beginning of the file
            char *pt=(char *)calloc(PAGE_SIZE,sizeof(char));
            fwrite(pt,PAGE_SIZE,sizeof(char),fp);
            fseek(fp,0,SEEK_END);
            fclose(fp);
            return RC_OK;
        }
        else{
            printf("File chosen not to overwrite");
            fclose(fp);
            return RC_OK;
        }
        
    }
    else if(fp==NULL)
    {
        fp = fopen(fileName,"w");
        char *pt=(char *)calloc(PAGE_SIZE,sizeof(char));
        fwrite(pt,PAGE_SIZE,sizeof(char),fp);
        fseek(fp,0,SEEK_END);
        fclose(fp);
        return RC_OK;
    }
    return RC_OK;
}
RC openPageFile (char *fileName, SM_FileHandle *fHandle)
{
    FILE *fp;
    fp = fopen(fileName,"r");
    if(fp==NULL)
    {
        printf("file not found");
        return RC_FILE_NOT_FOUND;
    }
    else
    {
        printf("opening file");
        fp=fopen(fileName,"r");
        fHandle->curPagePos=0;
        fHandle->fileName=fileName;
        fseek(fp,0,SEEK_END);//pointer to end of file
        
        fHandle->totalNumPages=(int)ftell(fp)/PAGE_SIZE;//dividing the filesize by Pagesize to get total no. of pages
        // ftell is used to give the fileSize
        fHandle->mgmtInfo=fp;//File Info in MgmtInfo
        return RC_OK;
    }
}
RC closePageFile (SM_FileHandle *fHandle)
{
    printf("closing the File");
    if(fclose(fHandle->mgmtInfo)==0)
    {
        printf("file closed succesfully");
        return RC_OK;
    }
    else{
        printf("file not found");
        return RC_FILE_NOT_FOUND;
    }
}
RC destroyPageFile (char *fileName)
{
    if(remove(fileName)==0)
    {
        printf("Deleted file successfully");
        return RC_OK;
    }
    else{
        RC_message="Couldn't find the requested file";
        printError(RC_FILE_NOT_FOUND);
        return RC_FILE_NOT_FOUND;
    }
    
}
RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    if (fHandle == NULL)
    {
        printf("File handler not initialized");
        return RC_FILE_HANDLE_NOT_INIT;
    }
    if(fopen(fHandle->fileName,"r") == NULL)//checking for file
    {
        return RC_FILE_NOT_FOUND;
    }
    if(pageNum > fHandle->totalNumPages)//checking if its an invalid page number
    {
        RC_message="Page with the given page number doesn't exists.";
        printError(RC_READ_NON_EXISTING_PAGE);
        return RC_READ_NON_EXISTING_PAGE;
    }
    fseek(fHandle->mgmtInfo , pageNum*PAGE_SIZE , SEEK_SET);
    
    if(fread(memPage, 1, PAGE_SIZE, fHandle->mgmtInfo) != PAGE_SIZE)//reading a block of size PAGE_SIZE
    {
        return RC_READ_NON_EXISTING_PAGE;
    }
    
    fread(memPage, 1,PAGE_SIZE,fHandle->mgmtInfo);

    
    
    fHandle->curPagePos = pageNum;//current page position is updated to the user input page Number.
    return RC_OK;
}
int getBlockPos (SM_FileHandle *fHandle)
{
    if(fHandle==NULL)
    {
        printf("File handler not initialized");
        return RC_FILE_HANDLE_NOT_INIT;
    }
    else if(fopen(fHandle->fileName,"r")==NULL)
    {
        printf("file not found");
        return RC_FILE_NOT_FOUND;
    }
    
    printf("the current page position is %d", (int)&fHandle->curPagePos);
    return fHandle->curPagePos;
    
}
RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    if (fHandle == NULL)
        return RC_FILE_HANDLE_NOT_INIT;
    
    if(fopen(fHandle->fileName,"r") == NULL)
        return RC_FILE_NOT_FOUND;
    
    fseek(fHandle->mgmtInfo , 0 , SEEK_SET);// seeking to the beginning of the file
    //reading the first 4096bytes
    if(fread(memPage, 1, PAGE_SIZE, fHandle->mgmtInfo) != PAGE_SIZE)
        return RC_READ_NON_EXISTING_PAGE;
    //setting the pointer position
    fHandle->curPagePos = (int)(ftell(fHandle->mgmtInfo)/PAGE_SIZE);
    return RC_OK;
    
}
RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    if (fHandle == NULL)
    {
        printf("File handler not initialized");
        return RC_FILE_HANDLE_NOT_INIT;
    }
    if(fopen(fHandle->fileName,"r") == NULL)//checking for file
    {
        return RC_FILE_NOT_FOUND;
    }
    if(fHandle->curPagePos < 1)          //checking if current page is less then 1.
    {
        return RC_READ_NON_EXISTING_PAGE;
    }
    fHandle->curPagePos=fHandle->curPagePos-1;
    fseek(fHandle->mgmtInfo,PAGE_SIZE,SEEK_CUR);
    if(fread(memPage, 1, PAGE_SIZE, fHandle->mgmtInfo) != PAGE_SIZE)
    {
        return RC_READ_NON_EXISTING_PAGE;
    }
    
    fread(memPage, 1,PAGE_SIZE,fHandle->mgmtInfo);
    fHandle->curPagePos = (int)(ftell(fHandle->mgmtInfo)/PAGE_SIZE);
    return RC_OK;
    
}
RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    if(fHandle==NULL)
    {
        printf("File handler not initialized");
        return RC_FILE_HANDLE_NOT_INIT;
    }
    else if(fopen(fHandle->fileName,"r")==NULL)
    {
        printf("file not found");
        return RC_FILE_NOT_FOUND;
    }
    if((fHandle->curPagePos) > (fHandle->totalNumPages) || (fHandle->curPagePos) < 1)
    {
        return RC_READ_NON_EXISTING_PAGE;
    }
    fseek(fHandle->mgmtInfo , (fHandle->curPagePos)*PAGE_SIZE , SEEK_SET);//reading a block of file
    if(fread(memPage, 1, PAGE_SIZE, fHandle->mgmtInfo) != PAGE_SIZE)
    {
        return RC_READ_NON_EXISTING_PAGE;
    }
    fHandle->curPagePos = (int)(ftell(fHandle->mgmtInfo)/PAGE_SIZE);
    
    return RC_OK;
}
RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    if (fHandle == NULL)
    {
        printf("File handler not initialized");
        return RC_FILE_HANDLE_NOT_INIT;
    }
    if(fopen(fHandle->fileName,"r") == NULL)//checking for file
    {
        return RC_FILE_NOT_FOUND;
    }
    if(fHandle->curPagePos > fHandle->totalNumPages-1)//checking if currentpos is greater than secondlast page..
    {
        return RC_READ_NON_EXISTING_PAGE;
    }
    fHandle->curPagePos=fHandle->curPagePos+1;
    fseek(fHandle->mgmtInfo,PAGE_SIZE,SEEK_CUR);
    if(fread(memPage, 1, PAGE_SIZE, fHandle->mgmtInfo) != PAGE_SIZE)
    {
        return RC_READ_NON_EXISTING_PAGE;
    }
    
    fread(memPage, 1,PAGE_SIZE,fHandle->mgmtInfo);
    
    fHandle->curPagePos = (int)(ftell(fHandle->mgmtInfo)/PAGE_SIZE);
    return RC_OK;
}
RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    if (fHandle == NULL)
    {
        printf("File handler not initialized");
        return RC_FILE_HANDLE_NOT_INIT;
    }
    if(fopen(fHandle->fileName,"r") == NULL)//checking for file
    {
        return RC_FILE_NOT_FOUND;
    }
    fseek(fHandle->mgmtInfo ,0 , SEEK_END);//seeking to End of the file
    
    if(fread(memPage, sizeof(char), PAGE_SIZE, fHandle->mgmtInfo) != PAGE_SIZE)// reading a block of size PAGE_SIZE
    {
        return RC_READ_NON_EXISTING_PAGE;
    }
    
    fread(memPage, sizeof(char),PAGE_SIZE,fHandle->mgmtInfo);

    
    
    fHandle->curPagePos = (int)(ftell(fHandle->mgmtInfo)/PAGE_SIZE);//updating current page position to the first page.
    return RC_OK;
}

RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    if (fHandle == NULL)
        return RC_FILE_HANDLE_NOT_INIT;
    if(fopen(fHandle->fileName,"r") == NULL) //Checking if file is present or not for opening
        return RC_FILE_NOT_FOUND;
    
    if(pageNum > ((fHandle->totalNumPages))) {
        RC_message="Page with the given page number doesn't exists.";
        printError(RC_READ_NON_EXISTING_PAGE);
        return RC_READ_NON_EXISTING_PAGE;
    }
    
    
    FILE *fp;
    fp = fopen(fHandle->fileName,"r+");
    fseek(fp , pageNum*PAGE_SIZE , SEEK_SET);
    //seeks to the particular page with respect to the start of the file
    if(fwrite(memPage,PAGE_SIZE,1,fp) != 1)
        return RC_WRITE_FAILED;
    //writes the data
    fp = fopen(fHandle->fileName,"r");
    fseek(fp , (pageNum+1)*PAGE_SIZE , SEEK_SET);
    fHandle->mgmtInfo = fp;
    fHandle->curPagePos = (int)(ftell(fHandle->mgmtInfo)/PAGE_SIZE);
    fHandle->totalNumPages = (int)(ftell(fp)/PAGE_SIZE);
    return RC_OK;
}

RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    return writeBlock (fHandle->curPagePos, fHandle, memPage);
}

RC appendEmptyBlock (SM_FileHandle *fHandle)
{
    SM_PageHandle newEmptyBlock;
    newEmptyBlock = (char *) calloc(PAGE_SIZE, sizeof(char)); //allocated page_size for new empty block
    
    fseek(fHandle->mgmtInfo,(fHandle->totalNumPages + 1)*PAGE_SIZE , SEEK_SET);
    if(fwrite(newEmptyBlock,sizeof(char),PAGE_SIZE,fHandle->mgmtInfo) == PAGE_SIZE) {
        fHandle->totalNumPages = fHandle->totalNumPages+1;
        fHandle->curPagePos = fHandle->totalNumPages;
//        fseek(fHandle->mgmtInfo,(fHandle->totalNumPages + 1)*PAGE_SIZE , SEEK_SET);
        return RC_OK;
    }
    else {
        return RC_WRITE_FAILED;
    }
    
}

RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle)
{
    if(fHandle->totalNumPages < numberOfPages) {
        int numOfPagesRequired = numberOfPages - fHandle->totalNumPages;
        int count;
        for(count=1;count<=numOfPagesRequired;count++) {
            appendEmptyBlock(fHandle);
        }
        return RC_OK;
    }
    else {
        return RC_OK;
    }
}
