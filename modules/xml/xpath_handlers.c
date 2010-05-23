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
#include "xpath_handlers.h"

FeriteVariable *ParseXPath( FeriteScript *script, XMLDoc *tree, const char *str )
{
    xmlXPathObjectPtr res = NULL;
    xmlNodeSetPtr cur = NULL;
    xmlXPathCompExprPtr comp = NULL;
    FeriteVariable *array = NULL,*element = NULL;
    register int i = 0;

    xmlXPathContextPtr ctxt = xmlXPathNewContext( tree->doc );
    ctxt->node = tree->node;

    array = ferite_create_uarray_variable( script, "xpath_result", FE_ARRAY_DEFAULT_SIZE, FE_STATIC );
    comp = xmlXPathCompile(BAD_CAST str);

    if (comp != NULL)
    {
        res = xmlXPathCompiledEval(comp, ctxt);
        xmlXPathFreeCompExpr(comp);

        switch( res->type )
        {

          case XPATH_UNDEFINED:
            ferite_error(NULL, 0, "Object is uninitialized\n");
            break;

          case XPATH_NODESET:

            cur = res->nodesetval;
            for (i = 0 ; i < cur->nodeNr ; i++)
            {
                element = create_element_node( script, tree->doc, cur->nodeTab[i] );		
                ferite_uarray_add( script, VAUA( array ), element, NULL, FE_ARRAY_ADD_AT_END );
            }
            break;

          case XPATH_NUMBER:

            if (xmlXPathIsNaN(res->floatval))
              element = fe_new_str_static("xpath_result", "NaN", 3, FE_CHARSET_DEFAULT);
            else
              element = fe_new_dbl_static("xpath_result", res->floatval);

            ferite_uarray_add( script, VAUA( array ), element, NULL, FE_ARRAY_ADD_AT_END );
            break;

          case XPATH_STRING:
            element = fe_new_str_static("xpath_result", res->stringval, 0, FE_CHARSET_DEFAULT);
            ferite_uarray_add( script, VAUA( array ), element, NULL, FE_ARRAY_ADD_AT_END );
            break;

          case XPATH_BOOLEAN:

            if (res->boolval)
              element = fe_new_str_static("xpath_result", "true", 0, FE_CHARSET_DEFAULT);
            else
              element = fe_new_str_static("xpath_result", "false", 0, FE_CHARSET_DEFAULT);

            ferite_uarray_add( script, VAUA( array ), element, NULL, FE_ARRAY_ADD_AT_END );
            break;

          default:
            ferite_error( script, 0, "Unimplemeted result type");
            break;
        }
    }
    if( res != NULL)
      xmlXPathFreeObject(res);

    xmlXPathFreeContext(ctxt);

    return array;
}
