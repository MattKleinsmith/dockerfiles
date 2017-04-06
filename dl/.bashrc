# ~/.bashrc: executed by bash(1) for non-login shells.
# see /usr/share/doc/bash/examples/startup-files (in the package bash-doc)
# for examples

# If not running interactively, don't do anything
[ -z "$PS1" ] && return

# don't put duplicate lines in the history. See bash(1) for more options
# ... or force ignoredups and ignorespace
HISTCONTROL=ignoredups:ignorespace

# append to the history file, don't overwrite it
shopt -s histappend

# for setting history length see HISTSIZE and HISTFILESIZE in bash(1)
HISTSIZE=1000
HISTFILESIZE=2000

# check the window size after each command and, if necessary,
# update the values of LINES and COLUMNS.
shopt -s checkwinsize

# make less more friendly for non-text input files, see lesspipe(1)
[ -x /usr/bin/lesspipe ] && eval "$(SHELL=/bin/sh lesspipe)"

# set variable identifying the chroot you work in (used in the prompt below)
if [ -z "$debian_chroot" ] && [ -r /etc/debian_chroot ]; then
    debian_chroot=$(cat /etc/debian_chroot)
fi

# set a fancy prompt (non-color, unless we know we "want" color)
case "$TERM" in
    xterm-color) color_prompt=yes;;
esac

# uncomment for a colored prompt, if the terminal has the capability; turned
# off by default to not distract the user: the focus in a terminal window
# should be on the output of commands, not on the prompt
#force_color_prompt=yes

if [ -n "$force_color_prompt" ]; then
    if [ -x /usr/bin/tput ] && tput setaf 1 >&/dev/null; then
	# We have color support; assume it's compliant with Ecma-48
	# (ISO/IEC-6429). (Lack of such support is extremely rare, and such
	# a case would tend to support setf rather than setaf.)
	color_prompt=yes
    else
	color_prompt=
    fi
fi

if [ "$color_prompt" = yes ]; then
    PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '
else
    PS1='${debian_chroot:+($debian_chroot)}\u@\h:\w\$ '
fi
unset color_prompt force_color_prompt

# If this is an xterm set the title to user@host:dir
case "$TERM" in
xterm*|rxvt*)
    PS1="\[\e]0;${debian_chroot:+($debian_chroot)}\u@\h: \w\a\]$PS1"
    ;;
*)
    ;;
esac

# enable color support of ls and also add handy aliases
if [ -x /usr/bin/dircolors ]; then
    test -r ~/.dircolors && eval "$(dircolors -b ~/.dircolors)" || eval "$(dircolors -b)"
    alias ls='ls --color=auto'
    #alias dir='dir --color=auto'
    #alias vdir='vdir --color=auto'

    alias grep='grep --color=auto'
    alias fgrep='fgrep --color=auto'
    alias egrep='egrep --color=auto'
fi

# some more ls aliases
alias ll='ls -alF'
alias la='ls -A'
alias l='ls -CF'

# Alias definitions.
# You may want to put all your additions into a separate file like
# ~/.bash_aliases, instead of adding them here directly.
# See /usr/share/doc/bash-doc/examples in the bash-doc package.

if [ -f ~/.bash_aliases ]; then
    . ~/.bash_aliases
fi

# enable programmable completion features (you don't need to enable
# this, if it's already enabled in /etc/bash.bashrc and /etc/profile
# sources /etc/bash.bashrc).
#if [ -f /etc/bash_completion ] && ! shopt -oq posix; then
#    . /etc/bash_completion
#fi

##############################################################################
# TensorBoard
alias tb="tensorboard --logdir=/tmp/tb"
alias rmtb="rm /tmp/tb"
# Jupyter
export CUDA_DEVICE_ORDER="PCI_BUS_ID"
alias j8="CUDA_VISIBLE_DEVICES=0 jupyter notebook --port=8888 --allow-root"
alias j9="CUDA_VISIBLE_DEVICES=1 jupyter notebook --port=8889 --allow-root"
alias j0="CUDA_VISIBLE_DEVICES=2 jupyter notebook --port=8890 --allow-root"
# Git
alias gs='git status'
alias gpull="git pull"
alias gl="git log --oneline"
alias gc="git checkout"
alias gd="git diff"
alias ga="git add"
alias gcm="git commit -m"
# Python
alias p="ipython"
alias p3="ipython3"
# Misc
function set-title(){
  if [[ -z "$ORIG" ]]; then
    ORIG=$PS1
  fi
  TITLE="\[\e]2;$*\a\]"
  PS1=${ORIG}${TITLE}
}
function ps2(){
    ps -fC $1
}
stty -ixon
alias size1="df -h | grep --color=never 'Used\|root'"
alias size2="du -h --max-depth=1 ."
# Directories
alias t="cd /nbs/Tiramisu"
alias d="cd /nbs/impactai/dstl"
alias nb="cd /nbs"
alias seg="cd /nbs/segstyle/"
export t="/tmp"
alias bundle="cd ~/.vim/bundle"
# Terminal settings
PROMPT_DIRTRIM=2
PS1="[\D{%T}] "$PS1
# Bashrc
export bashrc=~/.bashrc
export brc=~/.bashrc
alias vb="vim $brc"
alias sb="source $brc"
# Vim
export vrc=~/.vimrc
alias vv="vim $vrc"
function v(){
    vim "$1"
}
# Programming
alias tagsl="ctags -R --fields=+l --languages=python --python-kinds=-i -f ./tags ./"
#alias tags="ctags -R --fields=+l --languages=python --python-kinds=-iv -f ./tags $(python -c "import os, sys; print(' '.join('{}'.format(d) for d in sys.path if os.path.isdir(d)))") ./"
alias tags="ctags -R --fields=+l --languages=python --python-kinds=-iv -f ./tags $(python -c "import os, sys; print(' '.join('{}'.format(d) for d in sys.path if os.path.isdir(d)))") ./"
# Unsorted / New
function py(){
    jupyter nbconvert --to python "$1"
}
function ipy(){
    ipython --no-confirm-exit -i "$1"
}
