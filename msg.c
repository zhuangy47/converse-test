#include <stdio.h>
#include <converse.h>

int msg_index;

typedef struct myMsg {
    char header[CmiMsgHeaderSizeBytes];
    int payload;
} myMsg;




void myHandler(void *msg) {
    // receive logic
    myMsg* receivedMsg = (myMsg*)msg;
    CmiPrintf("Received ping at PE %d\n", CmiMyPe());

    // check if we have gotten back to the first PE  
    if (CmiMyPe() == 0) ++(receivedMsg->payload);    

    // reply logic
    int nextPE = (CmiMyPe() + 1) % CmiNumPes();
    CmiSetHandler(msg, msg_index);
    CmiPrintf("Sending ping from PE #%d to PE #%d\n", CmiMyPe(), nextPE);
    CmiSyncSendAndFree(nextPE, sizeof(myMsg), receivedMsg);

    if (receivedMsg->payload == 1) {
        CmiPrintf("Exiting PE #%d\n", CmiMyPe());
        CsdExitScheduler();
    }
}
void myInit() {
    msg_index = CmiRegisterHandler(myHandler);

    if (CmiMyPe() == 0) {
        myMsg* msg = (myMsg*)CmiAlloc(sizeof(myMsg));
        msg->payload = 0;
        CmiSetHandler(msg, msg_index);
        int nextPE = (CmiMyPe() + 1) % CmiNumPes();
        CmiPrintf("Sending ping from PE #%d to PE #%d\n", CmiMyPe(), nextPE);
        CmiSyncSendAndFree(nextPE, sizeof(myMsg), msg);
    }
}

int main(int argc, char **argv) {
  ConverseInit(argc,argv,myInit,0,0);
}
