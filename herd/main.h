#include <stdint.h>

/*
 * The polling logic in HERD requires the following:
 * 1. 0 < MICA_OP_GET < MICA_OP_PUT < HERD_OP_GET < HERD_OP_PUT
 * 2. HERD_OP_GET = MICA_OP_GET + HERD_MICA_OFFSET
 * 3. HERD_OP_PUT = MICA_OP_PUT + HERD_MICA_OFFSET
 *
 * This allows us to detect HERD requests by checking if the request region
 * opcode is more than MICA_OP_PUT. And then we can convert a HERD opcode to
 * a MICA opcode by subtracting HERD_MICA_OFFSET from it.
 */
#define HERD_MICA_OFFSET 10
#define HERD_OP_GET (MICA_OP_GET + HERD_MICA_OFFSET)
#define HERD_OP_PUT (MICA_OP_PUT + HERD_MICA_OFFSET)

#define HERD_NUM_BKTS (2 * 1024 * 1024)
#define HERD_LOG_CAP (1024 * 1024 * 1024)

//#define HERD_NUM_KEYS (8 * 1024 * 1024)
#define HERD_NUM_KEYS (100000)
#define HERD_VALUE_SIZE MICA_HERD_VALUE_SIZE
/* Request sizes */
#define HERD_GET_REQ_SIZE (16 + 1) /* 16 byte key + opcode */

/* Key, op, len, val */
#define HERD_PUT_REQ_SIZE (16 + 1 + 1 + HERD_VALUE_SIZE)

/* Configuration options */
#define MAX_SERVER_PORTS 4
#define NUM_WORKERS 2//if you want to test single cpu, change this to 1 and enable sole_worker
#define SOLE_WORKER //if we change workers to 1 directly, herd will crash
#define NUM_CLIENTS 2
//#define TEST_LATENCY //set num_clients 2, sole_worker, num_workers 2, window_size 1, unsig_batch 2
#if 1
    #define TEST_LATENCY_WRITE 1
    #define TEST_LATENCY_READ 0
    #define TEST_LATENCY_MODE TEST_LATENCY_WRITE
#endif
/* Performance options */
#define WINDOW_SIZE 1 /* Outstanding requests kept by each client */
#define NUM_UD_QPS 1   /* Number of UD QPs per port */
#define USE_POSTLIST 1

#define UNSIG_BATCH 2 /* XXX Check if increasing this helps */
#define UNSIG_BATCH_ (UNSIG_BATCH - 1)

/* SHM key for the 1st request region created by master. ++ for other RRs.*/
#define MASTER_SHM_KEY 24
//#define RR_SIZE (16 * 1024 * 1024) /* Request region size */
#define RR_SIZE (36 * 1024 * 1024) /* Request region size */
#define OFFSET(wn, cn, ws) \
  ((wn * NUM_CLIENTS * WINDOW_SIZE) + (cn * WINDOW_SIZE) + ws)

struct thread_params {
  int id;
  int base_port_index;
  int num_server_ports;
  int num_client_ports;
  int update_percentage;
  int postlist;
};

void* run_master(void* arg);
void* run_worker(void* arg);
void* run_client(void* arg);
