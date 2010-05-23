<!DOCTYPE style-sheet PUBLIC "-//James Clark//DTD DSSSL Style Sheet//EN" [
<!ENTITY docbook.dsl PUBLIC "-//Norman Walsh//DOCUMENT DocBook HTML Stylesheet//EN" CDATA DSSSL>
]>

<!--

  HTML-specific stylesheet customization for use by the online manual.
  Shamelessly stolen from php's online manual

-->

<style-sheet>
<style-specification id="docbook-ferite-website" use="docbook">
<style-specification-body>

(define %use-id-as-filename% #t)

(define %indent-programlisting-lines%
  ;; Indent lines in a 'ProgramListing'?
  "    ")

(define %shade-verbatim%
  ;; Should verbatim environments be shaded?
  #t)

(define ($shade-verbatim-attr$)
  ;; Attributes used to create a shaded verbatim environment.
  (list
   (list "BORDER" "0")
   (list "BGCOLOR" "#f0f0f0")
   (list "CELLPADDING" "5")
   (list "CELLSPACING" "1")
   (list "WIDTH" "90%")
   (list "ALIGN" "CENTER")))

(define %root-filename%
  ;; Name for the root HTML document
  "index")
  
(define %css-decoration%
  ;; Enable CSS decoration of elements
  #t)

(define (ferite-code code)
  (make processing-instruction
    data: (string-append " " code "?")))

(define newline "\U-000D")

(declare-flow-object-class processing-instruction
  "UNREGISTERED::James Clark//Flow Object Class::processing-instruction")

(define %html-ext% ".fsp")
(define %output-dir% "website")

;; XXX hackish workaround.  I'm not sure what side effects setting
;; the element number to 0 would have?
(define (generate-anchor #!optional (nd (current-node)))
  (string-append "AEN" (number->string (or (all-element-number nd) 0))))

(define (ferite-quote str)
  (string-replace str "'" "\\'"))

(define (ferite-header #!optional (nd (current-node)))
    (ferite-code
     (string-append
      newline
      "uses \"shared-manual.feh\";" newline
      (ferite-header-navigation nd) newline
      "manualHeader('" (ferite-quote (ferite-node-title nd)) "','" (ferite-quote (html-base-filename nd)) "');" newline)))

(define (ferite-footer #!optional (nd (current-node)))
  (ferite-code
   (string-append
    "manualFooter('" (ferite-quote (ferite-node-title nd)) "','" (ferite-quote (html-base-filename nd)) "');" newline)))

(define (ferite-node-title #!optional (nd (current-node)) (title-elem "title"))
  (let* ((preferred-title-node
	  (cond ((equal? (gi nd) "refentry")
		 (node-list-first
		  (select-elements
		   (children
		    (select-elements (children nd) "refnamediv"))
		   "refname")))
		(else
		 (node-list-first
		  (select-elements (children nd) title-elem)))))
	 (title-node
	  (if (node-list-empty? preferred-title-node)
	      (select-elements (children nd) "title") ;; fallback
	      preferred-title-node)))
    (data title-node)))

(define (ferite-header-nav-array nd)
  (let ((href (href-to nd))
	(title (ferite-quote (ferite-node-title nd "titleabbrev"))))
    (string-append
     "['" href "', '" title "']")))

(define (ferite-node-list-map func ndl)
  (let* ((node (node-list-first ndl))
	 (rest (node-list-rest ndl))
	 (item (func node)))
    (if (node-list-empty? rest)
	(list item)
	(append (list item) (ferite-node-list-map func rest)))))

(define (ferite-header-navigation nd)
  (let ((prev (prev-chunk-element nd))
	(next (next-chunk-element nd))
	(up (parent nd))
	(home (sgml-root-element))
	(toc-nodes (siblings (chunk-parent nd))))
    (string-append
     "setupNavigation([" 
     "  'home' => " (ferite-header-nav-array home) "," newline
     "  'prev' => " (ferite-header-nav-array prev) "," newline
     "  'next' => " (ferite-header-nav-array next) "," newline
     "  'up'   => " (ferite-header-nav-array up) "," newline
     "  'toc'  => [" 
     (join
      (ferite-node-list-map (lambda (nnl)
			      (string-append
			       "    "
			       (ferite-header-nav-array nnl)))
			    toc-nodes)
      (string-append "," newline))
     "] ]);")))

(define (html-document title-sosofo body-sosofo)
  (let ((doc-sosofo
	 (if (or (chunk?) (node-list=? (current-node) (sgml-root-element)))
	     (make sequence
		   (ferite-header (current-node))
		   body-sosofo
		   (ferite-footer (current-node)))
	     body-sosofo)))
    (if (chunk?)
	(make entity
	      system-id: (html-entity-file (html-file))
	      doc-sosofo)
	doc-sosofo)))


</style-specification-body>
</style-specification>
<external-specification id="docbook" document="docbook.dsl">
</style-sheet>

