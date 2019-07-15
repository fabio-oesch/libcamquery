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

static union prov_elt node;
static label_t secret;
static int method;

static void init( void ){
  struct path path;
  method = 1; // Boolean Flag

  kern_path("/usr/bin/subl", LOOKUP_FOLLOW, &path); // hardcoded path to sublime-text. This could be anything
  __vfs_getxattr(path.dentry, path.dentry->d_inode, XATTR_NAME_PROVENANCE, &node, sizeof(union prov_elt));
  print("Module Loaded!");
  if (method) { // tried to recreate the way from camflow-cli
    print("using set tracked");
    set_tracked(&node);
  } else { // uses the given API from camquery.h
    print("using set API from camquery");
    secret = generate_label("secret");
    prov_bloom_add(prov_taint(&node), secret);
  }
  __vfs_setxattr(path.dentry, path.dentry->d_inode, XATTR_NAME_PROVENANCE, &node, sizeof(union prov_elt), 0);
}

static int prov_flow(prov_entry_t* from, prov_entry_t* edge, prov_entry_t* to){
  if (method) {
    /* If I try to print out all provenance I receive here
       there are a lot of them which I do not require. */
    if (provenance_is_tracked(from) || provenance_is_tracked(to)) {
      print("From (tracking) type: %s, %s", node_str(prov_type(from)), (from)->arg_info.value);
      print("To (tracking) type: %s, %s", node_str(prov_type(to)), (to)->arg_info.value);
    }
  } else {
    /* Whereas when I use prov_bloom_add it will give me a
       lot more information but only a few of them are set
       as tracked.  */
    print("From type: %s, %s", node_str(prov_type(from)), (from)->arg_info.value);
    print("To type: %s, %s", node_str(prov_type(to)), (to)->arg_info.value);
    if (provenance_is_tracked(from) || provenance_is_tracked(to)) {
      print("From (tracking) type: %s, %s", node_str(prov_type(from)), (from)->arg_info.value);
      print("To (tracking) type: %s, %s", node_str(prov_type(to)), (to)->arg_info.value);
    }
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
