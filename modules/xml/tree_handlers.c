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
#include "tree_handlers.h"

void tree_error_handler(void * ctxt, const char * msg, ...)
{
    va_list ap;
    
    va_start(ap, msg );
    ferite_verror(ctxt, 0, (char *)msg, &ap );
    va_end( ap ); 
}

FeriteVariable *create_element_node( FeriteScript *script, xmlDocPtr doc, xmlNodePtr node ) 
{
    FeriteVariable *obj = NULL;
    FeriteClass *cls = NULL;
    
    if(( cls = ferite_find_class( script, script->mainns, "XML.Element" ) ) == NULL) {
        ferite_error( script , 0, "Can't locate class XML.Element" ); 
    }
    obj = ferite_new_object( script, cls, NULL );
    
    ((XMLDoc *)VAO(obj)->odata)->doc = doc; 
    ((XMLDoc *)VAO(obj)->odata)->node = node;
    
    return obj;
}

xmlXPathObjectPtr get_nodes_with_name_xpath( XMLDoc *tree, FeriteString *str ) 
{
    char *xpath, *x = "descendant-or-self::node()";	
    xmlXPathCompExprPtr comp = NULL;
    int length = strlen( x ) + str->length + 4;
    
    xpath = malloc(length);
    memset(xpath, '\0', length );
    sprintf( xpath, "%s/%s", x, str->data );
    comp = xmlXPathCompile( BAD_CAST xpath );
    free(xpath);
    
    if( comp != NULL ) {
        xmlXPathObjectPtr res = NULL; 
        xmlXPathContextPtr ctxt = xmlXPathNewContext( tree->doc );
        
        ctxt->node = tree->node;
        res = xmlXPathCompiledEval( comp, ctxt );
        xmlXPathFreeContext( ctxt );
        xmlXPathFreeCompExpr( comp );  
        return res;
    }
    return NULL;
}

void recursive_namespace_copy( xmlNodePtr target, xmlNodePtr ref )
{
    xmlNodePtr ptr = NULL;
    
    if( target->ns == NULL && ref != NULL )
    {
	  target->ns = ref->ns;
	  for( ptr = target->children; ptr != NULL; ptr = ptr->next )
            recursive_namespace_copy( ptr, ref );
	
   }
}

