#include <stdio.h>

EXEC SQL BEGIN DECLARE SECTION;
		VARCHAR username[20];
		VARCHAR password[20];
EXEC SQL END DECLARE SECTION;

EXEC SQL INCLUDE sqlca;

void insert();
void del();
void update();
void query();

int main()
{
	int operate;
	strcpy(username.arr,"scott");
	username.len=strlen(username.arr);
	strcpy(password.arr,"hotel000");
	password.len=strlen(password.arr);

	EXEC SQL WHENEVER SQLERROR GOTO logon_error;
	EXEC SQL CONNECT :username IDENTIFIED by :password;

	printf("connect database successful \n");
	while(1)
	{
		printf("**************select operate*************\n");
		printf("                                         \n");
		printf("              1.Insert                   \n");
		printf("              2.Delete                   \n");
		printf("              3.Update                   \n");
		printf("              4.Query                    \n");
		printf("              5.Quit                     \n");
		printf("*****************************************\n");
		printf("input your selection\n");
		scanf("%d",&operate);
		switch(operate)
		{
			case 1:
				insert();
				break;
			case 2:
				del();
				break;
			case 3:
				update();
				break;
			case 4:
				query():
				break;
			case 5:
				break;
			default:
				printf("invalid selection \n");
				break;
		}
		if(operate==5)
			break;
	}
	EXEC SQL COMMIT RELEASE;
	printf("logout\n");
	exit(0);

	logon_error:
		printf("invalid username or password\n");
		printf("\n%.70s\n",sqlca.splerrm.sqlerrmc);
		EXEC SQL WHENEVER SQLERROR CONTINUE;
		EXEC SQL ROLLBACK RELEASE;
		exit(1);


}
/*
	根据房间号增加房间，根据客人姓名增加客人，根据房间类型名增加房间类型
*/
void insert()
{
	EXEC SQL BEGIN DECLARE SECTION;
			int room_id;
			short ind_id;
			VARCHAR room_num[50];
			int type_id;
			short ind_type;
			char state[10];
			VARCHAR guest_name[50];
			char sex[5];
			char mobile[11];
			char arrive_time[50];
			char leave_time[50];
			VARCHAR type_name[50];
			VARCHAR type_price[50];
	EXEC SQL END DECLARE SECTION;
	int type;
	while(1)
	 {
			printf("**************select operate*************\n");
			printf("                                         \n");
			printf("              1.Insert room number       \n");
			printf("              2.Insert guset             \n");
			printf("              3.Insert room type         \n");
			printf("              4.exit                     \n");
		  printf("input your selection\n");
		  scanf("%d",type);
		  switch(type)
		  {
		  	case 1:
		  		printf("input room number\n");
		  		gets(room_unm.arr);
		  		room_num.len=strlen(room_num.arr);
		  		EXEC SQL SELECT TYEP_ID
		  				 INTO :type_id :ind_type
		  				 FROM ROOM
		  				 WHERE ROOM_NUM = :room_num;
		  		if(ind_type!=-1)
		  			{
		  				printf("already exit \n");
		  				return;
		  			}
		  		printf("input room type");
		  		gets(type_name.arr);
		  		type_name.len=strlen(type_name.arr);
		  		EXEC SQL SELECT TYPE_ID
		  				 INTO :type_id :ind_type
		  				 FROM ROOMTYPE
		  				 WHERE TYPE_NAME = :type_name;
		  		while(ind_type!=-1)
		  			{
		  				printf("valid type\n");
		  				printf("input room type\n");
		  				gets(type_name.arr);
		  				type_name.len=strlen(type_name.arr);
		  				EXEC SQL SELECT TYPE_ID
		  				 		 INTO :type_id :ind_type
		  				 		 FROM ROOMTYPE
		  				 		 WHERE TYPE_NAME = :type_name;
		  			}


		  		/********************************************
		  		//此处调用存储过程
		  		//EXEC SQL INSERT INTO ROOM(ROOM_NUM,TYPE_ID)
		  		  // VALUES(:room_num,:type_id);
		  		  //printf("insert successful\n");
		  		  *****************************************/

		  		EXEC SQL COMMIT;
		  		break;
		  	case 2:
		  		printf("input guest name\n");
		  		gets(guest_name.arr);
		  		room_num.len=strlen(guest_name.arr);
		  		printf("input guest sex\n");
		  		gets(sex);
		  		printf("input guest mobile number\n");
		  		gets(mobile);
		  		printf("input room type name\n");
		  		gets(type_name.arr);
		  		room_num.len=strlen(room_num.arr);
		  		printf("input arrive time\n");
                gets(arrive_time.arr);
                printf("input leave time\n");
                gets(leave_time);

                /*****room_info为视图**************************/
		  		EXEC SQL SELECT ROOM_ID
		  				 INTO :room_id
		  				 FROM ROOM_INFO
		  				 WHERE STATE='空闲'  and TYPE_NAME=:type_name;
		  	 /* while(ind_id==-1)
		  			{
		  				printf("valid room number\n");
		  				printf("input room number\n");
		  				gets(room_num.arr);
		  				room_num.len=strlen(room_num.arr);
		  				EXEC SQL SELECT ROOM_ID
		  				 		 INTO :room_id :ind_id
		  				 		 FROM ROOM
		  				 		 WHERE ROOM_NUM = :room_num;
		  			}
		  			*/

		  			while(room_id==-1)
		  			{
		  			    printf("this type of room full,please choose another type\n");
		  			    printf("input room type name\n");
                        gets(type_name);
                        EXEC SQL SELECT ROOM_ID
		  				 INTO :room_id
		  				 FROM ROOM_INFO
		  				 WHERE STATE='空闲'  and TYPE_NAME=:type_name;
		  			}


                /************************在此调用存储过程或函数**********************************/
		  		EXEC SQL INSERT INTO GUEST(GUEST_NAME,SEX,MOBILE,ROOM_ID)
		  		   VALUES(:guest_name,:sex,:mobile,:room_id);
		  		  //printf("insert successful\n");
		  	  EXEC SQL COMMIT;
		  		break;

		  	case 3:
		  		printf("input new room type\n");
		  		gets(type_name.arr);
		  		room_num.len=strlen(type_name.arr);
		  		printf("input type price\n");
		  		gets(type_price.arr);
		  		type_price.len=strlen(type_price.arr);
		  		EXEC SQL INSERT INTO ROOMTYPE(TYPE_NAME,TYPE_PRICE)
		  		   VALUES(:type_name,:type_price);
		  		  //printf("insert successful\n");
		  	  EXEC SQL COMMIT;
		  	  break;
		  	 case 4:
		  	 	break;
		  	 default:
		  	 	printf("valid input\n");
		  	 	break;
		  }
		  if(type==4)
		  	break;
	 }
}
void del()
{
		EXEC SQL BEGIN DECLARE SECTION;
			int room_id;
			short ind_id
			VARCHAR room_num[50];
			int type_id;
			short ind_type;
			VARCHAR guest_name[50];
			VARCHAR type_name[50];
	EXEC SQL END DECLARE SECTION;
	int type;
	while(1)
	 {
			printf("**************select operate*************\n");
			printf("                                         \n");
			printf("              1.delete room number       \n");
			printf("              2.delete guset             \n");
			printf("              3.delete room type         \n");
			printf("              4.exit                     \n");
		  printf("input your selection\n");
		  scanf("%d",type);
		  switch(type)
		  {
		  	case 1:
		  		printf("input room number\n");
		  		gets(room_num.arr);
		  		room_num.len=strlen(room_num.arr);
		  		EXEC SQL SELECT TYEP_ID
		  				 INTO :type_id :ind_type
		  				 FROM ROOM
		  				 WHERE ROOM_NUM = :room_num;
		  		if(ind_type==-1)
		  			{
		  				printf("not exit \n");
		  				return;
		  			}
		  		EXEC SQL DELETE
		  		   FROM ROOM
		  		   WHERE ROOM_NUM=:room_num;
		  		  //printf("insert successful\n");
		  		EXEC SQL COMMIT;
		  		break;
		  	case 2:
		  		printf("input guest name\n");
		  		gets(guest_name.arr);
		  		guest_name.len=strlen(guest_name.arr);
		  		EXEC SQL DELETE
		  		   FROM GUEST
		  		   WHERE GUEST_NAME=:guest_name;
		  		  //printf("insert successful\n");
		  		EXEC SQL COMMIT;
		  		break;
		  	case 3:
		  		printf("input  room type\n");
		  		gets(type_name.arr);
		  		room_num.len=strlen(type_name.arr);
		  		EXEC SQL DELETE
		  		   FROM ROOMTYPE
		  		   WHERE TYPE_NAME=:type_name;
		  	  EXEC SQL COMMIT;
		  	  break;
		  	 case 4:
		  	 	break;
		  	 default:
		  	 	printf("valid input\n");
		  	 	break;
		  }
		  if(type==4)
		  	break;
	 }
}
void update()
{
		EXEC SQL BEGIN DECLARE SECTION;
			int room_id;
			short ind_id
			VARCHAR room_num[50];
			int type_id;
			short ind_type;
			char state[10];
			VARCHAR guest_name[50];
			char sex[5];
			char mobile[11];
			char arrive_time[50];
			char leave_time[50];
			VARCHAR type_name[50];
			VARCHAR type_price[50];
	EXEC SQL END DECLARE SECTION;
	int type;
	while(1)
	 {
			printf("**************select operate*************\n");
			printf("                                         \n");
			printf("              1.update room number       \n");
			printf("              2.update guest             \n");
			printf("              3.update room type         \n");
			printf("              4.exit                     \n");
		  printf("input your selection\n");
		  scanf("%d",type);
		  switch(type)
		  {
		  	case 1:
		  		printf("input room number\n");
		  		gets(room_num.arr);
		  		room_num.len=strlen(room_num.arr);
		  		EXEC SQL SELECT TYEP_ID
		  				 INTO :type_id :ind_type
		  				 FROM ROOM
		  				 WHERE ROOM_NUM = :room_num;
		  		if(ind_type!=-1)
		  			{
		  				printf("not exit \n");
		  				return;
		  			}
		  		printf("input room type");
		  		gets(type_name.arr);
		  		type_name.len=strlen(type_name.arr);
		  		EXEC SQL SELECT TYPE_ID
		  				 INTO :type_id :ind_type
		  				 FROM ROOMTYPE
		  				 WHERE TYPE_NAME = :type_name;
		  		while(ind_type==-1)
		  			{
		  				printf("valid type\n");
		  				printf("input room type\n");
		  				gets(type_name.arr);
		  				type_name.len=strlen(type_name.arr);
		  				EXEC SQL SELECT TYPE_ID
		  				 		 INTO :type_id :ind_type
		  				 		 FROM ROOMTYPE
		  				 		 WHERE TYPE_NAME = :type_name;
		  			}
		  		printf("input room state");
		  		gets(state);
		  		EXEC SQL UPDATE ROOM
		  		   SET TYPE_ID=:type_id,STATE=:state
		  		   WHERE ROOM_NUM=:room_num;
		  		  //printf("insert successful\n");
		  		EXEC SQL COMMIT;
		  		break;
		  	case 2:
		  		printf("input guest name\n");
		  		gets(guest_name.arr);
		  		room_num.len=strlen(guest_name.arr);
		  		printf("input guest sex\n");
		  		gets(sex);
		  		printf("input guest mobile number\n");
		  		gets(mobile);
		  		printf("input guest room number\n");
		  		gets(room_num.arr);
		  		room_num.len=strlen(room_num.arr);
		  	  while(ind_id==-1)
		  			{
		  				printf("valid room number\n");
		  				printf("input room number\n");
		  				gets(room_num.arr);
		  				room_num.len=strlen(room_num.arr);
		  				EXEC SQL SELECT ROOM_ID
		  				 		 INTO :room_id :ind_id
		  				 		 FROM ROOM
		  				 		 WHERE ROOM_NUM = :room_num;
		  			}
		  		EXEC SQL UPDATE GUEST
		  		   SET SEX=:sex,MOBILE=:moblie,ROOM_ID=:room_id
		  		   WHERE GUEST_NAME=:guest_name;
		  	  EXEC SQL COMMIT;
		  		break;
		  	case 3:
		  		printf("input new room type\n");
		  		gets(type_name.arr);
		  		type_name.len=strlen(type_name.arr);
		  		printf("input type price\n");
		  		gets(type_price.arr);
		  		type_price.len=strlen(type_price.arr);
		  		EXEC SQL UPDATE ROOMTYPE
		  		   SET TYPE_PRICE=:type_price
		  		   WHERE TYPE_NAME=:type_name;
		  	  EXEC SQL COMMIT;
		  	  break;
		  	 case 4:
		  	 	break;
		  	 default:
		  	 	printf("valid input\n");
		  	 	break;
		  }
		  if(type==4)
		  	break;
	 }
}
void query()
{
		EXEC SQL BEGIN DECLARE SECTION;
			int room_id;
			short ind_id
			VARCHAR room_num[50];
			int type_id;
			short ind_type;
			char state[10];
			VARCHAR guest_name[50];
			short ind_guest;
			char sex[5];
			char mobile[11];
			char arrive_time[50];
			char leave_time[50];
			VARCHAR type_name[50];
			VARCHAR type_price[50];
	EXEC SQL END DECLARE SECTION;
	int type;
	while(1)
	 {
			printf("**************select operate*************\n");
			printf("                                         \n");
			printf("              1.query room numbe         \n");
			printf("              2.query guset              \n");
			printf("              3.query room type          \n");
			printf("              4.exit                     \n");
		  printf("input your selection\n");
		  scanf("%d",type);
		  switch(type)
		  {
		  	case 1:
		  		printf("input room number\n");
		  		gets(room_num.arr);
		  		room_num.len=strlen(room_num.arr);
		  		EXEC SQL SELECT TYEP_ID,STATE
		  				 INTO :type_id :ind_type,state
		  				 FROM ROOM
		  				 WHERE ROOM_NUM = :room_num;
		  		if(ind_type==-1)
		  			{
		  				printf("not exit \n");
		  				return;
		  			}
		  		EXEC SQL SELECT TYPE_NAME
		  				 INTO :type_name
		  				 FROM ROOMTYPE
		  				 WHERE TYPE_ID = :type_id;
		  		printf("room type is%s  room state is%s",type_name.arr,state);
		  		break;
		  	case 2:
		  		printf("input guest name\n");
		  		gets(guest_name.arr);
		  		room_num.len=strlen(guest_name.arr);
		  		EXEC SQL SELECT SEX,MOBILE,ROOM_ID
		  				 INTO :sex :ind_guest,:mobile,:room_id
		  				 FROM GUEST
		  				 WHERE GUEST_NAME = :guest_name;
		  	  while(ind_guset==-1)
		  			{
							printf("not exit \n");
		  				return;
		  			}
		  		EXEC SQL SELECT ROOM_NUM
		  				 INTO :room_num
		  				 FROM ROOM
		  				 WHERE ROOM_ID = :room_id;
		  		printf("sex is%s  mobile is%s  room number is%s",sex,mobile,room_num.arr);
		  		break;
		  	case 3:
		  		printf("input new room type\n");
		  		gets(type_name.arr);
		  		room_num.len=strlen(type_name.arr);
		  		EXEC SQL SELECT TYPE_PRICE
		  				 INTO :type_price
		  				 FROM ROOMTYPE
		  				 WHERE TYPE_NAME = :type_name;
		  		printf("type price is %s",type_price.arr);
		  	  break;
		  	 case 4:
		  	 	break;
		  	 default:
		  	 	printf("valid input\n");
		  	 	break;
		  }
		  if(type==4)
		  	break;
	 }
}
