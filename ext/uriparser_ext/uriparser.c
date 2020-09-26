#include <stdio.h>
#include <ruby.h>
#include <uriparser/Uri.h>

#define FALSE 0
#define TRUE 1
typedef char bool;

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
      if(NIL_P(attribute)) {                                      \
        data->attribute = Qnil;                                   \
      } else {                                                    \
        data->attribute = StringValue(attribute);                 \
      }                                                           \
      data->updated = TRUE;                                       \
      rb_gc_mark(old);                                            \
                                                                  \
      return data->attribute;                                     \
    }

#define RB_URIPARSER_ATTR_ACCESSOR(attribute, original)           \
    RB_URIPARSER_ATTR_READER(attribute, original);                \
    RB_URIPARSER_ATTR_WRITER(attribute);

#define VALID_TYPE(v) (!(NIL_P(v) || RB_TYPE_P(v, T_UNDEF)))

/* Parser structure reused across requests */
static UriParserStateA uri_parse_state;
static VALUE rb_mUriParser;
static VALUE rb_cUri_Class;

struct uri_data {
  UriUriA *uri; /* Parsed URI data */
  bool updated; /* flag if any field was updated */
  VALUE scheme;
  VALUE userinfo;
  VALUE host;
  VALUE str_port;
  VALUE path;
  VALUE query;
  VALUE fragment;
};

/* Helper methods prototypes */
static void reset_fields(struct uri_data *);
static void populate_fields(VALUE);
static VALUE compose_uri_from_data(struct uri_data *);
static int parse_uri(const char *, UriUriA *);
static void free_uri(UriUriA *);
static UriUriA* update_uri(VALUE);

static void
rb_uriparser_mark(void *p)
{
  struct uri_data *ptr = p;

  if (ptr) {
    reset_fields(ptr);
  }
}

static void
rb_uriparser_free(void *p)
{
  struct uri_data *ptr = p;

  if(ptr) {
    free_uri(ptr->uri);
    xfree(ptr);
  }
}

static VALUE
rb_uriparser_s_allocate(VALUE klass)
{
  struct uri_data *data = ALLOC(struct uri_data);

  if(data) {
    data->uri       = NULL;
    data->updated   = FALSE;
    data->scheme    = Qundef;
    data->userinfo  = Qundef;
    data->host      = Qundef;
    data->str_port  = Qundef;
    data->path      = Qundef;
    data->query     = Qundef;
    data->fragment  = Qundef;
  } else {
    rb_raise(rb_eRuntimeError, "unable to create UriParser::URI class");
  }

  return Data_Wrap_Struct(klass, rb_uriparser_mark, rb_uriparser_free, data);
}

static VALUE
rb_uriparser_s_parse(VALUE klass, VALUE uri_obj)
{
  char *str_uri = StringValueCStr(uri_obj);
  UriUriA *uri = ALLOC(UriUriA);
  struct uri_data *data;
  VALUE generic_uri;

  generic_uri = rb_class_new_instance(0, NULL, rb_cUri_Class);
  Data_Get_Struct(generic_uri, struct uri_data, data);

  data->uri = uri;

  if(parse_uri(str_uri, uri) != URI_SUCCESS) {
    rb_raise(rb_eStandardError, "unable to parse the URI: %s", str_uri);
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
          rb_str_cat(data->path, path_segment->text.first,
            path_segment->text.afterLast - path_segment->text.first +
              (*path_segment->text.afterLast == '/')); /* check if there is a slash to add */
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

/* TODO: Include option mask */
static VALUE
rb_uriparser_normalize_bang(VALUE self)
{
  struct uri_data *data;

  Data_Get_Struct(self, struct uri_data, data);
  update_uri(self);

  if(uriNormalizeSyntaxA(data->uri) != URI_SUCCESS) {
    rb_raise(rb_eStandardError, "unable to normalize the URI");
  }
  /* Invalidate any previous field value */
  reset_fields(data);

  return self;
}

static VALUE
rb_uriparser_escape(VALUE self)
{
  return Qnil;
}

static VALUE
rb_uriparser_unescape(VALUE self)
{
  return Qnil;
}

static VALUE
rb_uriparser_to_s(VALUE self)
{
  int chars_required;
  char *str_uri;
  UriUriA *uri = update_uri(self);
  VALUE obj_str_uri;

  if(uriToStringCharsRequiredA(uri, &chars_required) != URI_SUCCESS ||
      !(str_uri = ALLOC_N(char, ++chars_required)) ||
      uriToStringA(str_uri, uri, chars_required, NULL) != URI_SUCCESS) {
    rb_raise(rb_eStandardError, "unable to convert to string");
  }

  obj_str_uri = rb_str_new2(str_uri);
  xfree(str_uri);
  return obj_str_uri;
}

static void
reset_fields(struct uri_data *data)
{
  data->updated = FALSE;

  rb_gc_mark(data->scheme);
  data->scheme = Qundef;

  rb_gc_mark(data->userinfo);
  data->userinfo = Qundef;

  rb_gc_mark(data->host);
  data->host = Qundef;

  rb_gc_mark(data->str_port);
  data->str_port = Qundef;

  rb_gc_mark(data->path);
  data->path = Qundef;

  rb_gc_mark(data->query);
  data->query = Qundef;

  rb_gc_mark(data->fragment);
  data->fragment = Qundef;
}

static void
populate_fields(VALUE uri)
{
  rb_uriparser_get_scheme(uri);
  rb_uriparser_get_userinfo(uri);
  rb_uriparser_get_host(uri);
  rb_uriparser_get_str_port(uri);
  rb_uriparser_get_path(uri);
  rb_uriparser_get_query(uri);
  rb_uriparser_get_fragment(uri);
}

static VALUE
compose_uri_from_data(struct uri_data *data)
{
  VALUE str_uri = rb_str_new2("");

  if(VALID_TYPE(data->scheme)) {
    rb_str_cat2(str_uri, StringValueCStr(data->scheme));
    rb_str_cat2(str_uri, "://");
  }

  if(VALID_TYPE(data->userinfo)) {
    rb_str_cat2(str_uri, StringValueCStr(data->userinfo));
    rb_str_cat2(str_uri, "@");
  }

  if(VALID_TYPE(data->host)) {
    rb_str_cat2(str_uri, StringValueCStr(data->host));
  }

  if(VALID_TYPE(data->str_port)) {
    rb_str_cat2(str_uri, ":");
    rb_str_cat2(str_uri, StringValueCStr(data->str_port));
  }

  if(VALID_TYPE(data->path)) {
    rb_str_cat2(str_uri, StringValueCStr(data->path));
  }

  if(VALID_TYPE(data->query)) {
    rb_str_cat2(str_uri, "?");
    rb_str_cat2(str_uri, StringValueCStr(data->query));
  }

  if(VALID_TYPE(data->fragment)) {
    rb_str_cat2(str_uri, "#");
    rb_str_cat2(str_uri, StringValueCStr(data->fragment));
  }

  return str_uri;
}

static int
parse_uri(const char *str_uri, UriUriA *uri)
{
  uri_parse_state.uri = uri;

  return uriParseUriA(&uri_parse_state, str_uri);
}

static void
free_uri(UriUriA *uri)
{
  if(uri) {
    uriFreeUriMembersA(uri);
    xfree(uri);
  }
}

static UriUriA *
update_uri(VALUE uri_obj)
{
  struct uri_data *data;

  Data_Get_Struct(uri_obj, struct uri_data, data);
  if(!data->uri || data->updated) {
    VALUE new_uri;
    UriUriA *uri = ALLOC(UriUriA);

    if(data->updated) {
      populate_fields(uri_obj);
    }
    /* Compute and parse the new URI */
    new_uri = compose_uri_from_data(data);

    if(parse_uri(StringValueCStr(new_uri), uri) != URI_SUCCESS) {
      free_uri(uri);
      rb_gc_mark(new_uri);
      rb_raise(rb_eStandardError, "invalid URI (%s) to normalize", StringValueCStr(new_uri));
    }

    free_uri(data->uri);
    rb_gc_mark(new_uri);
    data->uri = uri;
  }

  return data->uri;
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

  rb_define_method(rb_cUri_Class, "normalize!", rb_uriparser_normalize_bang, 0);
  rb_define_method(rb_cUri_Class, "escape", rb_uriparser_escape, 0);
  rb_define_method(rb_cUri_Class, "unescape", rb_uriparser_unescape, 0);
  rb_define_method(rb_cUri_Class, "to_s", rb_uriparser_to_s, 0);

  rb_define_singleton_method(rb_mUriParser, "parse", rb_uriparser_s_parse, 1);
}
