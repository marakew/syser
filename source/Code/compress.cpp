
#include "compress.hpp"
#include "define.h"

//https://lwn.net/Articles/235121/
//https://lwn.net/Articles/237112/
//https://lore.kernel.org/lkml/20181127161913.23863-7-dave.rodgman@arm.com/
//https://git.kontron-electronics.de/linux/uboot-exceet/blob/a874c8c65f57ecfbb8c7961525ac4376b6fbab8b/fs/jffs2/compr_lzo.c
//https://www.codingnow.com/windsoul/package/lzoc.htm
//compress TODO

long _do_compress(BYTE *in, unsigned in_len, BYTE *out, unsigned *out_len, long *wrkmem)
{
    register BYTE *ip;
    BYTE *op;
    BYTE *in_end = in + in_len;
    BYTE *ip_end = in + in_len - 13;
    BYTE *ii;
    BYTE **dict = (BYTE **)wrkmem;
    op = out;
    ip = in;
    ii = ip;
    ip += 4;

    for(;;)
    {
		register BYTE *m_pos;
		unsigned m_off;
		unsigned m_len;
		unsigned dindex;
		dindex = ((0x21*(((((((unsigned)(ip[3])<<6)^ip[2])<<5)^ip[1])<<5)^ip[0]))>>5) & 0x3fff;
		m_pos = dict [dindex];
		if(((ULONG_PTR)m_pos < (ULONG_PTR)in) ||
			(m_off = (unsigned)((ULONG_PTR)ip-(ULONG_PTR)m_pos) ) <= 0 ||
			m_off > 0xbfff)
			goto literal;
		if(m_off <= 0x0800 || m_pos[3] == ip[3])
			goto try_match;
		dindex = (dindex & 0x7ff ) ^ 0x201f;
		m_pos = dict[dindex];
		if((ULONG_PTR)(m_pos) < (ULONG_PTR)(in) ||
			(m_off = (unsigned)( (int)((ULONG_PTR)ip-(ULONG_PTR)m_pos))) <= 0 ||
			m_off > 0xbfff)
		    goto literal;
		if (m_off <= 0x0800 || m_pos[3] == ip[3])
			goto try_match;
		goto literal;
try_match:
		if(*(unsigned short*)m_pos == *(unsigned short*)ip && m_pos[2]==ip[2])
			goto match;
literal:
		dict[dindex] = ip;
		++ip;
		if (ip >= ip_end)
			break;
		continue;
match:
		dict[dindex] = ip;
		if(ip - ii > 0)
		{
			register unsigned t = ip - ii;
			
			if (t <= 3)
				op[-2] |= (BYTE)t;
			else if(t <= 18)
				*op++ = (BYTE)(t-3);
			else
			{
				register unsigned tt = t-18;
				*op++ = 0;
				while(tt > 255)
				{
					tt -= 255;
					*op++ = 0;
				}
				*op++ = (BYTE)tt;
			}
			do *op++ = *ii++; while (--t > 0);
		}
		ip += 3;
		if(m_pos[3] != *ip++ || m_pos[4] != *ip++ || m_pos[5] != *ip++ ||
			m_pos[6] != *ip++ || m_pos[7] != *ip++ || m_pos[8] != *ip++ )
		{
			--ip;
			m_len = ip - ii;
			
			if(m_off <= 0x0800 )
			{
				--m_off;
				*op++ = (BYTE)(((m_len - 1) << 5) | ((m_off & 7) << 2));
				*op++ = (BYTE)(m_off >> 3);
			}
			else
				if (m_off <= 0x4000 )
				{
					-- m_off;
					*op++ = (BYTE)(32 | (m_len - 2));
					goto m3_m4_offset;
				} else
				{
					m_off -= 0x4000;
					*op++ = (BYTE)(16 | ((m_off & 0x4000) >> 11) | (m_len - 2));
					goto m3_m4_offset;
				}
		} else
		{
			{
				BYTE *end = in_end;
				BYTE *m = m_pos + 9;
				while (ip < end && *m == *ip)
					m++, ip++;
				m_len = (ip - ii);
			}
			
			if(m_off <= 0x4000)
			{
				--m_off;
				if (m_len <= 33)
					*op++ = (BYTE)(32 | (m_len - 2));
				else
				{
					m_len -= 33;
					*op++=32;
					goto m3_m4_len;
				}
			} else
			{
				m_off -= 0x4000;
				if(m_len <= 9)
					*op++ = (BYTE)(16|((m_off & 0x4000) >> 11) | (m_len - 2));
				else
				{
					m_len -= 9;
					*op++ = (BYTE)(16 | ((m_off & 0x4000) >> 11));
m3_m4_len:
					while (m_len > 255)
					{
						m_len -= 255;
						*op++ = 0;
					}
					*op++ = (BYTE)m_len;
				}
			}
m3_m4_offset:
			*op++ = (BYTE)((m_off & 63) << 2);
			*op++ = (BYTE)(m_off >> 6);
		}
		ii = ip;
		if (ip >= ip_end)
			break;
    }
    *out_len = op - out;
    return (unsigned) (in_end - ii);
}

long lzo_compress(void *in, long in_len, void *out)
{
    BYTE *op = (BYTE*)out + 4; //reserv for size
    unsigned t, out_len;
    if (in_len <= 13)
		t = in_len;
    else
    {
		long wrkmem[16384];
		t = _do_compress((BYTE*)in, in_len, op, &out_len, wrkmem);
		op += out_len;
    }
    if (t > 0)
    {
		BYTE *ii = (BYTE*)in + in_len - t;
		if (op == (BYTE*)out+4 && t <= 238)
			*op++ = (BYTE) (17 + t);
		else
			if (t <= 3)
				op[-2] |= (BYTE)t;
			else
				if (t <= 18)
					*op++ = (BYTE)(t-3);
				else
				{
					unsigned tt = t - 18;
					*op++ = 0;
					while (tt > 255) 
					{
						tt -= 255;
						*op++ = 0;
					}
					*op++ = (BYTE)tt;
				}
				do *op++ = *ii++; while (--t > 0);
    }
    *op++ = 17;
    *op++ = 0;
    *op++ = 0;
    *(unsigned int*)out = in_len; //write size
    return (op - (BYTE*)out);
}

long lzo_decompress(void *in, long in_len, void *out)
{
    register BYTE *op;
    register BYTE *ip;
    register unsigned int t;
    register BYTE *m_pos;

    BYTE *ip_end = (BYTE *)in + in_len; //not used

    op = (BYTE *)out;
    ip = (BYTE *)in+4; //skip size

    if (*ip > 17)
    {
        t = *ip++ - 17;
        if (t < 4)
            goto match_next;
        do { *op++ = *ip++; } while(--t > 0);
        goto first_literal_run;
    }

    while(1)
    {
        t = *ip++;
        if (t >= 16) goto match;
        if (t == 0)
        {
            while(*ip == 0)
            {
                t += 255;
                ip++;
            }
            t += 15 + *ip++;
        }

        *(unsigned *)op = *(unsigned *)ip;
        op += 4; ip += 4;
        if (--t > 0)    
        {
            if (t >= 4)
            {
                do {
                    *(unsigned *)op = *(unsigned *)ip;
                    op += 4; ip += 4; t -= 4;
                } while(t >= 4);
                if (t > 0) do { *op++ = *ip++; } while(--t > 0);
            } else
                do { *op++ = *ip++; } while(--t > 0);
        }

first_literal_run:

        t = *ip++;
        if (t >= 16)
            goto match;

        m_pos = op - 0x0801;
        m_pos -= t >> 2;
        m_pos -= *ip++ << 2;

        *op++ = *m_pos++;
        *op++ = *m_pos++;
        *op++ = *m_pos;

        goto match_done;

        while (1)
        {
match:
            if (t >= 64)
            {
                m_pos = op - 1;
                m_pos -= (t >> 2) & 7;
                m_pos -= *ip++ << 3;
                t = (t >> 5) - 1;
    
                goto copy_match;
            } else
            if (t >= 32)
            {
                t &= 31;
                if (t == 0)
                {
                    while (*ip == 0)
                    {
                        t += 255;
                        ip++;
                    }
                    t += 31 + *ip++;
                }
#if defined(__LITTLE_ENDIAN)
                m_pos = op - 1;
                m_pos -= (*(unsigned short *)ip) >> 2;
#else
                m_pos = op - 1;
                m_pos -= (ip[0] >> 2) + (ip[1] << 6);
#endif
                ip += 2;
            } else
            if (t >= 16)
            {
                m_pos = op;
                m_pos -= (t & 8) << 11;
                t &= 7;
                if (t == 0) 
                {
                    while (*ip == 0) 
                    {
                        t += 255;
                        ip++;
                    }
                    t += 7 + *ip++;
                }
#if defined(__LITTLE_ENDIAN)
                m_pos -= (*(unsigned short *)ip) >> 2;
#else
		m_pos -= (ip[0] >> 2) + (ip[1] << 6);
#endif
                ip += 2;
                if (m_pos == op)
                    goto eof_found;
                m_pos -= 0x4000;
            } else 
            {
                m_pos = op - 1;
                m_pos -= t >> 2;
                m_pos -= *ip++ << 2;
                *op++ = *m_pos++; 
                *op++ = *m_pos;
                goto match_done;
            }

            if (t >= 6 && (op - m_pos) >= 4) 
            {
                *(unsigned *)op = *(unsigned *)m_pos;
                op += 4; m_pos += 4; t -= 2;
                do {
                    *(unsigned *)op = *(unsigned *)m_pos;
                    op += 4; m_pos += 4; t -= 4;
                } while(t >= 4);
                if (t > 0) do { *op++ = *m_pos++; } while(--t > 0);
            } else
            {
copy_match:
               *op++ = *m_pos++; *op++ = *m_pos++;
               do { *op++ = *m_pos++; } while(--t > 0);
            }

match_done:

            t = ip[-2] & 3;
            if (t == 0) break;

match_next:
            do { *op++ = *ip++; } while(--t > 0);
            t = *ip++;
        }
   }

eof_found:
   return (op - (BYTE*)out);
}


