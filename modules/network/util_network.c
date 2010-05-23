/*
 * Copyright (C) 2001-2007 Chris Ross, Stephan Engstrom, Alex Holden et al
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * o Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * o Neither the name of the ferite software nor the names of its contributors may
 *   be used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "util_network.h"

int host_constructor(FeriteScript *script, FeriteObject *self, char *host, int type, int reverse)
{
#ifdef ENABLE_IPV6
    int i;
    struct sockaddr_in *sin4;
    struct sockaddr_in6 *sin6;
    char buf[INET6_ADDRSTRLEN];
    struct addrinfo hint, *ai, *aip;
    FeriteVariable *fv, *ar1, *ar2, *ar3;

    memset( &hint, '\0', sizeof(struct addrinfo) );
    switch(type) {
        case 0:
            hint.ai_family = PF_INET;
            break;
        case 1:
            hint.ai_family = PF_INET6;
            break;
        case 2:
        default:
            hint.ai_family = PF_UNSPEC;
            break;
    }

    if(reverse) 
        hint.ai_flags = AI_CANONNAME;
    else 
        hint.ai_flags = 0;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_protocol = 0;

    if((i = getaddrinfo(host, NULL, &hint, &ai))) {
        printf( "%s\n", gai_strerror(i));
        ferite_set_error(script, errno, "%s", gai_strerror(i));
        return -1;
    }

    fv = ferite_object_get_var(script, self, "host");
    VAS(fv) = ferite_str_new(script, host, 0, FE_CHARSET_DEFAULT);

    ar1 = ferite_object_get_var(script, self, "reversenames");
    ar2 = ferite_object_get_var(script, self, "types");
    ar3 = ferite_object_get_var(script, self, "addresses");

    for(aip = ai; aip; aip = aip->ai_next) {
        if(aip->ai_canonname) {
            fv = ferite_create_string_variable_from_ptr(script, "",
                                               aip->ai_canonname, 0,
                                               FE_CHARSET_DEFAULT, FE_STATIC);
            ferite_uarray_add(script, VAUA(ar1), fv, NULL, FE_ARRAY_ADD_AT_END);
        }
        switch(aip->ai_family) {
            case PF_INET:
                i = 0;
                break;
            case PF_INET6:
                i = 1;
                break;
            default:
                freeaddrinfo(ai);
                return -1; /* Shouldn't happen */
        }
        fv = ferite_create_number_long_variable(script, "", i, FE_STATIC);
        ferite_uarray_add(script, VAUA(ar2), fv, NULL, FE_ARRAY_ADD_AT_END);
        if(aip->ai_family == PF_INET) {
            sin4 = (struct sockaddr_in *)aip->ai_addr;
            inet_ntop(PF_INET, &sin4->sin_addr, buf, INET6_ADDRSTRLEN);
        } else {
            sin6 = (struct sockaddr_in6 *)aip->ai_addr;
            inet_ntop(PF_INET6, &sin6->sin6_addr, buf, INET6_ADDRSTRLEN);
        }
        fv = ferite_create_string_variable_from_ptr(script, "", buf, 0,
	                                 FE_CHARSET_DEFAULT, FE_STATIC);
        ferite_uarray_add(script, VAUA(ar3), fv, NULL, FE_ARRAY_ADD_AT_END);
    }

    freeaddrinfo(ai);

    return 0;

#else /* !ENABLE_IPV6 */
    char *p;
    int i, acnt;
    struct hostent *he;
    struct in_addr in, *inp;
    FeriteVariable *fv, *ar1, *ar2;

    if(type == 1) {
        ferite_set_error(script, EINVAL, "This operating system does not support IPV6");
        return -1;
    }

  /* We do this instead of simply calling gethostbyname() because
   * gethostbyname() does not do a DNS lookup if given a textual
   * IP address, but we want the full DNS information. */
    if(inet_aton(host, &in)) 
#ifndef WIN32
       he = gethostbyaddr(&in, sizeof(in), AF_INET);
#else
       he = gethostbyaddr(host, strlen(host), AF_INET);
#endif	
    else he = gethostbyname(host);
    if(!he) {
        switch(h_errno) {
            default:
            case HOST_NOT_FOUND:
                ferite_set_error(script, ENOENT, "Host not found");
                break;
            case NO_ADDRESS:
                ferite_set_error(script, ENOENT, "Requested host does not have an IP address");
                break;
            case NO_RECOVERY:
                ferite_set_error(script, ENOENT, "A non-recoverable name server error occured");
                break;
            case TRY_AGAIN:
                ferite_set_error(script, ENOENT, "A temporary name server error occured");
            break;
        }
        return -1;
    }

    if(he->h_addrtype != AF_INET) {
        ferite_set_error(script, ENOENT, "Got IP with unsupported address family %d", he->h_addrtype);
        return -1;
    }

    fv = ferite_object_get_var(script, self, "host");
    VAS(fv) = ferite_str_new(host, 0, FE_CHARSET_DEFAULT);

    ar1 = ferite_object_get_var(script, self, "types");
    ar2 = ferite_object_get_var(script, self, "addresses");
    for(acnt = 0; he->h_addr_list[acnt]; acnt++);
    for(i = 0; i < acnt; i++) {
        inp = (struct in_addr *)he->h_addr_list[i];
        p = inet_ntoa(*inp);
        fv = ferite_create_number_long_variable(script, "", 0, FE_STATIC);
        ferite_uarray_add(script, VAUA(ar1), fv, NULL, FE_ARRAY_ADD_AT_END);
        fv = ferite_create_string_variable_from_ptr(script, "", p, 0,
                                      FE_CHARSET_DEFAULT, FE_STATIC);
        ferite_uarray_add(script, VAUA(ar2), fv, NULL, FE_ARRAY_ADD_AT_END);
    }

    if(reverse) {
        ar1 = ferite_object_get_var(script, self, "reversenames");
        /* This is because the he structure we get from gethostbyaddr()
         * is statically allocated, and on some OSes may also be the
         * he structure that is returned by gethostbyname(), so before
         * doing lookups on the IP addresses, we need to copy them out
         * of the he structure to make sure they don't get trampled. */
        if(!(inp = fmalloc(sizeof(struct in_addr) * acnt))) return -1;
        for(i = 0; i < acnt; i++) {
            memcpy(&inp[i], (struct in_addr *)he->h_addr_list[i], sizeof(struct in_addr));
        }
        for(i = 0; i < acnt; i++) {
#ifndef WIN32
            he = gethostbyaddr(&inp[i], sizeof(struct in_addr), AF_INET);
#else
	    he = gethostbyaddr(inet_ntoa(inp[i]), strlen(inet_ntoa(inp[i])), AF_INET);
#endif
            if(he) {
                fv = ferite_create_string_variable_from_ptr(script, "",
                                he->h_name, 0, FE_CHARSET_DEFAULT, FE_STATIC);
            }
            else {
                fv = ferite_create_string_variable_from_ptr(script, "",
                         inet_ntoa(inp[i]), 0, FE_CHARSET_DEFAULT, FE_STATIC);
            }
            ferite_uarray_add(script, VAUA(ar1), fv, NULL, FE_ARRAY_ADD_AT_END);
        }
        ffree(inp);
    }

    return 0;
#endif /* ENABLE_IPV6 */
}

struct sockaddr *make_sockaddr(FeriteScript *script, char *host, short port, int *fam, int *size)
{
#ifdef ENABLE_IPV6
    int i;
    struct sockaddr *sa;
    struct addrinfo hint, *ai;

    memset( &hint, '\0', sizeof(struct addrinfo) );
    switch(*fam) {
        case 0:
            hint.ai_family = PF_INET;
            break;
        case 1:
            hint.ai_family = PF_INET6;
            break;
        case 2:
        default:
            hint.ai_family = PF_UNSPEC;
    }

    hint.ai_flags = 0;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_protocol = 0;

    if((i = getaddrinfo(host, NULL, &hint, &ai))) {
        ferite_set_error(script, errno, "%s", gai_strerror(i));
        return NULL;
    }

    if(ai->ai_family != PF_INET && ai->ai_family != PF_INET6) {
        ferite_set_error(script, EAFNOSUPPORT, "getaddrinfo() returned unsupported address family %d", ai->ai_family);
        freeaddrinfo(ai);
        return NULL;
    }

    *fam = ai->ai_family;
    *size = ai->ai_addrlen;

    if(!(sa = fmalloc(*size))) {
        freeaddrinfo(ai);
        return NULL;
    }
    memcpy(sa, ai->ai_addr, *size);

    freeaddrinfo(ai);

    if(*fam == PF_INET) ((struct sockaddr_in *)sa)->sin_port = htons(port);
    else ((struct sockaddr_in6 *)sa)->sin6_port = htons(port);

    return sa;

#else /* !ENABLE_IPV6 */
    struct hostent *he;
    struct sockaddr_in *si;

    switch(*fam) {
        case 0:
        case 2:
            *fam = AF_INET;
            break;
        case 1:
        default:
            ferite_set_error(script, 0, "Address family %d not supported", *fam);
        return NULL;
    }
    *size = sizeof(struct sockaddr_in);

    if(!(he = gethostbyname(host))) {
        switch(h_errno) {
            default:
            case HOST_NOT_FOUND:
                ferite_set_error(script, ENOENT, "Host not found");
                break;
            case NO_ADDRESS:
                ferite_set_error(script, ENOENT, "Requested host does not have an IP address");
                break;
            case NO_RECOVERY:
                ferite_set_error(script, ENOENT, "A non-recoverable name server error occured");
                break;
            case TRY_AGAIN:
                ferite_set_error(script, ENOENT, "A temporary name server error occured");
            break;
        }
        return NULL;
    }

    if(!(si = fmalloc(*size))) return NULL;

    si->sin_family = AF_INET;
    memcpy(&(si->sin_addr.s_addr), he->h_addr_list[0], sizeof(&(si->sin_addr.s_addr)));
    si->sin_port = htons(port);
 
    return (struct sockaddr *)si;
#endif
}

void set_remoteip(FeriteScript *script, FeriteObject *self, struct sockaddr *sa, int af)
{
    char pbuf[6];
    FeriteVariable *fv;
#ifdef ENABLE_IPV6
    struct sockaddr_in *sin;
    struct sockaddr_in6 *sin6;
    char ibuf[INET6_ADDRSTRLEN];

    if(af == 0) {
        af = PF_INET;
        sin = (struct sockaddr_in *)sa;
        inet_ntop(af, &sin->sin_addr, ibuf, INET6_ADDRSTRLEN);
        snprintf(pbuf, 6, "%u", ntohs(sin->sin_port));
    } else {
        af = PF_INET6;
        sin6 = (struct sockaddr_in6 *)sa;
        inet_ntop(af, &sin6->sin6_addr, ibuf, INET6_ADDRSTRLEN);
        snprintf(pbuf, 6, "%u", ntohs(sin6->sin6_port));
    }
    
    fv = ferite_object_get_var( script, self, "remoteip" );
    ferite_str_set( script, VAS(fv), ibuf, 0, FE_CHARSET_DEFAULT );
    
#else /* !ENABLE_IPV6 */
    struct sockaddr_in *sin = (struct sockaddr_in *)sa;
    
    fv = ferite_object_get_var( script, self, "remoteip" );
    ferite_str_set( VAS(fv), inet_ntoa(sin->sin_addr), 0, FE_CHARSET_DEFAULT );
    
    snprintf(pbuf, 6, "%u", ntohs(sin->sin_port));
#endif
    
    fv = ferite_object_get_var( script, self, "remoteport" );
    ferite_str_set( script, VAS(fv), pbuf, 0, FE_CHARSET_DEFAULT );
}

FeriteVariable *servent_to_Service(FeriteScript *script, struct servent *se)
{
    int i;
    FeriteClass *cls;
    FeriteVariable *fv, *ar, *sv;

    if(!(cls = ferite_find_class(script, script->mainns, "Network.Service")))
        FE_RETURN_NULL_OBJECT;

    if(!(sv = ferite_new_object(script, cls, NULL)))
        FE_RETURN_NULL_OBJECT;

    fv = ferite_object_get_var(script, VAO(sv), "name");
    VAS(fv) = ferite_str_new(script, se->s_name, 0, FE_CHARSET_DEFAULT);

    if(se->s_aliases[0]) {
        ar = ferite_object_get_var(script, VAO(sv), "aliases");
        for(i = 0; se->s_aliases[i]; i++) {
            fv = ferite_create_string_variable_from_ptr(script, "",
                           se->s_aliases[i], 0, FE_CHARSET_DEFAULT, FE_STATIC);
            ferite_uarray_add(script, VAUA(ar), fv, NULL, FE_ARRAY_ADD_AT_END);
        }
    }

    fv = ferite_object_get_var(script, VAO(sv), "port");
    VAI(fv) = ntohs(se->s_port);

    fv = ferite_object_get_var(script, VAO(sv), "proto");
    VAS(fv) = ferite_str_new(script, se->s_proto, 0, FE_CHARSET_DEFAULT);

    FE_RETURN_VAR(sv);
}
