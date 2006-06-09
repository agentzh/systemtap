#ifndef _LKET_B2A_H
#define _LKET_B2A_H
#include <glib.h>

#define LKET_MAGIC	0xAEFCDB6B

#define MAX_STRINGLEN	256

#define APPNAMELIST_LEN	256

#define SEQID_SIZE 4

#define EVT_SYS 1
#define EVT_USER 2

#define MAX_GRPID  255
#define MAX_HOOKID  255
#define MAX_EVT_TYPES 2

#define DEFAULT_OUTFILE_NAME "lket.out"


/* Group ID Definitions */
int _GROUP_REGEVT = 1;
int _GROUP_SYSCALL = 2;
int _GROUP_PROCESS = 3;
int _GROUP_IOSCHED = 4;
int _GROUP_TASK = 5;
int _GROUP_SCSI = 6;
int _GROUP_PAGEFAULT = 7;
int _GROUP_NETDEV = 8;

/* hookIDs defined inside each group */
int _HOOKID_REGSYSEVT = 1;
int _HOOKID_REGUSREVT = 2;

int _HOOKID_SYSCALL_ENTRY = 1;
int _HOOKID_SYSCALL_RETURN = 2;

int _HOOKID_PROCESS_SNAPSHOT = 1;
int _HOOKID_PROCESS_EXECVE = 2;
int _HOOKID_PROCESS_FORK = 3;

int _HOOKID_IOSCHED_NEXT_REQ = 1;
int _HOOKID_IOSCHED_ADD_REQ = 2;
int _HOOKID_IOSCHED_REMOVE_REQ = 3;

int _HOOKID_TASK_CTXSWITCH = 1;
int _HOOKID_TASK_CPUIDLE = 2;

int _HOOKID_SCSI_IOENTRY = 1;
int _HOOKID_SCSI_IO_TO_LLD = 2;
int _HOOKID_SCSI_IODONE_BY_LLD = 3;
int _HOOKID_SCSI_IOCOMP_BY_MIDLEVEL = 4;

int _HOOKID_PAGEFAULT = 1;

int _HOOKID_NETDEV_RECEIVE = 1;
int _HOOKID_NETDEV_TRANSMIT = 2;

typedef struct _lket_pkt_header {
	int16_t	total_size;
	int16_t	sys_size;
	int8_t	hookgroup;
	int8_t	hookid;
	int32_t sec;
	int32_t usec;
	int32_t	pid;
	int32_t	ppid;
	int32_t	tid;
	int8_t	cpu;
} __attribute__((packed)) lket_pkt_header;

typedef struct _appname_info {
	int pid;
	int ppid;
	int tid;
	long index;
	struct _appname_info *next;
} appname_info;

typedef struct {
	char evt_fmt[256][7]; /* e.g. INT8,STRING,INT16,... */
	char evt_names[256][64]; /* e.g. protocal,dev_name,buff_len,... */
	char fmt[256];  /* e.g. %1b,%0s,%2b,... */
	int count;
} event_desc;

/* 
 * handle the bothering sequence id generated by systemtap
 */
static int skip_sequence_id(FILE *fp);

/* 
 * search the lket_init_header structure in a set of input files 
 */
static void find_init_header(FILE **fp, const int total_infiles, FILE *outfp);

/* 
 * read the lket_pkt_header structure at the begining of the input file 
 */
static int get_pkt_header(FILE *fp, lket_pkt_header *phdr);

/* 
 * print the lket_pkt_header structure into the output file
 */
static void print_pkt_header(FILE *fp, lket_pkt_header *phdr);

void register_appname(int i, FILE *fp, lket_pkt_header *phdr);
gint compareFunc(gconstpointer a, gconstpointer b, gpointer user_data);
void destroyAppName(gpointer data);

void register_events(int evt_type, FILE *infp, size_t size);
void ascii_print(lket_pkt_header header, FILE *infp, FILE *outfile, int evt_type);
char *get_fmtstr(char *fmt);
#endif
