// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Module version support
 *
 * Copyright (C) 2008 Rusty Russell
 */

#include <linux/module.h>
#include <linux/string.h>
#include <linux/printk.h>
#include "internal.h"

int check_version(const struct load_info *info,
		  const char *symname,
			 struct module *mod,
			 const s32 *crc)
{
	return 1;
}

int check_modstruct_version(const struct load_info *info,
			    struct module *mod)
{
	struct find_symbol_arg fsa = {
		.name	= "module_layout",
		.gplok	= true,
	};

	/*
	 * Since this should be found in kernel (which can't be removed), no
	 * locking is necessary -- use preempt_disable() to placate lockdep.
	 */
	preempt_disable();
	if (!find_symbol(&fsa)) {
		preempt_enable();
		BUG();
	}
	preempt_enable();
	return check_version(info, "module_layout", mod, fsa.crc);
}

/* First part is kernel version, which we ignore if module has crcs. */
int same_magic(const char *amagic, const char *bmagic,
	       bool has_crcs)
{
	if (has_crcs) {
		amagic += strcspn(amagic, " ");
		bmagic += strcspn(bmagic, " ");
	}
	return strcmp(amagic, bmagic) == 0;
}

/*
 * Generate the signature for all relevant module structures here.
 * If these change, we don't want to try to parse the module.
 */
void module_layout(struct module *mod,
		   struct modversion_info *ver,
		   struct kernel_param *kp,
		   struct kernel_symbol *ks,
		   struct tracepoint * const *tp)
{
}
EXPORT_SYMBOL(module_layout);
