#include <iostream>

using namespace std;

/*===--------------- definition ------------------===*/

#define PAGE_SIZE 1024
#define STACK_SIZE (10 * PAGE_SIZE)
#define FRAME_SIZE (sizeof(u64))

typedef uint64_t u64;
typedef uint8_t u8;

struct Context {				// callee save registers
	u64 rbp = 0;
	u64 rbx = 0;
	u64 r12 = 0;
	u64 r13 = 0;
	u64 r14 = 0;
	u64 r15 = 0;
	
	u64 rsp = 0;
	u64 rip = 0;
};

extern "C" void popFromStack();
extern "C" void switchTo(Context *, Context *);

typedef void (*fp)();

class Coro {
private:
	Context cxt;
	Context *from;
	u8 stack[STACK_SIZE];
	fp closure;						// the task. no arguments now.
public:
	Coro(fp closure = nullptr) : from(nullptr), closure(closure) {		// [[noreturn]]
//		if (closure == nullptr) return;
		cxt.rsp = (u64)(stack + STACK_SIZE - FRAME_SIZE * 3);
		cxt.rip = (u64)closure;
	}
private:
	void switch_to(Context *other) { switchTo(other, &this->cxt); }
public:
	void yield() 						  			 { switch_to(this->from); }				// go back to the caller
	void resume(Coro *other)	;																				// go to one `coroutine` except `the main thread`
};

/*===-------------- implementations -----------------====*/

void Coro::resume(Coro *other)	      
{ 
	if (other->cxt.rbp == 0) {		// set the origin callee's stack exit entry
		other->cxt.rbp = other->cxt.rsp;		// not to matter the value
		*(u64*)&other->stack[STACK_SIZE - FRAME_SIZE] = (u64)&this->cxt;
		*(u64*)&other->stack[STACK_SIZE - FRAME_SIZE * 2] = (u64)&other->cxt;
		*(u64*)&other->stack[STACK_SIZE - FRAME_SIZE * 3] = (u64)popFromStack;		// cpu (rip)			// Then, stack are on `other` coro, but rip is on `popFromStack`. 
	}
	
	other->from = &this->cxt; 		// set the callee
	switch_to(&other->cxt); 
}

Coro main_coro;			// the main thread's context. It is very important.
