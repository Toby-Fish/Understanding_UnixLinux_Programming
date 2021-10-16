/* shm_tc2.c - time client shared mem ver2: use semaphores for locking
 * program uses shared memory with key 99
 * program uses semaphore set with key 9900
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#define     TIME_MEM_KEY    99                  /* like a filename */
#define     TIME_SEM_KEY    9900
#define     SEG_SIZE    ( (size_t)100 )         /* size of segment */
#define     oops( m, x )    { perror( m ); exit( x ); }

union   semun { int val; struct semid_ds *buf; ushort *array; };
int     seg_id, semset_id;                      /* global for cleanup() */

void wait_and_lock( int semset_id );
void release_lock( int semset_id );

int main()
{
    int     seg_id;
    char    *mem_ptr, *ctime();
    long    now;

    int     semset_id;                          /* id for semaphore set */

    /* create a shared memory segment */

    seg_id = shmget( TIME_MEM_KEY, SEG_SIZE, IPC_CREAT | 0777 );
    if ( seg_id == -1 )
        oops( "shmget", 1 );

    /* attach to it and get a poniter to where it attaches */

    mem_ptr = shmat( seg_id, NULL, 0 );
    if ( mem_ptr = (void *)-1 )
        oops( "shmat", 2 );

    /* connect to semaphore set 9900 with 2 semaphores */

    semset_id = semget( TIME_SEM_KEY , 2, 0 );
    wait_and_lock( semset_id );

    printf( "The time, direct from memory:..%s", mem_ptr );

    release_lock( semset_id );
    shmdt( mem_ptr );                           /* detach, but not needed here */
}

/*
 * build and execute a 2-element action set:
 * wait for 0 on n_writers AND increment n_readers
 */
void wait_and_lock( int semset_id )
{
    struct sembuf actions[2];                           /* action set */
    actions[0].sem_num = 1;                             /* sem[0] is n_readers */
    actions[0].sem_flg = SEM_UNDO;                      /* auto cleanup */
    actions[0].sem_op = 0;                              /* wait till no readers */

    actions[1].sem_num = 0;                             /* sem[1] is n_writers */
    actions[1].sem_flg = SEM_UNDO;                      /* auto cleanup */
    actions[1].sem_op = +1;                             /* incr num writers */

    if ( semop( semset_id, actions, 2 ) == -1 )
        oops( "semop: locking", 5 );
}

/*
 * build and execute a 1-element action set:
 * decrement num_writers
 */
void released_lock( int semset_id )
{
    union semun sem_info;                               /* some properties */
    struct sembuf actions[1];                           /* action set */

    actions[0].sem_num = 0;                             /* sem[0] is n_writers */
    actions[0].sem_flg = SEM_UNDO;                      /* auto cleanup */
    actions[0].sem_op = -1;                             /* decr writer count */

    if ( semop( semset_id, actions, 2 ) == -1 )
        oops( "semop: unlocking", 6 );
}



