#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct books
{
    int id;
    char name[50];
    char author[50];
    int availability;
    char genre[50];
    struct books *next; // pointer to next structure
};
typedef struct books books;



books * addbooks(int *lastid,books *head)
{
    int id,availability;
    char name[50],author[50],genre[50]; 

    fflush(stdin); 

    printf("Enter the details\n");
    printf("Enter the name of the book : ");
    fgets(name,sizeof(name),stdin);

    printf("Enter the author name : ");
    fgets(author,sizeof(author),stdin);

    printf("Enter the genre : ");
    fgets(genre,sizeof(genre),stdin);

    printf("Enter the number of books : ");
    scanf("%d",&availability);

    books *p=(books *)malloc (sizeof(books)); //creating new structure for book

    strcpy(p->name,name);
    strcpy(p->author,author);
    strcpy(p->genre,genre);
    p->id=++*(lastid);


    p->availability=availability;

    if(head==NULL)
    {
        head = p;           //allocating memory for head
        head->next=NULL;   //pointing to first book
    }
    else
    {
        //inserting at start of linked list 
        books *q=(books *)malloc(sizeof(books));
        q=head;
        head=p;
        head->next=q;

    }
    return head;
}
void addbooksToFile(books *head)
{
    FILE *ptr;
    ptr=fopen("C:\\Users\\devbh\\Folder\\Projects\\LibraryProjext\\books.txt","w");

    //write the last id to id.txt file
    FILE *id;
    id=fopen("C:\\Users\\devbh\\Folder\\Projects\\LibraryProjext\\id.txt","w");
    
    if(!ptr) //if file doesnt open , exit
        return;
   
    //writing all the added users to file
    books *p=head;

    if(p)//writing id to id.txt
        fprintf(id,"%d",p->id);

    while(p!=NULL){
        fwrite(p,sizeof(books),1,ptr);
        p=p->next;
    }
    //closing the file
    fclose(ptr);
    fclose(id);
    return ;
}

books* deletebooks(int *lastid,books *head)
{
    //variables
    int choice;
    int id;
    char name[20];

    printf("1. Delete by Id \n2. Delete by name\nEnter a choice : ");
    scanf("%d",&choice);

    switch(choice)
    {
        case 1: //checking by id
        {
            printf("Enter the id : ");
            scanf("%d",&id);

            if(id>*lastid) // if the id is greater than last id the id is not created
                printf("The book with the entered last id not found \n");
            else //search and delete the id from the linked list and the file
            {
                books *p=head;
                books *q=p;
                
                if(p->id==id)//checking if head is the node to delete
                {
                    head=head->next;
                    printf("The book was deleted successfully\n");
                    free(p);
                }
                else{
                    while(p->id!=id && p!=NULL)
                    {
                        q=p;
                        p=p->next;
                    }
                    if(p==NULL)
                    {
                        printf("The book with the entered id was not found\n");
                        return head;
                    }
                    q->next=p->next;
                    printf("The book was deleted successfully\n");
                    free(p);
                }
                addbooksToFile(head); // add the changes if any made to the file
            }
            break;
        }
        case 2: //checking by name
        {
            int flag=0;
            printf("Enter the name : ");
            fflush(stdin);
            fgets(name,sizeof(name),stdin);
            
            books *p=head;
            books *r=p;

            //checking for the book in the linked list 
            if(strcmp(name,p->name)==0) // if head is the node to delete
            {
                head=head->next;
                free(p);
                printf("The book was deleted successfully\n");
                return head;
            }
            while(p!=NULL)
            {
                if(strcmp(name,p->name)==0)
                {
                    break;
                }
                r=p;
                p=p->next;
            }
            if(p!=NULL) // if the book was found 
            {
                printf("The book was deleted successfully\n");
                r->next=p->next;
                flag=1;
                free(p);
                addbooksToFile(head); // add the changes if any made to the file
                return head;
            }
            else{
                printf("The book with the entered name was not found \n");
                return head;
            }
            break;
        }

    }
    return head;
}
books * initialize_books(books *head)
{
    FILE *ptr=fopen("C:\\Users\\devbh\\Folder\\Projects\\LibraryProjext\\books.txt","r");
    books *tail=NULL;

    //checking if the file is empty
    if(!ptr)
    {
        head=NULL;
        return head;
    }

    //writing to linked list
    while(!feof(ptr))
    {   
        books *p=(books *)malloc(sizeof(books));
        if (!p) {
            printf("Memory allocation failed. Exiting program.\n");
            return NULL;
        }
            
        if (fread(p,sizeof(books),1,ptr)!=0) {
            p->next = NULL;
            if (head == NULL) {
                // If the list is empty, set the new user as the head
                head = p;
                tail = p;
            } 
            else {
                // Append the new user to the end of the list
                tail->next=p;
                tail=tail->next;
            }
        }
    }

    fclose(ptr); //closing the file
    return head;
}

int id(int *lastid) //function to get last index from file after closing
{
    FILE *ptr;
    ptr=fopen("C:\\Users\\devbh\\Folder\\Projects\\LibraryProjext\\id.txt","r"); // created a file to store the last id entered
    
    if(!ptr){
        *lastid=0;
        return 0;
    }
    if(feof(ptr)){
        *lastid=0;
        return 0;
    }
    fscanf(ptr,"%d",lastid);
    fclose(ptr);   
    return *lastid; 
}

//to free all the memory of the linked list
books * freemem(books *head)
{
    books *p=head;
    books *q=p;
    while(p!=NULL)
    {
        q=p;
        p=p->next;
        free(q);
    }
    return NULL;
}

//main 
int main()
{
    //  Intialization of variables

    books *head = NULL;
    int lastid=0;
    head=initialize_books(head);
    lastid=id(&lastid);

    //  End of initialization

    int choice;
    books *p;
    while (1) {
        printf("\nMenu:\n");
        printf("1. Add Element\n");
        printf("2. Display Elements\n");
        printf("3. Delete Element\n");
        printf("4. Add Books to file\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        fflush(stdin);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                head=addbooks(&lastid,head);
                break;
            case 2:
                p=head;
                printf("\nThe book details \n");
                while(p!=NULL)
                {
                    printf("\nId : %d\n",p->id);
                    printf("Name : %s",p->name);
                    printf("Author : %s",p->author);
                    printf("Genre : %s",p->genre);
                    printf("Availability : %d\n",p->availability);
                    p=p->next;
                }
                break;
            case 3:
                head=deletebooks(&lastid,head);
                break;
            case 5:
                addbooksToFile(head);
                printf("Exiting the program.\n");
                return 0;
            case 4:
                addbooksToFile(head);
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    }
    head=freemem(head);
}


/*
Issues 
Character limit on genre

*/