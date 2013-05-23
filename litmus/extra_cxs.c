#include <litmus/extra_cxs.h>

#include <linux/percpu.h>
#include <linux/printk.h>


static DEFINE_PER_CPU(int, __do_extra_cxs);
#define _do_extra_cxs (__get_cpu_var(__do_extra_cxs))
#define do_extra_cxs_for(cpu_id) (per_cpu(__do_extra_cxs , cpu_id))

static DEFINE_PER_CPU(unsigned int, __extra_cxs_itr);
#define _extra_cxs_itr (__get_cpu_var(__extra_cxs_itr))
#define extra_cxs_itr_for(cpu_id) (per_cpu(__extra_cxs_itr, cpu_id))

static DEFINE_PER_CPU(unsigned int, __step_extra_cxs);
#define _step_extra_cxs (__get_cpu_var(__step_extra_cxs))
#define step_extra_cxs_for(cpu_id) (per_cpu(__step_extra_cxs, cpu_id))

inline void init_extra_cxs(void) {
	
	int cpu;
	for_each_online_cpu(cpu) {
		
		do_extra_cxs_for(cpu) = 0;
		extra_cxs_itr_for(cpu) = 0;
		step_extra_cxs_for(cpu) = 0;
	}
}

inline int set_extra_cxs(struct extra_cxs_t *_ecxs) {

	int ret = 0;
	unsigned long irq_flags;

	if (_ecxs->extra_cxs_cpu < 0)
		goto out;

	if (_ecxs->extra_cxs_cpu > NR_CPUS) {
		ret = 1;
		goto out;
	}

		
	local_irq_save(irq_flags);
		
	if (1 > _ecxs->do_extra_cxs) {
		
		do_extra_cxs_for(_ecxs->extra_cxs_cpu) = 0;
		extra_cxs_itr_for(_ecxs->extra_cxs_cpu) = 0;
		step_extra_cxs_for(_ecxs->extra_cxs_cpu) = 0;

	} else {
		
		do_extra_cxs_for(_ecxs->extra_cxs_cpu) = 1;
		extra_cxs_itr_for(_ecxs->extra_cxs_cpu) = _ecxs->init_extra_cxs;
		step_extra_cxs_for(_ecxs->extra_cxs_cpu) = _ecxs->step_extra_cxs;
	}
	local_irq_restore(irq_flags);


 out:
	return ret;
}

inline void extra_cxs(void) {

	unsigned long irq_flags;
	volatile unsigned int x;

	local_irq_save(irq_flags);
	if (_do_extra_cxs) {
	printk(KERN_EMERG "Starting extra_cxs\n");
		_extra_cxs_itr += _step_extra_cxs;
		x = 0;
		do {
			barrier();
			x++;
			barrier();
		} while (x<_extra_cxs_itr);
		_do_extra_cxs = 0;
		printk(KERN_EMERG "End of extra_cxs; took %u extra iterations\n",
		      x);
	}
	local_irq_restore(irq_flags);
}
