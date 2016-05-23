#include "libraries.h"

class Thread {
public:
        int current_thread;

#ifdef __linux__
	pthread_t *thread = new pthread_t();
	pthread_mutex_t *section_fot_print = new pthread_mutex_t();
	pthread_mutex_t *section_for_close = new pthread_mutex_t();
#endif

#ifdef _WIN32
	HANDLE thread_handle;
	CRITICAL_SECTION section_fot_print;
	CRITICAL_SECTION section_for_close;
	DWORD threadID;
#endif

	Thread(int & thread_number);
	bool can_print();
	bool print_pause();
	bool close();
	void start_of_print();
	void print_end();
	void end_of_print();
};

#ifdef _WIN32

DWORD WINAPI print_thread(void *);


Thread::Thread(int & thread_number) {
	current_thread = thread_number;
	InitializeCriticalSection(&section_fot_print);
	InitializeCriticalSection(&section_for_close);
	if (thread_number == 1) {
		InitializeCriticalSection(&section_for_threads);
	}
	thread_handle = CreateThread(NULL, 100, print_thread, (void*)this, 0, &threadID);
}


bool Thread::can_print() {
	if (TryEnterCriticalSection(&section_for_threads) == 0) {
		return false;
	}
	LeaveCriticalSection(&section_for_threads);
	return true;
}

void Thread::print_end() {
	LeaveCriticalSection(&section_fot_print);
	LeaveCriticalSection(&section_for_threads);
}

bool Thread::close() {
	if (TryEnterCriticalSection(&section_for_close) == 0)
		return true;
	LeaveCriticalSection(&section_for_close);
	return false;
}

bool Thread::print_pause() {
	if (TryEnterCriticalSection(&section_fot_print) == 0) {	
		return false;
	}
	LeaveCriticalSection(&section_fot_print);
	printf(" %d ", this->current_thread);
	return true;
}

void Thread::start_of_print() {
	EnterCriticalSection(&section_fot_print);
}

void Thread::end_of_print() {
	EnterCriticalSection(&section_for_close);
				printf(" %d ", this->current_thread);

}


DWORD WINAPI print_thread(void * name) {

	Thread *thread = (Thread*)name;
	while (true) {
		if (!(thread->print_pause())) {
			EnterCriticalSection(&section_for_threads);
			for (int i = 0; name_of_thread[(thread->current_thread) - 1][i] != 0; i++) {
				printf("%c", name_of_thread[(thread->current_thread) - 1][i]);
				fflush(stdout);
				Sleep(SLEEP_TIME_FOR_PRINT);
			}
			for (int i = 0; name_of_thread[AMOUNT_OF_THREADS][i] != 0; i++) {
				printf("%c", name_of_thread[AMOUNT_OF_THREADS][i]);
				fflush(stdout);
				Sleep(SLEEP_TIME_FOR_PRINT);
			}
			fflush(stdout);
			thread->print_end();
		}
		else {
			Sleep(SLEEP_TIME);
		}
		if (thread->close()) {
			break;
		}
	}
	return 0;
}


#elif __linux__
	
	void * print_thread(void *);


Thread::Thread(int & thread_number) {
	current_thread = thread_number;

	pthread_mutex_init(section_fot_print, NULL);
	pthread_mutex_lock(section_fot_print);
	pthread_mutex_init(section_for_close, NULL);
	if (thread_number == 1) {
		pthread_mutex_init(section_for_threads, NULL);
	}
	pthread_create(thread, NULL, print_thread, (void*)this);
}


bool Thread::can_print() {
	if (pthread_mutex_trylock(section_for_threads) != 0) return false;
	pthread_mutex_unlock(section_for_threads);
	return true;
}

void Thread::print_end() {
	pthread_mutex_lock(section_fot_print);
	pthread_mutex_unlock(section_for_threads);
}

bool Thread::close() {
	if (pthread_mutex_trylock(section_for_close) != 0) return true;
	pthread_mutex_unlock(section_for_close);
	return false;
}

bool Thread::print_pause() {
	if (pthread_mutex_trylock(section_fot_print) != 0) {
		return true;
	}
	pthread_mutex_unlock(section_fot_print);
	return false;
}

void Thread::start_of_print() {
	pthread_mutex_lock(section_for_threads);
	pthread_mutex_unlock(section_fot_print);
}

void Thread::end_of_print() {
	pthread_mutex_lock(section_for_close);
}

char _getch() {
	char buf = 0;
	struct termios old = { 0 };
	fflush(stdout);
	if (tcgetattr(0, &old)<0)
		perror("tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &old)<0)
		perror("tcsetattr ICANON");
	if (read(0, &buf, 1)<0)
		perror("read()");
	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if (tcsetattr(0, TCSADRAIN, &old)<0)
		perror("tcsetattr ~ICANON");

	return buf;
}

int _kbhit(void) {
	struct timeval tv;
	fd_set rdfs;

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	FD_ZERO(&rdfs);
	FD_SET(STDIN_FILENO, &rdfs);

	select(STDIN_FILENO + 1, &rdfs, NULL, NULL, &tv);
	return FD_ISSET(STDIN_FILENO, &rdfs);
}


void *print_thread(void * name) {
	Thread *thread = (Thread*)name;
	while (true) {
		if (!(thread->print_pause())) {
			for (int i = 0; name_of_thread[(thread->current_thread) - 1][i] != 0; i++) {
				printf("%c", name_of_thread[(thread->current_thread) - 1][i]);
				fflush(stdout);
				Sleep(SLEEP_TIME_FOR_PRINT);
			}
			for (int i = 0; name_of_thread[AMOUNT_OF_THREADS][i] != 0; i++) {
				printf("%c", name_of_thread[AMOUNT_OF_THREADS][i]);
				fflush(stdout);
				Sleep(SLEEP_TIME_FOR_PRINT);
			}
			fflush(stdout);
			thread->print_end();
		}
		else {
			Sleep(SLEEP_TIME);
		}
		if (thread->close()) {
			break;
		}
	}
	return 0;

#endif