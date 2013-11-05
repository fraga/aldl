#ifndef ALDLIO_H
#define ALDLIO_H

#include "aldl-types.h"

/* diagnostic comms ------------------------------*/

int aldl_reconnect(); /* go into diagnostic mode, returns 1 on success */

byte *aldl_get_packet(aldl_packetdef_t *p); /* get packet data */

/* generate request strings */
byte *generate_request(byte mode, byte message, aldl_commdef_t *comm);
byte *generate_mode(byte mode, aldl_commdef_t *comm);

/* use generate_request to fill a packet mode str */
byte *generate_pktcommand(aldl_packetdef_t *packet, aldl_commdef_t *comm);

/* serial comms-----------------------------------*/

int serial_init(char *port); /* initalize the serial handler */

/* set up lock structures */
void init_locks();

/* creates a dummy record so the linked list isn't broken when we want to start
   using it, call once at the beginning before acq starts */
void aldl_init_record(aldl_conf_t *aldl);

/* buffer management --------------------------------------*/

/* WARNING: only the acquisition loop should use these functions */

/* process data from all packets, create a record, and link it to the list */
aldl_record_t *process_data(aldl_conf_t *aldl);

/* remove a record from the linked list and deallocate */
void remove_record(aldl_record_t *rec);

/* return a pointer to the oldest record in the linked list; this is for
   garbage collection in acquisition event loops, so don't use it. */
aldl_record_t *oldest_record(aldl_conf_t *aldl);

/* record selection ---------------------------------------*/

/* return the newest or next record in the linked list.  if there is no such
   record, return NULL. */
aldl_record_t *newest_record(aldl_conf_t *aldl);
aldl_record_t *next_record(aldl_record_t *rec);

/* return the newest or next record in the linked list.  if there is no such
   record, wait forever until one is available, unless the connection to the
   ECM is lost, in which case return NULL. */
aldl_record_t *newest_record_wait(aldl_conf_t *aldl, aldl_record_t *rec);
aldl_record_t *next_record_wait(aldl_conf_t *aldl, aldl_record_t *rec);

/* return the newest or next record in the linked list.  if there is no such
   record, wait forever until one is available.  never return anything but a
   valid record. */
aldl_record_t *next_record_waitf(aldl_conf_t *aldl, aldl_record_t *rec);
aldl_record_t *newest_record_waitf(aldl_conf_t *aldl, aldl_record_t *rec);

/* get definition or data array index, returns -1 if not found */
int get_index_by_name(aldl_conf_t *aldl, char *name);

/* connection state management ----------------------------*/

/* this pauses until a 'connected' state is detected */
void pause_until_connected(aldl_conf_t *aldl);

/* this pauses until the buffer is full */
void pause_until_buffered(aldl_conf_t *aldl);

/* get/set connection state */
aldl_state_t get_connstate(aldl_conf_t *aldl);
void set_connstate(aldl_state_t s, aldl_conf_t *aldl);

/* misc locking -------------------------------------------*/

/* lock and unlock the statistics structure */
void lock_stats();
void unlock_stats();

/* terminating functions -------------------------------*/

void serial_close(); /* close the serial port */

/* misc. useful functions ----------------------*/

/* generate a checksum byte */
byte checksum_generate(byte *buf, int len);

/* generate a msg length byte */
byte calc_msglength(byte len);

/* test checksum byte of buf, 1 if ok */
int checksum_test(byte *buf, int len);

/* compare a byte string n(eedle) in h(aystack), nonzero if found */
int cmp_bytestring(byte *h, int hsize, byte *n, int nsize);

/* print a string of bytes in hex format */
void printhexstring(byte *str, int length);

/* return a string that describes a connection state */
char *get_state_string(aldl_state_t s);

/* cleanup function in main */
void main_exit();

#endif
