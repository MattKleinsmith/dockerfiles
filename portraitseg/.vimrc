set term=screen-256color
" Rebind <Leader> key
" I like to have it here becuase it is easier to reach than the default and
" it is next to ``m`` and ``n`` which I use for navigating between tabs.
" I mapped my Caps Lock to the Escape key.
let mapleader = ","
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" Vundle
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
set nocompatible              " be iMproved, required
filetype off                  " required

" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
" alternatively, pass a path where Vundle should install plugins
"call vundle#begin('~/some/path/here')

" let Vundle manage Vundle, required
Plugin 'VundleVim/Vundle.vim'

" plugin on GitHub repos
Plugin 'powerline/powerline'
Plugin 'jistr/vim-nerdtree-tabs'
"Plugin 'python-mode/python-mode'
Plugin 'scrooloose/nerdtree'
Plugin 'tmhedberg/SimpylFold'
Plugin 'tpope/vim-fugitive'
Plugin 'tpope/vim-sensible'
call vundle#end()            " required
filetype plugin indent on    " required

" ============================================================================
" Python mode
" ============================================================================
" Settings for python-mode
"map <Leader>g :call RopeGotoDefinition()<CR>
"let g:pymode_python = 'python3'
"let g:pymode_lint_ignore="W0401,E501"
"let g:pymode_rope_goto_definition_bind = "<C-]>"
"let g:pymode_run_bind = "<C-S-e>"
"let g:pymode_doc_bind = "<C-S-d>"
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
set encoding=utf-8

" .vimrc file modified by Dan Sheffner

" Lots of credit to:
" Sample .vimrc file by Martin Brochhaus
" Presented at PyCon APAC 2012

" Automatic reloading of .vimrc
autocmd! bufwritepost nested .vimrc source %
if !exists("*ReloadConfigs")
  function ReloadConfigs()
      :source ~/.vimrc
      if has("gui_running")
          :source ~/.gvimrc
      endif
  endfunction
  command! Recfg call ReloadConfigs()
endif

" gets rid of extra space
autocmd BufWritePre * %s/\s\+$//e

" map ctrl n to line numbers
set number
set relativenumber
:nmap <C-N><C-N> :set invnumber<CR>
:nmap <F3> :set invrelativenumber<CR>

" Mouse and backspace
set bs=2 " make backspace behave like normal again


" Quick save command
noremap <C-S> :update<CR><C-c>
vnoremap <C-S> <C-C>:update<CR><C-c>
inoremap <C-S> <C-O>:update<CR><C-c>

" Quick quit command
noremap <C-Z> :quit<CR>
vnoremap <C-Z> <C-C>:quit<CR>
inoremap <C-Z> <C-O>:quit<CR>
noremap <Leader>e :quit<CR> " Quit current window
noremap <Leader>E :qa!<CR> " Quit all windows

" bind Ctrl+<movement> keys to move around the windows, instead of using Ctrl+w + <movement>
" Every unnecessary keystroke that can be saved is good for your health :)
map <C-j> <C-w>j
map <C-k> <C-w>k
map <C-l> <C-w>l
map <C-h> <C-w>h

" easier moving between tabs
map <Leader>n <esc>:tabprevious<CR>
map <Leader>m <esc>:tabnext<CR>

" map sort function to a key
vnoremap <Leader>s :sort<CR>

" easier moving of code blocks
" Try to go into visual mode (v), thenselect several lines of code here and
" then press ``>`` several times.
vnoremap < <gv " better indentation
vnoremap > >gv " better indentation

" Show whitespace
" MUST be inserted BEFORE the colorscheme command
autocmd ColorScheme * highlight ExtraWhitespace ctermbg=none guibg=none
au InsertLeave * match ExtraWhitespace /\s\+$/

" Color scheme
" mkdir -p ~/.vim/colors && cd ~/.vim/colors
" wget -O wombat256mod.vim http://www.vim.org/scripts/download_script.php?src_id=13400
set t_Co=256
color wombat256mod

syntax on
"
"set nocompatible
"set nocp

" Showing line numbers and length
set tw=0 " width of document (used by gd)
"set nowrap " don't automatically wrap on load
"set fo-=t " don't automatically wrap text when typing
set wrap linebreak nolist
set breakindent
set colorcolumn=80
highlight ColorColumn ctermbg=233


" easier formatting of paragraphs
vmap Q gq
nmap Q gqap

" Useful settings
set history=700
set undolevels=700

" Tab settings
set tabstop=4
set softtabstop=4
set shiftwidth=4
set shiftround
set expandtab

" Make search case insensitive
"set hlsearch
set incsearch
set ignorecase
set smartcase

" Disable backup and swap files - they trigger too many events
" for file system watchers
set nobackup
set nowritebackup
set noswapfile

" ============================================================================
" Python IDE Setup
" ============================================================================

" Settings for powerline
"set rtp+=$HOME/.local/lib/python3.5/site-packages/powerline/bindings/vim/
set rtp+=$HOME/.vim/bundle/powerline/powerline/bindings/vim/
set laststatus=2
let g:miniBufExplForceSyntaxEnable = 1


set wildignore+=*.pyc
set wildignore+=*_build/*
set wildignore+=*/coverage/*




map <Leader>b Oimport ipdb; ipdb.set_trace() # BREAKPOINT<C-c>

" Better navigating through omnicomplete option list
" See http://stackoverflow.com/questions/2170023/how-to-map-keys-for-popup-menu-in-vim
set completeopt=longest,menuone
function! OmniPopup(action)
  if pumvisible()
    if a:action == 'j'
      return "\<C-N>"
    elseif a:action == 'k'
      return "\<C-P>"
    endif
  endif
  return a:action
endfunction

inoremap <silent><C-j> <C-R>=OmniPopup('j')<CR>
inoremap <silent><C-k> <C-R>=OmniPopup('k')<CR>

" Python folding
" mkdir -p ~/.vim/ftplugin
" wget -O ~/.vim/ftplugin/python_editing.vim http://www.vim.org/scripts/download_script.php?src_id=5492
set nofoldenable

set wildmenu
set wildmode=list:longest,full

set clipboard=unnamedplus

" load up the nerd tree
"autocmd vimenter * NERDTree
map <Leader>t <plug>NERDTreeTabsToggle<CR>

" move nerdtree to the right
let g:NERDTreeWinPos = "right"
" " move to the first buffer
autocmd VimEnter * wincmd p

" paste toggle
set pastetoggle=<F2>

" ============================================================================
" My stuff
" ============================================================================
"------------------------------------------------"
nmap ; :
"------------------------------------------------"
" Put plugins and dictionaries in this dir (also on Windows)
let vimDir = '$HOME/.vim'
let &runtimepath.=','.vimDir
" Keep undo history across sessions by storing it in a file
if has('persistent_undo')
    let myUndoDir = expand(vimDir . '/undodir')
    " Create dirs
    call system('mkdir ' . vimDir)
    call system('mkdir ' . myUndoDir)
    let &undodir = myUndoDir
    set undofile
endif
"------------------------------------------------"
set title
"------------------------------------------------"
set path+=**
set wildignore+=*/node_modules/*,*/vendor/*
"------------------------------------------------"
" Enable folding
set foldmethod=indent
set foldlevel=99
" Enable folding with the spacebar
nnoremap <space> za
let g:SimpylFold_docstring_preview=1
" Run python from Vim
nnoremap <buffer> <F9> :!ipython --no-confirm-exit -i %<CR><CR>
nnoremap <buffer> <S-F9> :!ipython %<CR>
nnoremap - ddjP
nnoremap _ ddkP
nnoremap <S-F5> :so $vrc
nnoremap <F5> :edit<CR>
nnoremap <C-c> :noh<CR><C-c>
nmap <Delete> O<C-c>
nmap <CR> o<C-c>
" Remove automatic comment insertion
autocmd FileType * setlocal formatoptions-=c formatoptions-=r formatoptions-=o
