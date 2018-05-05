#include <ruby.h>
#include <stdint.h>
#include "listpack.h"
#include "listpack_malloc.h"

static VALUE Listpack;

typedef struct {
  unsigned char *lp;
  unsigned char *p;
} listpack_rb;

// Internal methods

/* Loads a listpack format string into the internal struct */
VALUE _rb_listpack_load(VALUE self, VALUE rb_str)
{
  listpack_rb *list;
  Data_Get_Struct(self, listpack_rb, list);
  size_t str_len = RSTRING_LENINT(rb_str);

  if (list->lp != NULL) {
    lp_free(list->lp);
  }

  list->lp = lp_malloc(str_len);

  if (list->lp == NULL) {
    rb_raise(rb_eNoMemError, "no memory for listpack");
  }

  memcpy(list->lp, RSTRING_PTR(rb_str), str_len);

  return Qtrue;
}

/* Converts a value from the list to the correct Ruby type */
VALUE _rb_listpack_getvalue(unsigned char *p)
{
  if(!p) {
    return Qnil;
  }

  unsigned char * result;
  int64_t num;

  result = lpGet(p, &num, NULL);

  if(result) {
    return rb_str_new((char *) result, num);
  } else {
    return rb_int_new(num);
  }
}

// Public methods
VALUE rb_listpack_append(VALUE self, VALUE rb_ele)
{
  listpack_rb *list;
  Data_Get_Struct(self, listpack_rb, list);

  list->lp = lpAppend(list->lp, (unsigned char *) RSTRING_PTR(rb_ele), RSTRING_LENINT(rb_ele));

  if (list->lp == NULL) {
    rb_raise(rb_eNoMemError, "no memory for listpack");
  }

  return Qtrue;
}

VALUE rb_listpack_current(VALUE self)
{
  listpack_rb *list;
  Data_Get_Struct(self, listpack_rb, list);

  if (list->p == NULL) {
    return Qnil;
  }
  else {
    return _rb_listpack_getvalue(list->p);
  }
}

VALUE rb_listpack_first(VALUE self)
{
  listpack_rb *list;
  unsigned char *result;
  Data_Get_Struct(self, listpack_rb, list);

  result = lpFirst(list->lp);

  return _rb_listpack_getvalue(result);
}

VALUE rb_listpack_initialize(int argc, VALUE* argv, VALUE self)
{
  listpack_rb *list;
  VALUE data;
  Data_Get_Struct(self, listpack_rb, list);
  rb_scan_args(argc, argv, "01", &data);

  if (NIL_P(data)) {
    list->lp = lpNew();
  }
  else {
    _rb_listpack_load(self, data);
  }

  return Qnil;
}

VALUE rb_listpack_insert(VALUE self, VALUE rb_ele)
{
  listpack_rb *list;
  Data_Get_Struct(self, listpack_rb, list);

  list->lp = lpInsert(list->lp, (unsigned char *) RSTRING_PTR(rb_ele), RSTRING_LENINT(rb_ele), list->p, LP_AFTER, &list->p);

  if (list->lp == NULL) {
    rb_raise(rb_eNoMemError, "no memory for listpack");
  }

  return Qtrue;
}

VALUE rb_listpack_last(VALUE self)
{
  listpack_rb *list;
  unsigned char *result;
  Data_Get_Struct(self, listpack_rb, list);

  result = lpLast(list->lp);

  return _rb_listpack_getvalue(result);
}

VALUE rb_listpack_next(VALUE self)
{
  listpack_rb *list;
  Data_Get_Struct(self, listpack_rb, list);

  if (list->p == NULL) {
    list->p = lpFirst(list->lp);
  } else {
    list->p = lpNext(list->lp, list->p);
  }

  return _rb_listpack_getvalue(list->p);
}

VALUE rb_listpack_seek(VALUE self, VALUE rb_idx)
{
  listpack_rb *list;
  Data_Get_Struct(self, listpack_rb, list);

  list->p = lpSeek(list->lp, NUM2UINT(rb_idx));

  return Qnil;
}

VALUE rb_listpack_size(VALUE self)
{
  listpack_rb *list;
  Data_Get_Struct(self, listpack_rb, list);

  return rb_uint_new(lpLength(list->lp));
}

VALUE rb_listpack_to_s(VALUE self)
{
  listpack_rb *list;
  Data_Get_Struct(self, listpack_rb, list);

  return rb_str_new((char *) list->lp, lpBytes(list->lp));
}

static void deallocate(listpack_rb *p_list)
{
  lpFree(p_list->lp);
  xfree(p_list);
}

static VALUE allocate(VALUE klass)
{
  listpack_rb *list;

  if ((list = ALLOC(listpack_rb)) != NULL)
  {
    list->lp = NULL;
    list->p = NULL;
  }

  return Data_Wrap_Struct(klass, NULL, deallocate, list);
}

VALUE rb_class_listpack_dump(VALUE self, VALUE rb_ary)
{
  Check_Type(rb_ary, T_ARRAY);
  unsigned long len = (unsigned int) RARRAY_LEN(rb_ary);
  unsigned char *lp = lpNew();
  unsigned int i = 0;
  VALUE current;

  for (i = 0; i < len; i++) {
      current = rb_ary_entry(rb_ary, i);
      // TODO: convert integer to string if necessary
      Check_Type(current, T_STRING);
      lp = lpAppend(lp, (unsigned char *) RSTRING_PTR(current), RSTRING_LENINT(current));
      if (lp == NULL) {
        rb_raise(rb_eNoMemError, "no memory for listpack");
      }
  }

  return rb_str_new((char *) lp, lpBytes(lp));
}

VALUE rb_class_listpack_load(VALUE self, VALUE rb_str)
{
  Check_Type(rb_str, T_STRING);
  char *lp = RSTRING_PTR(rb_str);
  unsigned char *p = lpFirst((unsigned char *) lp);
  VALUE r_array = rb_ary_new();

  while(p) {
    rb_ary_push(r_array, _rb_listpack_getvalue(p));
    p = lpNext((unsigned char *) lp, p);
  }

  return r_array;
}

void Init_listpack_c(void) {
  Listpack = rb_define_class("Listpack", rb_cObject);
  rb_define_alloc_func(Listpack, allocate);

  rb_define_singleton_method(Listpack, "dump", rb_class_listpack_dump, 1);
  rb_define_singleton_method(Listpack, "load", rb_class_listpack_load, 1);

  rb_define_method(Listpack, "append", rb_listpack_append, 1);
  rb_define_method(Listpack, "current", rb_listpack_current, 0);
  rb_define_method(Listpack, "first", rb_listpack_first, 0);
  rb_define_method(Listpack, "initialize", rb_listpack_initialize, -1);
  rb_define_method(Listpack, "insert", rb_listpack_insert, 1);
  rb_define_method(Listpack, "last", rb_listpack_next, 0);
  rb_define_method(Listpack, "next", rb_listpack_next, 0);
  rb_define_method(Listpack, "prev", rb_listpack_first, 0);
  rb_define_method(Listpack, "seek", rb_listpack_seek, 1);
  rb_define_method(Listpack, "size", rb_listpack_size, 0);
  rb_define_method(Listpack, "to_s", rb_listpack_to_s, 0);
}