#include <string.h>
#include <sys/socket.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <stdio.h>
#include <stdlib.h> 
	 
#define PORT 8111

//------------------------------Function declarations---------------------------------------//

int client(int sock);
int menu(int sock,int type);
int user_function(int sock,int choice);
int crud_train(int sock,int choice);
int crud_user(int sock,int choice);

int main(void) { 
	int sock; 
    	struct sockaddr_in server; 
    	char server_reply[50],*server_ip;
	server_ip = "127.0.0.1"; 
     
    	sock = socket(AF_INET, SOCK_STREAM, 0); 
    	if (sock == -1) { 
       	printf("Could not create socket"); 
    	} 
    
    	server.sin_addr.s_addr = inet_addr(server_ip); 
    	server.sin_family = AF_INET; 
    	server.sin_port = htons(PORT); 
   
    	if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0)
       	perror("connect failed. Error"); 
    
	while(client(sock)!=3);
    	close(sock); 
    	
	return 0; 
} 

//-------------------- First function which is called-----------------------------//

int client(int sock){
	int choice,valid;
	printf("\n\t************************************");
	printf("\n\t      TRAIN RESERVATION SYSTEM      \n");
	printf("\t************************************\n");
	printf("\t1. Sign In\n");
	printf("\t2. Sign Up\n");
	printf("\t3. Exit\n");
	printf("\tEnter Your Choice : ");
	scanf("%d", &choice);
	write(sock, &choice, sizeof(choice));
	if (choice == 1){					// Log in
		system("clear");
		int id,type;
		char password[50];
		printf("\n\t*****************************************");
		printf("\n\t               Login Menu                ");
		printf("\n\t*****************************************");
		printf("\n\tEnter login id : ");
		scanf("%d", &id);
		strcpy(password,getpass("\tEnter password : "));
		write(sock, &id, sizeof(id));
		write(sock, &password, sizeof(password));
		read(sock, &valid, sizeof(valid));
		if(valid){
			printf("\n\tLogin successful !!\n");
			read(sock,&type,sizeof(type));
			while(menu(sock,type)!=-1);
			//system("clear");
			return 1;
		}
		else{
			printf("\n\tLogin Failed !!\n\tReason : invalid id/password !!\n");
			return 1;
		}
	}

	else if(choice == 2){					// Sign up
		system("clear");
		int type,id;
		char name[50],password[50],secret_pin[6];
		system("clear");
		printf("\n\t*****************************************");
		printf("\n\t              Sign up Menu               ");
		printf("\n\t*****************************************");
		printf("\n\tFollowing types of accounts can be created : \n");
		printf("\t0. Admin\n\t1. Agent\n\t2. Customer\n");
		printf("\tYour Response : ");
		scanf("%d", &type);
		printf("\tEnter Your Name : ");
		scanf("%s", name);
		strcpy(password,getpass("\tEnter Your Password : "));

		
		write(sock, &type, sizeof(type));
		write(sock, &name, sizeof(name));
		write(sock, &password, strlen(password));
		
		read(sock, &id, sizeof(id));
		printf("\n\tYour login id is : %d\n\n\tREMEMBER IT FOR FURTHER LOGINS !!\n", id);
		return 2;
	}
	else							// Log out
		return 3;
	
}

//-------------------- Main menu function-----------------------------//

int menu(int sock,int type){
	int choice;
	if(type==2 || type==1){					// Agent and Customer
		printf("\n\t*****************************************");
		printf("\n\t          Agent/Customer Menu            ");
		printf("\n\t*****************************************");
		printf("\n\t1. Book Ticket\n");
		printf("\t2. View Bookings\n");
		printf("\t3. Update Booking\n");
		printf("\t4. Cancel booking\n");
		printf("\t5. Logout\n");
		printf("\tYour Choice : ");
		scanf("%d",&choice);
		write(sock,&choice,sizeof(choice));
		return user_function(sock,choice);
	}
	else if(type==0){					// Admin
		printf("\n\t*****************************************");
		printf("\n\t              Admin Menu                 ");
		printf("\n\t*****************************************");
		printf("\n\t1. CRUD operations on train\n");
		printf("\t2. CRUD operations on user\n");
		printf("\t3. Logout\n");
		printf("\tYour Choice : ");
		scanf("%d",&choice);
		write(sock,&choice,sizeof(choice));
			if(choice==1){
				system("clear");
				printf("\n\t*****************************************");
				printf("\n\t          Train CRUD operations          ");
				printf("\n\t*****************************************");
				printf("\n\t1. Add train\n");
				printf("\t2. View train\n");
				printf("\t3. Modify train\n");
				printf("\t4. Delete train\n");
				printf("\tYour Choice : ");
				scanf("%d",&choice);	
				write(sock,&choice,sizeof(choice));
				return crud_train(sock,choice);
			}
			else if(choice==2){
				system("clear");
				printf("\n\t*****************************************");
				printf("\n\t           User CRUD operations          ");
				printf("\n\t*****************************************");
				printf("\n\t1. Add User\n");
				printf("\t2. View all users\n");
				printf("\t3. Modify user\n");
				printf("\t4. Delete user\n");
				printf("\tYour Choice : ");
				scanf("%d",&choice);
				write(sock,&choice,sizeof(choice));
				return crud_user(sock,choice);
			
			}
			else if(choice==3)
				return -1;
	}	
	
}

//-------------------------------- crud operations on train-----------------------------//
int crud_train(int sock,int choice){
	int valid = 0;
	if(choice==1){				// Add train response
		system("clear");
		char tname[50];
		printf("\n\t*****************************************");
		printf("\n\t                Add Train                ");
		printf("\n\t*****************************************");
		printf("\n\tEnter train name : ");
		scanf("%s",tname);
		write(sock, &tname, sizeof(tname));
		read(sock,&valid,sizeof(valid));	
		if(valid)
			printf("\n\tTrain added successfully !!\n");

		return valid;	
	}
	
	else if(choice==2){			// View train response
		system("clear");
		int no_of_trains;
		int tno;
		char tname[50];
		int tseats;
		int aseats;
		printf("\n\t*****************************************");
		printf("\n\t               View Trains               ");
		printf("\n\t*****************************************");
		read(sock,&no_of_trains,sizeof(no_of_trains));

		printf("\n\tT_no\t\tT_name\t\tT_seats\t\tA_seats\n");
		while(no_of_trains--){
			read(sock,&tno,sizeof(tno));
			read(sock,&tname,sizeof(tname));
			read(sock,&tseats,sizeof(tseats));
			read(sock,&aseats,sizeof(aseats));
			
			if(strcmp(tname, "deleted")!=0)
				printf("\t%d\t\t%s\t\t%d\t\t%d\n",tno,tname,tseats,aseats);
		}

		return valid;	
	}
	
	else if (choice==3){			// Update train response
		system("clear");
		int tseats,choice=2,valid=0,tid;
		char tname[50];
		write(sock,&choice,sizeof(int));
		crud_train(sock,choice);
		printf("\n\t*****************************************");
		printf("\n\t              Update Train               ");
		printf("\n\t*****************************************");
		printf("\n\tEnter the train number you want to modify : ");
		scanf("%d",&tid);
		write(sock,&tid,sizeof(tid));
		
		printf("\n\t1. Train Name\n\t2. Total Seats\n");
		printf("\tYour Choice : ");
		scanf("%d",&choice);
		write(sock,&choice,sizeof(choice));
		
		if(choice==1){
			read(sock,&tname,sizeof(tname));
			printf("\n\tCurrent name : %s",tname);
			printf("\n\tUpdated name : ");
			scanf("%s",tname);
			write(sock,&tname,sizeof(tname));
		}
		else if(choice==2){
			int booked_seats;
			read(sock,&booked_seats,sizeof(booked_seats));
			read(sock,&tseats,sizeof(tseats));
			printf("\n\tTotal seats : %d",tseats);
		L:	printf("\n\tCurrently booked seats : %d",booked_seats);
			printf("\n\tPlease enter seats greater than equal to booked seats !!");
			printf("\n\tUpdated value : ");
			scanf("%d",&tseats);
			if(tseats<booked_seats) goto L;
			write(sock,&tseats,sizeof(tseats));
		}
		read(sock,&valid,sizeof(valid));
		if(valid)
			printf("\n\n\tTrain data updated successfully !!\n");
		return valid;
	}

	else if(choice==4){				// Delete train response
		system("clear");
		int choice=2,tid,valid=0;
		write(sock,&choice,sizeof(int));
		crud_train(sock,choice);
		
		printf("\n\t*****************************************");
		printf("\n\t              Delete Train               ");
		printf("\n\t*****************************************");
		printf("\n\tEnter the train number you want to delete : ");
		scanf("%d",&tid);
		write(sock,&tid,sizeof(tid));
		read(sock,&valid,sizeof(valid));
		if(valid)
			printf("\n\tTrain deleted successfully !!\n");
		return valid;
	}
	
}

//-------------------------------- crud operations on user-----------------------------//

int crud_user(int sock,int choice){
	int valid = 0;
	if(choice==1){							// Add user
		system("clear");
		int type,id;
		char name[50],password[50];
		printf("\n\t*****************************************");
		printf("\n\t                Add User                 ");
		printf("\n\t*****************************************");
		printf("\n\tFollowing types of accounts can be created : \n");
		printf("\t1. Agent\n\t2. Customer\n");
		printf("\tYour choice : ");
		scanf("%d", &type);
		printf("\n\tUser Name : ");
		scanf("%s", name);
		strcpy(password,getpass("\tPassword : "));
		write(sock, &type, sizeof(type));
		write(sock, &name, sizeof(name));
		write(sock, &password, strlen(password));
		read(sock,&valid,sizeof(valid));	
		if(valid){
			read(sock,&id,sizeof(id));
			printf("\n\tYour login id is : %d\n\n\tREMEMBER IT FOR FURTHER LOGINS !!\n", id);
		}
		return valid;	
	}
	
	else if(choice==2){						// View user list
		system("clear");
		int no_of_users;
		int id,type;
		char uname[50];
		printf("\n\t*****************************************");
		printf("\n\t               View Users                ");
		printf("\n\t*****************************************");
		read(sock,&no_of_users,sizeof(no_of_users));

		printf("\n\tU_id\t\tU_name\t\tU_type\n");
		while(no_of_users--){
			read(sock,&id,sizeof(id));
			read(sock,&uname,sizeof(uname));
			read(sock,&type,sizeof(type));
			
			if(strcmp(uname, "deleted")!=0)
				printf("\t%d\t\t%s\t\t%d\n",id,uname,type);
		}

		return valid;	
	}

	else if (choice==3){						// Update user
		system("clear");
		int choice=2,valid=0,uid;
		char name[50],pass[50];
		write(sock,&choice,sizeof(int));
		crud_user(sock,choice);
		printf("\n\t*****************************************");
		printf("\n\t               Update User               ");
		printf("\n\t*****************************************");
		printf("\n\tEnter the U_id you want to modify : ");
		scanf("%d",&uid);
		write(sock,&uid,sizeof(uid));
		
		printf("\n\t1. User Name\n\t2. Password\n");
		printf("\tYour Choice : ");
		scanf("%d",&choice);
		write(sock,&choice,sizeof(choice));
		
		if(choice==1){
			read(sock,&name,sizeof(name));
			printf("\n\tCurrent name : %s",name);
			printf("\n\tUpdated name : ");
			scanf("%s",name);
			write(sock,&name,sizeof(name));
			read(sock,&valid,sizeof(valid));
		}
		else if(choice==2){
			printf("\n\tEnter Current password : ");
			scanf("%s",pass);
			write(sock,&pass,sizeof(pass));
			read(sock,&valid,sizeof(valid));
			if(valid){
				printf("\n\tEnter new password : ");
				scanf("%s",pass);
			}
			else
				printf("\n\tIncorrect password !!\n");
			
			write(sock,&pass,sizeof(pass));
		}
		if(valid){
			read(sock,&valid,sizeof(valid));
			if(valid)
				printf("\n\tUser data updated successfully !!\n");
		}
		return valid;
	}

	else if(choice==4){						// Delete a user
		system("clear");
		int choice=2,uid,valid=0;
		write(sock,&choice,sizeof(int));
		crud_user(sock,choice);
		
		printf("\n\t*****************************************");
		printf("\n\t               Delete User               ");
		printf("\n\t*****************************************");
		printf("\n\tEnter the id you want to delete : ");
		scanf("%d",&uid);
		write(sock,&uid,sizeof(uid));
		read(sock,&valid,sizeof(valid));
		if(valid)
			printf("\n\tUser deleted successfully !!\n");
		return valid;
	}
}

//-------------------------------- User function to book tickets -----------------------------//
int user_function(int sock,int choice){
	int valid =0;
	if(choice==1){										// Book tickets
		system("clear");
		int view=2,tid,seats;
		write(sock,&view,sizeof(int));
		crud_train(sock,view);
		printf("\n\t*****************************************");
		printf("\n\t              Book Tickets               ");
		printf("\n\t*****************************************");
		printf("\n\tEnter the train number you want to book: ");
		scanf("%d",&tid);
		write(sock,&tid,sizeof(tid));
				
		printf("\n\tEnter the no. of seats you want to book : ");
		scanf("%d",&seats);
		write(sock,&seats,sizeof(seats));
	
		read(sock,&valid,sizeof(valid));
		if(valid)
			printf("\n\tTickets booked successfully !!\n");
		else
			printf("\n\tSeats were not available !!\n");

		return valid;
	}
	
	else if(choice==2){									// View the bookings
		system("clear");
		printf("\n\t*****************************************");
		printf("\n\t              View Bookings              ");
		printf("\n\t*****************************************");
		int no_of_bookings;
		int id,tid,seats;
		read(sock,&no_of_bookings,sizeof(no_of_bookings));

		printf("\n\tB_id\t\tT_no\t\tSeats\n");
		while(no_of_bookings--){
			read(sock,&id,sizeof(id));
			read(sock,&tid,sizeof(tid));
			read(sock,&seats,sizeof(seats));
			
			if(seats!=0)
				printf("\t%d\t\t%d\t\t%d\n",id,tid,seats);
		}

		return valid;
	}

	else if(choice==3){									// Update a booking (increment/ decrement seats)
		system("clear");
		int choice = 2,bid,val,valid;
		user_function(sock,choice);
		printf("\n\t*****************************************");
		printf("\n\t             Update Booking              ");
		printf("\n\t*****************************************");
		printf("\n\tEnter the B_id you want to modify : ");
		scanf("%d",&bid);
		write(sock,&bid,sizeof(bid));

		printf("\n\t1. Increase number of seats\n\t2. Decrease number of seats\n");
		printf("\tYour Choice : ");
		scanf("%d",&choice);
		write(sock,&choice,sizeof(choice));

		if(choice==1){
			printf("\n\tNo. of tickets to increase : ");
			scanf("%d",&val);
			write(sock,&val,sizeof(val));
		}
		else if(choice==2){
			printf("\n\tNo. of tickets to decrease : ");
			scanf("%d",&val);
			write(sock,&val,sizeof(val));
		}
		read(sock,&valid,sizeof(valid));
		if(valid)
			printf("\n\tBooking updated successfully !!\n");
		else
			printf("\n\tUpdation failed. No more seats available !!\n");
		return valid;
	}
	
	else if(choice==4){									// Cancel the entire booking
		system("clear");
		int choice = 2,bid,valid;
		user_function(sock,choice);
		printf("\n\t*****************************************");
		printf("\n\t          Cancel Entire Booking          ");
		printf("\n\t*****************************************");
		printf("\n\tEnter the B_id you want to cancel : ");
		scanf("%d",&bid);
		write(sock,&bid,sizeof(bid));
		read(sock,&valid,sizeof(valid));
		if(valid)
			printf("\n\tBookings cancelled successfully !!\n");
		else
			printf("\n\tCancellation failed !!\n");
		return valid;
	}
	else if(choice==5)									// Logout
		return -1;
	
}
