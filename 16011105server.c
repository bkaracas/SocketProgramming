#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 8082
typedef struct {
char ad[30];
//char numara[30];
}contactlist;


typedef struct {
char ad[30];
//char numara[30];
contactlist cnt[30];
int cntcount;
}user;

typedef struct {
char gonderen[30];
char alici[30];
char mesaj[180];
}message;




int main()
{
	char kisi[30], okunacakkisi[30],userad[30];
	int secim=1,i,mesajsayisi,kisiekle=1,kisisil=0,count=0;
	int sfd, cfd, usercount=0, id=0,temp,yenikullanici;
	int server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	message * msg;
	msg=(message*)calloc(1,sizeof(message));			//mesajlara yer açılması
	user *usr;
	usr=(user*)calloc(1,sizeof(user));				//kullanıcılara yer açılması
    

	sfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero((char *) &server_address, sizeof(server_address));
   	server_address.sin_family = AF_INET;
   	server_address.sin_addr.s_addr = htonl(INADDR_ANY);		//soket oluşturulması için gerekli 
    	server_address.sin_port = htons(PORT);				//işlemler
    	server_len = sizeof(server_address);	
    	bind(sfd, (struct sockaddr *)&server_address,server_len);


	listen(sfd, 5);
        /* Accept connection. */
        client_len = sizeof(client_address);
	while(1) {
	printf("\nSunucu calisiyor...\n"); 
        cfd = accept(sfd,(struct sockaddr *)&client_address, &client_len);
	int dongu=0;


	read(cfd,&yenikullanici,sizeof(int));
	if(yenikullanici){
		read(cfd,userad,30);
	//printf("\nusercount:%d",usercount);
	if(usercount==0){						//daha önce kullanıcı eklenmediyse 
		usercount=1;						//kisi sayısını 1 yaptım
		usr=(user*)realloc(usr,((usercount+1)*sizeof(user)));	//dinamik memory allocation ile yer büyüttüm
		strcpy(usr[0].ad,userad);				
		usr[0].cntcount=0;}
		else{
			for(i=0;i<usercount;i++){			//kisi sayısı birden fazlaysa bu adın olup olmadığını 
				if(strcmp(userad,usr[i].ad)==0){	//kontrol edip varsa alan açmadım, yoksa alttaki "if"te alan açıp
				id=i;					//son yere bu kullanıcıyı atadım.
				i=usercount;				//clientta server ile bağlantı kuran kullanıcının bellekte hangi 
				dongu=1;}}				//sırada olduğunu id değişkeni ile kontrol ettim.
	if(dongu==0){
		usr=(user*)realloc(usr,((++usercount)*sizeof(user)));
		strcpy(usr[i].ad,userad);
		usr[i].cntcount=0;
		id=i;}}

/*for(i=0;i<=usercount;i++)
		printf("\nkullanici %d: %s",i,usr[i].ad);*/
}



	secim=0;	
	read(cfd,&secim,4*(sizeof(int)));				//switch case için secim değişkeninin client tan alınması
 	//printf("\nsecim=%d",secim);   
	kisiekle=1; kisisil=0;

switch(secim){

	case 1:
	printf("\nid:%d",id);		
	temp=usr[id].cntcount;
	send(cfd,&temp,sizeof(int),0);					//contact list yazdırılması
	for(i=0;i<usr[id].cntcount;i++){
	send(cfd,usr[id].cnt[i].ad,30,0);  
	}break;

	case 2:
	kisiekle=1;
	read(cfd,kisi,30);
	for(i=0;i<usr[id].cntcount;i++){
		if(strcmp(kisi,usr[id].cnt[i].ad)==0){
		printf("\nbu kullanici zaten ekli");			//kişi ekleme
		kisiekle=0;
		send(cfd,&kisiekle,sizeof(int),0);	}}
	if(kisiekle==1){
	strcpy(usr[id].cnt[usr[id].cntcount].ad,kisi);
	usr[id].cntcount++; 
	printf("\n %s adli kullanici listenize eklendi",usr[id].cnt[usr[id].cntcount-1]);} break ;  

	case 3:
	read(cfd,kisi,30);
	for(i=0;i<usr[id].cntcount;i++){
		if(strcmp(kisi,usr[id].cnt[i].ad)==0){
		for(i;i<usr[id].cntcount-1;i++)
	strcpy(usr[id].cnt[i].ad,usr[id].cnt[i+1].ad);			//kişi silme
	kisisil=1;
	usr[id].cntcount--;
	send(cfd,&kisisil,sizeof(int),0);break; } } break;

	case 4:
	read(cfd,msg[count].gonderen,30);
	read(cfd,msg[count].alici,30);
        read(cfd,msg[count].mesaj,180);					//mesaj gönderme
	printf("\nmesaj gonderildi %s\n",msg[count].mesaj);
	msg=(message*)realloc(msg,((++count)+1)*sizeof(message)); break;

	case 5:read(cfd,kisi,30);
	mesajsayisi=0;
	printf("\nkisi:%s\n",kisi);
	for(i=0;i<count;i++){
	printf("\ncount:%d\n",count);
	printf("\nkisi:%s\n",kisi);
	printf("\nalici:%s\n",msg[i].alici);				//mesajları kontrol etme
	if(strcmp(kisi,msg[i].alici)==0)
	mesajsayisi++;}
	printf("\nmesajsayisi:%d",mesajsayisi);
	send(cfd,&mesajsayisi,sizeof(int),0);
	for(i=0;i<count;i++)
	if(strcmp(kisi,msg[i].alici)==0)
	send(cfd,msg[i].gonderen,30,0); break;

	case 6:
	read(cfd,kisi,30);
	read(cfd,okunacakkisi,30);					//mesaj okuma
	for(i=0;i<=count-1;i++){
	if((strcmp(kisi,msg[i].alici)==0)&&(strcmp(okunacakkisi,msg[i].gonderen)==0)){
	send(cfd,msg[i].mesaj,180,0);					
	send(cfd,msg[i].gonderen,30,0);
	if(i+1>=count){
	strcpy(case6,"0");
	i++;}
	}}
	
break;
	default: break;

}



close(cfd);

   }
}
