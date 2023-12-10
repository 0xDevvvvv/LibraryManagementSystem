#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>

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

typedef struct User {
    int id;
    char name[20];
    char address[50];
    char number[15]; // Added number field
    char recentBook[50];
    struct User *next;
} User;


//-------------------------------------------FUNCTIONS FOR BOOKS--------------------------------------------

books * addbooks(int *booklastid,books *head)
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
    p->id=++*(booklastid);
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
    ptr=fopen("books.txt","w");

    //write the last id to id.txt file
    FILE *id;
    id=fopen("Bookid.txt","w");
   
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

books* deletebooks(int *booklastid,books *head)
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
            if(id>*booklastid) // if the id is greater than last id the id is not created
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
                        printf("The book with the entered id was not foundn\n");
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
    FILE *ptr=fopen("books.txt","r");

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


int id(int *booklastid) //function to get last index from file after closing
{
    FILE *ptr;
    ptr=fopen("Bookid.txt","r");
// created a file to store the last id entered
   
    if(!ptr){
        *booklastid=0;
        return 0;
    }
    if(feof(ptr)){
        *booklastid=0;
        return 0;
    }
    fscanf(ptr,"%d",booklastid);
    fclose(ptr);  
    return *booklastid;
}


//to free all the memory of the linked list
books * freeBookmemory(books *head)
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
void booktransaction(books *booksnode, User *usernode) {
    FILE* fp = fopen("booksTransactions.txt", "a");

    if (fp == NULL) {
        printf("Error: File cannot be opened!!");
        return;
    }

    // Get current date and time
    time_t curr_time;
    int tm_info;
    struct tm* time_info;

    time(&curr_time);
    char date[20];
    strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", localtime(&curr_time));

    fprintf(fp, "Transaction Details:\n");
    fprintf(fp, "BookID: %d\n", booksnode->id);
    fprintf(fp, "Book Name: %s", booksnode->name);
    fprintf(fp, "Author: %s", booksnode->author);
    fprintf(fp, "UserId: %d\n", usernode->id);
    fprintf(fp, "User Name: %s", usernode->name);
    fprintf(fp, "Date and Time Published: %s\n", date);
    fprintf(fp, "Status: Issued\n\n");


    fclose(fp);
}



books *search(books *head)
{
    int choice;
    int id;
    char search[50];
    printf("\nSearch book by :\n1. ID\n2. Name\nEnter a choice : ");
    fflush(stdin);
    scanf("%d",&choice);
    books *p=head;
    if(!p)
    {
        printf("\nThe book was not found\n")        ;
        return NULL;
    }
    fflush(stdin);
    switch(choice)
    {
        case 1:
            printf("Enter the Book ID : ");
            scanf("%d",&id);
            while(p!=NULL)
            {
                if(p->id==id)
                {
                    printf("\nThe Book Details \n");
                    printf("Id : %d\n",p->id);
                    printf("Name : %s",p->name);
                    printf("Author : %s",p->author);
                    printf("Genre : %s",p->genre);
                    printf("Availability : %d\n",p->availability);
                    return p;
                }
                p=p->next;
            }
            printf("\nThe book was not found\n");
            break;
        case 2:
            printf("Enter the name of the book : ");
            fgets(search,sizeof(search),stdin);
            while(p!=NULL)
            {
                if(strcmp(search,p->name)==0)
                {
                    printf("\nThe Book Details \n");
                    printf("Id : %d\n",p->id);
                    printf("Name : %s",p->name);
                    printf("Author : %s",p->author);
                    printf("Genre : %s",p->genre);
                    printf("Availability : %d\n",p->availability);
                    return p;
                }
                p=p->next;
            }
            printf("\nThe book was not found\n");
            break;
        default:
            printf("Enter a valid choice\n");
            break;

    }
}

void Issuebook(User *userList, books *bookList) {
    int userId, foundUser = 0;
    char bookName[50];
    printf("Enter userID: ");
    scanf("%d", &userId);

    // Check if user list is empty
    if (userList == NULL) {
        printf("User list is empty. Add a user first.\n");
        // Call add user function
        // Add your code to add a new user to the linked list here
        return;
    }

    User *currUser = userList;
    while (currUser != NULL) {
        if (currUser->id == userId) {
            foundUser = 1;
            break;
        }
        currUser = currUser->next;
    }

    if (!foundUser) {
        printf("User not found!!\n");
        // Call add user function
        // Add your code to add a new user to the linked list here
        return;
    }
    fflush(stdin);  
    books *currBook;
    currBook=search(bookList);
    if(currBook!=NULL)
    {
        if(currBook->availability==0)
        {
            printf("Book not available for issuing!!\n");
            return;
        }
        currBook->availability--;
        strcpy(currUser->recentBook,currBook->name);
        booktransaction(currBook,currUser);
        addbooksToFile(bookList);
        printf("Book issued successfully!!\n");
    }
    
}
//--------------------------------------------------FUNCTIONS FOR USERS--------------------------------------->>

User *addUser(int *userlastid, User *head) {
    char name[20], address[50], number[15], recentBook[50];

    fflush(stdin);
    printf("Enter the details\n");
    printf("Enter the name of the user: ");
    fgets(name, sizeof(name), stdin);
    printf("Enter the address: ");
    fgets(address, sizeof(address), stdin);
    printf("Enter the number: ");
    fgets(number, sizeof(number), stdin);
    strcpy(recentBook,"None");

    User *user = (User *)malloc(sizeof(User));
    strcpy(user->name, name);
    strcpy(user->address, address);
    strcpy(user->number, number);
    strcpy(user->recentBook, recentBook);
    user->id = ++*(userlastid);
    user->next = NULL;

    if (head == NULL) {
        head = user;
    } else {
        user->next = head;
        head = user;
    }

    return head;
}

void addUsersToFile(User *head) {
    FILE *ptr;
    ptr = fopen("users.txt", "w");
    // write the last id to id.txt file
    FILE *id;
    id = fopen("id.txt", "w");

    if (!ptr) // if the file doesn't open, exit
        return;

    // writing all the added users to file
    User *p = head;
    if (p) // writing id to id.txt
        fprintf(id, "%d", p->id);

    while (p != NULL) {
        fwrite(p, sizeof(User), 1, ptr);
        p = p->next;
    }
    // closing the file
    fclose(ptr);
    fclose(id);
    return;
}

User* deleteUsers(int *userlastid, User *head) {
    // variables
    int choice;
    int id;
    char name[20];
    printf("1. Delete by Id \n2. Delete by name\nEnter a choice : ");
    scanf("%d", &choice);
    switch (choice) {
        case 1: // checking by id
        {
            printf("Enter the id : ");
            scanf("%d", &id);
            if (id > *userlastid) // if the id is greater than last id, the id is not created
                printf("The user with the entered id was not found \n");
            else // search and delete the id from the linked list and the file
            {
                User *p = head;
                User *q = p;

                if (p->id == id) // checking if head is the node to delete
                {
                    head = head->next;
                    printf("The user was deleted successfully\n");
                    free(p);
                } else {
                    while (p != NULL && p->id != id) {
                        q = p;
                        p = p->next;
                    }
                    if (p == NULL) {
                        printf("The user with the entered id was not found\n");
                        return head;
                    }
                    q->next = p->next;
                    printf("The user was deleted successfully\n");
                    free(p);
                }
                addUsersToFile(head); // add the changes if any made to the file
            }
            break;
        }
        case 2: // checking by name
        {
            int flag = 0;
            printf("Enter the name : ");
            fflush(stdin);
            fgets(name, sizeof(name), stdin);

            User *p = head;
            User *r = p;
            // checking for the user in the linked list
            if (strcmp(name, p->name) == 0) // if head is the node to delete
            {
                head = head->next;
                free(p);
                printf("The user was deleted successfully\n");
                return head;
            }
            while (p != NULL) {
                if (strcmp(name, p->name) == 0) {
                    break;
                }
                r = p;
                p = p->next;
            }
            if (p != NULL) // if the user was found
            {
                printf("The user was deleted successfully\n");
                r->next = p->next;
                flag = 1;
                free(p);
                addUsersToFile(head); // add the changes if any made to the file
                return head;
            } else {
                printf("The user with the entered name was not found \n");
                return head;
            }
            break;
        }
    }
    return head;
}

User *initialize_users(User *head) {
    FILE *ptr = fopen("users.txt", "r");
    User *tail = NULL;

    // checking if the file is empty
    if (!ptr) {
        head = NULL;
        return head;
    }

    // writing to linked list
    while (!feof(ptr)) {
        User *p = (User *)malloc(sizeof(User));
        if (!p) {
            printf("Memory allocation failed. Exiting program.\n");
            return NULL;
        }

        if (fread(p, sizeof(User), 1, ptr) != 0) {
            p->next = NULL;
            if (head == NULL) {
                // If the list is empty, set the new user as the head
                head = p;
                tail = p;
            } else {
                // Append the new user to the end of the list
                tail->next = p;
                tail = tail->next;
            }
        }
    }

    fclose(ptr); // closing the file
    return head;
}

//    ptr = fopen("C:\\Users\\Naruto\\Desktop\\a\\libraryManagement\\users.txt", "w");

int getLastUserId() {
    int userlastid = 0;
    FILE *ptr = fopen("Userid.txt", "r");

    if (!ptr) {
        return userlastid;
    }

    if (feof(ptr)) {
        fclose(ptr);
        return userlastid;
    }

    fscanf(ptr, "%d", &userlastid);
    fclose(ptr);

    return userlastid;
}

User *freeUsersMemory(User *head) {
    User *p = head;
    User *q = p;

    while (p != NULL) {
        q = p;
        p = p->next;
        free(q);
    }

    return NULL;
}

//---------------------------------------------MAIN FUNCTIONS-----------------------------------------

int main()
{
   User *userList = NULL;
    int userlastid = 0;
    userList = initialize_users(userList);
    userlastid = getLastUserId();
   
   
    books *head = NULL;
    int booklastid=0;
    head=initialize_books(head);
    booklastid=id(&booklastid);
	
	int choice;
    books *p;
	User *user;
	
	 while (1) {
        printf("\nMenu:\n");
        printf("1. Users \n");
        printf("2. Books\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch(choice)
        {
            case 1:
                while (choice!=4)
                {
                    printf("\nMenu:\n");
                    printf("1. Add User\n");
                    printf("2. Display Users\n");
                    printf("3. Delete User\n"); 
                    printf("4. Exit\n");
                    printf("Enter your choice: ");
                    fflush(stdin);
                    scanf("%d", &choice);
                    switch(choice)
                    {
                        case 1:
                            userList = addUser(&userlastid, userList);
                            break;
                        case 2:
                            user = userList;
                            printf("\nThe user details\n");
                            while (user != NULL) {
                                printf("\nID : %d\n", user->id);
                                printf("Name : %s", user->name);
                                printf("Address : %s", user->address);
                                printf("Number : %s", user->number);
                                printf("Recent Book : %s", user->recentBook);
                                user = user->next;
                            }
                            break;
                        case 3:
                            userList = deleteUsers(&userlastid, userList);
                            break;
                        case 4:
                            addUsersToFile(userList);
                            break;
                        default:
                            printf("Invalid choice. Please enter a valid option.\n");
                            break;
                    }
                }
                break;
            case 2:
                while(choice!=6)
                {
                    printf("\nMenu:\n");
                    printf("1. Issue Book\n");
                    printf("2. Add Book\n");
                    printf("3. Search For Books\n");
                    printf("4. Display Book\n");
                    printf("5. Delete Book\n");
                    printf("6. Exit\n");
                    printf("Enter your choice: ");
                    fflush(stdin);
                    scanf("%d", &choice);
                    switch(choice)
                    {
                        case 1:
                            Issuebook(userList, head);
                            break;
                        case 2:
                            head=addbooks(&booklastid,head);
                            break;
                        case 3:
                            search(head);
                            break;
                        case 4:
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
                        case 5:
                            head=deletebooks(&booklastid,head);
                            break;
                        case 6:
                            addbooksToFile(head);
                            break;
                        default:
                            printf("Invalid choice. Please enter a valid option.\n");
                            break;
                    }
                }
                break;
            case 3:
                addbooksToFile(head);
				addUsersToFile(userList);
                head=freeBookmemory(head);
                exit(0);
            default:
                printf("Invalid choice. Please enter a valid option.\n");
                break;


        }	
    }
}  