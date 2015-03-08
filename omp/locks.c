#include <stdio.h>
#include <omp.h>
extern void do_something_else(int); extern void go_for_it(int);

int main() {
	omp_lock_t lck1, lck2; int id;

	omp_init_lock(&lck1);
	omp_init_lock(&lck2);

#pragma omp parallel shared(lcki, lck2) private(id)
  {
		id = omp_get_thread_num() ;

		omp_set_lock(&lck1);
		printf("thread %d has the lock \n", id);
		printf("thread %d ready to release the lock \n", id);
		omp_unset_lock(&lck1);

		while (! omp_test_lock(&lck2)) {
			do_something_else(id); // do something useful while waiting for the lock
		}
		go_for_it(id); // Thread has the lock

		omp_unset_lock(&lck2);
  }
  omp_destroy_lock(&lck1);
  omp_destroy_lock(&lck2);
}
		
