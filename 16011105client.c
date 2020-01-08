/*code of client*/

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define PORT 8082

typedef struct {
char gonderen[30];
char alici[30];
char mesaj[180];
}kullan;

int main(int argc, char *argv[]) {
int secim, mesajsayisi,kisisil=0,kisiekle, i,cntcount,devam=1,control=0,yenikullanici=1;
char kontrol[30],mesaj1[180],kisi[30];
int sfd;
int bytes;
struct sockaddr_in sa;
ssize_t numRead;

kullan p;
printf("kullanici adinizi giriniz.");								//kullanıcı adının kullanıcıdan alınması
scanf("%s",p.gonderen);
	

while(1){

    if ((sfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 1;										//soket oluşturulması için gerekli 
    }													//işlemler				
															
    bzero(&sa, sizeof sa);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(PORT);
    sa.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sfd, (struct sockaddr *)&sa, sizeof sa) < 0) {
        perror("connect");								//sunucuya bağlanma
        close(sfd);
        return 2;
    }
if(control==1){
printf("\n\nyeni bir islem yapmak ister misiniz?\n1-EVET\n0-HAYIR");
scanf("%d",&devam);}									//aynı kullanıcı bir defa işlem yapınca devam edilsin mi diye 
if(devam==1){										//sorulması 1-evet 0-hayır
control=1;
	send(sfd,&yenikullanici,sizeof(int),0);
	if(yenikullanici){
	send(sfd,p.gonderen,30,0);}							//kullanıcı adının gönderilmesi

	yenikullanici=0;

printf(" 1-Kişileri Listele \n 2-arkadas ekle \n 3-Kullanici sil \n 4-mesaj yaz\n 5-Mesaj kontrol et\n 6-mesaj oku\n \n");
scanf("%d",&secim);
send(sfd,&secim,4*(sizeof(int)),0);							//switch case için secim değişkeninin servera gönderilmesi
switch(secim){

	case 1: read(sfd,&cntcount,sizeof(int));
		printf("\n%s\n","your contacts:");
		for(i=0;i<cntcount;i++){
		read(sfd,kisi,30);printf("\n%d - %s",i+1,kisi);				//contact list yazdırılması
		}printf("\ncount:%d",cntcount);
	break;

	case 2: printf("\neklemek istediğiniz kisnin adini giriniz.");
		scanf("%s",kisi);
		send(sfd,kisi,30,0);
		read(sfd,&kisiekle,sizeof(int));
		if(kisiekle==0){							//kişi ekleme
		printf("\nbu kullanici zaten ekli");}
		else{
		printf("\n%s adli kullanici listenize eklendi",kisi);
		} break;
	case 3:
		printf("\nsilmek istediğiniz kisinin adini giriniz.");
		scanf("%s",kisi);
		send(sfd,kisi,30,0);							//kişi silme
		read(sfd,&kisisil,sizeof(int));
		if(kisisil==1){
		printf("\nbu kullanici silindi");
		kisisil=0;
			}break;
	case 4: printf("\nAlici: ");
        	scanf("%s",p.alici);
		printf("\nMesaj: ");
        	scanf("%s",p.mesaj);							//mesaj gönderme
       		send(sfd,p.gonderen,30,0);
        	send(sfd,p.alici,30,0);
        	send(sfd,p.mesaj,180,0);
		close(sfd); 
		break;
	case 5: send(sfd,p.gonderen,30,0);
		read(sfd,&mesajsayisi,sizeof(int));
		printf("\nmesaj sayisi%d\n",mesajsayisi);	 
		for(i=0;i<mesajsayisi;i++) {						//mesajları kontrol etme
		read(sfd,kontrol,30);
		printf("\nalici:%s\n ",kontrol);} break;



	case 6: send(sfd,p.gonderen,30,0); 
		printf("\nMesajlarini okumak istediginiz kisiyi giriniz.\n"); 
		scanf("%s",kisi);
		send(sfd,kisi,30,0); 
		for(i=0;i<mesajsayisi;i++){						//mesaj okuma
		read(sfd,mesaj1,180);
		read(sfd,kisi,30);
		printf("\ngonderen: %s\nmesaj:%s",kisi ,mesaj1);} break;

	}
close(sfd);
}else {close(sfd); break;}}
   return 0;
}


