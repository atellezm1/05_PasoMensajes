#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main() {
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key = 1234;
    message_buf rbuf, sbuf;
    size_t buf_length;

    if ((msqid = msgget(key, msgflg)) < 0) {
        perror("msgget");
        exit(1);
    }

    // 1. Recibe "Are you hearing me?" (mtype = 1) sin imprimir
    if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
        perror("msgrcv (1)");
        exit(1);
    }

    // 2. Responde "Loud and clear" (mtype = 2)
    sbuf.mtype = 2;
    strcpy(sbuf.mtext, "Loud and clear");
    buf_length = strlen(sbuf.mtext) + 1;

    if (msgsnd(msqid, &sbuf, buf_length, 0) < 0) {
        perror("msgsnd (2)");
        exit(1);
    }
    printf("%s\n", sbuf.mtext);

    // 3. Recibe "I can hear you too" (mtype = 3) sin imprimir
    if (msgrcv(msqid, &rbuf, MSGSZ, 3, 0) < 0) {
        perror("msgrcv (3)");
        exit(1);
    }

    // Elimina la cola de mensajes del sistema
    msgctl(msqid, IPC_RMID, NULL);

    return 0;
}
