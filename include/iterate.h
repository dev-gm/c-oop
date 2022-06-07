#ifndef _ITERATE_H

#define _ITERATE_H

#define foreach(var, iterator, type) \
	for (type *var = (iterator).begin(); \
		var <= iterator.end() && var != NULL; \
		++(var))

#define foreach_reversed(var, iterator, type) \
	for (type *var = (iterator).end(); \
		var >= iterator.begin() && var != NULL; \
		--(var))

#endif
