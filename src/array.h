#ifndef ARRAY_H
#define ARRAY_H
#include "first.h"

#include "buffer.h"

struct data_unset; /* declaration */

struct data_methods {
	struct data_unset *(*copy)(const struct data_unset *src); \
	void (*free)(struct data_unset *p); \
	int (*insert_dup)(struct data_unset *dst, struct data_unset *src); \
	void (*print)(const struct data_unset *p, int depth);
};

typedef enum { TYPE_STRING, TYPE_ARRAY, TYPE_INTEGER, TYPE_CONFIG, TYPE_OTHER } data_type_t;
#define DATA_UNSET \
	buffer key; \
	const struct data_methods *fn; /* function table */ \
	data_type_t type

typedef struct data_unset {
	DATA_UNSET;
} data_unset;

typedef struct {
	data_unset **data;
	data_unset **sorted;

	uint32_t used; /* <= INT32_MAX */
	uint32_t size;
} array;

typedef struct {
	DATA_UNSET;

	buffer value;
} data_string;

data_string *data_string_init(void);

typedef struct {
	DATA_UNSET;

	array value;
} data_array;

data_array *data_array_init(void);

typedef struct {
	DATA_UNSET;

	int value;
} data_integer;

data_integer *data_integer_init(void);

__attribute_returns_nonnull__
array *array_init(void);

__attribute_cold__
void array_copy_array(array *dst, const array *src);

__attribute_cold__
void array_free_data(array *a);

void array_free(array *a);

__attribute_hot__
void array_reset_data_strings(array *a);

__attribute_cold__
void array_insert_unique(array *a, data_unset *entry);

__attribute_cold__
data_unset *array_pop(array *a); /* only works on "simple" lists with autogenerated keys */

__attribute_cold__
__attribute_pure__
int array_is_vlist(const array *a);

__attribute_cold__
__attribute_pure__
int array_is_kvany(const array *a);

__attribute_cold__
__attribute_pure__
int array_is_kvarray(const array *a);

__attribute_cold__
__attribute_pure__
int array_is_kvstring(const array *a);

__attribute_pure__
data_unset *array_get_element_klen(const array *a, const char *key, size_t klen);

__attribute_cold__
__attribute_pure__
data_unset *array_get_data_unset(const array *a, const char *key, size_t klen);

__attribute_cold__
data_unset *array_extract_element_klen(array *a, const char *key, size_t klen); /* removes found entry from array */

__attribute_returns_nonnull__
int * array_get_int_ptr(array *a, const char *k, size_t klen);

__attribute_returns_nonnull__
buffer * array_get_buf_ptr(array *a, const char *k, size_t klen);

void array_insert_value(array *a, const char *v, size_t vlen);

static inline void array_set_key_value(array * const a, const char * const k, const size_t klen, const char * const v, const size_t vlen);

static inline void array_set_key_value(array * const a, const char * const k, const size_t klen, const char * const v, const size_t vlen) {
    buffer_copy_string_len(array_get_buf_ptr(a, k, klen), v, vlen);
}

__attribute_cold__
void array_replace(array *a, data_unset *entry);

__attribute_cold__
int array_print(const array *a, int depth);

__attribute_cold__
void array_print_indent(int depth);

__attribute_cold__
__attribute_pure__
size_t array_get_max_key_length(const array *a);

__attribute_pure__
data_unset * array_match_key_prefix_klen (const array * const a, const char * const s, const size_t slen);

__attribute_pure__
data_unset * array_match_key_prefix_nc_klen (const array * const a, const char * const s, const size_t slen);

__attribute_pure__
data_unset * array_match_key_prefix (const array * const a, const buffer * const b);

__attribute_pure__
data_unset * array_match_key_prefix_nc (const array * const a, const buffer * const b);

__attribute_pure__
const buffer * array_match_value_prefix (const array * const a, const buffer * const b);

__attribute_pure__
const buffer * array_match_value_prefix_nc (const array * const a, const buffer * const b);

__attribute_pure__
data_unset * array_match_key_suffix (const array * const a, const buffer * const b);

__attribute_pure__
data_unset * array_match_key_suffix_nc (const array * const a, const buffer * const b);

__attribute_pure__
const buffer * array_match_value_suffix (const array * const a, const buffer * const b);

__attribute_pure__
const buffer * array_match_value_suffix_nc (const array * const a, const buffer * const b);

__attribute_pure__
data_unset * array_match_path_or_ext (const array * const a, const buffer * const b);

#endif
