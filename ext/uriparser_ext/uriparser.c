#include <stdio.h>
#include <ruby.h>
#include <uriparser/Uri.h>

#define URI_TEXT_RANGE(uri_ptr, uri_field) ((uri_ptr->uri_field.afterLast == uri_ptr->uri_field.first) ? Qnil :  \
    rb_str_new(uri_ptr->uri_field.first, uri_ptr->uri_field.afterLast - uri_ptr->uri_field.first))

#define RB_URIPARSER_ATTR_READER(attribute, original)             \
    static VALUE                                                  \
    rb_uriparser_get_##attribute(VALUE self)                      \
    {                                                             \
      struct uri_data *data;                                      \
                                                                  \
      /* lazy load */                                             \
      Data_Get_Struct(self, struct uri_data, data);               \
      if(RB_TYPE_P(data->attribute, T_UNDEF)) {                   \
        if(data->uri) {                                           \
          data->attribute = URI_TEXT_RANGE(data->uri, original);  \
        } else {                                                  \
          data->attribute = Qnil;                                 \
        }                                                         \
      }                                                           \
                                                                  \
      return data->attribute;                                     \
    }

#define RB_URIPARSER_ATTR_WRITER(attribute)                       \
    static VALUE                                                  \
    rb_uriparser_set_##attribute(VALUE self, VALUE attribute)     \
    {                                                             \
      VALUE old;                                                  \
      struct uri_data *data;                                      \
                                                                  \
      Data_Get_Struct(self, struct uri_data, data);               \
      old = data->attribute;                                      \
      data->attribute = StringValue(attribute);                   \
      rb_gc_mark(old);                                            \
      return data->attribute;                                     \
    } 

#define RB_URIPARSER_ATTR_ACCESSOR(attribute, original)           \
    RB_URIPARSER_ATTR_READER(attribute, original);                \
    RB_URIPARSER_ATTR_WRITER(attribute);

/* Parser structure reused across requests */
static UriParserStateA uri_parse_state; 
static VALUE rb_mUriParser;
static VALUE rb_cUri_Class;

struct uri_data {
  UriUriA *uri; /* Parsed URI data */
  VALUE scheme;
  VALUE userinfo;
  VALUE password;
  VALUE host;
  VALUE str_port;
  VALUE path;
  VALUE query;
  VALUE opaque;
  VALUE registry;
  VALUE fragment;
};

static void
rb_uriparser_mark(void *p)
{
  struct uri_data *ptr = p;
  
  if (ptr) {
    rb_gc_mark(ptr->scheme);
    rb_gc_mark(ptr->userinfo);
    rb_gc_mark(ptr->password);
    rb_gc_mark(ptr->host);
    rb_gc_mark(ptr->str_port);
    rb_gc_mark(ptr->path);
    rb_gc_mark(ptr->query);
    rb_gc_mark(ptr->opaque);
    rb_gc_mark(ptr->registry);
    rb_gc_mark(ptr->fragment);
  }
}

static void 
rb_uriparser_free(void *p)
{
  struct uri_data *ptr = p;
  
  if(ptr) {
    uriFreeUriMembersA(ptr->uri);
    xfree(ptr->uri);
    xfree(ptr);
  }
}

static VALUE
rb_uriparser_s_allocate(VALUE klass)
{
  struct uri_data *data = ALLOC(struct uri_data);

  if(data) {
    data->uri       = NULL;
    data->scheme    = Qundef;
    data->userinfo  = Qundef;
    data->password  = Qundef;
    data->host      = Qundef;
    data->str_port  = Qundef;
    data->path      = Qundef;
    data->query     = Qundef;
    data->opaque    = Qundef;
    data->registry  = Qundef;
    data->fragment  = Qundef;
  } else {
    rb_raise(rb_eRuntimeError, "unable to create UriParser::Generic class");
  }

  return Data_Wrap_Struct(klass, rb_uriparser_mark, rb_uriparser_free, data);
}

static VALUE
rb_uriparser_s_parse(VALUE klass, VALUE uri_obj)
{
  char *uri_str = StringValueCStr(uri_obj);
  UriUriA *uri = ALLOC(UriUriA);
  struct uri_data *data;
  VALUE generic_uri;

  generic_uri = rb_class_new_instance(0, NULL, rb_cUri_Class);
  Data_Get_Struct(generic_uri, struct uri_data, data);

  data->uri = uri;
  uri_parse_state.uri = uri;
  if(uriParseUriA(&uri_parse_state, uri_str) != URI_SUCCESS) {
    rb_raise(rb_eStandardError, "unable to parse the URI");
  }

  return generic_uri;
}

static VALUE
rb_uriparser_initialize(VALUE self) 
{
  return self;
}

RB_URIPARSER_ATTR_ACCESSOR(scheme, scheme);
RB_URIPARSER_ATTR_ACCESSOR(userinfo, userInfo);
RB_URIPARSER_ATTR_ACCESSOR(host, hostText);
RB_URIPARSER_ATTR_ACCESSOR(str_port, portText);
RB_URIPARSER_ATTR_ACCESSOR(query, query);
RB_URIPARSER_ATTR_ACCESSOR(fragment, fragment);
RB_URIPARSER_ATTR_WRITER(path);

static VALUE
rb_uriparser_get_path(VALUE self)
{
  struct uri_data *data;

  /* lazy load */
  Data_Get_Struct(self, struct uri_data, data);
  if(RB_TYPE_P(data->path, T_UNDEF)) {
    if(data->uri) {
      if(data->uri->pathHead) {
        /* starts with slash */
        UriPathSegmentA *path_segment = data->uri->pathHead;
        data->path = rb_str_new("/", 1);
        do { /* go through the linked list */ 
          /* check if there is a slash to add */
          if(*path_segment->text.afterLast == '/') {
            rb_str_cat(data->path, path_segment->text.first, 
              path_segment->text.afterLast - path_segment->text.first + 1);
          } else {
            rb_str_cat(data->path, path_segment->text.first, 
              path_segment->text.afterLast - path_segment->text.first);
          }
          path_segment = path_segment->next;
        } while(path_segment);
      } else {
        data->path = rb_str_new("", 0);
      }
    } else {
      data->path = Qnil;
    }
  }

  return data->path;
}

void
Init_uriparser_ext()
{
  rb_mUriParser = rb_define_module("UriParser");
  rb_cUri_Class = rb_define_class_under(rb_mUriParser, "URI", rb_cObject);
  
  rb_define_alloc_func(rb_cUri_Class, rb_uriparser_s_allocate);
  rb_define_method(rb_cUri_Class, "initialize", rb_uriparser_initialize, 0);
  rb_define_method(rb_cUri_Class, "scheme", rb_uriparser_get_scheme, 0);
  rb_define_method(rb_cUri_Class, "scheme=", rb_uriparser_set_scheme, 1);
  rb_define_method(rb_cUri_Class, "userinfo", rb_uriparser_get_userinfo, 0);
  rb_define_method(rb_cUri_Class, "userinfo=", rb_uriparser_set_userinfo, 1);
  rb_define_method(rb_cUri_Class, "host", rb_uriparser_get_host, 0);
  rb_define_method(rb_cUri_Class, "host=", rb_uriparser_set_host, 1);
  rb_define_method(rb_cUri_Class, "str_port", rb_uriparser_get_str_port, 0);
  rb_define_method(rb_cUri_Class, "str_port=", rb_uriparser_set_str_port, 1);
  rb_define_method(rb_cUri_Class, "path", rb_uriparser_get_path, 0);
  rb_define_method(rb_cUri_Class, "path=", rb_uriparser_set_path, 1);
  rb_define_method(rb_cUri_Class, "query", rb_uriparser_get_query, 0);
  rb_define_method(rb_cUri_Class, "query=", rb_uriparser_set_query, 1);
  rb_define_method(rb_cUri_Class, "fragment", rb_uriparser_get_fragment, 0);
  rb_define_method(rb_cUri_Class, "fragment=", rb_uriparser_set_fragment, 1);

  /* TODO: include opaque and registry methods */

  rb_define_singleton_method(rb_mUriParser, "parse", rb_uriparser_s_parse, 1);
}