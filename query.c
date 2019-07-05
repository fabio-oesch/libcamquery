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

static void init( void ){
  print("Module Loaded!");
  struct path path;
  kern_path("/usr/bin/ls", LOOKUP_FOLLOW, &path);
  print("inode of ls: %ld", path.dentry->d_inode->i_ino);
}

static int prov_flow(prov_entry_t* from, prov_entry_t* edge, prov_entry_t* to){
  if (strcmp((from)->arg_info.value, "/usr/bin/vim") == 0) {
    set_propagate(from);
  }
  if (provenance_does_propagate(from)) {
    print("From type: %s, %s", node_str(prov_type(from)), (from)->arg_info.value);
    print("To type: %s, %s", node_str(prov_type(to)), (to)->arg_info.value);
  }
  /* if (strcmp((from)->arg_info.value, "/usr/bin/libreoffice") == 0) { */
  /* //if ((from)->task_info.pid == 3511) { */
  /*   //print("Pre From id: %llu", node_identifier(from).id); */
  /*   //print("Pre From type: %s", node_str(prov_type(from))); */
  /*   print("From id: %llu", node_identifier(from).id); */
  /*   print("From type: %s", node_str(prov_type(from))); */
  /*   print("Edge id: %llu", node_identifier(edge).id); */
  /*   print("Edge type: %d", prov_type_is_relation(prov_type(edge))); */
  /*   print("To id: %llu", node_identifier(to).id); */
  /*   print("To type: %s", node_str(prov_type(to))); */
  /*   //print("From:\t(%s),\t%s,\t%llu" , node_str(prov_type(from)), (from)->arg_info.value, arr[var - 1]); */
  /*   //print("Edge:\t-%s,%s->", relation_str(prov_type(edge)), (edge)->arg_info.value); */
  /*   //print("To:\t(%s),\t%s", node_str(prov_type(to)), (to)->arg_info.value); */
  /* } else if (in_array(node_identifier(from).id)) { */
  /*   arr[var++] = node_identifier(to).id; */
  /*   print("In list from\t%llu", node_identifier(from).id); */
  /*   print("In list to\t%llu", node_identifier(to).id); */
  /* } */
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
