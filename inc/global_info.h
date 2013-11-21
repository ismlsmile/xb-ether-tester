/* 
 * ������Ϊ��ѡ���Դ������
 * �������İ�Ȩ(����Դ�뼰�����Ʒ����汾)��һ�й������С�
 * ����������ʹ�á�������������
 * ��Ҳ�������κ���ʽ���κ�Ŀ��ʹ�ñ�����(����Դ�뼰�����Ʒ����汾)���������κΰ�Ȩ���ơ�
 * =====================
 * ����: ������
 * ����: sunmingbao@126.com
 */


#ifndef __GLOBAL_INFO_H_
#define __GLOBAL_INFO_H_

#include <windows.h>
#include <stdint.h>
#include "defs.h"


extern const char version[4];

extern HINSTANCE g_hInstance;
extern TCHAR szAppName[];
extern int init_over;

extern HWND    hwnd_frame;
extern HWND hwndTip;
extern int  doc_modified;

extern TCHAR szSubWinClassName[];
extern HWND  hwnd_left;
extern HWND  hwnd_tv;


extern TCHAR szRightWinClassName[];
extern HWND    hwnd_right;
extern HWND    hwnd_stream;
extern HWND    hwnd_fc;
extern HWND    hwnd_lv;

extern TCHAR szBottomWinClassName[];
extern HWND    hwnd_bottom;

#define    SPLT_WIDTH    (6)
extern TCHAR szSpltWeClassName[];
extern TCHAR szSpltNsClassName[];

extern HWND    hwnd_splt_we;
extern HWND    hwnd_splt_ns;

extern int we_pos;
extern int ns_pos;

extern HWND    hwnd_toolbar;
extern int toolbar_height;

extern HWND    hwnd_statusbar;
extern int statusbar_height;

extern TCHAR szHexEditWinClassName[];
extern char  *test_buf;
extern int cur_data_len;
extern int is_read_only;

extern int cur_strm_idx;
extern HWND    hwnd_hex_edit_pcap;
extern HWND    hwnd_hex_edit_strm;
extern HWND hwnd_dynamic_edit;

extern TCHAR szFlatTabWinClassName[];
extern int flat_tab_height;

extern TCHAR szStatsWinClassName[];
extern HWND hwnd_stats;

extern int need_stop;
extern struct timeval time_elapsed;
extern struct timeval last_stat_tv;
extern int snd_stopped, rcv_stopped;
extern int snd_started, rcv_started;

extern uint64_t cap_save_cnt, cap_save_bytes_cnt;

DWORD WINAPI  wpcap_snd_test(LPVOID lpParameter);
DWORD WINAPI  wpcap_rcv_test(LPVOID lpParameter);

extern int cap_stopped;
extern int need_cap_stop;

DWORD WINAPI  rcv_pkt_2(LPVOID lpParameter);

int reg_sys_win_classes();
int register_frame();
int register_left_win();
int register_right_win();
int register_bottom_win();
int register_splt_wins();
int register_hex_edit_win();
int register_stats_win();
int CreateToolbar();
int CreateStatusBar();
int create_windows(int iCmdShow);
void add_tip(HWND htip_control, HWND hwnd, TCHAR *tips);

void resize_window(HWND hwnd);
void hex_win_sel(HWND  hwnd, int offset, int len);
void hex_win_desel(HWND  hwnd, int offset, int len);
extern int cxChar, cyChar;
void init_char_size_info();
void update_statusbar();

#define  WM_SPLITTER_X     (WM_USER + 3)
#define  WM_SPLITTER_Y     (WM_USER + 4)
static inline void send_splitter_x(HWND  hwnd, int pos)
{
    SendMessage(hwnd, WM_SPLITTER_X, pos, 0);
}

static inline void send_splitter_y(HWND  hwnd, int pos)
{
    SendMessage(hwnd, WM_SPLITTER_Y, pos, 0);
}

typedef struct
{
    uint64_t send_total, send_fail, rcv_total;
    uint64_t send_total_bytes, send_fail_bytes, rcv_total_bytes;
} t_pkt_stat;
extern t_pkt_stat gt_pkt_stat, gt_pkt_stat_pre, gt_pkt_stat_tmp;

void update_stats(t_pkt_stat *pt_pkt_stat, unsigned long time_gap);
void clear_stats();
void init_stats_ui();
void stream_edit_data_change(HWND  hwnd, int offset);

#define    ETHERNET_HDR_LEN  14
#define    MAX_IP_PACKET_LEN 65535
#define    MAX_PACKET_LEN    (ETHERNET_HDR_LEN+MAX_IP_PACKET_LEN)
#define    MIN_PACKET_LEN    48

#define    CFG_FILE_FILTER    "ethernet test config(*.etc)\0*.etc\0\0"
#define    CFG_FILE_SUFFIX    "etc"

#define    PCAP_FILE_FILTER    "tcpdump/pcap file(*.pcap)\0*.pcap\0\0"
#define    PCAP_FILE_SUFFIX    "pcap"

typedef struct
{
    unsigned char dst[6];
    unsigned char src[6];
    unsigned short type;
    unsigned char payload[0];

} __attribute__ ((aligned (1))) t_ether_packet;

typedef struct
{
    char     valid;
    uint32_t flags;
    uint16_t offset;
    uint8_t  width;
    char     bits_from;
    char     bits_len;
    char     rsv[4];
    uint8_t base_value[8];
    uint8_t max_value[8];
    uint32_t step_size;
} __attribute__ ((aligned (1))) t_rule;

#define    MAX_FIELD_RULE_NUM    (10)
typedef struct
{
    int  selected;
    char snd_cnt;
    char rsv[7];
    char name[64];
    uint32_t flags;
    char    rule_num;
    char    rule_idx[MAX_FIELD_RULE_NUM];
    t_rule  at_rules[MAX_FIELD_RULE_NUM];
    int len;
    union
    {
        t_ether_packet eth_packet;
        unsigned char data[MAX_PACKET_LEN];
    };

    //non save info
    uint32_t err_flags;

} __attribute__ ((aligned (1))) t_stream;

#define    ERR_IP_CHECKSUM     (0x1<<30)
#define    ERR_UDP_CHECKSUM    (0x1<<29)
#define    ERR_TCP_CHECKSUM    (0x1<<28)
#define    ERR_PKT_LEN         (0x1<<27)
#define    ERR_ICMP_CHECKSUM    (0x1<<26)
#define    ERR_IGMP_CHECKSUM    (0x1<<25)

#define    STREAM_HDR_LEN    ((unsigned long)(void *)(&(((t_stream *)NULL)->data)))

#define    MAX_STREAM_NUM    100
extern int  nr_cur_stream;
extern t_stream    *g_apt_streams[MAX_STREAM_NUM];
void init_stream(t_stream    *pt_streams);
void make_frags(const t_stream *pt_stream, int frag_num);
void get_pkt_desc_info(char *info, void* p_eth_hdr, uint32_t err_flags);
uint32_t  build_err_flags(t_ether_packet *pt_eth, int len);
void delete_all_rule(t_stream *pt_stream);

extern char *protocol_name_map[];

#include <pcap.h>
typedef struct
{
    struct pcap_pkthdr header;
    uint32_t err_flags;
    u_char  pkt_data[0];
} t_dump_pkt;

extern HWND    hwnd_net_card_comb;
extern HWND    hwnd_capture_checkbox;
extern int need_capture;

void rx_tx_init();
void init_net_card_combbox(HWND hwnd_comb);
int select_if(int idx);
int is_filter_valid(char *packet_filter);

int re_populate_items();
LRESULT CALLBACK my_lv_proc (HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
extern WNDPROC old_lv_proc;

int stream2dump(char *file_name);
int save_stream(char *file_path);
int load_stream(char *file_path);
int load_stream_from_dump_file(char *file_path);
extern t_stream gt_edit_stream;
BOOL CALLBACK StreamEditDlgProc (HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam);
void rule_fileds_init(t_stream *pt_stream);
void rule_fileds_update(t_stream *pt_stream);
void rule_fileds_init_all_pkt();
BOOL CALLBACK PktViewDlgProc (HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam);
BOOL CALLBACK PktCapDlgProc(HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam);
BOOL CALLBACK PktCapCfgDlgProc(HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam);
BOOL CALLBACK FragDlgProc(HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam);
void *alloc_stream();
void init_protocol_comb(HWND comb, int add_all);
extern HWND hPktCapDlg;
void update_pkt_cap_stats(t_ether_packet *pt_pkt);


#define SAMPLE_PKT_LEN 74
extern unsigned char sample_pkt[SAMPLE_PKT_LEN];

#define    SPEED_TYPE_HIGH      0
#define    SPEED_TYPE_LOW       1
#define    SPEED_TYPE_FASTEST   2

#define    SND_MODE_CONTINUE    0
#define    SND_MODE_BURST       1
typedef struct
{
    int speed_type;
    int speed_value;
    int snd_mode;
    int snd_times_cnt;
    char rsv[32];
} t_fc_cfg;
extern t_fc_cfg  gt_fc_cfg;

extern int snd_gap_s;
extern int snd_gap_us;

#define PKT_CAP_CFG_MODE_NORMAL     0
#define PKT_CAP_CFG_MODE_ADVANCED   1
#define PKT_CAP_FILE_WHILE_SND    "C:\\WINDOWS\\temp\\fsm_haha_1.pcap"
#define PKT_CAP_FILE_ONLY_CAP     "C:\\WINDOWS\\temp\\fsm_haha_2.pcap"
#define PKT_CAP_PKT_TYPE_L2    1
#define PKT_CAP_PKT_TYPE_IP    2
#define PKT_CAP_PKT_TYPE_ALL    3


#define PKT_CAP_PORT_MIN    0
#define PKT_CAP_PORT_MAX    65535

#define    MAX_CAP_FILTER_STR_LEN    256
typedef struct
{
    int need_save_capture;
    int pkt_cap_cfg_mode;
    int pkt_cap_pkt_type;
    int pkt_cap_sip, pkt_cap_dip, pkt_cap_protocol;
    int pkt_cap_sport, pkt_cap_dport;
    int filter_str_len;
    char rsv[32];
    char filter_str_usr[MAX_CAP_FILTER_STR_LEN];
} __attribute__ ((aligned (1))) t_pkt_cap_cfg;
#define    PKT_CAP_CFG_FIX_LEN    ((unsigned long)(void *)(&(((t_pkt_cap_cfg *)NULL)->filter_str_usr)))

extern t_pkt_cap_cfg  gt_pkt_cap_cfg;


extern char pkt_cap_filter_str[MAX_CAP_FILTER_STR_LEN];
extern char pcap_file_to_view[MAX_FILE_PATH_LEN];
extern int  cur_view_pkt_idx;

extern HWND    hwnd_b_tab;
extern int err_cnt, info_cnt;
void print_mem(void *start_addr, uint32_t length);
void PrintText(TCHAR * szFormat, ...);

void WriteInfo(TCHAR * szFormat, ...);
#define sys_log(format, args...) \
    do \
    { \
        WriteInfo("%s(%d), %s:\r\n"format"\r\n", __FILE__, __LINE__, __FUNCTION__, ##args); \
        info_cnt++; \
    } while (0)
    
#define sys_err_log(format, args...) \
    do \
    { \
        WriteInfo("%s(%d), %s:\r\n"format":GetLastError():%d\r\n", __FILE__, __LINE__, __FUNCTION__, ##args, GetLastError()); \
        err_cnt++; \
    } while (0)

#endif
