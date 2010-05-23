" Vim syntax file
" Language:	Ferite
" Maintainer:	Espen Nilsen <nilsenea@plu.edu>
" Note:		Modified cpp.vim syntax file maintained by Ken Shan <ccshan@post.harvard.com> for ferite
" Last change:	2001 November 10

" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
if version < 600
  syntax clear
elseif exists("b:current_syntax")
  finish
endif

" Read the Ferite syntax to start with
if version < 600
  so <sfile>:p:h/c.vim
else
  runtime! syntax/c.vim
  unlet b:current_syntax
endif

" Ferite extentions
syn keyword feStatement		new self uses return super break continue include delete rename isa instanceof lambda with arguments closure using deliver recipient
syn keyword feConditional	if else
syn keyword feRepeat		for do while foreach
syn keyword feAccess		global static native final atomic private protected public abstract attribute_missing method_missing
syn keyword feException		iferr fix else
syn keyword feStructure		protocol implements class function namespace extends modifies
syn keyword feType		    number string array object void
syn keyword feOperator		operator typeid
syn keyword feNumber		NPOS
syn keyword feBoolean		true false null

" Default highlighting
if version >= 508 || !exists("did_fe_syntax_inits")
  if version < 508
    let did_fe_syntax_inits = 1
    command -nargs=+ HiLink hi link <args>
  else
    command -nargs=+ HiLink hi def link <args>
  endif
  HiLink feAccess		feStatement
  HiLink feOperator		feStatement
  HiLink feStatement		Statement
  HiLink feRepeat		Repeat
  HiLink feConditional		Conditional
  HiLink feException		Exception
  HiLink feType			Type
  HiLink feStructure		Structure
  HiLink feNumber		Number
  HiLink feBoolean		Boolean
  delcommand HiLink
endif

let b:current_syntax = "fe"

" vim: ts=8
