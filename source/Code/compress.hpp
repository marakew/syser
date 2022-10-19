
#ifndef _COMPRESS_HPP_
#define _COMPRESS_HPP_

long lzo_compress(void *in, long in_len, void *out);
long lzo_decompress(void *in, long in_len, void *out);

#endif
