"""""""""""""""""""""""""""""""""""""""
"cscope
"""""""""""""""""""""""""""""""""""""""
if has("cscope")
    set csprg=/usr/bin/cscope
    " 'csto' 的值决定 |:cstag| 命令查找的次序
    set csto=0
    "C-]同时搜索tags和cscope数据库
    set nocst
    " 当添加一个数据库时，成功与否不会被显示
    set nocsverb
    " add any database in current directory
    if filereadable("cscope.out")
        cs add cscope.out
        " else add database pointed to by environment
    elseif $CSCOPE_DB != ""
        cs add $CSCOPE_DB
    endif
    set csverb
    "set cscopetag
    set cscopequickfix=s-,g-,c-,d-,t-,e-,f-,i-
endif
"递归向上级目录查找cscope数据库
if has("gui_running")
   if has("cscope")
      let s:MyCurDir = getcwd()
      let s:MyOldDir = s:MyCurDir

      while 1
         if filereadable("cscope.out")
            if 0 == cscope_connection(2, "cscope.out", s:MyCurDir)
               exec "silent! cs add cscope.out " . s:MyCurDir
            endif
"            exec "setlocal path=\"" . s:MyCurDir . "/**\""
"            setlocal cpt-=i
            break
         endif
         if s:MyCurDir =~ '^[a-zA-Z]:\\$'
            break
         endif
         cd ..
         let s:MyCurDir = getcwd()
      endwhile

      exec "cd " . s:MyOldDir . ""

   endif
else
   if has("cscope")
      let s:MyCurDir = getcwd()
      let s:MyOldDir = s:MyCurDir

      while 1
         if filereadable("cscope.out")
            if 0 == cscope_connection(2, "cscope.out", s:MyCurDir)
               exec "silent! cs add cscope.out " . s:MyCurDir
            endif
"            exec "setlocal path=\"" . s:MyCurDir . "/**\""
"            setlocal cpt-=i
            break
         endif
         if s:MyCurDir =~ '^/$'
            break
         endif
         cd ..
         let s:MyCurDir = getcwd()
      endwhile

      let s:MyOldDir = escape(s:MyOldDir,' ')

      exec "cd " . s:MyOldDir
   endif
endif

map <C-\> :cstag <C-R>=expand("<cword>")<CR><CR>
nmap <C-\>s :cs find s <C-R>=expand("<cword>")<CR><CR>
nmap <C-\>g :cs find g <C-R>=expand("<cword>")<CR><CR>
nmap <C-\>c :cs find c <C-R>=expand("<cword>")<CR><CR>
nmap <C-\>t :cs find t <C-R>=expand("<cword>")<CR><CR>
nmap <C-\>e :cs find e <C-R>=expand("<cword>")<CR><CR>
nmap <C-\>f :cs find f <C-R>=expand("<cfile>")<CR><CR>
nmap <C-\>i :cs find i ^<C-R>=expand("<cfile>")<CR>$<CR>
nmap <C-\>d :cs find d <C-R>=expand("<cword>")<CR><CR>

" Using 'CTRL-spacebar' then a search type makes the vim window
" split horizontally, with search result displayed in
" the new window.

nmap <C-Space>s :scs find s <C-R>=expand("<cword>")<CR><CR>
nmap <C-Space>g :scs find g <C-R>=expand("<cword>")<CR><CR>
nmap <C-Space>c :scs find c <C-R>=expand("<cword>")<CR><CR>
nmap <C-Space>t :scs find t <C-R>=expand("<cword>")<CR><CR>
nmap <C-Space>e :scs find e <C-R>=expand("<cword>")<CR><CR>
nmap <C-Space>f :scs find f <C-R>=expand("<cfile>")<CR><CR>
nmap <C-Space>i :scs find i ^<C-R>=expand("<cfile>")<CR>$<CR>
nmap <C-Space>d :scs find d <C-R>=expand("<cword>")<CR><CR>

" Hitting CTRL-space *twice* before the search type does a vertical
" split instead of a horizontal one

nmap <C-Space><C-Space>s
            \:vert scs find s <C-R>=expand("<cword>")<CR><CR>
nmap <C-Space><C-Space>g
            \:vert scs find g <C-R>=expand("<cword>")<CR><CR>
nmap <C-Space><C-Space>c
            \:vert scs find c <C-R>=expand("<cword>")<CR><CR>
nmap <C-Space><C-Space>t
            \:vert scs find t <C-R>=expand("<cword>")<CR><CR>
nmap <C-Space><C-Space>e
            \:vert scs find e <C-R>=expand("<cword>")<CR><CR>
nmap <C-Space><C-Space>i
            \:vert scs find i ^<C-R>=expand("<cfile>")<CR>$<CR>
nmap <C-Space><C-Space>d
            \:vert scs find d <C-R>=expand("<cword>")<CR><CR>


"""""""""""""""""""""""""""""""""""
"ctags设置
"""""""""""""""""""""""""""""""""""
set tags=~/.vim/cpp/cpp_src/tags
set tags+=./tags,tags,../tags,../../tags
set tags+=~/tags
set tags+=~/.vim/cpp/boost
set tags+=~/.vim/cpp/posix
"智能补全ctags -R --c++-kinds=+p --fields=+iaS --extra=+q
let OmniCpp_DefaultNamespaces = ["std"]
let OmniCpp_GlobalScopeSearch = 1  " 0 or 1
let OmniCpp_NamespaceSearch = 1   " 0 ,  1 or 2
let OmniCpp_DisplayMode = 1
let OmniCpp_ShowScopeInAbbr = 0
let OmniCpp_ShowPrototypeInAbbr = 1
let OmniCpp_ShowAccess = 1
let OmniCpp_MayCompleteDot = 1
let OmniCpp_MayCompleteArrow = 1
let OmniCpp_MayCompleteScope = 1
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" CTags的设定
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"Taglist插件的设置
let Tlist_Ctags_Cmd = '/usr/bin/ctags'

" 按照名称排序
let Tlist_Sort_Type = "name"

" 在右侧显示窗口
let Tlist_Use_Right_Window = 1

" 压缩方式
let Tlist_Compart_Format = 1

" 如果只有一个buffer，kill窗口也kill掉buffer
let Tlist_Exist_OnlyWindow = 1

" 不要关闭其他文件的tags
let Tlist_File_Fold_Auto_Close = 0

" 不要显示折叠树
let Tlist_Enable_Fold_Column = 0

"不同时显示多个文件的tag，只显示当前文件的
let g:Tlist_Show_One_File = 1

"""""""""""""""""""""""""""""""""""
"a.vim
"""""""""""""""""""""""""""""""""""
autocmd FileType c,cpp,cs source ~/.vim/cpp/a.vim

""""""""""""""""""""""""""""""""""
"crefvim
"There are several ways to specify a word CRefVim should search for in order
"to view help:
"
"  <Leader>cr normal mode:  get help for word under cursor
"                           Memory aid cr: (c)-(r)eference
"  <Leader>cr visual mode:  get help for visually selected text
"                           Memory aid cr: (c)-(r)eference
"  <Leader>cw:              prompt for word CRefVim should search for
"                           Memory aid cw: (c)-reference (w)hat
"  <Leader>cc:              jump to table of contents of the C-reference manual
"                           Memory aid cc: (c)-reference (c)ontents
"
"Note: by default <Leader> is \, e.g. press \cr to invoke C-reference
"
"Note: The best way to search for an operator (++, --, %, ...) is to visually
"select it and press <Leader>cr.
""""""""""""""""""""""""""""""""""""

""""""""""""""""""""""""""""""""""""
"stlrefvim
""""""""""""""""""""""""""""""""""""
autocmd FileType c,cpp,cs source ~/.vim/cpp/stlrefvim.vim
"For example, in the expression:
"    std::for_each(start, end, PrintFunc)
"    place the cursor over for_each and type <Leader>tr to go to documentation for this function.  Type <Leader>te to go to an example that uses this function.


"""""""""""""""""""""""""""""
" F5编译和运行C程序，F6编译和运行C++程序
" 请注意，下述代码在windows下使用会报错
" 需要去掉./这两个字符
"""""""""""""""""""""""""""""
" C的编译和运行
"func! CompileRunGcc()
"    exec "w"
"    exec "!gcc -Wall -g % -o %<"
"    exec "! ./%<"
"endfunc
"
"" C++的编译和运行
"func! CompileRunGpp()
"    exec "w"
"    exec "!g++ -Wall -g % -o %<"
"    exec "! ./%<"
"endfunc
"
func! ReTag()
    exec "w"
    exec "!ctags -R --c++-kinds=+p --fields=+iaS --extra=+q"
    set tags+=./tags
endfunc

map <F7> :call ReTag()<CR>
""""""""""""""""""""""""""""""
"Doxygen
""""""""""""""""""""""""""""""

autocmd FileType c,cpp,cs source ~/.vim/cpp/DoxygenToolkit.vim
let g:DoxygenToolkit_authorName = "kofshower"
let g:DoxygenToolkit_briefTag_funcName = "yes"
let g:DoxygenToolkit_briefTag_className = "yes"
let g:DoxygenToolkit_briefTag_structName = "yes"
let g:DoxygenToolkit_briefTag_enumName = "yes"
let g:DoxygenToolkit_briefTag_namespaceName = "yes"
"let g:DoxygenToolkit_licenseTag = "\<enter>Copyright (C) 2010 TL50. All right reserved."$
let g:DoxygenToolkit_paramTag_pre="@param "
let g:DoxygenToolkit_paramTag_post=": "

"let g:DoxygenToolkit_briefTag_pre="@Synopsis  "
"let g:DoxygenToolkit_paramTag_pre="@Param "
"let g:DoxygenToolkit_returnTag="@Returns   "
"let g:DoxygenToolkit_blockHeader="--------------------------------------------------------------------------"
"let g:DoxygenToolkit_blockFooter="----------------------------------------------------------------------------"
"let g:DoxygenToolkit_authorName="Chai JunJun"
" let g:DoxygenToolkit_licenseTag="My own license\<enter>"   <-- Do not forget
" ending "\<enter>"
"let g:DoxygenToolkit_licenseTag="GPL\<enter>Free soft,no warranted\<enter>We are all in the mud, but some of us are looking at the stars.\<enter>We are not born for ourselves.\<enter>"
let g:DoxygenToolkit_undocTag="DOXIGEN_SKIP_BLOCK"
"# default use /**...*/ as comment. With this option /// comments are used.
"let g:DoxygenToolkit_commentType="C++"
"let g:DoxygenToolkit_briefTag_funcName = "yes"  " to include function name between briefTag_pre and briefTag_post

"""""""""""""""""""""""""""""""""
"supertab
"""""""""""""""""""""""""""""""""
let g:SuperTabDefaultCompletionType="<C-X><C-O>"
let g:SuperTabRetainCompletionType=2

"""""""""""""""""""""""""""""""""
"alias
"""""""""""""""""""""""""""""""""
:abbreviate usestd using namespace std;

"""""""""""""""""""""""""""""""""
"c/c++ IDE
"""""""""""""""""""""""""""""""""
source ~/.vim/cpp/side-c.vim
""""""""""""""""""""""""""""""""""
""C语言注释和反注释,虽然有EnhancedCommentify,但还是习惯自己写的
""Uncomment
":nnoremap <buffer> <silent> <F2>uc :.s+\(// *\\|/\* *\\| *\*/\)++g<CR>:silent! .s/\|+/\/*/g<CR>:silent! .s/+\|/*\//g<CR>:noh<CR>
""Comment
":nnoremap <buffer> <silent> <F2>cm :silent! .s/\/\*/\|+/g<CR>:silent! .s/\*\//+\|/g<CR>:.s/^\(.*\)$/\/* \1 *\//<CR>:noh<CR>
""Uncomment sub-comment
":vnoremap <buffer> <silent> <F2>usubcm :s+/\@<=\*\@=\\|\*\@<=/\@=+ +g<CR>:noh<CR>
""Virtual comment
":vnoremap <buffer> <silent> * :<ESC>'<O<Home>/*<ESC>'>o<Home>*/<ESC>:silent! '<,'>s/\/\*/\|+/g<CR>:silent! '<,'>s/\*\//+\|/g<CR>:noh<CR>
""Virtual uncomment
":vnoremap <buffer> <silent> u* :s+\(/\*\\|\*/\)++<CR>:silent! '<,'>s/\|+/\/*/g<CR>:silent! '<,'>s/+\|/*\//g<CR>:noh<CR>
""Virtual Partially comment
":vnoremap <buffer> <silent> <F2>pcm xi/*  */<ESC>hhP
"

"浮动窗口显示tag, 需要gui, 有点小bug，有时会报错
if has('gui_running')
    set ballooneval
    set balloondelay=100
    set balloonexpr=MyTagDisplay()

    function! MyTagDisplay()
        if empty(tagfiles())
            return ''
        endif
        let s:MyTagShow = []
        let s:MyTaglist = taglist("^" . escape(v:beval_text,'\*.$^') . "$")
        for s:MyTag in s:MyTaglist
            if s:MyTag['cmd'] =~ '^/\^.*\$/$'
                let s:MyTagShow += [strpart(s:MyTag['cmd'],2,strlen(s:MyTag['cmd'])-4)]
            endif
        endfor
        return join (s:MyTagShow,"\n")
    endfunction
endif

"""""""""""""""""""""""""""""""""""
"设置语法折叠
"""""""""""""""""""""""""""""""""""
let g:NeoComplCache_EnableAtStartup = 1
let g:NeoComplCache_SmartCase = 1
let g:NeoComplCache_EnableUnderbarCompletion = 1
:NeoComplCacheEnable
map <c-c> ,c<space>
let g:winManagerWindowLayout='FileExplorer|TagList|BufExplorer'
let g:winManagerWidth=35
nmap <F4> :WMToggle<cr>
"nmap tl :Tlist<cr>
autocmd FileType c,cpp,cs source ~/.vim/cpp/valgrind.vim
let g:valgrind_arguments='--leak-check=yes --num-callers=5000'
