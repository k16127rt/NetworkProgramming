#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdio.h>//printfを使うから必要
#define BUF_SIZE 256
#include<string.h>//strlen使うから


//メッセージを受信して表示するコード
void commun(int sock) {
	char buf[BUF_SIZE];
	int len_r;//受信文字数
	char *message = "";
	if(send(sock, message, strlen(message), 0) != strlen(message))
		DieWithError("send()sent a message of unexpected bytes");
	if((len_r = recv(sock, buf, BUF_SIZE, 0)) <= 0)
		DieWithError("recv() failed");
	buf[len_r] = '\0';
	printf("%s\n", buf);
	if(send(sock, buf, strlen(buf), 0) != strlen(buf))
	DieWithError("send() sent a message of unexpected bytes");
}

int main(int argc, char **argv) {
	//clisok:ほかのとこでやってねってやつ
	//servsock:待ち受け専用
	int cliSock;
	int servSock = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servAddress;
	struct sockaddr_in clientAddress;
	unsigned int szClientAddr;
	
	servAddress.sin_family = AF_INET;
	servAddress.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY:インターネットのアドレスだったらなんでもいい
	servAddress.sin_port = htons(10001);//10001番ポートも接続してきたら対応する
	bind(servSock, (struct sockaddr *)&servAddress, sizeof(servAddress));
	listen(servSock, 5);//順番待ちできるクライアント数(接続待ち数)
	while(1){
		szClientAddr = sizeof(clientAddress);
		cliSock = accepct(servSock, (struct sockaddr *)&clientAddress, &szClientAddr);
		commun(cliSock);
	}
	
	close(servSock);
	return 0;
}