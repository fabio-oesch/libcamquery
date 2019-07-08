/*
 *
 * Author: Thomas Pasquier <thomas.pasquier@bristol.ac.uk>
 * Author: Xueyuan Han <hanx@g.harvard.edu>
 *
 * Copyright (C) 2017-2018 Harvard University, University of Bristol
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 */
#include "include/camquery.h"

/* static uint64_t arr[50]; */
/* static int var; */
static union prov_elt node;

static void init( void ){
  print("Module Loaded!");
  struct path path;
  kern_path("/usr/bin/ls", LOOKUP_FOLLOW, &path);
  int rc = __vfs_getxattr(path.dentry, path.dentry->d_inode, XATTR_NAME_PROVENANCE, &node, sizeof(union prov_elt));
  set_tracked(&node);
  __vfs_setxattr(path.dentry, path.dentry->d_inode, XATTR_NAME_PROVENANCE, &node, sizeof(union prov_elt), 0);
}

static int prov_flow(prov_entry_t* from, prov_entry_t* edge, prov_entry_t* to){
  if (provenance_is_tracked(from) || provenance_is_tracked(to)) {
    print("From type: %s, %s", node_str(prov_type(from)), (from)->arg_info.value);
    print("To type: %s, %s", node_str(prov_type(to)), (to)->arg_info.value);
  }
  return 0;
}

struct provenance_query_hooks hooks = {
  .flow=&prov_flow,
  .free=NULL,
  .alloc=NULL,
};

QUERY_DESCRIPTION("An example query");
QUERY_LICSENSE("GPL");
QUERY_AUTHOR("John Doe");
QUERY_VERSION("1.0");
QUERY_NAME("My Example Query");

register_query(init, hooks);
