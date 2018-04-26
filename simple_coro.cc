#include <iostream>

using namespace std;

/*===--------------- definition ------------------===*/

#define PAGE_SIZE 1024
#define STACK_SIZE (2 * PAGE_SIZE)

typedef uint64_t u64;
typedef uint8_t u8;

struct Context {	// callee save registers
	u64 rbp = 0;
	u64 rbx = 0;
	u64 r12 = 0;
	u64 r13 = 0;
	u64 r14 = 0;
	u64 r15 = 0;
	
	u64 rsp = 0;
	u64 rip = 0;
};

extern "C" void switchTo(Context *, Context *);

typedef void (*fp)();

class Coro {
private:
	Context cxt;
	Context *from;
	u8 stack[STACK_SIZE];
	fp closure;				// task.
public:
	Coro(fp closure = nullptr) : from(nullptr), closure(closure) {		// [[noreturn]]
		if (closure == nullptr) return;
		cxt.rsp = (u64)(stack + STACK_SIZE);
		cxt.rip = (u64)closure;
	}
private:
	void switch_to(Context *other) { switchTo(other, &this->cxt); }
public:
	void yield() 				  { switch_to(this->from); }								// go back to the caller
	void resume(Coro *other)	  { other->from = &this->cxt; switch_to(&other->cxt); }		// go to one `coroutine` except `the main thread`
};

/*===-------------- implementations -----------------====*/

Coro main_coro;			// the main thread's context. It is very important.

/*===---------------- test ---------------------===*/

Coro *first;
Coro *second;

int main(int argc, char *argv[]) 
{
	first = new Coro([](){
		cout << "haha" << endl;
		first->resume(second);
		first->yield();
		cout << "hehe" << endl;
	});
	second = new Coro([](){
		cout << "hoho" << endl;
		second->yield();
		cout << "heihei" << endl;
	});
	
	main_coro.resume(first);
	
	cout << "over." << endl;
}