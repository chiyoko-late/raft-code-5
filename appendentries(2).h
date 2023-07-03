#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <stddef.h>
#include <errno.h>
#include <fcntl.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <thread>
#include <mutex>
#include "my_sock.h"

#define SERVER_ADDR "0.0.0.0"
#define STRING (10LL)
#define ALL_ACCEPTED_ENTRIES (100000L)
#define ENTRY_NUM (10000LL)

// using namespace std;

uint64_t c1,
    c2;
struct timespec ts1, ts2;
double t;

static uint64_t rdtscp()
{
    uint64_t rax;
    uint64_t rdx;
    uint32_t aux;
    asm volatile("rdtscp"
                 : "=a"(rax), "=d"(rdx), "=c"(aux)::);
    return (rdx << 32) | rax;
}

struct append_entry
{
    char entry[STRING];
};

struct AppendEntriesRPC_Argument
{
    int term;
    int leaderID;
    int prevLogIndex;
    int prevLogTerm;
    int leaderCommit;
    std::vector<append_entry> entries = std::vector<append_entry>(ENTRY_NUM);
};

struct AppendEntriesRPC_Result
{
    int term;
    bool success;
};

struct LOG
{
    int term;
    int index;
    std::vector<append_entry> entries = std::vector<append_entry>(ENTRY_NUM);
};

struct AllServer_PersistentState
{
    int currentTerm;
    int voteFor;
    LOG log;
};

struct AllServer_VolatileState
{
    int commitIndex;
    int LastAppliedIndex;
};

struct Leader_VolatileState
{
    std::vector<int> nextIndex = std::vector<int>(2);
    std::vector<int> matchIndex = std::vector<int>(2);
};

char *filename;
char *logfilename;

int fdo;
// // logfile初期化
void make_logfile(char *name)
{
    filename = name;
    fdo = open(filename, (O_CREAT | O_RDWR), 0644);
    if (fdo == -1)
    {
        printf("file open error\n");
        exit(1);
    }
    printf("fdo: %d\n", fdo);
    return;
}

void read_prev(int prevLogIndex, int *read_index, int *read_term)
{
    int read_log[2];
    lseek(fdo, sizeof(struct LOG) * (prevLogIndex - 1), SEEK_SET);
    read(fdo, read_log, sizeof(int) * 2);
    *read_term = read_log[0];
    *read_index = read_log[1];
    return;
}

void write_log(
    int prevLogIndex, struct LOG *log)
{
    // lseek(fdo, (sizeof(ENTRY_NUM * STRING) * prevLogIndex) + 8, SEEK_SET);
    write(fdo, log, (sizeof(ENTRY_NUM * STRING) * prevLogIndex) + 8);
    fsync(fdo);
    return;
}

void read_log(int prevLogIndex)
{
    struct LOG *r_log = new struct LOG;

    lseek(fdo, (sizeof(ENTRY_NUM * STRING) + 8) * prevLogIndex, SEEK_SET);
    read(fdo, r_log, (sizeof(ENTRY_NUM * STRING) + 8));
    printf("%d\n", r_log->index);
    printf("%d\n", r_log->term);
    return;
}
