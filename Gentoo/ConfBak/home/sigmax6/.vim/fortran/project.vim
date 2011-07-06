set nu
let g:NeoComplCache_EnableAtStartup = 1
let g:NeoComplCache_SmartCase = 1
let g:NeoComplCache_EnableUnderbarCompletion = 1
:NeoComplCacheEnable
map <c-c> ,c<space>
let g:winManagerWindowLayout='FileExplorer|TagList|BufExplorer'
let g:winManagerWidth=35
nmap <F4> :WMToggle<cr>
"""""""""""""""""""""""""""""
" F6编译和运行fortran程序，F6编译和运fortran程序
" 请注意，下述代码在windows下使用会报错
" 需要去掉./这两个字符
"""""""""""""""""""""""""""""
"map <F6> :call CompileRunGpp()<CR>
func! CompileRunGpp()
    exec "w"
    exec "!gfortran -Wall -g % -o %<"
    exec "! ./%<"
endfunc
"au FileType cpp map <F5> :call CompileRunGpp()<CR>
"au FileType c map <F5> :call CompileRunGcc()<CR>
func! ReTag()
    exec "w"
    exec "!ctags -R --c++-kinds=+p --fields=+iaS --extra=+q"
    set tags+=./tags
endfunc

map <F7> :call ReTag()<CR>
