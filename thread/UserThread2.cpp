#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdarg.h>

#define HAVE_SETITIMER          (1)

#if HAVE_SETITIMER
#define DEFAULT_TIMEOUT		(100)	//milliseconds
#else
#define DEFAULT_TIMEOUT		(1)	//seconds
#endif

#define DEFAULT_STACK_SIZE      (10 * 1024)
#define ERROR(msg...)           error_at_line(-1, errno, __FILE__, __LINE__, ##msg)

inline void
LOG(const char* fmt, ...)
{
	va_list        args;
	char           buf[1024];
	struct timeval tv;

	va_start(args, fmt);
	gettimeofday(&tv, NULL);
	int len = snprintf(buf, sizeof(buf), "[%ld.%ld] ", (long) tv.tv_sec, (long) tv.tv_usec);
	vsnprintf(buf + len, sizeof(buf) - len, fmt, args);
	va_end(args);

	printf("%s", buf);
}

class UserThread
{
public:
        static void
        schedule()
        {
		signal(SIGALRM, onTimedOut);

		if(getcontext(&scheduler) == -1) {
			ERROR("getcontext");
		}

                if(current == NULL) {
                        current = head;
                } else {
                        current = current->next;
                }

                if(current) {
                        LOG("switching to %s...\n", current->getName());

			setTimeout(current->timeout);

                        if(setcontext(&current->context) == -1) {
                                ERROR("setcontext");
                        }
                } else {
                        LOG("no current thread, exiting scheduler loop...\n");
                }
        }

protected:
        UserThread(unsigned int timeout = DEFAULT_TIMEOUT, size_t stackSize = DEFAULT_STACK_SIZE):
                prev(NULL), next(NULL), timeout(timeout)
        {
                stackBase = (unsigned char*) calloc(stackSize, 1);

                if(stackBase == NULL) {
                        ERROR("calloc");
                }
                this->stackSize = stackSize;

                link();
        }

        virtual
        ~UserThread()
        {
                free(stackBase);
        }

        virtual void
        run()
        {
        }

        virtual const char*
        getName() const
        {	return "<noname>";
        }

        void
        idle()
        {
		setTimeout(0);
                if(swapcontext(&context, &scheduler) == -1) {
                        ERROR("swapcontext");
                }
		setTimeout(timeout);
        }

	static void
	setTimeout(unsigned int millis)
	{
#if HAVE_SETITIMER
		struct itimerval t;
		memset(&t, 0, sizeof(t));
		t.it_value.tv_usec = millis * 1000;	//in microseconds

		setitimer(ITIMER_REAL, &t, NULL);	
#else
		alarm(current->timeout);
#endif
	}

private:
        static void
        threadRoutine(UserThread* self)
        {
                if(self) {
			setTimeout(self->timeout);
                        self->run();
			setTimeout(0);
                        self->unlink();
                }
        }

	static void
	onTimedOut(int arg)
	{
		LOG("Force switching context...\n");

		if(swapcontext(&current->context, &scheduler) == -1) {
			ERROR("setcontext");
		}
	}

        void
        link()
        {
                if(head) {
                        this->prev = head->prev;
                        this->next = head;

                        this->prev->next = this;
                        this->next->prev = this;
                } else {
                        this->prev = this;
                        this->next = this;
                }
                head = this;

                if(getcontext(&context) == -1) {
                        ERROR("getcontext");
                }
                context.uc_stack.ss_sp   = stackBase;
                context.uc_stack.ss_size = stackSize;
                context.uc_link          = &scheduler;
                makecontext(&context, (void(*)()) &threadRoutine, 1, this);
        }

        void
        unlink()
        {
                if(prev && next) {
                        if(prev == this) {
                                head    = NULL;
                                current =  NULL;
                        } else {
                                prev->next = next;
                                next->prev = prev;

                                if(head == this) {
                                        head = next;
                                }
                                if(current == this) {
                                        current = prev;
                                }
                        }
                }
        }

        static UserThread* head;
        static UserThread* current;
        static ucontext_t  scheduler;
	static jmp_buf     interrupt;

        UserThread*        prev;
        UserThread*        next;
        ucontext_t         context;
        size_t             stackSize;
        unsigned char*     stackBase;
	unsigned int       timeout;
};

UserThread* UserThread::head    = NULL;
UserThread* UserThread::current = NULL;
ucontext_t  UserThread::scheduler;
jmp_buf     UserThread::interrupt;

///////////////////////////////////////////////////////////////////

class Worker: public UserThread
{
        int   num;
        int   cur;
	char* name;
public:
        Worker(int num): num(num), cur(-1)
        {
		char buf[256];
		snprintf(buf, sizeof(buf), "Worker[%d]", num);
		name = strdup(buf);
        }

        virtual
        ~Worker()
        {
		free(name);
        }

protected:
        virtual void
        run()
        {
                for(cur = 0; cur < 800000; cur++) {
                        LOG("Worker[%d]: %d\n", num, cur);
//                      idle();
                }
                LOG("Worker[%d]: exited.\n", num);
        }

        virtual const char*
        getName() const
        {
                return name;
        }
};

int
main(int argc, char** argv)
{
        Worker worker1(1);
        Worker worker2(2);
        Worker worker3(3);
        Worker worker4(4);
        Worker worker5(5);

        UserThread::schedule();
        return 0;
}

/* vim:set ts=8 sw=8 foldmethod=syntax foldlevel=1 nu: */

