#ifndef _SYS_EXTRA_CXS_H_
#define	_SYS_EXTRA_CXS_H_


#ifdef CONFIG_EXTRA_CXS

struct extra_cxs_t {
	int do_extra_cxs;
	unsigned int init_extra_cxs;
	unsigned int step_extra_cxs;
};

inline int
 set_extra_cxs(struct extra_cxs_t*);
inline void extra_cxs(void);

#else /* !CONFIG_EXTRA_CXS */

#endif


#endif /* !_SYS_EXTRA_CXS_H_ */
