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
    message_buf sbuf, rbuf;
    size_t buf_length;

    if ((msqid = msgget(key, msgflg)) < 0) {
        perror("msgget");
        exit(1);
    }

    // 1. Envía "Are you hearing me?" (mtype = 1)
    sbuf.mtype = 1;
    strcpy(sbuf.mtext, "Are you hearing me?");
    buf_length = strlen(sbuf.mtext) + 1;

    if (msgsnd(msqid, &sbuf, buf_length, 0) < 0) {
        perror("msgsnd (1)");
        exit(1);
    }
    printf("%s\n", sbuf.mtext);

    // 2. Recibe "Loud and clear" (mtype = 2) sin imprimir
    if (msgrcv(msqid, &rbuf, MSGSZ, 2, 0) < 0) {
        perror("msgrcv (2)");
        exit(1);
    }

    // 3. Replica "I can hear you too" (mtype = 3)
    sbuf.mtype = 3;
    strcpy(sbuf.mtext, "I can hear you too");
    buf_length = strlen(sbuf.mtext) + 1;

    if (msgsnd(msqid, &sbuf, buf_length, 0) < 0) {
        perror("msgsnd (3)");
        exit(1);
    }
    printf("%s\n", sbuf.mtext);

    return 0;
}
