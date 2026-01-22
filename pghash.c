#include <stdint.h>

/* ------------------ makra z PG ------------------ */
#define rot(x,k) (((x)<<(k)) | ((x)>>(32-(k))))

#define mix(a,b,c) \
{ \
  a -= c; a ^= rot(c, 4); c += b; \
  b -= a; b ^= rot(a, 6); a += c; \
  c -= b; c ^= rot(b, 8); b += a; \
  a -= c; a ^= rot(c,16); c += b; \
  b -= a; b ^= rot(a,19); a += c; \
  c -= b; c ^= rot(b, 4); b += a; \
}

#define final(a,b,c) \
{ \
  c ^= b; c -= rot(b,14); \
  a ^= c; a -= rot(c,11); \
  b ^= a; b -= rot(a,25); \
  c ^= b; c -= rot(b,16); \
  a ^= c; a -= rot(c, 4); \
  b ^= a; b -= rot(a,14); \
  c ^= b; c -= rot(b,24); \
}

/* ------------------ UInt64GetDatum ------------------ */
static inline uint64_t UInt64GetDatum(uint64_t X)
{
    return (uint64_t) X;
}

/* ------------------ hash_bytes_uint32_extended ------------------ */
uint64_t hash_bytes_uint32_extended(uint32_t k, uint64_t seed)
{
    uint32_t a, b, c;

    a = b = c = 0x9e3779b9 + (uint32_t)sizeof(uint32_t) + 3923095;

    if (seed != 0)
    {
        a += (uint32_t)(seed >> 32);
        b += (uint32_t)seed;
        mix(a, b, c);
    }

    a += k;

    final(a, b, c);

    return ((uint64_t)b << 32) | (uint64_t)c;
}

/* ------------------ hash_uint32_extended ------------------ */
static inline uint64_t hash_uint32_extended(uint32_t k, uint64_t seed)
{
    return (uint64_t) hash_bytes_uint32_extended(k, seed);
}

/* ------------------ hashint8extended ------------------ */
uint64_t hashint8extended(int64_t val, int64_t seed)
{
    uint32_t lohalf = (uint32_t) val;
    uint32_t hihalf = (uint32_t) (val >> 32);

    lohalf ^= (val >= 0) ? hihalf : ~hihalf;

    return hash_uint32_extended(lohalf, seed);
}

static inline uint64_t unsigned2signed(int64_t x)
{
    return (uint64_t)x;  // rzutowanie wystarczy, wrap automatyczny
}

/* ------------------ pg_bucket_hash ------------------ */
uint64_t pg_bucket_hash(int64_t tripId, uint64_t bucketNum)
{
    const uint64_t seed  = 8816678312871386365ULL;
    const uint64_t add   = 5305509591434766563ULL;

    // 1. hashint8extended (64-bit signed)
    int64_t h_signed = (int64_t) hashint8extended(tripId, seed);

    // 2. unsigned2signed -> konwersja signed -> unsigned
    uint64_t h_unsigned = unsigned2signed(h_signed);

    // 3. dodanie magicznej liczby i modulo 2^64 (wrapuje automatycznie)
    uint64_t added = h_unsigned + add;

    // 4. modulo bucketNum
    return added % bucketNum;
}

