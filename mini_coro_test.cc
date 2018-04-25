#include "mini_coro.cc"

using namespace std;


/*===---------------- test ---------------------===*/

Coro *first;
Coro *second;

Coro *third;

Coro *fourth;
Coro *fifth;
Coro *sixth;

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
		second->yield();
	});
	
	main_coro.resume(first);
	
	cout << "------------------" << endl;
	
	
	// another test
	
	third = new Coro([](){
		cout << "haha" << endl;
	});
	
	main_coro.resume(third);
	
	cout << "------------------" << endl;
	
	
	// another another test
	
	fourth = new Coro([](){
		cout << "1" << endl;
		fourth->resume(fifth);
		cout << "2" << endl;
		fourth->resume(sixth);
		cout << "4" << endl;
		fourth->resume(fifth);
		cout << "6" << endl;
	});
	fifth = new Coro([](){
		fifth->yield();
		cout << "5" << endl;
		fifth->yield();
	});
	sixth = new Coro([](){
		cout << "3" << endl;
		sixth->yield();
		sixth->resume(fourth);
	});
	
	main_coro.resume(fourth);
	
	
}