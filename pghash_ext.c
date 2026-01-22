#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "php_pghash.h"
#include "zend_exceptions.h"
#include <stdint.h>

/*
 * Funkcja zaimplementowana w pghash.c
 *
 * uint64_t pg_bucket_hash(int64_t tripId, uint64_t bucketNum);
 */
uint64_t pg_bucket_hash(int64_t tripId, uint64_t bucketNum);


/* ===== ARGINFO ===== */

ZEND_BEGIN_ARG_INFO_EX(arginfo_pg_bucket_hash, 0, 0, 2)
    ZEND_ARG_TYPE_INFO(0, tripId, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, bucketNum, IS_LONG, 0)
ZEND_END_ARG_INFO()


/* ===== PHP FUNCTION ===== */

PHP_FUNCTION(pg_bucket_hash)
{
    zend_long tripId;
    zend_long bucketNum;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_LONG(tripId)
        Z_PARAM_LONG(bucketNum)
    ZEND_PARSE_PARAMETERS_END();

    if (bucketNum <= 0) {
        zend_argument_value_error(2, "must be greater than 0");
        RETURN_THROWS();
    }

    uint64_t result = pg_bucket_hash(
        (int64_t) tripId,
        (uint64_t) bucketNum
    );

    /*
     * Wynik jest zawsze < bucketNum,
     * więc bezpiecznie mieści się w zend_long
     */
    RETURN_LONG((zend_long) result);
}


/* ===== FUNCTION TABLE ===== */

static const zend_function_entry pghash_functions[] = {
    PHP_FE(pg_bucket_hash, arginfo_pg_bucket_hash)
    PHP_FE_END
};


/* ===== MODULE ENTRY ===== */

zend_module_entry pghash_module_entry = {
    STANDARD_MODULE_HEADER,
    "pghash",
    pghash_functions,
    NULL, /* MINIT */
    NULL, /* MSHUTDOWN */
    NULL, /* RINIT */
    NULL, /* RSHUTDOWN */
    NULL, /* MINFO */
    PHP_PGHASH_VERSION,
    STANDARD_MODULE_PROPERTIES
};


#ifdef COMPILE_DL_PGHASH
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(pghash)
#endif

