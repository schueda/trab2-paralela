MAIN=main

main: $(MAIN).tex
	@pdflatex $(MAIN)
	@bibtex   $(MAIN)
	@pdflatex $(MAIN)
	@pdflatex $(MAIN)
	open $(MAIN).pdf

bib:
	@bibtex $(MAIN)

clean:
	@find . -xdev -iname \*~    -print -delete
	@find . -xdev -iname \*.aux -print -delete
	-rm -f texput.log
	-rm -f $(MAIN).log $(MAIN).lot $(MAIN).lof
	-rm -f $(MAIN).toc $(MAIN).bbl $(MAIN).blg
	-rm -f $(MAIN).acr $(MAIN).smb
	-rm -f $(MAIN).fdb_latexmk $(MAIN).fls

purge: clean
	@rm -f "$(MAIN).pdf"
