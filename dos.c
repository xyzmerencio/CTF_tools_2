#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sockfd;
    struct sockaddr_in target_addr;
    char *target_ip = "192.168.1.100";  // Altere para o IP de destino
    int target_port = 8080;             // Altere para a porta de destino
    char *message = "Pacote UDP enviado!";

    // Criando o socket UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Erro ao criar socket");
        return 1;
    }

    // Configurando o endereço de destino
    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(target_port);
    target_addr.sin_addr.s_addr = inet_addr(target_ip);

    // Enviar pacotes de forma contínua
    while (1) {
        int sent_bytes = sendto(sockfd, message, strlen(message), 0,
                                (struct sockaddr *)&target_addr, sizeof(target_addr));
        if (sent_bytes < 0) {
            perror("Erro ao enviar pacote");
            break;
        } else {
            printf("Enviado: %s\n", message);
        }
        sleep(1);  // Espera de 1 segundo entre envios
    }

    close(sockfd);
    return 0;
}

