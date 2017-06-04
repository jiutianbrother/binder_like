#ifndef		_BASE_BINDER_H
#define		_BASE_BINDER_H

#ifdef	__cplusplus
extern "C"
{
#endif

/* client对象在binder中的抽象 */
typedef struct binder_proc {
    pid_t pid;
}binder_proc;


#ifdef	__cplusplus
}
#endif

#endif
