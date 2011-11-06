#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>

#define DEFAULT_STACK_SIZE      (10 * 1024)
#define ERROR(msg...)           error_at_line(-1, errno, __FILE__, __LINE__, ##msg)

class UserThread
{
public:
        static void
        schedule()
        {
                if(getcontext(&scheduler) == -1) {
                        ERROR("getcontext");
                }

                if(current == NULL) {
                        current = head;
                } else {
                        current = current->next;
                }

                if(current) {
                        printf("switching to ");
                        current->dump();

                        if(setcontext(&current->context) == -1) {
                                ERROR("setcontext");
                        }
                } else {
                        printf("no current thread, exiting scheduler loop...\n");
                }
        }

protected:
        UserThread(size_t stackSize = DEFAULT_STACK_SIZE):
                prev(NULL), next(NULL)
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

        virtual void
        dump()
        {
        }

        void
        idle()
        {
                if(swapcontext(&context, &scheduler) == -1) {
                        ERROR("swapcontext");
                }
        }

private:
        static void
        threadRoutine(UserThread* self)
        {
                if(self) {
                        self->run();
                        self->unlink();
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

        UserThread*        prev;
        UserThread*        next;
        ucontext_t         context;
        size_t             stackSize;
        unsigned char*     stackBase;
};

UserThread* UserThread::head    = NULL;
UserThread* UserThread::current = NULL;
ucontext_t  UserThread::scheduler;

///////////////////////////////////////////////////////////////////

class Worker: public UserThread
{
        int num;
        int cur;
public:
        Worker(int num): num(num), cur(-1)
        {
        }

        virtual
        ~Worker()
        {
        }

protected:
        virtual void
        run()
        {
                for(cur = 0; cur < 5; cur++) {
                        printf("Worker[%d]: %d\n", num, cur);
                        idle();
                }
                printf("Worker[%d]: exited.\n", num);
        }

        virtual void
        dump()
        {
                printf("Worker[%d]...\n", num);
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

/* vim:set ts=8:set sw=8:set foldmethod=syntax: */

