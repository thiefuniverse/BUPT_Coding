#include <stdio.h>
#include <string.h>

#include "protocol.h"
#include "datalink.h"

#define DATA_TIMER  5000
#define ACK_TIMER  500
#define MAX_SEQ 101U
#define NR_BUFS ((MAX_SEQ+1)/2)
typedef enum {
    false, true
} boolean;
typedef unsigned char frame_kind;
#define seq_nr unsigned char
#define inc(k) if((k)<MAX_SEQ) (k)++; else (k)=0

struct FRAME {
    unsigned char kind;
    /* FRAME_DATA */
    unsigned char ack;
    unsigned char seq;
    unsigned char data[PKT_LEN];
    unsigned int padding;
};

static unsigned char next_frame_to_send = 0, nbuffered = 0;
static unsigned char frame_expected = 0, ack_expected = 0;
static unsigned char out_buffer[NR_BUFS][PKT_LEN], in_buffer[NR_BUFS][PKT_LEN];
static boolean no_nak = true, arrived[NR_BUFS];
static seq_nr oldest_frame = 0, too_far = NR_BUFS;
static int phl_ready = 0;


static boolean between(seq_nr a, seq_nr b, seq_nr c)
{
    if (((a <= b) && (b < c)) || ((c < a) && (a <= b)) || ((b < c) && (c < a)))
        return true;
    else
        return false;
}

static void put_frame(unsigned char *frame, int len)
{
    *(unsigned int *) (frame + len) = crc32(frame, len);
    send_frame(frame, len + 4);
    phl_ready = 0;
}

static void send_data_frame(frame_kind fk, seq_nr frame_nr)
{
    struct FRAME s;

    s.kind = fk;
    s.seq = frame_nr;
    s.ack = (unsigned char) ((frame_expected + MAX_SEQ) % (MAX_SEQ + 1));
    if (fk == FRAME_DATA)
    {
        memcpy(s.data, out_buffer[frame_nr % NR_BUFS], PKT_LEN);
        start_timer(frame_nr % NR_BUFS, DATA_TIMER);
        dbg_frame("Send DATA %d %d, ID %d\n", s.seq, s.ack, *(short *) s.data);
    }
    if (fk == FRAME_NAK)
    {
        no_nak = false;
        dbg_frame("Send NAK  %d\n", s.ack);
    }
    if (fk == FRAME_ACK)
        dbg_frame("Send ACK  %d\n", s.ack);
    put_frame((unsigned char *) &s, 3 + PKT_LEN);
    stop_ack_timer();
}

static void send_ack_frame(void)
{
    struct FRAME s;

    s.kind = FRAME_ACK;
    s.ack = (unsigned char) ((frame_expected + MAX_SEQ) % (MAX_SEQ + 1));

    dbg_frame("Send ACK  %d\n", s.ack);

    put_frame((unsigned char *) &s, 2);
}

/*****************************************************************************/
/* static void send_nak_frame(void)                                          */
/* {                                                                         */
/*     struct FRAME s;                                                       */
/*                                                                           */
/*     s.kind = FRAME_NAK;                                                   */
/*     s.ack = (unsigned char) ((frame_expected + MAX_SEQ) % (MAX_SEQ + 1)); */
/*                                                                           */
/*     dbg_frame("Send NAK  %d\n", s.ack);                                   */
/*                                                                           */
/*     put_frame((unsigned char *) &s, 2);                                   */
/* }                                                                         */
/*****************************************************************************/

int main(int argc, char **argv) {
    int event, arg;
    struct FRAME f;
    int len = 0;

    protocol_init(argc, argv);
    lprintf("Designed by Shantom, build: " __DATE__"  "__TIME__"\n");

    disable_network_layer();

    for(int i=0;i<NR_BUFS;i++)
        arrived[i]=false;

    int i = 0;
    for (i = 0; i < 1e10; i++)
    {
        event = wait_for_event(&arg);

        switch (event)
        {
            case NETWORK_LAYER_READY:
                nbuffered++;
                get_packet(out_buffer[next_frame_to_send % NR_BUFS]);
                send_data_frame(FRAME_DATA, next_frame_to_send);
                inc(next_frame_to_send);
                break;

            case PHYSICAL_LAYER_READY:
                phl_ready = 1;
                break;

            case FRAME_RECEIVED:
                len = recv_frame((unsigned char *) &f, sizeof f);
                if (len < 5 || crc32((unsigned char *) &f, len) != 0)
                {
                    dbg_event("**** Receiver Error, Bad CRC Checksum\n");
                    if (no_nak)
                        send_data_frame(FRAME_NAK,0);
                    break;
                }
                if (f.kind == FRAME_ACK)
                    dbg_frame("Recv ACK  %d\n", f.ack);
                if (f.kind == FRAME_DATA) {
                    dbg_frame("Recv DATA %d %d, ID %d\n", f.seq, f.ack, *(short *) f.data);
                    if ((f.seq != frame_expected) && no_nak)
                        send_data_frame(FRAME_NAK,0);
                    else
                        start_ack_timer(ACK_TIMER);

                    if (between(frame_expected, f.seq, too_far) && (arrived[f.seq % NR_BUFS] == false)) {
                        arrived[f.seq % NR_BUFS] = true;
                        memcpy(in_buffer[f.seq % NR_BUFS], f.data, PKT_LEN);
                        while (arrived[frame_expected % NR_BUFS])
                        {
                            put_packet(in_buffer[frame_expected % NR_BUFS], PKT_LEN);
                            no_nak = true;
                            arrived[frame_expected % NR_BUFS] = false;
                            inc(frame_expected);
                            inc(too_far);
                            start_ack_timer(ACK_TIMER);
                        }
                    }
                }
                if ((f.kind == FRAME_NAK) &&
                    between(ack_expected, (unsigned char) ((f.ack + 1) % (MAX_SEQ + 1)), next_frame_to_send))
                {
                    dbg_frame("Recv NAK  %d\n", f.ack);
                    send_data_frame(FRAME_DATA, (unsigned char) ((f.ack + 1) % (MAX_SEQ + 1)));
                }

                while (between(ack_expected, f.ack, next_frame_to_send))
                {
                    nbuffered--;
                    stop_timer(ack_expected % NR_BUFS);
                    inc(ack_expected);
                    oldest_frame=ack_expected;
                }

                break;
            case ACK_TIMEOUT:
                send_ack_frame();
                break;
            case DATA_TIMEOUT:
                dbg_event("---- DATA %d timeout\n", arg);
                send_data_frame(FRAME_DATA, oldest_frame);
                break;
            default:
                break;
        }

        if (nbuffered < NR_BUFS && phl_ready)
            enable_network_layer();
        else
            disable_network_layer();
    }
}

