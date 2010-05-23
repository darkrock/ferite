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

#ifndef __SAX_HANDLERS_H__
#define __SAX_HANDLERS_H__

#include <ferite.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>

typedef struct __sax_record {
   FeriteScript *script;
   FeriteObject *obj;
   int           firstParse;
} SaxRecord;


xmlParserInputPtr sax_resolveEntity(void *ctx, const xmlChar *publicId, const xmlChar *systemId);
void sax_internalSubset(void *ctx, const xmlChar *name, const xmlChar *ExternalID, const xmlChar *SystemID);
void sax_externalSubset (void *ctx, const xmlChar *name, const xmlChar *ExternalID, const xmlChar *SystemID);
xmlEntityPtr sax_getEntity (void *ctx, const xmlChar *name);
xmlEntityPtr sax_getParameterEntity (void *ctx, const xmlChar *name);
void sax_entityDecl (void *ctx, const xmlChar *name, int type, const xmlChar *publicId, const xmlChar *systemId, xmlChar *content);
void sax_notationDecl(void *ctx, const xmlChar *name, const xmlChar *publicId, const xmlChar *systemId);
void sax_attributeDecl(void *ctx, const xmlChar *elem, const xmlChar *name, int type, int def, const xmlChar *defaultValue, xmlEnumerationPtr tree);
void sax_elementDecl(void *ctx, const xmlChar *name, int type, xmlElementContentPtr content);
void sax_unparsedEntityDecl(void *ctx, const xmlChar *name, const xmlChar *publicId, const xmlChar *systemId, const xmlChar *notationName);
void sax_setDocumentLocator (void *ctx, xmlSAXLocatorPtr loc);
void sax_startDocument (void *ctx);
void sax_endDocument (void *ctx);
void sax_startElement (void *ctx, const xmlChar *name, const xmlChar **atts);
void sax_endElement (void *ctx, const xmlChar *name);
void sax_attribute (void *ctx, const xmlChar *name, const xmlChar *value);
void sax_reference (void *ctx, const xmlChar *name);
void sax_characters (void *ctx, const xmlChar *ch, int len);
void sax_ignorableWhitespace (void *ctx, const xmlChar *ch, int len);
void sax_processingInstruction (void *ctx, const xmlChar *target, const xmlChar *data);
void sax_comment (void *ctx, const xmlChar *value);
void sax_cdataBlock (void *ctx, const xmlChar *value, int len);
void sax_warning (void *ctx, const char *msg, ...);
void sax_error (void *ctx, const char *msg, ...);
void sax_fatalError (void *ctx, const char *msg, ...);
int sax_isStandalone (void *ctx);
int sax_hasInternalSubset (void *ctx);
int sax_hasExternalSubset (void *ctx);

int sax_xmlParseFile( xmlSAXHandlerPtr sax, SaxRecord *obj, char *filename );
int sax_xmlParseChunk( xmlSAXHandlerPtr sax, SaxRecord *obj, FeriteString *chunk );

static int sax_xmlExecuteCtxt( xmlParserCtxtPtr ctxt );

#define SAXObj  ((xmlSAXHandlerPtr)self->odata)

#endif /* __SAX_HANDLERS_H__ */
