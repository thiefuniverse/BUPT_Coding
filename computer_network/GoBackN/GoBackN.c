#include <stdio.h>
#include <string.h>

#include "protocol.h"
#include "datalink.h"

#define DATA_TIMER  2000
#define ACK_TIMER 500
#define MAX_SEQ 5
#define inc(k) if(k<MAX_SEQ) k++;else k=0

typedef enum{false,true} boolean;

static unsigned char buffer[MAX_SEQ+1][PKT_LEN], nbuffered=0;
static unsigned char frame_expected = 0,next_frame_to_send=0,ack_expected=0;
static int phl_ready = 0;

struct FRAME
{
    unsigned char kind; /* FRAME_DATA */
    unsigned char ack;
    unsigned char seq;
    unsigned char data[PKT_LEN];
    unsigned int  padding;
};

static void put_frame(unsigned char *frame, int len)
{
    *(unsigned int *)(frame + len) = crc32(frame, len);
    send_frame(frame, len + 4);
    phl_ready = 0;
}

static void send_data_frame(void)
{
    struct FRAME s;

    s.kind = FRAME_DATA;
    s.seq = next_frame_to_send;
    s.ack = (frame_expected+MAX_SEQ)%(MAX_SEQ+1);
    memcpy(s.data, buffer[next_frame_to_send], PKT_LEN);

    dbg_frame("Send DATA %d %d, ID %d\n", s.seq, s.ack, *(short *)s.data);

    put_frame((unsigned char *)&s, 3 + PKT_LEN);
    start_timer(next_frame_to_send, DATA_TIMER);
}

static void send_ack_frame(void)
{
    struct FRAME s;

    s.kind = FRAME_ACK;
    s.ack = (frame_expected+MAX_SEQ)%(MAX_SEQ+1);

    dbg_frame("Send ACK  %d\n", s.ack);
    start_ack_timer(ACK_TIMER);

    put_frame((unsigned char *)&s, 2);
}

/*new add*/
static boolean between(unsigned int a, unsigned int b, unsigned int c)
{
    if((a<=b)&&(b<c)||(c<a)&&(a<=b)||(b<c)&&(c<a))
        return true;
    else
        return false;
}

int main(int argc, char **argv)
{
    protocol_init(argc, argv);
    lprintf("Designed by Jiang Yanjun, build: " __DATE__"  "__TIME__"\n");

    int event, arg;
    struct FRAME f;
    int len = 0;

    enable_network_layer();

    while(true)
    {
        event = wait_for_event(&arg);

        switch (event) {
        case NETWORK_LAYER_READY:
            get_packet(buffer[next_frame_to_send]);
            nbuffered++;
            send_data_frame();
            inc(next_frame_to_send);
            break;

        case PHYSICAL_LAYER_READY:
            phl_ready = 1;
            break;

        case FRAME_RECEIVED:
            len = recv_frame((unsigned char *)&f, sizeof f);
            if (len < 5 || crc32((unsigned char *)&f, len) != 0)
            {
                dbg_event("**** Receiver Error, Bad CRC Checksum\n");
                break;
            }
            if (f.kind == FRAME_ACK)
              {
                //      printf("\nThis is ACK %d\n",f.ack);
                dbg_frame("Recv ACK  %d\n", f.ack);

              }
            if (f.kind == FRAME_DATA)
            {
                dbg_frame("Recv DATA %d %d, ID %d\n", f.seq, f.ack, *(short *)f.data);
                if (f.seq == frame_expected)
                {
                    put_packet(f.data, len - 7);
                    inc(frame_expected);
                }
                         send_ack_frame();
            }
            while(between(ack_expected,f.ack,next_frame_to_send))
            {
                nbuffered--;
                stop_timer(ack_expected);
                inc(ack_expected);
            }
//            if (f.ack == frame_nr) {
//                stop_timer(frame_nr);
//                nbuffered--;
//                frame_nr = 1 - frame_nr;
//            }
            break;

        case DATA_TIMEOUT:
            dbg_event("---- DATA %d timeout\n", arg);
            next_frame_to_send=ack_expected;
            int i;
            for(i=1;i<=nbuffered;i++)
            {
                send_data_frame();
                inc(next_frame_to_send);
            }
//            send_data_frame();
            break;
        case ACK_TIMEOUT:
          send_ack_frame();
          break;
        }


        if (nbuffered < MAX_SEQ && phl_ready)
            enable_network_layer();
        else
            disable_network_layer();
   }
}
