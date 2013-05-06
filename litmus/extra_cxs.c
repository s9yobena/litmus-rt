#include <litmus/extra_cxs.h>
#include <linux/spinlock.h>

DEFINE_SPINLOCK(cxs_itr_lock);

static int do_extra_cxs;
static unsigned int extra_cxs_itr;
static unsigned int step_extra_cxs;

inline int set_extra_cxs(struct extra_cxs_t *_ecxs) {

	unsigned long cxs_lock_flags;

	spin_lock_irqsave(&cxs_itr_lock, cxs_lock_flags);
	
	if (0 == _ecxs->do_extra_cxs) {
		
		do_extra_cxs = 0;
		extra_cxs_itr = 0;
		step_extra_cxs = 0;
	} else {
		
		do_extra_cxs = 1;
		extra_cxs_itr = _ecxs->init_extra_cxs;
		step_extra_cxs = _ecxs->step_extra_cxs;
	}
	
	spin_unlock_irqrestore(&cxs_itr_lock, cxs_lock_flags);
	return 0;
}

inline void extra_cxs(void) {

	unsigned long cxs_lock_flags;
	volatile unsigned int x;

	spin_lock_irqsave(&cxs_itr_lock, cxs_lock_flags);
	if (do_extra_cxs) {
		extra_cxs_itr += step_extra_cxs;
		x = 0;
		do {
			barrier();
			x++;
			barrier();
		} while (x<extra_cxs_itr);
	}
	spin_unlock_irqrestore(&cxs_itr_lock, cxs_lock_flags);
}
