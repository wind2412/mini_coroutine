#include "mini_coro.cc"

using namespace std;


/*===---------------- test ---------------------===*/

Coro *first;
Coro *second;

Coro *third;

int main(int argc, char *argv[]) 
{
	// first test
	
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
	
	
	// another test
	
	
	third = new Coro([](){
		cout << "haha" << endl;
		third->yield();
	});
	
	main_coro.resume(third);
	
	cout << "over." << endl;
}