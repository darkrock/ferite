/*
 * Copyright (C) 2000-2002 Chris Ross and various contributors
 * Copyright (C) 1999-2000 Chris Ross
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
void test_jedi();

int main( int argc, char **argv )
{
   test_jedi();
}

void test_jedi()
{
   char *ptr;

   printf( "RUNNING JEDI TESTS\n" );
   ferite_jedi_memory_init();
  
   ptr = ferite_jedi_malloc( 10 );
   ptr = ferite_jedi_malloc( 10 );
   strcpy( ptr, "TEST" );
   ptr = ferite_jedi_malloc( 10 );
   strcpy( ptr, "TEST" );
   ferite_jedi_free( ptr );
   ptr = ferite_jedi_malloc( 10 );
   ferite_jedi_free( ptr );
   ferite_jedi_free( ptr );
   ferite_jedi_free( ptr );
   ferite_jedi_free( ptr );
   ptr = ferite_jedi_malloc( 10 );
   strcpy( ptr, "TEST" );
   strcpy( ptr, "TEST" );
   strcpy( ptr, "TEST" );
   ptr = ferite_jedi_malloc( 10 );
   strcpy( ptr, "TEST" );
   ferite_jedi_free( ptr );
   ptr = ferite_jedi_malloc( 1024 );
   strcpy( ptr, "TEST" );
   ptr = ferite_jedi_malloc( 1024 );
   ferite_jedi_free( ptr );
   ptr = ferite_jedi_malloc( 1024 );
   strcpy( ptr, "TEST" );
   strcpy( ptr, "TEST" );
   strcpy( ptr, "TEST" );
   strcpy( ptr, "TEST" );
   strcpy( ptr, "TEST" );
   strcpy( ptr, "TEST" );
   strcpy( ptr, "TEST" );
   ptr = ferite_jedi_malloc( 1024 );
   ferite_jedi_free( ptr );
   ferite_jedi_free( ptr );
   ptr = ferite_jedi_malloc( 1024 );
   ferite_jedi_free( ptr );
   ptr = ferite_jedi_malloc( 1024 );
   strcpy( ptr, "TEST" );
   printf( "tst1: %s\n", ptr );
   ferite_jedi_free( ptr );
   ferite_jedi_free( ptr );
   
   ptr = ferite_jedi_calloc( 1024, sizeof(char) );
   ferite_jedi_free( ptr );
   printf( "tst2: %s\n", ptr );
   strcpy( ptr, "TEST" );
   printf( "tst3: %s\n", ptr );
   ferite_jedi_free( ptr );
   
   ferite_jedi_memory_deinit();
   printf( "FINSIHED JEDI TESTS\n" );
}
