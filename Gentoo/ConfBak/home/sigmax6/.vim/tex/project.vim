function! Texd()
    w
    !xelatex %
"    silent !xpdf %:r.pdf
endfunction

map <F5> :call Texd()<CR>
set nu

