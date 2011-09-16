//
//  f_pcap.h
//
//  Author: Martin Janiczek (martin@janiczek.cz)
//
//  Date Created: 25.8.2011
//  Last Updated: 14.9.2011
//

#include "pipa.h"

#define PCAP_HDR_SIZE   24
#define PACKET_HDR_SIZE 16 // 4 * uint32
#define PACKET_LEN_POS  8  // third uint32 in the struct is our length
                           // (0 - first, 4 - second, 8 - third)

unsigned int packet_len;

void f_pcap_init   ();
void f_pcap_header ();
void f_pcap_read   ();
