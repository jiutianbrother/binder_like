#ifndef	    _CORE_BINDER_H
#define	    _CORE_BINDER_H

#define	    BINDER_CURRENT_PROTOCOL_VERSION	7

//模拟binder驱动的ioctl命令
typedef enum BINDER_COMMAND {
    BINDER_WRITE_READ = 0,
    
    BINDER_SET_IDLE_TIMEOUT,

    BINDER_SET_MAX_THREADS,

    BINDER_SET_IDLE_PRIORITY,

    BINDER_SET_CONTEX_MGR,

    BINDER_THREAD_EXIT,

    BINDER_VERSION,

    BINDER_MAX_COMMAND_VALUE
}BINDER_COMMAND;

/* used for BINDER_WRITE_READ */
typedef struct binder_write_read {
    signed long write_size;
    signed long write_consumed;
    unsigned long write_buffer;

    signed long read_size;
    signed long read_consumed;
    unsigned long read_buffer;
}binder_write_read;

#endif
