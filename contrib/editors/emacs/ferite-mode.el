;;; ferite-mode.el -- major mode for editing ferite code

;; Copyright (C) 2005 Michael D Henderson
;; Derived from php-mode.el (1.1.0), Copyright (C) 1999-2004 Turadg Aleahmad

;; Maintainer: Michael D Henderson <blueronin at users.sourceforge.net>
;; Keywords: ferite languages script
;; Created: 2005-01-09
;; Modified: 2005-01-16
;; X-URL:   http://ferite-mode.sourceforge.net/

(defconst ferite-version "1.0.2"
  "Ferite Mode version number.")

;;; License

;; This program is free software; you can redistribute it and/or
;; modify it under the terms of the GNU General Public License
;; as published by the Free Software Foundation; either version 2
;; of the License, or (at your option) any later version.

;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with this program; if not, write to the Free Software
;; Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

;; ferite mode is a major mode for editing ferite scripts. It's an extension
;; of C mode; thus it inherits all C mode's navigation functionality.  But it
;; colors according to the ferite grammar and indents according to my fetishes.
;; It also includes a couple handy IDE-type features such as documentation
;; search and a source and class browser.

;;; Contributors: (in chronological order)

;; Michael D Henderson

;;; Changelog:
;; 1.0.2
;;   Add copyright notice from php-mode.el.
;; 1.0.1
;;   Updated keyword list. Twiddled with syntax and highlighting.
;; 1.0.0
;;   Created. The following sites helped me figure out what to do
;;     http://two-wugs.net/emacs/mode-tutorial.html
;;     http://php-mode.sourceforge.net
;;

;; TODO
;;   Add feritedoc


;;; Commentary:
;; 


;;; History:
;; 

;;; Code:

(require 'speedbar)
(require 'font-lock)
(require 'cc-mode)
(require 'custom)
(eval-when-compile
  (require 'regexp-opt))

;; Local variables
(defgroup ferite nil
  "Major mode for editing ferite code."
  :prefix "ferite-"
  :group 'languages)

(defcustom ferite-speedbar-config t
  "*When set to true automatically configures Speedbar to observe ferite files.\
Ignores ferite-file patterns option; fixed to expression \"\\.\\(fe\\|fec?\\)\""
  :type 'boolean
  :group 'ferite)

(defcustom ferite-mode-speedbar-open nil
  "Normally `ferite-mode' starts with the speedbar closed.\
Turning this on will open it whenever `ferite-mode' is loaded."
  :type 'boolean
  :group 'ferite)

(defcustom ferite-manual-url "http://www.ferite.org/www-manual/index.fe"
  "*URL at which to find ferite manual.\
You can replace \"en\" with your ISO language code."
  :type 'string
  :group 'ferite)

(defcustom ferite-search-url "http://www.ferite.org/"
  "*URL at which to search for documentation on a word."
  :type 'string
  :group 'ferite)

;;;###autoload
(defcustom ferite-file-patterns (list "\\.\\(fe\\|fec\\)\\'")
  "*List of file patterns for which to automatically invoke `ferite-mode'."
  :type '(repeat (regexp :tag "Pattern"))
  :group 'ferite)
(defcustom ferite-mode-user-hook nil
  "List of functions to be executed on entry to `ferite-mode'."
  :type 'hook
  :group 'ferite)

(defcustom ferite-mode-force-pear nil
  "Normally PEAR coding rules are enforced only when the filename contains\
\"PEAR.\" Turning this on will force PEAR rules on all ferite files."
  :type 'boolean
  :group 'ferite)

;; Note whether we're in XEmacs
(defconst xemacsp (string-match "Lucid\\|XEmacs" emacs-version)
  "Non nil if using XEmacs.")

;;;###autoload
(define-derived-mode ferite-mode c-mode "ferite"
  "Major mode for editing ferite code.\n\n\\{ferite-mode-map}"

  (setq comment-start "// "
	comment-end   ""
	comment-start-skip "// *")

  (setq c-class-key ferite-class-key)
  (setq c-conditional-key ferite-conditional-key)

  (defvar ferite-mode-syntax-table ferite-mode-syntax-table)
  ;; The above causes XEmacs to handle shell-style comments correctly,
  ;; but fails to work in GNU Emacs which fails to interpret \n as the
  ;; end of the comment.
  (if xemacsp (progn
		(modify-syntax-entry ?# "< b" ferite-mode-syntax-table)
		(modify-syntax-entry ?\n "> b" ferite-mode-syntax-table)
		))

  (make-local-variable 'font-lock-defaults)
  (setq font-lock-defaults
 	'((ferite-font-lock-keywords-1
 	   ferite-font-lock-keywords-2
 	   ;; Comment-out the next line if the font-coloring is too
 	   ;; extreme/ugly for you.
 	   ; ferite-font-lock-keywords-3
 	   )
 	  nil				; KEYWORDS-ONLY
 	  nil				; CASE-FOLD
 	  nil				; SYNTAX-ALIST
 	  nil				; SYNTAX-BEGIN
 	  (font-lock-syntactic-keywords . ferite-font-lock-syntactic-keywords)))

  (setq font-lock-maximum-decoration t
	case-fold-search t		; ferite vars are case-sensitive
	imenu-generic-expression ferite-imenu-generic-expression)

  ;; Do force newline at end of file.
  (set (make-local-variable 'require-final-newline) t)
  (set (make-local-variable 'next-line-add-newlines) t)

  ;; PEAR coding standards
  (make-local-hook 'ferite-mode-pear-hook)
  (add-hook 'ferite-mode-pear-hook
	    (lambda nil (set (make-local-variable 'tab-width) 4)) nil t)
  (add-hook 'ferite-mode-pear-hook
	    (lambda nil (set (make-local-variable 'c-basic-offset) 4)) nil t)
  (add-hook 'ferite-mode-pear-hook
	    (lambda nil (set (make-local-variable 'c-hanging-comment-ender-p) nil)) nil t)
  (add-hook 'ferite-mode-pear-hook
	    (lambda nil (set (make-local-variable 'indent-tabs-mode) nil)) nil t)
  (add-hook 'ferite-mode-pear-hook
	    (lambda nil (c-set-offset 'block-open' - )) nil t)
  (add-hook 'ferite-mode-pear-hook
	    (lambda nil (c-set-offset 'block-close' 0 )) nil t)

  (if (or ferite-mode-force-pear
	  (and (stringp buffer-file-name)
	       (string-match "PEAR\\|pear"
			     (buffer-file-name))
	       (string-match "\\.fe$" (buffer-file-name))))
      (run-hooks 'ferite-mode-pear-hook))

  (run-hooks 'ferite-mode-user-hook)
  )

;; Make ferite-mode the default mode for ferite source code buffers.
;;;###autoload
(let ((ferite-file-patterns-temp ferite-file-patterns))
  (while ferite-file-patterns-temp
    (add-to-list 'auto-mode-alist
		  (cons (car ferite-file-patterns-temp) 'ferite-mode))
    (setq ferite-file-patterns-temp (cdr ferite-file-patterns-temp))))

;; Handle Speedbar
(if ferite-mode-speedbar-open
    (speedbar 1))
(if (and ferite-speedbar-config (symbolp 'speedbar))
    (speedbar-add-supported-extension "\\.\\(fe\\)"))

;; Make a menu keymap (with a prompt string)
;; and make it the menu bar item's definition.
(define-key ferite-mode-map [menu-bar] (make-sparse-keymap))
(define-key ferite-mode-map [menu-bar ferite]
  (cons "ferite" (make-sparse-keymap "ferite")))

;; Define specific subcommands in this menu.
(define-key ferite-mode-map [menu-bar ferite complete-function]
  '("Complete function name" . ferite-complete-function))
(define-key ferite-mode-map
  [menu-bar ferite browse-manual]
  '("Browse manual" . ferite-browse-manual))
(define-key ferite-mode-map
  [menu-bar ferite search-documentation]
  '("Search documentation" . ferite-search-documentation))

;; Define function name completion function
(defun ferite-complete-function ()
  "Complete the function name at the point from known ferite functions."
  (interactive)
  (message "ferite-complete-function not implemented yet")
  ;; how to read the list of functions from a separate file?
  )

;; Define function documentation function
(defun ferite-search-documentation ()
  "Search ferite documentation for the word at the point."
  (interactive)
  (browse-url (concat ferite-search-url (current-word t)))
  )

;; Define function for browsing manual
(defun ferite-browse-manual ()
  "Bring up manual for ferite."
  (interactive)
  (browse-url ferite-manual-url)
  )

;; Define shortcut
(define-key ferite-mode-map
  "\C-c\C-f"
  'ferite-search-documentation)

;; Define shortcut
(define-key ferite-mode-map
  "\C-c\C-m"
  'ferite-browse-manual)

(defconst ferite-constants
  (eval-when-compile
    (regexp-opt
     '(;; core constants
       "__LINE__" "__FILE__" "__SCRIPT__"
       "FERITE_OS" "FERITE_VERSION"

       ;; Disabled by default because they slow buffer loading
       ;; If you have use for them, decomment the strings
       ;; that you want colored.
       ;; To compile, you may have to increase 'max-specpdl-size'
       )))
  "Ferite constants.")

(defconst ferite-keywords
  (eval-when-compile
    (regexp-opt
     ;; "class", "new" and "extends" get special treatment
     ;; "case" and  "default" get special treatment elsewhere
     '("abstract" "alias" "and" "arguments"
       "break"
       "case" "closure" "constructor" "continue"
       "deliver" "do"
       "else" "eval" "exit" "extends"
       "false" "final" "fix" "for" "function"
       "global"
       "if" "iferr" "implements" "include" "isa" "instanceof"
       "modifies" "module-header" "module-init" "module-deinit" "module-register" "module-unregister"
       "native" "namespace" "not" "null"
       "or"
       "private" "protected" "protocol" "public"
       "recipient" "rename" "return"
       "self" "static" "super" "switch"
       "true"
       "uses" "using"
       "while")))
  "Ferite keywords.")

(defconst ferite-identifier
  (eval-when-compile
    '"[a-zA-Z\_\x7f-\xff][a-zA-Z0-9\_\x7f-\xff]*")
  "Characters in a ferite identifier.")

(defconst ferite-types
  (eval-when-compile
    (regexp-opt '("array" "number" "object" "string" "void")))
  "Ferite types.")

(defconst ferite-superglobals
  (eval-when-compile
    (regexp-opt '("zatrue" "zafalse")))
  "Ferite superglobal variables.")

;; Set up font locking
(defconst ferite-font-lock-keywords-1
  (list
   ;; Fontify constants
   (cons
    (concat "\\<\\(" ferite-constants "\\)\\>")
    'font-lock-constant-face)

   ;; Fontify keywords
   (cons
    (concat "\\<\\(" ferite-keywords "\\)\\>")
    'font-lock-keyword-face)

   ;; Fontify keywords and targets, and case default tags.
   (list "\\<\\(break\\|case\\|continue\\)\\>[ \t]*\\(-?\\(?:\\sw\\|\\s_\\)+\\)?"
	 '(1 font-lock-keyword-face) '(2 font-lock-constant-face t t))
   ;; This must come after the one for keywords and targets.
   '(":" ("^[ \t]*\\(\\(?:\\sw\\|\\s_\\)+\\)[ \t]*:[ \t]*$"
	  (beginning-of-line) (end-of-line)
	  (1 font-lock-constant-face)))
   )
  "Subdued level highlighting for ferite mode.")

(defconst ferite-font-lock-keywords-2
  (append
   ferite-font-lock-keywords-1
   (list

    ;; class declaration
    '("\\<\\(class\\|interface\\|namespace\\|protocol\\)[ \t]*\\(\\(?:\\sw\\|\\s_\\)+\\)?"
      (1 font-lock-keyword-face) (2 font-lock-type-face nil t))
    ;; handle several words specially, to include following word,
    ;; thereby excluding it from unknown-symbol checks later
    ;; FIX to handle implementing multiple
    ;; currently breaks on "class Foo implements Bar, Baz"
    '("\\<\\(new\\|extends\\|implements\\)\\s-+\\$?\\(\\(?:\\sw\\|\\s_\\)+\\)"
      (1 font-lock-keyword-face) (2 font-lock-type-face))

    ;; function declaration
    '("\\<\\(function\\)\\s-+&?\\(\\(?:\\sw\\|\\s_\\)+\\)\\s-*("
      (1 font-lock-keyword-face)
      (2 font-lock-function-name-face nil t))

    ;; class hierarchy
    '("\\(self\\|parent\\)\\W" (1 font-lock-constant-face nil nil))

    ;; method and variable features
    '("\\<\\(final\\|private\\|protected\\|public\\|static\\)\\s-+\\$?\\(?:\\sw\\|\\s_\\)+"
      (1 font-lock-keyword-face))

    ;; method features
    '("^[ \t]*\\(abstract\\|static\\|final\\)\\s-+\\$?\\(?:\\sw\\|\\s_\\)+"
      (1 font-lock-keyword-face))

    ;; variable features
    '("^[ \t]*\\(static\\|const\\)\\s-+\\$?\\(?:\\sw\\|\\s_\\)+"
      (1 font-lock-keyword-face))
    ))
  "Medium level highlighting for ferite mode.")

(defconst ferite-font-lock-keywords-3
  (append
   ferite-font-lock-keywords-2
   (list

    ;; <word> or </word> for HTML
    '("</?\\sw+[^>]*>" . font-lock-constant-face)

    ;; HTML entities
    '("&\\w+;" . font-lock-variable-name-face)

    ;; warn about '$' immediately after ->
    '("\\$\\(?:\\sw\\|\\s_\\)+->\\s-*\\(\\$\\)\\(\\(?:\\sw\\|\\s_\\)+\\)"
      (1 font-lock-warning-face) (2 default))

    ;; warn about $word.word -- it could be a valid concatenation,
    ;; but without any spaces we'll assume $word->word was meant.
    '("\\$\\(?:\\sw\\|\\s_\\)+\\(\\.\\)\\sw"
      1 font-lock-warning-face)

    ;; Warn about ==> instead of =>
    '("==+>" . font-lock-warning-face)

    ;; exclude casts from bare-word treatment (may contain spaces)
    `(,(concat "(\\s-*\\(" ferite-types "\\)\\s-*)")
      1 font-lock-type-face)

    ;; ferite5: function declarations may contain classes as parameters type
    `(,(concat "[(,]\\s-*\\(\\(?:\\sw\\|\\s_\\)+\\)\\s-+\\$\\(?:\\sw\\|\\s_\\)+\\>")
      1 font-lock-type-face)

    ;; Fontify variables and function calls
    '("\\$\\(this\\|that\\)\\W" (1 font-lock-constant-face nil nil))
    `(,(concat "\\$\\(" ferite-superglobals "\\)\\W")
      (1 font-lock-constant-face nil nil)) ; $_GET & co
    '("\\$\\(\\(?:\\sw\\|\\s_\\)+\\)" (1 font-lock-variable-name-face)) ; $variable
    '("->\\(\\(?:\\sw\\|\\s_\\)+\\)" (1 font-lock-variable-name-face t t)) ; ->variable
    '("->\\(\\(?:\\sw\\|\\s_\\)+\\)\\s-*(" . (1 default t t)) ; ->function_call
    '("\\(?:\\sw\\|\\s_\\)+::\\(?:\\sw\\|\\s_\\)+\\s-*(" . default) ; class::method call
    '("\\<\\(?:\\sw\\|\\s_\\)+\\s-*[[(]" . default)	; word( or word[
    '("\\<[0-9]+" . default)		; number (also matches word)

    ;; Warn on any words not already fontified
    '("\\<\\(?:\\sw\\|\\s_\\)+\\>" . font-lock-warning-face)
    ))
  "Gauchy level highlighting for ferite mode.")

(defconst ferite-font-lock-syntactic-keywords
  (if xemacsp nil
    ;; Mark shell-style comments.  font-lock handles this in a
    ;; separate pass from normal syntactic scanning (somehow), so we
    ;; get a chance to mark these in addition to C and C++ style
    ;; comments.  This only works in GNU Emacs, not XEmacs 21 which
    ;; seems to ignore this same code if we try to use it.
    (list
     ;; Mark _all_ # chars as being comment-start.  That will be
     ;; ignored when inside a quoted string.
     '("\\(\#\\)"
       (1 (11 . nil)))
     ;; Mark all newlines ending a line with # as being comment-end.
     ;; This causes a problem, premature end-of-comment, when '#'
     ;; appears inside a multiline C-style comment.  Oh well.
     '("#.*\\([\n]\\)"
       (1 (12 . nil)))
     )))

;; Define the imenu-generic-expression for ferite mode.
;; To use, execute M-x imenu, then click on Functions or Classes,
;; then select given function/class name to go to its definition.
;; [Contributed by Gerrit Riessen]
(defvar ferite-imenu-generic-expression
 '(
   ("Functions"
    "^\\s-*function\\s-+\\([a-zA-Z0-9_]+\\)\\s-*(" 1)
   ("Classes"
    "^\\s-*class\\s-+\\([a-zA-Z0-9_]+\\)\\s-*" 1)
   )
 "Imenu generic expression for ferite Mode.  See `imenu-generic-expression'."
 )

;; Add "foreach" to conditional introducing keywords
(defconst ferite-conditional-key nil)
(let ((all-kws "for\\|if\\|do\\|else\\|while\\|switch\\|iferr\\|fix")
      (front   "\\<\\(")
      (back    "\\)\\>[^_]"))
  (setq ferite-conditional-key (concat front all-kws back)))

(defconst ferite-class-kwds "class\\|interface")

(defconst ferite-class-key
  (concat
   "\\(" ferite-class-kwds "\\)\\s +"
   c-symbol-key				      ;name of the class
   "\\(\\s *extends\\s *" c-symbol-key "\\)?" ;maybe followed by superclass
   "\\(\\s *implements *[^{]+{\\)?"	      ;maybe the adopted protocols list
   ))

;; Create "default" symbol for GNU Emacs so that both XEmacs and GNU
;; emacs can refer to the default face by a variable named "default".
(unless (boundp 'default)
  (defvar default 'default))

;; Create faces for XEmacs
(unless (boundp 'font-lock-keyword-face)
  (copy-face 'bold 'font-lock-keyword-face))
(unless (boundp 'font-lock-constant-face)
  (copy-face 'font-lock-keyword-face 'font-lock-constant-face))

(provide 'ferite-mode)

;;; ferite-mode.el ends here
