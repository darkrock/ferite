/*
 * Copyright (C) 2000-2003 Chris Ross and various contributors
 * Copyright (C) 1999-2000 Chris Ross
 * Copyright (C) 2004 Christian M. Stamgren
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies of the Software, its documentation and marketing & publicity
 * materials, and acknowledgment shall be given in the documentation, materials
 * and software packages that this Software was used.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <ferite.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include "xml_header.h"
#include "sax_handlers.h"

#define DELIVER_STRING_CALLBACK( NAME, STRING, LENGTH ) \
    do { \
        SaxRecord             *sr = ctxt; \
        FeriteObject          *obj = sr->obj; \
        FeriteScript          *script = sr->script; \
        FeriteFunction        *function = NULL; \
        FeriteVariable        **params = NULL; \
        FeriteString          *buf; \
        if( (function = ferite_object_get_function( script, obj, NAME )) != NULL ) { \
            buf = ferite_str_new(script,(char *)STRING, LENGTH, FE_CHARSET_DEFAULT); \
            params = ferite_create_parameter_list_from_data( script, "s", buf ); \
            ferite_variable_destroy( script, ferite_call_function( sr->script, obj, NULL, function, params ) ); \
            ferite_delete_parameter_list( script, params ); \
            ferite_str_destroy( script,buf ); \
        } \
    } while(0)
#define DELIVER_2STRING_CALLBACK( NAME, STRING, LENGTH, STRING1, LENGTH1 ) \
    do { \
        SaxRecord             *sr = ctxt; \
        FeriteObject          *obj = sr->obj; \
        FeriteScript          *script = sr->script; \
        FeriteFunction        *function = NULL; \
        FeriteVariable        **params = NULL; \
        FeriteString          *buf, *buf1; \
        if( (function = ferite_object_get_function( script, obj, NAME )) != NULL ) { \
            buf = ferite_str_new(script,(char *)STRING, LENGTH, FE_CHARSET_DEFAULT); \
            buf1 = ferite_str_new(script,(char *)STRING1, LENGTH1, FE_CHARSET_DEFAULT); \
            params = ferite_create_parameter_list_from_data( script, "ss", buf, buf1 ); \
            ferite_variable_destroy( script, ferite_call_function( sr->script, obj, NULL, function, params ) ); \
            ferite_delete_parameter_list( script, params ); \
            ferite_str_destroy( script,buf ); \
            ferite_str_destroy( script,buf1 ); \
        } \
    } while(0)
#define DELIVER_3STRING_CALLBACK( NAME, STRING, LENGTH, STRING1, LENGTH1, STRING2, LENGTH2 ) \
    do { \
        SaxRecord             *sr = ctxt; \
        FeriteObject          *obj = sr->obj; \
        FeriteScript          *script = sr->script; \
        FeriteFunction        *function = NULL; \
        FeriteVariable        **params = NULL; \
        FeriteString          *buf, *buf1, *buf2; \
        if( (function = ferite_object_get_function( script, obj, NAME )) != NULL ) { \
            buf = ferite_str_new(script,(char *)STRING, LENGTH, FE_CHARSET_DEFAULT); \
            buf1 = ferite_str_new(script,(char *)STRING1, LENGTH1, FE_CHARSET_DEFAULT); \
            buf2 = ferite_str_new(script,(char *)STRING2, LENGTH2, FE_CHARSET_DEFAULT); \
            params = ferite_create_parameter_list_from_data( script, "sss", buf, buf1, buf2 ); \
            ferite_variable_destroy( script, ferite_call_function( sr->script, obj, NULL, function, params ) ); \
            ferite_delete_parameter_list( script, params ); \
            ferite_str_destroy( script,buf ); \
            ferite_str_destroy( script,buf1 ); \
            ferite_str_destroy( script,buf2 ); \
        } \
    } while(0) 

xmlParserInputPtr sax_resolveEntity(void *ctxt, const xmlChar *publicId, const xmlChar *systemId)
{
    printf( "In Function %s - %s\n", __FUNCTION__, ((SaxRecord*)ctxt)->obj->name );
    return NULL;
}

void sax_internalSubset(void *ctxt, const xmlChar *name, const xmlChar *ExternalID, const xmlChar *SystemID)
{
    DELIVER_3STRING_CALLBACK("internalSubset", name, 0, ExternalID, 0, SystemID, 0);
}

void sax_externalSubset (void *ctxt, const xmlChar *name, const xmlChar *ExternalID, const xmlChar *SystemID)
{
    DELIVER_3STRING_CALLBACK("externalSubset", name, 0, ExternalID, 0, SystemID, 0);
}

xmlEntityPtr sax_getEntity (void *ctxt, const xmlChar *name)
{
    xmlEntityPtr en = xmlGetPredefinedEntity(name);
    return en;
}

xmlEntityPtr sax_getParameterEntity (void *ctxt, const xmlChar *name)
{
    printf( "In Function %s - %s\n", __FUNCTION__, ((SaxRecord*)ctxt)->obj->name );
    return NULL;
}

void sax_entityDecl (void *ctxt, const xmlChar *name, int type, const xmlChar *publicId, const xmlChar *systemId, xmlChar *content)
{
    printf( "In Function %s - %s\n", __FUNCTION__, ((SaxRecord*)ctxt)->obj->name );
}

void sax_notationDecl(void *ctxt, const xmlChar *name, const xmlChar *publicId, const xmlChar *systemId)
{
    printf( "In Function %s - %s\n", __FUNCTION__, ((SaxRecord*)ctxt)->obj->name );
}

void sax_attributeDecl(void *ctxt, const xmlChar *elem, const xmlChar *name, int type, int def, const xmlChar *defaultValue, xmlEnumerationPtr tree)
{
    printf( "In Function %s - %s\n", __FUNCTION__, ((SaxRecord*)ctxt)->obj->name );
}

void sax_elementDecl(void *ctxt, const xmlChar *name, int type, xmlElementContentPtr content)
{
    printf( "In Function %s - %s\n", __FUNCTION__, ((SaxRecord*)ctxt)->obj->name );
}

void sax_unparsedEntityDecl(void *ctxt, const xmlChar *name, const xmlChar *publicId, const xmlChar *systemId, const xmlChar *notationName)
{
    printf( "In Function %s - %s\n", __FUNCTION__, ((SaxRecord*)ctxt)->obj->name );
}

void sax_setDocumentLocator (void *ctxt, xmlSAXLocatorPtr loc)
{
/*   printf( "In Function %s - %s\n", __FUNCTION__, ((SaxRecord*)ctxt)->obj->name );*/
}

void sax_startDocument( void *ctxt )
{
    SaxRecord             *sr = ctxt;
    FeriteObject          *obj = sr->obj;
    FeriteScript          *script = sr->script;
    FeriteFunction        *function = NULL;

    if( (function = ferite_object_get_function( script, obj, "startDocument" )) != NULL )
        ferite_variable_destroy( script, ferite_call_function( sr->script, obj, NULL, function, NULL ) );
}

void sax_endDocument( void *ctxt )
{
    SaxRecord             *sr = ctxt;
    FeriteObject          *obj = sr->obj;
    FeriteScript          *script = sr->script;
    FeriteFunction        *function = NULL;

    if( (function = ferite_object_get_function( script, obj, "endDocument" )) != NULL )
        ferite_variable_destroy( script, ferite_call_function( sr->script, obj, NULL, function, NULL ) );
}

void sax_startElement (void *ctxt, const xmlChar *name, const xmlChar **atts)
{
    FeriteVariable        *array;
    int i = 0;
    SaxRecord             *sr = ctxt;
    FeriteObject          *obj = sr->obj;
    FeriteScript          *script = sr->script;
    FeriteFunction        *function = NULL;
    FeriteVariable        **params = NULL, *var = NULL;
    FeriteString          *tname;

    if( (function = ferite_object_get_function( script, obj, "startElement" )) != NULL )
    {
        tname = ferite_str_new(script, (char *)name, 0, FE_CHARSET_DEFAULT);
        array = ferite_create_uarray_variable( script, "retval-string_split", 10, FE_STATIC );
        if( atts != NULL )
        {
            for( i = 0; atts[i] != NULL; i++ )
            {
                var = fe_new_str( (char *)atts[i], (atts[i++] == NULL ? "" : (char *)atts[i]), 0, FE_CHARSET_DEFAULT );
                ferite_uarray_add( script, VAUA( array ), var, (char *)atts[i-1], FE_ARRAY_ADD_AT_END );
            }
        }
        params = ferite_create_parameter_list_from_data( script, "sa", tname, VAUA(array) );
        ferite_variable_destroy( script, ferite_call_function( sr->script, obj, NULL, function, params ) );
        ferite_delete_parameter_list( script, params );
        ferite_variable_destroy( script, array );
        ferite_str_destroy( script, tname );
    }
}

void sax_endElement (void *ctxt, const xmlChar *name)
{
    DELIVER_STRING_CALLBACK("endElement", name, 0);
}

void sax_attribute (void *ctxt, const xmlChar *name, const xmlChar *value)
{
    DELIVER_2STRING_CALLBACK( "attribute", name, 0, value, 0 );
}

void sax_reference (void *ctxt, const xmlChar *name)
{
    DELIVER_STRING_CALLBACK( "reference", name, 0 );
}

void sax_characters (void *ctxt, const xmlChar *ch, int len)
{
    DELIVER_STRING_CALLBACK( "characters", ch, len );
}

void sax_ignorableWhitespace (void *ctxt, const xmlChar *ch, int len)
{
    DELIVER_STRING_CALLBACK( "ignorableWhitespace", ch, len );
}

void sax_processingInstruction (void *ctxt, const xmlChar *target, const xmlChar *data)
{
    DELIVER_2STRING_CALLBACK( "processingInstruction", target, 0, data, 0 );
}

void sax_comment (void *ctxt, const xmlChar *value)
{
    DELIVER_STRING_CALLBACK( "command", value, strlen((char*)value) );
}

void sax_cdataBlock (void *ctxt, const xmlChar *value, int len)
{
    DELIVER_STRING_CALLBACK( "cdata", value, len );
}

void sax_warning (void *ctxt, const char *msg, ...)
{
    SaxRecord             *sr = ctxt;
    va_list ap;

    va_start( ap, msg );
    ferite_vwarning( sr->script, (char *)msg, &ap );
    va_end( ap );
}

void sax_error (void *ctxt, const char *msg, ...)
{
    SaxRecord             *sr = ctxt;
    va_list ap;

    va_start( ap, msg );
    ferite_verror( sr->script, 0, (char *)msg, &ap );
    va_end( ap );
}

void sax_fatalError (void *ctxt, const char *msg, ...)
{
    SaxRecord             *sr = ctxt;
    va_list ap;

    va_start( ap, msg );
    ferite_verror( sr->script, (char *)msg, &ap );
    va_end( ap );
}

int sax_isStandalone (void *ctxt)
{
    printf( "In Function %s - %s\n", __FUNCTION__, ((SaxRecord*)ctxt)->obj->name );
	return FE_FALSE;
}

int sax_hasInternalSubset (void *ctxt)
{
    printf( "In Function %s - %s\n", __FUNCTION__, ((SaxRecord*)ctxt)->obj->name );
	return FE_FALSE;
}

int sax_hasExternalSubset (void *ctxt)
{
    printf( "In Function %s - %s\n", __FUNCTION__, ((SaxRecord*)ctxt)->obj->name );
	return FE_FALSE;
}

int sax_xmlParseFile( xmlSAXHandlerPtr sax, SaxRecord *obj, char *filename )
{
    xmlParserCtxtPtr ctxt = NULL;

    ctxt = xmlCreateFileParserCtxt( filename );
    if( ctxt == NULL )
    {
        ferite_error( obj->script, 0, "Unable to find file %s\n", filename );
        return 0;
    }
    ctxt->sax = sax;
    ctxt->userData = obj;

    return sax_xmlExecuteCtxt( ctxt );
}

int sax_xmlParseChunk( xmlSAXHandlerPtr sax, SaxRecord *obj, FeriteString *chunk )
{
    xmlParserCtxtPtr ctxt = NULL;

    ctxt = xmlCreateMemoryParserCtxt( chunk->data, chunk->length );
    if( ctxt == NULL )
    {
        ferite_error( obj->script, 0, "Unable to parse chunk: %s\n", chunk );
        return 0;
    }
    ctxt->sax = sax;
    ctxt->userData = obj;

    return sax_xmlExecuteCtxt( ctxt );
}

static int sax_xmlExecuteCtxt( xmlParserCtxtPtr ctxt )
{
    int ret = 0;
    xmlParseDocument( ctxt );

    if( ctxt->wellFormed )
      ret = 1;
    else
      ret = 0;

    if( ctxt->sax != NULL )
      ctxt->sax = NULL;

    xmlFreeParserCtxt( ctxt );

    return ret;
}
