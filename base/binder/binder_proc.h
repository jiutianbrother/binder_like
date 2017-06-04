#ifndef	    _BINDER_BASE_BINDER_PROC_H
#define	    _BINDER_BASE_BINDER_PROC_H

class BinderProc {

public:
    BinderProc(pid_t pid = -1);

public:
    pid_t mPid;
};

#endif
